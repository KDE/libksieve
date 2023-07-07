/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionservermetadata.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "editor/sieveeditorutil.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveConditionServerMetaData::SieveConditionServerMetaData(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("servermetadata"), i18n("Server Meta Data"), parent)
{
}

QWidget *SieveConditionServerMetaData::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto selectType = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    selectType->setObjectName(QStringLiteral("selecttype"));
    connect(selectType, &SelectMatchTypeComboBox::valueChanged, this, &SieveConditionServerMetaData::valueChanged);
    lay->addWidget(selectType);

    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    lay->addLayout(grid);

    auto lab = new QLabel(i18n("Mailbox:"));
    grid->addWidget(lab, 0, 0);

    auto mailbox = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(mailbox);
    mailbox->setObjectName(QStringLiteral("mailbox"));
    connect(mailbox, &QLineEdit::textChanged, this, &SieveConditionServerMetaData::valueChanged);
    grid->addWidget(mailbox, 0, 1);

    lab = new QLabel(i18n("Annotations:"));
    grid->addWidget(lab, 1, 0);

    auto annotation = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(annotation);
    connect(annotation, &QLineEdit::textChanged, this, &SieveConditionServerMetaData::valueChanged);
    annotation->setObjectName(QStringLiteral("annotation"));
    grid->addWidget(annotation, 1, 1);

    lab = new QLabel(i18n("Value:"));
    grid->addWidget(lab, 2, 0);

    AbstractRegexpEditorLineEdit *value = AutoCreateScriptUtil::createRegexpEditorLineEdit();
    connect(value, &AbstractRegexpEditorLineEdit::textChanged, this, &SieveConditionServerMetaData::valueChanged);
    connect(selectType, &SelectMatchTypeComboBox::switchToRegexp, value, &AbstractRegexpEditorLineEdit::switchToRegexpEditorLineEdit);
    value->setObjectName(QStringLiteral("value"));
    grid->addWidget(value, 2, 1);

    return w;
}

QString SieveConditionServerMetaData::code(QWidget *w) const
{
    const SelectMatchTypeComboBox *selectType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("selecttype"));
    bool isNegative = false;
    const QString matchString = selectType->code(isNegative);

    QString result = (isNegative ? QStringLiteral("not ") : QString()) + QStringLiteral("servermetadata %1 ").arg(matchString);

    const QLineEdit *mailbox = w->findChild<QLineEdit *>(QStringLiteral("mailbox"));
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

QStringList SieveConditionServerMetaData::needRequires(QWidget *w) const
{
    const SelectMatchTypeComboBox *selectType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("selecttype"));
    return QStringList() << QStringLiteral("servermetadata") << selectType->needRequires();
}

bool SieveConditionServerMetaData::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionServerMetaData::serverNeedsCapability() const
{
    return QStringLiteral("servermetadata");
}

QString SieveConditionServerMetaData::help() const
{
    return i18n(
        "This test retrieves the value of the server annotation \"annotation-name\".  The retrieved value is compared to the \"key-list\". The test returns "
        "true if the annotation exists and its value matches any of the keys.");
}

void SieveConditionServerMetaData::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool notCondition, QString &error)
{
    int index = 0;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1String("str")) {
            const QString tagValue = element.readElementText();
            switch (index) {
            case 0: {
                auto mailbox = w->findChild<QLineEdit *>(QStringLiteral("mailbox"));
                mailbox->setText(tagValue);
                break;
            }
            case 1: {
                auto annotation = w->findChild<QLineEdit *>(QStringLiteral("annotation"));
                annotation->setText(tagValue);
                break;
            }
            case 2: {
                auto value = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("value"));
                value->setCode(tagValue);
                break;
            }
            default:
                tooManyArguments(tagName, index, 3, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionServerMetaData::setParamWidgetValue too many argument " << index;
                break;
            }
            ++index;
        } else if (tagName == QLatin1String("tag")) {
            auto selectType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("selecttype"));
            selectType->setCode(AutoCreateScriptUtil::tagValueWithCondition(element.readElementText(), notCondition), name(), error);
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionServerMetaData::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl SieveConditionServerMetaData::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditionservermetadata.cpp"
