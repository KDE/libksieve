/*
  SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef KSIEVE_EDITORSIEVEGUI_TEST_H
#define KSIEVE_EDITORSIEVEGUI_TEST_H

#include "ksieveui/editor/sieveeditor.h"

class SieveEditorTestWidget : public KSieveUi::SieveEditor
{
    Q_OBJECT
public:
    explicit SieveEditorTestWidget(QWidget *parent = nullptr);
    ~SieveEditorTestWidget();

private Q_SLOTS:
    void slotValueChanged(bool);
};

#endif
