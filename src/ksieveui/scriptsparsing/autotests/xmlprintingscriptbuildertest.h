/*
   SPDX-FileCopyrightText: 2017 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef XMLPRINTINGSCRIPTBUILDERTEST_H
#define XMLPRINTINGSCRIPTBUILDERTEST_H

#include <QObject>

class XMLPrintingScriptBuilderTest : public QObject
{
    Q_OBJECT
public:
    explicit XMLPrintingScriptBuilderTest(QObject *parent = nullptr);
    ~XMLPrintingScriptBuilderTest() = default;
private Q_SLOTS:
    void shouldCreateXml_data();
    void shouldCreateXml();
};

#endif // XMLPRINTINGSCRIPTBUILDERTEST_H
