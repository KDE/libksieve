/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVESCRIPTDEBUGGERWARNING_H
#define SIEVESCRIPTDEBUGGERWARNING_H

#include <KMessageWidget>
#include "ksieveui_private_export.h"
namespace KSieveUi {
class KSIEVEUI_TESTS_EXPORT SieveScriptDebuggerWarning : public KMessageWidget
{
    Q_OBJECT
public:
    explicit SieveScriptDebuggerWarning(QWidget *parent = nullptr);
    ~SieveScriptDebuggerWarning();

    void setErrorMessage(const QString &msg);
    void setWarningMessage(const QString &msg);
};
}
#endif // SIEVESCRIPTDEBUGGERWARNING_H
