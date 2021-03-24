/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <KPIMTextEdit/PlainTextEditor>

namespace KSieveUi
{
class SieveScriptDebuggerResultEditor : public KPIMTextEdit::PlainTextEditor
{
    Q_OBJECT
public:
    explicit SieveScriptDebuggerResultEditor(QWidget *parent = nullptr);
    ~SieveScriptDebuggerResultEditor() override;

protected:
    void addExtraMenuEntry(QMenu *menu, QPoint pos) override;

private:
    void slotClear();
    void slotSaveAs();
};
}
