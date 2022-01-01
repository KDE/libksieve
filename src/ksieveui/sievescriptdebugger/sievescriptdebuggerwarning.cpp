/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdebuggerwarning.h"

using namespace KSieveUi;

SieveScriptDebuggerWarning::SieveScriptDebuggerWarning(QWidget *parent)
    : KMessageWidget(parent)
{
    setCloseButtonVisible(true);
    hide();
}

SieveScriptDebuggerWarning::~SieveScriptDebuggerWarning() = default;

void SieveScriptDebuggerWarning::setErrorMessage(const QString &msg)
{
    setMessageType(KMessageWidget::Error);
    setText(msg);
    animatedShow();
}

void SieveScriptDebuggerWarning::setWarningMessage(const QString &msg)
{
    setMessageType(KMessageWidget::Warning);
    setText(msg);
    animatedShow();
}
