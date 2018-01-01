/*
   Copyright (C) 2013-2018 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "sieveconditionmailboxexists.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include <KLocalizedString>
#include <QLineEdit>

#include <QWidget>
#include <QHBoxLayout>
#include "libksieve_debug.h"
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveConditionMailboxExists::SieveConditionMailboxExists(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("mailboxexists"), i18n("Mailbox exists"), parent)
{
}

QWidget *SieveConditionMailboxExists::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setMargin(0);
    w->setLayout(lay);

    QLineEdit *edit = new QLineEdit;
    connect(edit, &QLineEdit::textChanged, this, &SieveConditionMailboxExists::valueChanged);
    edit->setClearButtonEnabled(true);
    lay->addWidget(edit);
    edit->setObjectName(QStringLiteral("edit"));

    return w;
}

QString SieveConditionMailboxExists::code(QWidget *w) const
{
    const QLineEdit *edit = w->findChild<QLineEdit *>(QStringLiteral("edit"));
    const QString editValue = edit->text();
    return QStringLiteral("mailboxexists \"%1\"").arg(editValue) + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionMailboxExists::needRequires(QWidget *) const
{
    return QStringList() << QStringLiteral("mailbox");
}

bool SieveConditionMailboxExists::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionMailboxExists::serverNeedsCapability() const
{
    return QStringLiteral("mailbox");
}

QString SieveConditionMailboxExists::help() const
{
    return i18n(
        "The \"mailboxexists\" test is true if all mailboxes listed in the \"mailbox-names\" argument exist in the mailstore, and each allows the user in whose context the Sieve script runs to \"deliver\" messages into it.");
}

bool SieveConditionMailboxExists::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool /*notCondition*/, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("str")) {
            const QString tagValue = element.readElementText();
            QLineEdit *edit = w->findChild<QLineEdit *>(QStringLiteral("edit"));
            edit->setText(AutoCreateScriptUtil::quoteStr(tagValue));
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionMailboxExists::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }

    return true;
}

QUrl SieveConditionMailboxExists::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
