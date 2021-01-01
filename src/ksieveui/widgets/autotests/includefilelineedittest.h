/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef INCLUDEFILELINEEDITTEST_H
#define INCLUDEFILELINEEDITTEST_H

#include <QObject>

class IncludeFileLineEditTest : public QObject
{
    Q_OBJECT
public:
    explicit IncludeFileLineEditTest(QObject *parent = nullptr);
    ~IncludeFileLineEditTest() = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // INCLUDEFILELINEEDITTEST_H
