/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveeditorparsingmissingfeaturewarning.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/sievescriptparsingerrordialog.h"

#include <KLocalizedString>
#include <QAction>

using namespace KSieveUi;

SieveEditorParsingMissingFeatureWarning::SieveEditorParsingMissingFeatureWarning(TextEditorType type, QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Error);
    setText(i18n("Some errors were found during parsing. <a href=\"sieveerrordetails\">(Details…)</a>"));
    connect(this, &SieveEditorParsingMissingFeatureWarning::linkActivated, this, &SieveEditorParsingMissingFeatureWarning::slotShowDetails);

    switch (type) {
    case TextEditor: {
        auto action = new QAction(i18nc("@action", "Switch to Graphical Mode"), this);
        connect(action, &QAction::triggered, this, &SieveEditorParsingMissingFeatureWarning::slotSwitchInGraphicalMode);
        addAction(action);

        action = new QAction(i18nc("@action", "Stay in Text Mode"), this);
        connect(action, &QAction::triggered, this, &SieveEditorParsingMissingFeatureWarning::slotInActualMode);
        addAction(action);
        break;
    }
    case GraphicEditor: {
        auto action = new QAction(i18nc("@action", "Switch to Text Mode"), this);
        connect(action, &QAction::triggered, this, &SieveEditorParsingMissingFeatureWarning::slotSwitchInTextMode);
        addAction(action);

        action = new QAction(i18nc("@action", "Stay in Graphical Mode"), this);
        connect(action, &QAction::triggered, this, &SieveEditorParsingMissingFeatureWarning::slotInActualMode);
        addAction(action);
        break;
    }
    }

    setWordWrap(true);
}

SieveEditorParsingMissingFeatureWarning::~SieveEditorParsingMissingFeatureWarning() = default;

QString SieveEditorParsingMissingFeatureWarning::initialScript() const
{
    return mScript;
}

void SieveEditorParsingMissingFeatureWarning::slotShowDetails(const QString &content)
{
    if (content == QLatin1StringView("sieveerrordetails")) {
        SieveScriptParsingErrorDialog dlg(this);
        dlg.setError(mScript, mirrors);
        dlg.exec();
    }
}

void SieveEditorParsingMissingFeatureWarning::setErrors(const QString &initialScript, const QString &errors)
{
    mirrors = errors;
    mScript = initialScript;
}

void SieveEditorParsingMissingFeatureWarning::slotSwitchInTextMode()
{
    Q_EMIT switchToTextMode();
    setVisible(false);
}

void SieveEditorParsingMissingFeatureWarning::slotSwitchInGraphicalMode()
{
    Q_EMIT switchToGraphicalMode();
    setVisible(false);
}

void SieveEditorParsingMissingFeatureWarning::slotInActualMode()
{
    setVisible(false);
}

#include "moc_sieveeditorparsingmissingfeaturewarning.cpp"
