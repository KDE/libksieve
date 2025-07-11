/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionhasflag.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
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
SieveConditionHasFlag::SieveConditionHasFlag(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, u"hasflag"_s, i18n("Has Flag"), parent)
{
    hasVariableSupport = sieveCapabilities().contains(QLatin1StringView("variables"));
}

QWidget *SieveConditionHasFlag::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);
    auto selecttype = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    selecttype->setObjectName(QLatin1StringView("matchtype"));
    connect(selecttype, &SelectMatchTypeComboBox::valueChanged, this, &SieveConditionHasFlag::valueChanged);
    lay->addWidget(selecttype);

    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    lay->addLayout(grid);

    int row = 0;
    if (hasVariableSupport) {
        auto lab = new QLabel(i18nc("@label:textbox", "Variable name\n (if empty it uses internal variable):"));
        grid->addWidget(lab, row, 0);

        auto variableName = new QLineEdit;
        KLineEditEventHandler::catchReturnKey(variableName);
        variableName->setObjectName(QLatin1StringView("variablename"));
        connect(variableName, &QLineEdit::textChanged, this, &SieveConditionHasFlag::valueChanged);
        grid->addWidget(variableName, row, 1);
        ++row;
    }
    auto lab = new QLabel(i18nc("@label:textbox", "Value:"));
    grid->addWidget(lab, row, 0);

    AbstractRegexpEditorLineEdit *value = AutoCreateScriptUtil::createRegexpEditorLineEdit();
    connect(value, &AbstractRegexpEditorLineEdit::textChanged, this, &SieveConditionHasFlag::valueChanged);
    connect(selecttype, &SelectMatchTypeComboBox::switchToRegexp, value, &AbstractRegexpEditorLineEdit::switchToRegexpEditorLineEdit);
    value->setObjectName(QLatin1StringView("value"));
    grid->addWidget(value, row, 1);

    return w;
}

QString SieveConditionHasFlag::code(QWidget *w) const
{
    const SelectMatchTypeComboBox *matchTypeCombo = w->findChild<SelectMatchTypeComboBox *>(u"matchtype"_s);
    bool isNegative = false;
    const QString matchString = matchTypeCombo->code(isNegative);

    QString result = AutoCreateScriptUtil::negativeString(isNegative) + u"hasflag %1"_s.arg(matchString);

    if (hasVariableSupport) {
        const QLineEdit *variableName = w->findChild<QLineEdit *>(u"variablename"_s);
        const QString variableNameStr = variableName->text();
        if (!variableNameStr.isEmpty()) {
            result += QLatin1StringView(" \"") + variableNameStr + u'"';
        }

        const AbstractRegexpEditorLineEdit *value = w->findChild<AbstractRegexpEditorLineEdit *>(u"value"_s);
        const QString valueStr = value->code();
        result += QLatin1StringView(" \"") + valueStr + u'"';
    }
    return result + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionHasFlag::needRequires(QWidget *w) const
{
    QStringList lst;
    if (sieveCapabilities().contains(QLatin1StringView("imap4flags"))) {
        lst << u"imap4flags"_s;
    } else {
        lst << u"imapflags"_s;
    }
    if (hasVariableSupport) {
        lst << u"variables"_s;
    }
    const SelectMatchTypeComboBox *matchTypeCombo = w->findChild<SelectMatchTypeComboBox *>(u"matchtype"_s);
    lst << matchTypeCombo->needRequires();
    return lst;
}

bool SieveConditionHasFlag::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionHasFlag::serverNeedsCapability() const
{
    return u"variables"_s;
#if 0
    if (sieveCapabilities().contains(QLatin1StringView("variables"))) {
    }
    if (sieveCapabilities().contains(QLatin1StringView("imap4flags"))) {
        return u"imap4flags"_s;
    } else {
        return u"imapflags"_s;
    }
#endif
}

QString SieveConditionHasFlag::help() const
{
    return i18n("The hasflag test evaluates to true if any of the variables matches any flag name.");
}

void SieveConditionHasFlag::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool notCondition, QString &error)
{
    QStringList strList;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("tag")) {
            auto matchTypeCombo = w->findChild<SelectMatchTypeComboBox *>(u"matchtype"_s);
            matchTypeCombo->setCode(AutoCreateScriptUtil::tagValueWithCondition(element.readElementText(), notCondition), name(), error);
        } else if (tagName == QLatin1StringView("str")) {
            strList << element.readElementText();
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionExists::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }

    switch (strList.count()) {
    case 1: {
        auto value = w->findChild<AbstractRegexpEditorLineEdit *>(u"value"_s);
        value->setCode(strList.at(0));
        break;
    }
    case 2:
        if (hasVariableSupport) {
            auto variableName = w->findChild<QLineEdit *>(u"variablename"_s);
            variableName->setText(strList.at(0));
            auto value = w->findChild<AbstractRegexpEditorLineEdit *>(u"value"_s);
            value->setCode(strList.at(1));
        } else {
            qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionHasFlag has not variable support";
        }
        break;
    default:
        qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionHasFlag::setParamWidgetValue str list count not correct :" << strList.count();
        break;
    }
}

QUrl SieveConditionHasFlag::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditionhasflag.cpp"
