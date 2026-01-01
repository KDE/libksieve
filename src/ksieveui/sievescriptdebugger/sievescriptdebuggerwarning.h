/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include <KMessageWidget>
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT SieveScriptDebuggerWarning : public KMessageWidget
{
    Q_OBJECT
public:
    explicit SieveScriptDebuggerWarning(QWidget *parent = nullptr);
    ~SieveScriptDebuggerWarning() override;

    void setErrorMessage(const QString &msg);
    void setWarningMessage(const QString &msg);
};
}
