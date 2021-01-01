/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVEPREVIEWGENERATEDCODEWIDGETTEST_H
#define SIEVEPREVIEWGENERATEDCODEWIDGETTEST_H

#include <QObject>

class SievePreviewGeneratedCodeWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit SievePreviewGeneratedCodeWidgetTest(QObject *parent = nullptr);
    ~SievePreviewGeneratedCodeWidgetTest() = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

#endif // SIEVEPREVIEWGENERATEDCODEWIDGETTEST_H
