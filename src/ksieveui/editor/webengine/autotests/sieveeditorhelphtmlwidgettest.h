/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVEEDITORHELPHTMLWIDGETTEST_H
#define SIEVEEDITORHELPHTMLWIDGETTEST_H

#include <QObject>

class SieveEditorHelpHtmlWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit SieveEditorHelpHtmlWidgetTest(QObject *parent = nullptr);
    ~SieveEditorHelpHtmlWidgetTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // SIEVEEDITORHELPHTMLWIDGETTEST_H
