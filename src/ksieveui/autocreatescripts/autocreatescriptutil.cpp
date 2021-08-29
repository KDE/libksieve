/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autocreatescriptutil_p.h"

#include <KLocalizedString>

#include <KPluginFactory>
#include <KPluginLoader>
#include <KPluginMetaData>
#include <QRegularExpression>
#include <QStringList>

#include "autocreatescripts/sieveconditions/widgets/regexpeditorlineedit.h"

#include <KSieveUi/AbstractMoveImapFolderWidget>
#include <KSieveUi/AbstractSelectEmailLineEdit>

#include <widgets/moveimapfolderwidget.h>

#include "kcoreaddons_version.h"
#include <autocreatescripts/sieveactions/widgets/addresslineedit.h>
using namespace KSieveUi;

QString AutoCreateScriptUtil::createMultiLine(const QString &str)
{
    const QString result = QStringLiteral("\n%1\n.\n;\n").arg(str);
    return result;
}

QString AutoCreateScriptUtil::createList(const QString &str, QChar separator, bool addEndSemiColon)
{
    const QStringList list = str.trimmed().split(separator);
    const int count = list.count();
    switch (count) {
    case 0:
        return QString();
    case 1:
        return QLatin1String("\"") + list.first() + QLatin1String("\"");
    default: {
        const QString result = createList(list, addEndSemiColon);
        return result;
    }
    }
}

QString AutoCreateScriptUtil::quoteStr(const QString &str, bool protectSlash)
{
    QString st = str;
    if (protectSlash) {
        st = AutoCreateScriptUtil::protectSlash(str);
    }
    return st.replace(QLatin1String("\""), QStringLiteral("\\\""));
}

QString AutoCreateScriptUtil::protectSlash(QString str)
{
    return str.replace(QLatin1Char('\\'), QStringLiteral("\\\\"));
}

QString AutoCreateScriptUtil::createList(const QStringList &lst, bool addSemiColon, bool protectSlash)
{
    QString result;
    result = QLatin1Char('[');
    bool wasFirst = true;
    for (QString str : lst) {
        if (protectSlash) {
            str = AutoCreateScriptUtil::protectSlash(str);
        }
        result += (wasFirst ? QString() : QStringLiteral(",")) + QStringLiteral(" \"%1\"").arg(quoteStr(str, false));
        wasFirst = false;
    }
    result += QLatin1String(" ]");
    if (addSemiColon) {
        result += QLatin1Char(';');
    }

    return result;
}

QStringList AutoCreateScriptUtil::createListFromString(QString str)
{
    QStringList lst;
    if (str.startsWith(QLatin1Char('[')) && str.endsWith(QLatin1String("];"))) {
        str.remove(0, 1);
        str.remove(str.length() - 2, 2);
    } else if (str.startsWith(QLatin1Char('[')) && str.endsWith(QLatin1String("]"))) {
        str.remove(0, 1);
        str.remove(str.length() - 1, 1);
    } else {
        return lst;
    }
    lst = str.split(QStringLiteral(", "));
    QStringList resultLst;
    resultLst.reserve(lst.count());
    for (QString s : std::as_const(lst)) {
        s.remove(QLatin1Char('"'));
        resultLst << s.trimmed();
    }
    lst = resultLst;
    return lst;
}

QString AutoCreateScriptUtil::createAddressList(const QString &str, bool addSemiColon)
{
    if (str.trimmed().startsWith(QLatin1Char('[')) && str.trimmed().endsWith(QLatin1Char(']'))) {
        return str;
    }
    return createList(str, QLatin1Char(';'), addSemiColon);
}

QString AutoCreateScriptUtil::negativeString(bool isNegative)
{
    return isNegative ? QStringLiteral("not ") : QString();
}

QString AutoCreateScriptUtil::tagValueWithCondition(const QString &tag, bool notCondition)
{
    return (notCondition ? QStringLiteral("[NOT]") : QString()) + QLatin1Char(':') + tag;
}

QString AutoCreateScriptUtil::tagValue(const QString &tag)
{
    return QLatin1Char(':') + tag;
}

QString AutoCreateScriptUtil::strValue(QXmlStreamReader &element)
{
    if (element.readNextStartElement()) {
        const QStringRef textElementTagName = element.name();
        if (textElementTagName == QLatin1String("str")) {
            return element.readElementText();
        } else {
            element.skipCurrentElement();
        }
    }
    return QString();
}

QString AutoCreateScriptUtil::listValueToStr(QXmlStreamReader &element)
{
    const QStringList lst = AutoCreateScriptUtil::listValue(element);
    // Don't add semicolon
    return createList(lst, false);
}

QStringList AutoCreateScriptUtil::listValue(QXmlStreamReader &element)
{
    QStringList lst;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("str")) {
            lst << element.readElementText();
        } else {
            element.skipCurrentElement();
        }
    }
    return lst;
}

