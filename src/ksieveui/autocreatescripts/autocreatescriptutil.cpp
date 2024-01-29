/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autocreatescriptutil_p.h"

#include <KLocalizedString>

#include <KPluginFactory>
#include <KPluginMetaData>
#include <QRegularExpression>
#include <QStringList>

#include "autocreatescripts/sieveconditions/widgets/regexpeditorlineedit.h"

#include <KSieveUi/AbstractMoveImapFolderWidget>
#include <KSieveUi/AbstractSelectEmailLineEdit>

#include <widgets/moveimapfolderwidget.h>

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
        return {};
    case 1:
        return QLatin1StringView("\"") + list.first() + QLatin1String("\"");
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
    return st.replace(QLatin1StringView("\""), QStringLiteral("\\\""));
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
    result += QLatin1StringView(" ]");
    if (addSemiColon) {
        result += QLatin1Char(';');
    }

    return result;
}

QStringList AutoCreateScriptUtil::createListFromString(QString str)
{
    QStringList lst;
    if (str.startsWith(QLatin1Char('[')) && str.endsWith(QLatin1StringView("];"))) {
        str.remove(0, 1);
        str.remove(str.length() - 2, 2);
    } else if (str.startsWith(QLatin1Char('[')) && str.endsWith(QLatin1StringView("]"))) {
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
        const QStringView textElementTagName = element.name();
        if (textElementTagName == QLatin1StringView("str")) {
            return element.readElementText();
        } else {
            element.skipCurrentElement();
        }
    }
    return {};
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
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("str")) {
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
    const QString fullWhatsThis = QLatin1StringView("<qt>") + help + QStringLiteral("<br><a href=\'%1\'>%2</a></qt>").arg(href, i18n("More information"));
    return fullWhatsThis;
}

QString AutoCreateScriptUtil::indentation()
{
    return QStringLiteral("    ");
}

KSieveUi::AbstractMoveImapFolderWidget *AutoCreateScriptUtil::createImapFolderWidget()
{
    KSieveUi::AbstractMoveImapFolderWidget *edit = nullptr;
    const KPluginMetaData editWidgetPlugin(QStringLiteral("pim6/libksieve/imapfoldercompletionplugin"));

    const auto result = KPluginFactory::instantiatePlugin<KSieveUi::AbstractMoveImapFolderWidget>(editWidgetPlugin);
    if (result) {
        edit = result.plugin;
    } else {
        edit = new KSieveUi::MoveImapFolderWidget;
    }
    return edit;
}

KSieveUi::AbstractSelectEmailLineEdit *AutoCreateScriptUtil::createSelectEmailsWidget()
{
    KSieveUi::AbstractSelectEmailLineEdit *edit = nullptr;
    const KPluginMetaData editWidgetPlugin(QStringLiteral("pim6/libksieve/emaillineeditplugin"));

    const auto result = KPluginFactory::instantiatePlugin<KSieveUi::AbstractSelectEmailLineEdit>(editWidgetPlugin);
    if (result) {
        edit = result.plugin;
    } else {
        edit = new AddressLineEdit;
    }
    return edit;
}

AbstractRegexpEditorLineEdit *AutoCreateScriptUtil::createRegexpEditorLineEdit(QWidget *parent)
{
    auto *edit = new KSieveUi::RegexpEditorLineEdit(parent);
    return edit;
}

QString AutoCreateScriptUtil::generateConditionComment(const QString &comment)
{
    QString strComment;
    if (!comment.trimmed().isEmpty()) {
        const QList<QStringView> commentList = QStringView(comment).split(QLatin1Char('\n'));
        for (const QStringView str : commentList) {
            if (str.isEmpty()) {
                strComment += QLatin1Char('\n');
            } else {
                if (!strComment.isEmpty()) {
                    strComment += QLatin1Char('\n');
                }
                strComment += QLatin1StringView(" #") + str;
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
