/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QObject>

class XMLPrintingScriptBuilderTest : public QObject
{
    Q_OBJECT
public:
    explicit XMLPrintingScriptBuilderTest(QObject *parent = nullptr);
    ~XMLPrintingScriptBuilderTest() override = default;
private Q_SLOTS:
    void shouldCreateXml_data();
    void shouldCreateXml();
};
