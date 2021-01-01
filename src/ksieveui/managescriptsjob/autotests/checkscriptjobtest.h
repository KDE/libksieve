/*
  SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef CHECKSCRIPTJOBTEST_H
#define CHECKSCRIPTJOBTEST_H

#include <QObject>

class CheckScriptJobTest : public QObject
{
    Q_OBJECT
public:
    explicit CheckScriptJobTest(QObject *parent = nullptr);
    ~CheckScriptJobTest();

private Q_SLOTS:
    void canBeStarted();
};

#endif // CHECKSCRIPTJOBTEST_H
