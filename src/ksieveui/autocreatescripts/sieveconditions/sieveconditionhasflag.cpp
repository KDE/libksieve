/*
   Copyright (C) 2013-2019 Laurent Montel <montel@kde.org>

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
#include "sieveconditionhasflag.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>
#include <QLineEdit>

#include <QWidget>
#include <QHBoxLayout>
#include "libksieve_debug.h"
#include <QXmlStreamReader>
#include <QLabel>

using namespace KSieveUi;
SieveConditionHasFlag::SieveConditionHasFlag(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("hasflag"), i18n("Has Flag"), parent)
{
    hasVariableSupport = sieveCapabilities().contains(QLatin1String("variables"));
}

QWidget *SieveConditionHasFlag::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setContentsMargins(0, 0, 0, 0);
    w->setLayout(lay);
    SelectMatchTypeComboBox *selecttype = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    selecttype->setObjectName(QStringLiteral("matchtype"));
    connect(selecttype, &SelectMatchTypeComboBox::valueChanged, this, &SieveConditionHasFlag::valueChanged);
    lay->addWidget(selecttype);

    QGridLayout *grid = new QGridLayout;
    grid->setContentsMargins(0, 0, 0, 0);
    lay->addLayout(grid);

    int row = 0;
    if (hasVariableSupport) {
        QLabel *lab = new QLabel(i18n("Variable name\n (if empty it uses internal variable):"));
        grid->addWidget(lab, row, 0);

        QLineEdit *variableName = new QLineEdit;
        variableName->setObjectName(QStringLiteral("variablename"));
        connect(variableName, &QLineEdit::textChanged, this, &SieveConditionHasFlag::valueChanged);
        grid->addWidget(variableName, row, 1);
        ++row;
    }
    QLabel *lab = new QLabel(i18n("Value:"));
    grid->addWidget(lab, row, 0);

    AbstractRegexpEditorLineEdit *value = AutoCreateScriptUtil::createRegexpEditorLineEdit();
    connect(value, &AbstractRegexpEditorLineEdit::textChanged, this, &SieveConditionHasFlag::valueChanged);
    connect(selecttype, &SelectMatchTypeComboBox::switchToRegexp, value, &AbstractRegexpEditorLineEdit::switchToRegexpEditorLineEdit);
    value->setObjectName(QStringLiteral("value"));
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

bool SieveConditionHasFlag::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool notCondition, QString &error)
{
    QStringList strList;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("tag")) {
            SelectMatchTypeComboBox *matchTypeCombo = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
            matchTypeCombo->setCode(AutoCreateScriptUtil::tagValueWithCondition(element.readElementText(), notCondition), name(), error);
        } else if (tagName == QLatin1String("str")) {
            strList << element.readElementText();
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionExists::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }

    switch (strList.count()) {
    case 1:
    {
        AbstractRegexpEditorLineEdit *value = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("value"));
        value->setCode(strList.at(0));
        break;
    }
    case 2:
        if (hasVariableSupport) {
            QLineEdit *variableName = w->findChild<QLineEdit *>(QStringLiteral("variablename"));
            variableName->setText(strList.at(0));
            AbstractRegexpEditorLineEdit *value = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("value"));
            value->setCode(strList.at(1));
        } else {
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionHasFlag has not variable support";
        }
        break;
    default:
        qCDebug(LIBKSIEVE_LOG) << " SieveConditionHasFlag::setParamWidgetValue str list count not correct :" << strList.count();
        break;
    }

    return true;
}

QUrl SieveConditionHasFlag::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
