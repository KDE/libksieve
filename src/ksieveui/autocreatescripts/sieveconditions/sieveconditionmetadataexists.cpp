/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionmetadataexists.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include "util/sieveimapaccountsettings.h"
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <KLineEditEventHandler>
#include <KSieveUi/AbstractMoveImapFolderWidget>
#include <QGridLayout>
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveConditionMetaDataExists::SieveConditionMetaDataExists(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, u"metadataexists"_s, i18n("Metadata exists"), parent)
{
}

QWidget *SieveConditionMetaDataExists::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    w->setLayout(grid);

    auto lab = new QLabel(i18nc("@label:textbox", "Mailbox:"));
    grid->addWidget(lab, 0, 0);

    KSieveUi::AbstractMoveImapFolderWidget *mailbox = AutoCreateScriptUtil::createImapFolderWidget();
    mailbox->setSieveImapAccountSettings(sieveImapAccountSettings());
    connect(mailbox, &KSieveUi::AbstractMoveImapFolderWidget::textChanged, this, &SieveConditionMetaDataExists::valueChanged);
    mailbox->setObjectName(QLatin1StringView("mailbox"));
    grid->addWidget(mailbox, 0, 1);

    lab = new QLabel(i18nc("@label:textbox", "Annotation:"));
    grid->addWidget(lab, 1, 0);

    auto value = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(value);
    connect(value, &QLineEdit::textChanged, this, &SieveConditionMetaDataExists::valueChanged);
    value->setObjectName(QLatin1StringView("value"));
    grid->addWidget(value, 1, 1);

    return w;
}

QString SieveConditionMetaDataExists::code(QWidget *w) const
{
    const KSieveUi::AbstractMoveImapFolderWidget *mailbox = w->findChild<KSieveUi::AbstractMoveImapFolderWidget *>(u"mailbox"_s);
    const QString mailboxStr = mailbox->text();

    const QLineEdit *value = w->findChild<QLineEdit *>(u"value"_s);
    const QString valueStr = value->text();
    return u"metadataexists \"%1\" \"%2\""_s.arg(mailboxStr, valueStr) + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionMetaDataExists::needRequires(QWidget *) const
{
    return QStringList() << u"mboxmetadata"_s;
}

bool SieveConditionMetaDataExists::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionMetaDataExists::serverNeedsCapability() const
{
    return u"mboxmetadata"_s;
}

QString SieveConditionMetaDataExists::help() const
{
    return i18n("The \"metadataexists\" test is true if all of the annotations listed in the \"annotation-names\" argument exist for the specified mailbox.");
}

void SieveConditionMetaDataExists::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool /*notCondition*/, QString &error)
{
    int index = 0;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("str")) {
            const QString tagValue = element.readElementText();
            if (index == 0) {
                auto mailbox = w->findChild<KSieveUi::AbstractMoveImapFolderWidget *>(u"mailbox"_s);
                mailbox->setText(tagValue);
            } else if (index == 1) {
                auto value = w->findChild<QLineEdit *>(u"value"_s);
                value->setText(AutoCreateScriptUtil::quoteStr(tagValue));
            } else {
                tooManyArguments(tagName, index, 2, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionServerMetaDataExists::setParamWidgetValue to many attribute " << index;
            }
            ++index;
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionServerMetaDataExists::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl SieveConditionMetaDataExists::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditionmetadataexists.cpp"
