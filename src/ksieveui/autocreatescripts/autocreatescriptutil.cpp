/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

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

#include "widgets/moveimapfolderwidget.h"

#include "autocreatescripts/sieveactions/widgets/addresslineedit.h"
using namespace KSieveUi;
using namespace Qt::Literals::StringLiterals;
QString AutoCreateScriptUtil::createMultiLine(const QString &str)
{
    const QString result = u"\n%1\n.\n;\n"_s.arg(str);
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
        return QLatin1StringView("\"") + list.first() + QLatin1StringView("\"");
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
    return st.replace(QLatin1StringView("\""), u"\\\""_s);
}

QString AutoCreateScriptUtil::protectSlash(QString str)
{
    return str.replace(u'\\', u"\\\\"_s);
}

QString AutoCreateScriptUtil::createList(const QStringList &lst, bool addSemiColon, bool protectSlash)
{
    QString result;
    result = u'[';
    bool wasFirst = true;
    for (QString str : lst) {
        if (protectSlash) {
            str = AutoCreateScriptUtil::protectSlash(str);
        }
        result += (wasFirst ? QString() : u","_s) + u" \"%1\""_s.arg(quoteStr(str, false));
        wasFirst = false;
    }
    result += QLatin1StringView(" ]");
    if (addSemiColon) {
        result += u';';
    }

    return result;
}

QStringList AutoCreateScriptUtil::createListFromString(QString str)
{
    QStringList lst;
    if (str.startsWith(u'[') && str.endsWith(QLatin1StringView("];"))) {
        str.remove(0, 1);
        str.remove(str.length() - 2, 2);
    } else if (str.startsWith(u'[') && str.endsWith(QLatin1StringView("]"))) {
        str.remove(0, 1);
        str.remove(str.length() - 1, 1);
    } else {
        return lst;
    }
    lst = str.split(u", "_s);
    QStringList resultLst;
    resultLst.reserve(lst.count());
    for (QString s : std::as_const(lst)) {
        s.remove(u'"');
        resultLst << s.trimmed();
    }
    lst = resultLst;
    return lst;
}

QString AutoCreateScriptUtil::createAddressList(const QString &str, bool addSemiColon)
{
    if (str.trimmed().startsWith(u'[') && str.trimmed().endsWith(u']')) {
        return str;
    }
    return createList(str, u';', addSemiColon);
}

QString AutoCreateScriptUtil::negativeString(bool isNegative)
{
    return isNegative ? u"not "_s : QString();
}

QString AutoCreateScriptUtil::tagValueWithCondition(const QString &tag, bool notCondition)
{
    return (notCondition ? u"[NOT]"_s : QString()) + u':' + tag;
}

QString AutoCreateScriptUtil::tagValue(const QString &tag)
{
    return u':' + tag;
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
    static QRegularExpression reg(u"^\\[\\s*\".*\"\\s*]$"_s);
    if (!(valueStr.startsWith(u'[')) && valueStr.endsWith(u']')) {
        valueStr = u"\"%1\""_s.arg(valueStr);
    } else if (valueStr.contains(reg)) {
    } else {
        valueStr = u"\"%1\""_s.arg(valueStr);
    }

    return valueStr;
}

void AutoCreateScriptUtil::comboboxItemNotFound(const QString &searchItem, const QString &name, QString &error)
{
    error += i18n("Cannot find item \"%1\" in widget \"%2\"", searchItem, name) + u'\n';
}

QString AutoCreateScriptUtil::createFullWhatsThis(const QString &help, const QString &href)
{
    if (href.isEmpty()) {
        return help;
    }
    const QString fullWhatsThis = QLatin1StringView("<qt>") + help + u"<br><a href=\'%1\'>%2</a></qt>"_s.arg(href, i18n("More information"));
    return fullWhatsThis;
}

QString AutoCreateScriptUtil::indentation()
{
    return u"    "_s;
}

KSieveUi::AbstractMoveImapFolderWidget *AutoCreateScriptUtil::createImapFolderWidget()
{
    KSieveUi::AbstractMoveImapFolderWidget *edit = nullptr;
    const KPluginMetaData editWidgetPlugin(u"pim6/libksieve/imapfoldercompletionplugin"_s);

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
    const KPluginMetaData editWidgetPlugin(u"pim6/libksieve/emaillineeditplugin"_s);

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
        const QList<QStringView> commentList = QStringView(comment).split(u'\n');
        for (const QStringView str : commentList) {
            if (str.isEmpty()) {
                strComment += u'\n';
            } else {
                if (!strComment.isEmpty()) {
                    strComment += u'\n';
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
        originalComment += u'\n' + comment;
    }
    return originalComment;
}
