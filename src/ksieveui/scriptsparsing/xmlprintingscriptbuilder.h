/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KSIEVE_KSIEVEUI_XMLPRINTINGSCRIPTBUILDER_H
#define KSIEVE_KSIEVEUI_XMLPRINTINGSCRIPTBUILDER_H

#include "ksieveui_private_export.h"
#include <ksieve/scriptbuilder.h>
#include <QString>
class QXmlStreamWriter;
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT XMLPrintingScriptBuilder : public KSieve::ScriptBuilder
{
public:
    XMLPrintingScriptBuilder();
    XMLPrintingScriptBuilder(int indent);
    ~XMLPrintingScriptBuilder();

    void taggedArgument(const QString &tag) override;
    void stringArgument(const QString &string, bool multiLine, const QString & /*fixme*/) override;
    void numberArgument(unsigned long number, char quantifier) override;
    void commandStart(const QString &identifier, int lineNumber) override;
    void commandEnd(int lineNumber) override;
    void testStart(const QString &identifier) override;
    void testEnd() override;
    void testListStart() override;
    void testListEnd() override;
    void blockStart(int lineNumber) override;
    void blockEnd(int lineNumber) override;
    void stringListArgumentStart() override;
    void stringListArgumentEnd() override;
    void stringListEntry(const QString &string, bool multiline, const QString &hashComment) override;
    void hashComment(const QString &comment) override;
    void bracketComment(const QString &comment) override;

    void lineFeed() override;
    void error(const KSieve::Error &error) override;
    void finished() override;

    QString result() const;
    QString error() const;
    bool hasError() const;

    void clear();

private:
    void initialize(int indent = 0);
    void write(const QString &key, const QString &value);
    void write(const QString &key, const QString &qualifiedName, const QString &attribute, const QString &value);

    QXmlStreamWriter *mStream;
    QString mResult;
    QString mError;
};
}
#endif
