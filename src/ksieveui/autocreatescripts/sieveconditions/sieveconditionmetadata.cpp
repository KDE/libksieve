/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionmetadata.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "editor/sieveeditorutil.h"
#include "util/sieveimapaccountsettings.h"
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <KLineEditEventHandler>
#include <KSieveUi/AbstractMoveImapFolderWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveConditionMetaData::SieveConditionMetaData(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("metadata"), i18n("Meta Data"), parent)
{
}

QWidget *SieveConditionMetaData::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto selectType = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    selectType->setObjectName(QLatin1StringView("selecttype"));
    connect(selectType, &SelectMatchTypeComboBox::valueChanged, this, &SieveConditionMetaData::valueChanged);
    lay->addWidget(selectType);

    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    lay->addLayout(grid);

    auto lab = new QLabel(i18n("Mailbox:"));
    grid->addWidget(lab, 0, 0);

    KSieveUi::AbstractMoveImapFolderWidget *mailbox = AutoCreateScriptUtil::createImapFolderWidget();
    mailbox->setSieveImapAccountSettings(sieveImapAccountSettings());

    connect(mailbox, &KSieveUi::AbstractMoveImapFolderWidget::textChanged, this, &SieveConditionMetaData::valueChanged);
    mailbox->setObjectName(QLatin1StringView("mailbox"));
    grid->addWidget(mailbox, 0, 1);

    lab = new QLabel(i18n("Annotations:"));
    grid->addWidget(lab, 1, 0);

    auto annotation = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(annotation);
    connect(annotation, &QLineEdit::textChanged, this, &SieveConditionMetaData::valueChanged);
    annotation->setObjectName(QLatin1StringView("annotation"));
    grid->addWidget(annotation, 1, 1);

    lab = new QLabel(i18n("Value:"));
    grid->addWidget(lab, 2, 0);

    AbstractRegexpEditorLineEdit *value = AutoCreateScriptUtil::createRegexpEditorLineEdit();
    connect(value, &AbstractRegexpEditorLineEdit::textChanged, this, &SieveConditionMetaData::valueChanged);
    connect(selectType, &SelectMatchTypeComboBox::switchToRegexp, value, &AbstractRegexpEditorLineEdit::switchToRegexpEditorLineEdit);
    value->setObjectName(QLatin1StringView("value"));
    grid->addWidget(value, 2, 1);

    return w;
}

QString SieveConditionMetaData::code(QWidget *w) const
{
    const SelectMatchTypeComboBox *selectType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("selecttype"));
    bool isNegative = false;
    const QString matchString = selectType->code(isNegative);

    QString result = AutoCreateScriptUtil::negativeString(isNegative) + QStringLiteral("metadata %1 ").arg(matchString);

    const KSieveUi::AbstractMoveImapFolderWidget *mailbox = w->findChild<KSieveUi::AbstractMoveImapFolderWidget *>(QStringLiteral("mailbox"));
    const QString mailboxStr = mailbox->text();

    result += QStringLiteral("\"%1\" ").arg(mailboxStr);

    const QLineEdit *annotation = w->findChild<QLineEdit *>(QStringLiteral("annotation"));
    const QString annotationStr = annotation->text();

    result += QStringLiteral("\"%1\" ").arg(annotationStr);

    const AbstractRegexpEditorLineEdit *value = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("value"));
    const QString valueStr = value->code();

    result += QStringLiteral("\"%1\"").arg(valueStr);
    return result + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionMetaData::needRequires(QWidget *w) const
{
    const SelectMatchTypeComboBox *selectType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("selecttype"));
    return QStringList() << QStringLiteral("mboxmetadata") << selectType->needRequires();
}

bool SieveConditionMetaData::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionMetaData::serverNeedsCapability() const
{
    return QStringLiteral("mboxmetadata");
}

QString SieveConditionMetaData::help() const
{
    return i18n(
        "This test retrieves the value of the mailbox annotation \"annotation-name\" for the mailbox \"mailbox\". The retrieved value is compared to the "
        "\"key-list\". The test returns true if the annotation exists and its value matches any of the keys.");
}

void SieveConditionMetaData::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool notCondition, QString &error)
{
    int index = 0;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("str")) {
            const QString tagValue = element.readElementText();
            switch (index) {
            case 0: {
                auto mailbox = w->findChild<KSieveUi::AbstractMoveImapFolderWidget *>(QStringLiteral("mailbox"));
                mailbox->setText(tagValue);
                break;
            }
            case 1: {
                auto annotation = w->findChild<QLineEdit *>(QStringLiteral("annotation"));
                annotation->setText(AutoCreateScriptUtil::quoteStr(tagValue));
                break;
            }
            case 2: {
                auto value = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("value"));
                value->setCode(AutoCreateScriptUtil::quoteStr(tagValue));
                break;
            }
            default:
                tooManyArguments(tagName, index, 3, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionMetaData::setParamWidgetValue too many argument " << index;
                break;
            }
            ++index;
        } else if (tagName == QLatin1StringView("tag")) {
            auto selectType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("selecttype"));
            selectType->setCode(AutoCreateScriptUtil::tagValueWithCondition(element.readElementText(), notCondition), name(), error);
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionMetaData::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl SieveConditionMetaData::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditionmetadata.cpp"
