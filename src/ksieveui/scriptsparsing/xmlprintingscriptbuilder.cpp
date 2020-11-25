/*
   SPDX-FileCopyrightText: 2013-2017 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "xmlprintingscriptbuilder.h"
#include <ksieve/parser.h>
using KSieve::Parser;

#include <ksieve/error.h>
#include "libksieve_debug.h"
#include <QXmlStreamWriter>

using namespace KSieveUi;
XMLPrintingScriptBuilder::XMLPrintingScriptBuilder(int indent)
    : KSieve::ScriptBuilder()
{
    initialize(indent);
}

XMLPrintingScriptBuilder::XMLPrintingScriptBuilder()
    : KSieve::ScriptBuilder()
{
    initialize();
}

void XMLPrintingScriptBuilder::initialize(int indent)
{
    mStream = new QXmlStreamWriter(&mResult);
    if (indent == 0) {
        mStream->setAutoFormatting(false);
    } else {
        mStream->setAutoFormatting(true);
        mStream->setAutoFormattingIndent(indent);
    }
    mStream->writeStartDocument();
    mStream->writeStartElement(QStringLiteral("script"));
}

XMLPrintingScriptBuilder::~XMLPrintingScriptBuilder()
{
    delete mStream;
}

void XMLPrintingScriptBuilder::taggedArgument(const QString &tag)
{
    write(QStringLiteral("tag"), tag);
}

void XMLPrintingScriptBuilder::stringArgument(const QString &string, bool multiLine, const QString & /*fixme*/)
{
    if (multiLine) {
        write(QStringLiteral("str"), QStringLiteral("type"), QStringLiteral("multiline"), string);
    } else {
        write(QStringLiteral("str"), QStringLiteral("type"), QStringLiteral("quoted"), string);
    }
}

void XMLPrintingScriptBuilder::numberArgument(unsigned long number, char quantifier)
{
    if (quantifier) {
        write(QStringLiteral("num"), QStringLiteral("quantifier"), QStringLiteral("%1").arg(quantifier), QString::number(number));
    } else {
        write(QStringLiteral("num"), QString(), QString(), QString::number(number));
    }
}

void XMLPrintingScriptBuilder::commandStart(const QString &identifier, int lineNumber)
{
    Q_UNUSED(lineNumber)
    if (identifier == QLatin1String("else") ||
            identifier == QLatin1String("break") ||
            identifier == QLatin1String("require") ||
            identifier == QLatin1String("foreverypart") ||
            identifier == QLatin1String("if") ||
            identifier == QLatin1String("elsif")) {
        mStream->writeStartElement(QStringLiteral("control"));
        mStream->writeAttribute(QStringLiteral("name"), identifier);
    } else {
        mStream->writeStartElement(QStringLiteral("action"));
        mStream->writeAttribute(QStringLiteral("name"), identifier);
    }
}

void XMLPrintingScriptBuilder::commandEnd(int lineNumber)
{
    Q_UNUSED(lineNumber)
    mStream->writeEndElement();
}

void XMLPrintingScriptBuilder::testStart(const QString &identifier)
{
    mStream->writeStartElement(QStringLiteral("test"));
    mStream->writeAttribute(QStringLiteral("name"), identifier);
}

void XMLPrintingScriptBuilder::testEnd()
{
    mStream->writeEndElement();
}

void XMLPrintingScriptBuilder::testListStart()
{
    mStream->writeStartElement(QStringLiteral("testlist"));
}

void XMLPrintingScriptBuilder::testListEnd()
{
    mStream->writeEndElement();
}

void XMLPrintingScriptBuilder::blockStart(int lineNumber)
{
    Q_UNUSED(lineNumber)
    mStream->writeStartElement(QStringLiteral("block"));
}

void XMLPrintingScriptBuilder::blockEnd(int lineNumber)
{
    Q_UNUSED(lineNumber)
    mStream->writeEndElement();
}

void XMLPrintingScriptBuilder::stringListArgumentStart()
{
    mStream->writeStartElement(QStringLiteral("list"));
}

void XMLPrintingScriptBuilder::stringListArgumentEnd()
{
    mStream->writeEndElement();
}

void XMLPrintingScriptBuilder::stringListEntry(const QString &string, bool multiline, const QString &hashComment)
{
    stringArgument(string, multiline, hashComment);
}

void XMLPrintingScriptBuilder::hashComment(const QString &comment)
{
    write(QStringLiteral("comment"), QStringLiteral("type"), QStringLiteral("hash"), comment);
}

void XMLPrintingScriptBuilder::bracketComment(const QString &comment)
{
    write(QStringLiteral("comment"), QStringLiteral("type"), QStringLiteral("bracket"), comment);
}

void XMLPrintingScriptBuilder::lineFeed()
{
    mStream->writeEmptyElement(QStringLiteral("crlf"));
}

void XMLPrintingScriptBuilder::error(const KSieve::Error &error)
{
    mError = QLatin1String("Error: ") + error.asString();
    mResult += mError;
}

void XMLPrintingScriptBuilder::finished()
{
    mStream->writeEndElement();
}

void XMLPrintingScriptBuilder::write(const QString &key, const QString &value)
{
    if (value.isEmpty()) {
        mStream->writeEmptyElement(key);
        return;
    }
    mStream->writeStartElement(key);
    mStream->writeCharacters(value);
    mStream->writeEndElement();
}

void XMLPrintingScriptBuilder::write(const QString &key, const QString &qualifiedName, const QString &attribute, const QString &value)
{
    if (value.isEmpty()) {
        mStream->writeEmptyElement(key);
        return;
    }
    if (attribute.isEmpty()) {
        mStream->writeStartElement(key);
    } else {
        mStream->writeStartElement(key);
        mStream->writeAttribute(qualifiedName, attribute);
    }
    mStream->writeCharacters(value);
    mStream->writeEndElement();
}

QString XMLPrintingScriptBuilder::result() const
{
    return mResult;
}

QString XMLPrintingScriptBuilder::error() const
{
    return mError;
}

bool XMLPrintingScriptBuilder::hasError() const
{
    return !mError.isEmpty();
}

void XMLPrintingScriptBuilder::clear()
{
    mResult.clear();
    mError.clear();
}
