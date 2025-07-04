/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionsetvariable.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectvariablemodifiercombobox.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveActionSetVariable::SieveActionSetVariable(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, u"set"_s, i18n("Variable"), parent)
{
    mHasRegexCapability = sieveCapabilities().contains(QLatin1StringView("regex"));
}

QWidget *SieveActionSetVariable::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    w->setLayout(grid);

    auto modifier = new SelectVariableModifierComboBox;
    modifier->setObjectName(QLatin1StringView("modifier"));
    connect(modifier, &SelectVariableModifierComboBox::valueChanged, this, &SieveActionSetVariable::valueChanged);
    grid->addWidget(modifier, 0, 0);

    if (mHasRegexCapability) {
        auto protectAgainstUseRegexp = new QCheckBox(i18nc("@option:check", "Protect special character"));
        connect(protectAgainstUseRegexp, &QCheckBox::clicked, this, &SieveActionSetVariable::valueChanged);
        protectAgainstUseRegexp->setObjectName(QLatin1StringView("regexprotect"));
        grid->addWidget(protectAgainstUseRegexp, 0, 1);
    }

    auto lab = new QLabel(i18nc("@label:textbox", "Value:"));
    grid->addWidget(lab, 1, 0);

    auto value = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(value);
    value->setObjectName(QLatin1StringView("value"));
    connect(value, &QLineEdit::textChanged, this, &SieveActionSetVariable::valueChanged);
    grid->addWidget(value, 1, 1);

    lab = new QLabel(i18nc("@label:textbox", "In variable:"));
    grid->addWidget(lab, 2, 0);

    auto variable = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(variable);
    variable->setObjectName(QLatin1StringView("variable"));
    connect(variable, &QLineEdit::textChanged, this, &SieveActionSetVariable::valueChanged);
    grid->addWidget(variable, 2, 1);

    return w;
}

QUrl SieveActionSetVariable::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

void SieveActionSetVariable::setLocalVariable(QWidget *w, const SieveGlobalVariableActionWidget::VariableElement &var)
{
    auto value = w->findChild<QLineEdit *>(u"value"_s);
    value->setText(var.variableValue);
    auto variable = w->findChild<QLineEdit *>(u"variable"_s);
    variable->setText(AutoCreateScriptUtil::protectSlash(var.variableName));
}

void SieveActionSetVariable::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("str")) {
            const QString tagValue = element.readElementText();
            auto value = w->findChild<QLineEdit *>(u"value"_s);
            value->setText(tagValue);
            if (element.readNextStartElement()) {
                const QStringView variableTagName = element.name();
                if (variableTagName == QLatin1StringView("str")) {
                    auto variable = w->findChild<QLineEdit *>(u"variable"_s);
                    variable->setText(AutoCreateScriptUtil::protectSlash(element.readElementText()));
                }
            } else {
                return;
            }
        } else if (tagName == QLatin1StringView("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1StringView("quoteregex")) {
                if (mHasRegexCapability) {
                    auto protectAgainstUseRegexp = w->findChild<QCheckBox *>(u"regexprotect"_s);
                    protectAgainstUseRegexp->setChecked(true);
                } else {
                    error += i18n("Script needs regex support, but server does not have it.") + u'\n';
                }
            } else {
                auto modifier = w->findChild<SelectVariableModifierComboBox *>(u"modifier"_s);
                modifier->setCode(AutoCreateScriptUtil::tagValue(tagValue), name(), error);
            }
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveActionSetVariable::setParamWidgetValue unknown tagName " << tagName;
        }
    }
}

QString SieveActionSetVariable::code(QWidget *w) const
{
    QString result = u"set "_s;
    const SelectVariableModifierComboBox *modifier = w->findChild<SelectVariableModifierComboBox *>(u"modifier"_s);
    const QString modifierStr = modifier->code();
    if (!modifierStr.isEmpty()) {
        result += modifierStr + u' ';
    }

    if (mHasRegexCapability) {
        const QCheckBox *protectAgainstUseRegexp = w->findChild<QCheckBox *>(u"regexprotect"_s);
        if (protectAgainstUseRegexp->isChecked()) {
            result += QLatin1StringView(":quoteregex ");
        }
    }

    const QLineEdit *value = w->findChild<QLineEdit *>(u"value"_s);
    const QString valueStr = value->text();
    result += u"\"%1\" "_s.arg(valueStr);

    const QLineEdit *variable = w->findChild<QLineEdit *>(u"variable"_s);
    const QString variableStr = variable->text();
    result += u"\"%1\";"_s.arg(variableStr);

    return result;
}

QStringList SieveActionSetVariable::needRequires(QWidget *) const
{
    return QStringList() << u"variables"_s;
}

bool SieveActionSetVariable::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionSetVariable::serverNeedsCapability() const
{
    return u"variables"_s;
}

QString SieveActionSetVariable::help() const
{
    QString helpStr = i18n("The \"set\" action stores the specified value in the variable identified by name.");
    if (mHasRegexCapability) {
        helpStr += u'\n'
            + i18n("This modifier adds the necessary quoting to ensure that the expanded text will only match a literal occurrence if used as a parameter "
                   "to :regex.  Every character with special meaning (. , *, ? , etc.) is prefixed with \\ in the expansion");
    }
    return helpStr;
}

#include "moc_sieveactionsetvariable.cpp"
