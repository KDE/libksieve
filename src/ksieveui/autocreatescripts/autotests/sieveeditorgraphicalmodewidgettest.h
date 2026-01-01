/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SieveEditorGraphicalModeWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveEditorGraphicalModeWidgetTest(QObject *parent = nullptr);
    ~SieveEditorGraphicalModeWidgetTest() override = default;
private Q_SLOTS:
    void shouldLoadScripts();
    void shouldLoadScripts_data();
};
