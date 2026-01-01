/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "ksievecore_export.h"
#include "scriptbuilder.h"
#include <QString>
class QXmlStreamWriter;
namespace KSieveCore
{
class KSIEVECORE_EXPORT XMLPrintingScriptBuilder : public KSieve::ScriptBuilder
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

    [[nodiscard]] QString result() const;
    [[nodiscard]] QString error() const;
    [[nodiscard]] bool hasError() const;

    void clear();

private:
    KSIEVECORE_NO_EXPORT void initialize(int indent = 0);
    KSIEVECORE_NO_EXPORT void write(const QString &key, const QString &value);
    KSIEVECORE_NO_EXPORT void write(const QString &key, const QString &qualifiedName, const QString &attribute, const QString &value);

    QXmlStreamWriter *mStream = nullptr;
    QString mResult;
    QString mError;
};
}
