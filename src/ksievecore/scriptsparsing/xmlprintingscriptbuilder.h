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
/*!
 * \class KSieveCore::XMLPrintingScriptBuilder
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/XMLPrintingScriptBuilder
 *
 * \brief The XMLPrintingScriptBuilder class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT XMLPrintingScriptBuilder : public KSieve::ScriptBuilder
{
public:
    /*!
     * Constructs an XMLPrintingScriptBuilder with default indentation.
     */
    XMLPrintingScriptBuilder();

    /*!
     * Constructs an XMLPrintingScriptBuilder with specified indentation.
     * \param indent The indentation level.
     */
    explicit XMLPrintingScriptBuilder(int indent);
    /*!
     * Destroys the XMLPrintingScriptBuilder.
     */
    ~XMLPrintingScriptBuilder() override;

    /*!
     * Processes a tagged argument in the script.
     * \param tag The tag string.
     */
    void taggedArgument(const QString &tag) override;
    /*!
     * Processes a string argument in the script.
     * \param string The string value.
     * \param multiLine true if the string spans multiple lines.
     */
    void stringArgument(const QString &string, bool multiLine, const QString & /*fixme*/) override;
    /*!
     * Processes a numeric argument in the script.
     * \param number The number value.
     * \param quantifier The unit quantifier (e.g., 'K' for kilo).
     */
    void numberArgument(unsigned long number, char quantifier) override;
    /*!
     * Marks the start of a Sieve command.
     * \param identifier The command name.
     * \param lineNumber The line number in the source script.
     */
    void commandStart(const QString &identifier, int lineNumber) override;
    /*!
     * Marks the end of a Sieve command.
     * \param lineNumber The line number in the source script.
     */
    void commandEnd(int lineNumber) override;
    /*!
     * Marks the start of a Sieve test.
     * \param identifier The test name.
     */
    void testStart(const QString &identifier) override;
    /*!
     * Marks the end of a Sieve test.
     */
    void testEnd() override;
    /*!
     * Marks the start of a test list.
     */
    void testListStart() override;
    /*!
     * Marks the end of a test list.
     */
    void testListEnd() override;
    /*!
     * Marks the start of a block.
     * \param lineNumber The line number in the source script.
     */
    void blockStart(int lineNumber) override;
    /*!
     * Marks the end of a block.
     * \param lineNumber The line number in the source script.
     */
    void blockEnd(int lineNumber) override;
    /*!
     * Marks the start of a string list argument.
     */
    void stringListArgumentStart() override;
    /*!
     * Marks the end of a string list argument.
     */
    void stringListArgumentEnd() override;
    /*!
     * Adds an entry to a string list.
     * \param string The string entry.
     * \param multiline true if the string spans multiple lines.
     * \param hashComment Associated hash comment.
     */
    void stringListEntry(const QString &string, bool multiline, const QString &hashComment) override;
    /*!
     * Processes a hash-style comment.
     * \param comment The comment text.
     */
    void hashComment(const QString &comment) override;
    /*!
     * Processes a bracket-style comment.
     * \param comment The comment text.
     */
    void bracketComment(const QString &comment) override;

    /*!
     * Adds a line feed to the output.
     */
    void lineFeed() override;
    /*!
     * Reports an error encountered during parsing.
     * \param error The error information.
     */
    void error(const KSieve::Error &error) override;
    /*!
     * Marks the end of script processing.
     */
    void finished() override;

    /*!
     * Returns the generated XML output.
     * \return The result as an XML string.
     */
    [[nodiscard]] QString result() const;
    /*!
     * Returns any error message generated.
     * \return The error string.
     */
    [[nodiscard]] QString error() const;
    /*!
     */
    [[nodiscard]] bool hasError() const;

    /*!
     */
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