QString AutoCreateScriptUtil::fixListValue(QString valueStr)
{
    static QRegularExpression reg(QStringLiteral("^\\[\\s*\".*\"\\s*]$"));
    if (!(valueStr.startsWith(QLatin1Char('[')) && valueStr.endsWith(QLatin1Char(']')))) {
        valueStr = QStringLiteral("\"%1\"").arg(valueStr);
    } else if (valueStr.contains(reg)) {
    } else {
        valueStr = QStringLiteral("\"%1\"").arg(valueStr);
    }

    return valueStr;
}

void AutoCreateScriptUtil::comboboxItemNotFound(const QString &searchItem, const QString &name, QString &error)
{
    error += i18n("Cannot find item \"%1\" in widget \"%2\"", searchItem, name) + QLatin1Char('\n');
}

QString AutoCreateScriptUtil::createFullWhatsThis(const QString &help, const QString &href)
{
    if (href.isEmpty()) {
        return help;
    }
    const QString fullWhatsThis = QLatin1String("<qt>") + help + QStringLiteral("<br><a href=\'%1\'>%2</a></qt>").arg(href, i18n("More information"));
    return fullWhatsThis;
}

QString AutoCreateScriptUtil::indentation()
{
    return QStringLiteral("    ");
}

KSieveUi::AbstractMoveImapFolderWidget *AutoCreateScriptUtil::createImapFolderWidget()
{
    KSieveUi::AbstractMoveImapFolderWidget *edit = nullptr;
#if KCOREADDONS_VERSION < QT_VERSION_CHECK(5, 86, 0)
    KPluginLoader loader(QStringLiteral("libksieve/imapfoldercompletionplugin"));
    KPluginFactory *factory = loader.factory();
    if (factory) {
        edit = factory->create<KSieveUi::AbstractMoveImapFolderWidget>();
#else
    const KPluginMetaData editWidgetPlugin(QStringLiteral("libksieve/imapfoldercompletionplugin"));

    const auto result = KPluginFactory::instantiatePlugin<KSieveUi::AbstractMoveImapFolderWidget>(editWidgetPlugin);
    if (result) {
        edit = result.plugin;
    } else {
#endif
        edit = new KSieveUi::MoveImapFolderWidget;
    }
    return edit;
}

KSieveUi::AbstractSelectEmailLineEdit *AutoCreateScriptUtil::createSelectEmailsWidget()
{
    KSieveUi::AbstractSelectEmailLineEdit *edit = nullptr;
#if KCOREADDONS_VERSION < QT_VERSION_CHECK(5, 86, 0)
    KPluginLoader loader(QStringLiteral("libksieve/emaillineeditplugin"));
    KPluginFactory *factory = loader.factory();
    if (factory) {
        edit = factory->create<KSieveUi::AbstractSelectEmailLineEdit>();
#else
    const KPluginMetaData editWidgetPlugin(QStringLiteral("libksieve/emaillineeditplugin"));

    const auto result = KPluginFactory::instantiatePlugin<KSieveUi::AbstractSelectEmailLineEdit>(editWidgetPlugin);
    if (result) {
        edit = result.plugin;
    } else {
#endif
        edit = new AddressLineEdit;
    }
    return edit;
}

AbstractRegexpEditorLineEdit *AutoCreateScriptUtil::createRegexpEditorLineEdit(QWidget *parent)
{
    KSieveUi::AbstractRegexpEditorLineEdit *edit = nullptr;
#if KCOREADDONS_VERSION < QT_VERSION_CHECK(5, 86, 0)
    KPluginLoader loader(QStringLiteral("libksieve/regexpeditorlineeditplugin"));
    KPluginFactory *factory = loader.factory();
    if (factory) {
        edit = factory->create<KSieveUi::AbstractRegexpEditorLineEdit>(parent);
#else
    const KPluginMetaData editWidgetPlugin(QStringLiteral("libksieve/regexpeditorlineeditplugin"));

    const auto result = KPluginFactory::instantiatePlugin<KSieveUi::AbstractRegexpEditorLineEdit>(editWidgetPlugin, parent);
    if (result) {
        edit = result.plugin;
#endif
    } else {
        edit = new KSieveUi::RegexpEditorLineEdit(parent);
    }
    return edit;
}

QString AutoCreateScriptUtil::generateConditionComment(const QString &comment)
{
    QString strComment;
    if (!comment.trimmed().isEmpty()) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        const QVector<QStringView> commentList = QStringView(comment).split(QLatin1Char('\n'));
        for (const QStringView str : commentList) {
#else
        const QVector<QStringRef> commentList = comment.splitRef(QLatin1Char('\n'));
        for (const QStringRef &str : commentList) {
#endif
            if (str.isEmpty()) {
                strComment += QLatin1Char('\n');
            } else {
                if (!strComment.isEmpty()) {
                    strComment += QLatin1Char('\n');
                }
                strComment += QLatin1String(" #") + str;
            }
        }
    }
    return strComment;
}

QString AutoCreateScriptUtil::loadConditionComment(QString originalComment, const QString &comment)
{
    if (originalComment.isEmpty()) {
        originalComment = comment;
    } else {
        originalComment += QLatin1Char('\n') + comment;
    }
    return originalComment;
}
