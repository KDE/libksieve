/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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
#include "sieveeditorparsingmissingfeaturewarning.h"
#include "autocreatescripts/sievescriptparsingerrordialog.h"

#include <KLocalizedString>
#include <QAction>

#include <QPointer>

using namespace KSieveUi;

SieveEditorParsingMissingFeatureWarning::SieveEditorParsingMissingFeatureWarning(TextEditorType type, QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Error);
    setText(i18n("Some errors were found during parsing. <a href=\"sieveerrordetails\">(Details...)</a>"));
    connect(this, &SieveEditorParsingMissingFeatureWarning::linkActivated, this, &SieveEditorParsingMissingFeatureWarning::slotShowDetails);

    switch (type) {
    case TextEditor:
    {
        QAction *action = new QAction(i18n("Switch to Graphical Mode"), this);
        connect(action, &QAction::triggered, this, &SieveEditorParsingMissingFeatureWarning::slotSwitchInGraphicalMode);
        addAction(action);

        action = new QAction(i18n("Stay in Text Mode"), this);
        connect(action, &QAction::triggered, this, &SieveEditorParsingMissingFeatureWarning::slotInActualMode);
        addAction(action);
        break;
    }
    case GraphicEditor:
    {
        QAction *action = new QAction(i18n("Switch to Text Mode"), this);
        connect(action, &QAction::triggered, this, &SieveEditorParsingMissingFeatureWarning::slotSwitchInTextMode);
        addAction(action);

        action = new QAction(i18n("Stay in Graphical Mode"), this);
        connect(action, &QAction::triggered, this, &SieveEditorParsingMissingFeatureWarning::slotInActualMode);
        addAction(action);
        break;
    }
    }

    setWordWrap(true);
}

SieveEditorParsingMissingFeatureWarning::~SieveEditorParsingMissingFeatureWarning()
{
}

QString SieveEditorParsingMissingFeatureWarning::initialScript() const
{
    return mScript;
}

void SieveEditorParsingMissingFeatureWarning::slotShowDetails(const QString &content)
{
    if (content == QLatin1String("sieveerrordetails")) {
        QPointer<SieveScriptParsingErrorDialog> dlg = new SieveScriptParsingErrorDialog(this);
        dlg->setError(mScript, mErrors);
        dlg->exec();
        delete dlg;
    }
}

void SieveEditorParsingMissingFeatureWarning::setErrors(const QString &initialScript, const QString &errors)
{
    mErrors = errors;
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
