/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MOVEIMAPFOLDERWIDGETTEST_H
#define MOVEIMAPFOLDERWIDGETTEST_H

#include <QObject>

class MoveImapFolderWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit MoveImapFolderWidgetTest(QObject *parent = nullptr);
    ~MoveImapFolderWidgetTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue();
};

#endif // MOVEIMAPFOLDERWIDGETTEST_H
