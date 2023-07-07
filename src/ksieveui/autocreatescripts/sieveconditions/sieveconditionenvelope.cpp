/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionenvelope.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectaddresspartcombobox.h"
#include "widgets/selectheadertypecombobox.h"

#include <KLocalizedString>

#include "libksieveui_debug.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveConditionEnvelope::SieveConditionEnvelope(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("envelope"), i18n("Envelope"), parent)
{
}

QWidget *SieveConditionEnvelope::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto selectAddressPart = new SelectAddressPartComboBox(mSieveGraphicalModeWidget);
    connect(selectAddressPart, &SelectAddressPartComboBox::valueChanged, this, &SieveConditionEnvelope::valueChanged);
    selectAddressPart->setObjectName(QStringLiteral("addresspartcombobox"));
    lay->addWidget(selectAddressPart);

    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    lay->addLayout(grid);

    auto selectMatchCombobox = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    selectMatchCombobox->setObjectName(QStringLiteral("matchtypecombobox"));
    connect(selectMatchCombobox, &SelectMatchTypeComboBox::valueChanged, this, &SieveConditionEnvelope::valueChanged);
    grid->addWidget(selectMatchCombobox, 0, 0);

    auto selectHeaderType = new SelectHeaderTypeComboBox(true);
    selectHeaderType->setObjectName(QStringLiteral("headertypecombobox"));
    connect(selectHeaderType, &SelectHeaderTypeComboBox::valueChanged, this, &SieveConditionEnvelope::valueChanged);
    grid->addWidget(selectHeaderType, 0, 1);

    auto lab = new QLabel(i18n("address:"));
    grid->addWidget(lab, 1, 0);

    AbstractRegexpEditorLineEdit *edit = AutoCreateScriptUtil::createRegexpEditorLineEdit();
    edit->setObjectName(QStringLiteral("editaddress"));
    connect(edit, &AbstractRegexpEditorLineEdit::textChanged, this, &SieveConditionEnvelope::valueChanged);
    connect(selectMatchCombobox, &SelectMatchTypeComboBox::switchToRegexp, edit, &AbstractRegexpEditorLineEdit::switchToRegexpEditorLineEdit);
    edit->setClearButtonEnabled(true);
    edit->setPlaceholderText(i18n("Use ; to separate emails"));
    grid->addWidget(edit, 1, 1);

    return w;
}

QString SieveConditionEnvelope::code(QWidget *w) const
{
    const SelectMatchTypeComboBox *selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtypecombobox"));
    bool isNegative = false;
    const QString matchTypeStr = selectMatchCombobox->code(isNegative);

    const SelectAddressPartComboBox *selectAddressPart = w->findChild<SelectAddressPartComboBox *>(QStringLiteral("addresspartcombobox"));
    const QString selectAddressPartStr = selectAddressPart->code();

    const SelectHeaderTypeComboBox *selectHeaderType = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headertypecombobox"));
    const QString selectHeaderTypeStr = selectHeaderType->code();

    const AbstractRegexpEditorLineEdit *edit = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("editaddress"));
    const QString addressStr = AutoCreateScriptUtil::createAddressList(edit->code().trimmed(), false);
    return AutoCreateScriptUtil::negativeString(isNegative)
        + QStringLiteral("envelope %1 %2 %3 %4").arg(selectAddressPartStr, matchTypeStr, selectHeaderTypeStr, addressStr)
        + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionEnvelope::needRequires(QWidget *w) const
{
    const SelectAddressPartComboBox *selectAddressPart = w->findChild<SelectAddressPartComboBox *>(QStringLiteral("addresspartcombobox"));
    const SelectMatchTypeComboBox *selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtypecombobox"));
    return QStringList() << QStringLiteral("envelope") << selectAddressPart->extraRequire() << selectMatchCombobox->needRequires();
}

bool SieveConditionEnvelope::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionEnvelope::serverNeedsCapability() const
{
    return QStringLiteral("envelope");
}

QString SieveConditionEnvelope::help() const
{
    return i18n("The \"envelope\" test is true if the specified part of the [SMTP] (or equivalent) envelope matches the specified key.");
}

void SieveConditionEnvelope::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool notCondition, QString &error)
{
    int indexTag = 0;
    int indexStr = 0;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();

        if (tagName == QLatin1String("tag")) {
            const QString tagValue = element.readElementText();
            if (indexTag == 0) {
                QString err;
                auto selectAddressPart = w->findChild<SelectAddressPartComboBox *>(QStringLiteral("addresspartcombobox"));
                selectAddressPart->setCode(AutoCreateScriptUtil::tagValue(tagValue), name(), err);
                // all: is default sometime we don't add it.
                if (!err.isEmpty()) {
                    auto selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtypecombobox"));
                    selectMatchCombobox->setCode(AutoCreateScriptUtil::tagValueWithCondition(tagValue, notCondition), name(), error);
                }
            } else if (indexTag == 1) {
                auto selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtypecombobox"));
                selectMatchCombobox->setCode(AutoCreateScriptUtil::tagValueWithCondition(tagValue, notCondition), name(), error);
            } else {
                tooManyArguments(tagName, indexTag, 2, error);
                qCDebug(LIBKSIEVE_LOG) << "SieveConditionEnvelope::setParamWidgetValue too many argument :" << indexTag;
            }
            ++indexTag;
        } else if (tagName == QLatin1String("str")) {
            if (indexStr == 0) {
                auto selectHeaderType = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headertypecombobox"));
                selectHeaderType->setCode(element.readElementText());
            } else if (indexStr == 1) {
                auto edit = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("editaddress"));
                edit->setCode(AutoCreateScriptUtil::quoteStr(element.readElementText()));
            } else {
                tooManyArguments(tagName, indexStr, 2, error);
                qCDebug(LIBKSIEVE_LOG) << "SieveConditionEnvelope::setParamWidgetValue too many argument indexStr " << indexStr;
            }
            ++indexStr;
        } else if (tagName == QLatin1String("list")) {
            if (indexStr == 0) {
                auto selectHeaderType = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headertypecombobox"));
                selectHeaderType->setCode(AutoCreateScriptUtil::listValueToStr(element));
            } else if (indexStr == 1) {
                auto edit = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("editaddress"));
                edit->setCode(AutoCreateScriptUtil::listValueToStr(element));
            }
            ++indexStr;
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionEnvelope::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl KSieveUi::SieveConditionEnvelope::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditionenvelope.cpp"
