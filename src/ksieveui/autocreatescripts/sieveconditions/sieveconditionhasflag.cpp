/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionhasflag.h"
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
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("hasflag"), i18n("Has Flag"), parent)
{
    hasVariableSupport = sieveCapabilities().contains(QLatin1String("variables"));
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
        auto lab = new QLabel(i18n("Variable name\n (if empty it uses internal variable):"));
        grid->addWidget(lab, row, 0);

        auto variableName = new QLineEdit;
        KLineEditEventHandler::catchReturnKey(variableName);
        variableName->setObjectName(QLatin1StringView("variablename"));
        connect(variableName, &QLineEdit::textChanged, this, &SieveConditionHasFlag::valueChanged);
        grid->addWidget(variableName, row, 1);
        ++row;
    }
    auto lab = new QLabel(i18n("Value:"));
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
    const SelectMatchTypeComboBox *matchTypeCombo = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
    bool isNegative = false;
    const QString matchString = matchTypeCombo->code(isNegative);

    QString result = AutoCreateScriptUtil::negativeString(isNegative) + QStringLiteral("hasflag %1").arg(matchString);

    if (hasVariableSupport) {
        const QLineEdit *variableName = w->findChild<QLineEdit *>(QStringLiteral("variablename"));
        const QString variableNameStr = variableName->text();
        if (!variableNameStr.isEmpty()) {
            result += QLatin1String(" \"") + variableNameStr + QLatin1Char('"');
        }

        const AbstractRegexpEditorLineEdit *value = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("value"));
        const QString valueStr = value->code();
        result += QLatin1String(" \"") + valueStr + QLatin1Char('"');
    }
    return result + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionHasFlag::needRequires(QWidget *w) const
{
    QStringList lst;
    if (sieveCapabilities().contains(QLatin1String("imap4flags"))) {
        lst << QStringLiteral("imap4flags");
    } else {
        lst << QStringLiteral("imapflags");
    }
    if (hasVariableSupport) {
        lst << QStringLiteral("variables");
    }
    const SelectMatchTypeComboBox *matchTypeCombo = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
    lst << matchTypeCombo->needRequires();
    return lst;
}

bool SieveConditionHasFlag::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionHasFlag::serverNeedsCapability() const
{
    return QStringLiteral("variables");
#if 0
    if (sieveCapabilities().contains(QLatin1String("variables"))) {
    }
    if (sieveCapabilities().contains(QLatin1String("imap4flags"))) {
        return QStringLiteral("imap4flags");
    } else {
        return QStringLiteral("imapflags");
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
        if (tagName == QLatin1String("tag")) {
            auto matchTypeCombo = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
            matchTypeCombo->setCode(AutoCreateScriptUtil::tagValueWithCondition(element.readElementText(), notCondition), name(), error);
        } else if (tagName == QLatin1String("str")) {
            strList << element.readElementText();
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
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
        auto value = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("value"));
        value->setCode(strList.at(0));
        break;
    }
    case 2:
        if (hasVariableSupport) {
            auto variableName = w->findChild<QLineEdit *>(QStringLiteral("variablename"));
            variableName->setText(strList.at(0));
            auto value = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("value"));
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
