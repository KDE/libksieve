/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <editor/sievetextedit.h>

namespace KSieveUi
{
class SieveScriptDebuggerTextEdit : public KSieveUi::SieveTextEdit
{
    Q_OBJECT
public:
    explicit SieveScriptDebuggerTextEdit(QWidget *parent = nullptr);
    ~SieveScriptDebuggerTextEdit() override;

protected:
    void addExtraMenuEntry(QMenu *menu, QPoint pos) override;

private:
    void slotAddDebug();
};
}
