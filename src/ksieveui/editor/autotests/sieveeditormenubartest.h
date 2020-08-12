/*
  SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef SIEVEEDITORMENUBARTEST_H
#define SIEVEEDITORMENUBARTEST_H

#include <QObject>

class SieveEditorMenuBarTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveEditorMenuBarTest(QObject *parent = nullptr);
    ~SieveEditorMenuBarTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldEmitSignals();
    void shouldDisableActions();
};

#endif // SIEVEEDITORMENUBARTEST_H
