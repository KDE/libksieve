/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieveui/editor/sieveeditor.h"

class SieveEditorTestWidget : public KSieveUi::SieveEditor
{
    Q_OBJECT
public:
    explicit SieveEditorTestWidget(QWidget *parent = nullptr);
    ~SieveEditorTestWidget() override;

private Q_SLOTS:
    void slotValueChanged(bool);
};
