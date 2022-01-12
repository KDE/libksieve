/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionheader.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectheadertypecombobox.h"

#include <KLocalizedString>

#include "libksieve_debug.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveConditionHeader::SieveConditionHeader(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("header"), i18n("Header"), parent)
{
}

QWidget *SieveConditionHeader::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto matchTypeCombo = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    matchTypeCombo->setObjectName(QStringLiteral("matchtypecombobox"));
    connect(matchTypeCombo, &SelectMatchTypeComboBox::valueChanged, this, &SieveConditionHeader::valueChanged);
    lay->addWidget(matchTypeCombo);

    auto grid = new QGridLayout;
    lay->addLayout(grid);

    auto headerType = new SelectHeaderTypeComboBox;
    headerType->setObjectName(QStringLiteral("headertype"));
    connect(headerType, &SelectHeaderTypeComboBox::valueChanged, this, &SieveConditionHeader::valueChanged);
    grid->addWidget(headerType, 0, 0, 1, 2);

    auto lab = new QLabel(i18n("With value:"));
    grid->addWidget(lab, 1, 0);

    AbstractRegexpEditorLineEdit *value = AutoCreateScriptUtil::createRegexpEditorLineEdit();
    connect(value, &AbstractRegexpEditorLineEdit::textChanged, this, &SieveConditionHeader::valueChanged);
    connect(matchTypeCombo, &SelectMatchTypeComboBox::switchToRegexp, value, &AbstractRegexpEditorLineEdit::switchToRegexpEditorLineEdit);
    value->setObjectName(QStringLiteral("value"));
    grid->addWidget(value, 1, 1);
    return w;
}

QString SieveConditionHeader::code(QWidget *w) const
{
    const SelectMatchTypeComboBox *matchTypeCombo = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtypecombobox"));
    bool isNegative = false;
    const QString matchString = matchTypeCombo->code(isNegative);

    const SelectHeaderTypeComboBox *headerType = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headertype"));
    const QString headerStr = headerType->code();

    const AbstractRegexpEditorLineEdit *edit = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("value"));
    QString valueStr = edit->code();

    valueStr = AutoCreateScriptUtil::fixListValue(valueStr);
    return AutoCreateScriptUtil::negativeString(isNegative) + QStringLiteral("header %1 %2 %3").arg(matchString, headerStr, valueStr)
        + AutoCreateScriptUtil::generateConditionComment(comment());
}

QString SieveConditionHeader::help() const
{
    return i18n("The \"header\" test evaluates to true if the value of any of the named headers, ignoring leading and trailing whitespace, matches any key.");
}

void SieveConditionHeader::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool notCondition, QString &error)
{
    int index = 0;
    QString commentStr;
    while (element.readNextStartElement()) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        const QStringRef tagName = element.name();
#else
        const QStringView tagName = element.name();
#endif

        if (tagName == QLatin1String("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1String("comparator")) {
                qCWarning(LIBKSIEVE_LOG) << " comparator support not implemented yet!";
            } else {
                auto selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtypecombobox"));
                selectMatchCombobox->setCode(AutoCreateScriptUtil::tagValueWithCondition(tagValue, notCondition), name(), error);
            }
        } else if (tagName == QLatin1String("str")) {
            if (index == 0) {
                auto headerType = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headertype"));
                headerType->setCode(element.readElementText());
            } else if (index == 1) {
                auto value = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("value"));
                QString st = AutoCreateScriptUtil::quoteStr(element.readElementText(), true);
                value->setCode(st);
            } else {
                tooManyArguments(tagName, index, 2, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionHeader::setParamWidgetValue too many argument " << index;
            }
            ++index;
        } else if (tagName == QLatin1String("list")) {
            // Header list
            if (index == 0) {
                auto headerType = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headertype"));
                headerType->setCode(AutoCreateScriptUtil::listValueToStr(element));
            } else if (index == 1) {
                auto value = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("value"));
                value->setCode(AutoCreateScriptUtil::listValueToStr(element));
            } else {
                tooManyArguments(tagName, index, 2, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionHeader::setParamWidgetValue too many argument " << index;
            }
            ++index;
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionHeader::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QStringList KSieveUi::SieveConditionHeader::needRequires(QWidget *w) const
{
    const SelectMatchTypeComboBox *matchTypeCombo = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtypecombobox"));
    return matchTypeCombo->needRequires();
}
