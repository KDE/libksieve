/*
   SPDX-FileCopyrightText: 2013-2017 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KSIEVE_KSIEVEUI_XMLPRINTINGSCRIPTBUILDER_H
#define KSIEVE_KSIEVEUI_XMLPRINTINGSCRIPTBUILDER_H

#include "ksieveui_private_export.h"
#include <QString>
#include <ksieve/scriptbuilder.h>
class QXmlStreamWriter;
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT XMLPrintingScriptBuilder : public KSieve::ScriptBuilder
{
public:
    XMLPrintingScriptBuilder();
    XMLPrintingScriptBuilder(int indent);
    ~XMLPrintingScriptBuilder() override;

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

    Q_REQUIRED_RESULT QString result() const;
    Q_REQUIRED_RESULT QString error() const;
    Q_REQUIRED_RESULT bool hasError() const;

    void clear();

private:
    void initialize(int indent = 0);
    void write(const QString &key, const QString &value);
    void write(const QString &key, const QString &qualifiedName, const QString &attribute, const QString &value);

    QXmlStreamWriter *mStream = nullptr;
    QString mResult;
    QString mError;
};
}
#endif
