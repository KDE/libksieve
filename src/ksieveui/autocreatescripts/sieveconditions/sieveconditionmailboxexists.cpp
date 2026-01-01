/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionmailboxexists.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <QHBoxLayout>
#include <QWidget>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveConditionMailboxExists::SieveConditionMailboxExists(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, u"mailboxexists"_s, i18n("Mailbox exists"), parent)
{
}

QWidget *SieveConditionMailboxExists::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto edit = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(edit);
    connect(edit, &QLineEdit::textChanged, this, &SieveConditionMailboxExists::valueChanged);
    edit->setClearButtonEnabled(true);
    lay->addWidget(edit);
    edit->setObjectName(QLatin1StringView("edit"));

    return w;
}

QString SieveConditionMailboxExists::code(QWidget *w) const
{
    const QLineEdit *edit = w->findChild<QLineEdit *>(u"edit"_s);
    const QString editValue = edit->text();
    return u"mailboxexists \"%1\""_s.arg(editValue) + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionMailboxExists::needRequires(QWidget *) const
{
    return QStringList() << u"mailbox"_s;
}

bool SieveConditionMailboxExists::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionMailboxExists::serverNeedsCapability() const
{
    return u"mailbox"_s;
}

QString SieveConditionMailboxExists::help() const
{
    return i18n(
        "The \"mailboxexists\" test is true if all mailboxes listed in the \"mailbox-names\" argument exist in the mailstore, and each allows the user in "
        "whose context the Sieve script runs to \"deliver\" messages into it.");
}

void SieveConditionMailboxExists::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool /*notCondition*/, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("str")) {
            const QString tagValue = element.readElementText();
            auto edit = w->findChild<QLineEdit *>(u"edit"_s);
            edit->setText(AutoCreateScriptUtil::quoteStr(tagValue));
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionMailboxExists::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl SieveConditionMailboxExists::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditionmailboxexists.cpp"
