/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "xmlprintingscriptbuilder.h"
using namespace Qt::Literals::StringLiterals;

#include "parser.h"

#include "error.h"
#include <QXmlStreamWriter>

using namespace KSieveCore;
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
    mStream->writeStartElement(u"script"_s);
}

XMLPrintingScriptBuilder::~XMLPrintingScriptBuilder()
{
    delete mStream;
}

void XMLPrintingScriptBuilder::taggedArgument(const QString &tag)
{
    write(u"tag"_s, tag);
}

void XMLPrintingScriptBuilder::stringArgument(const QString &string, bool multiLine, const QString & /*fixme*/)
{
    if (multiLine) {
        write(u"str"_s, u"type"_s, QStringLiteral("multiline"), string);
    } else {
        write(u"str"_s, u"type"_s, QStringLiteral("quoted"), string);
    }
}

void XMLPrintingScriptBuilder::numberArgument(unsigned long number, char quantifier)
{
    if (quantifier) {
        write(u"num"_s, u"quantifier"_s, QStringLiteral("%1").arg(quantifier), QString::number(number));
    } else {
        write(u"num"_s, QString(), QString(), QString::number(number));
    }
}

void XMLPrintingScriptBuilder::commandStart(const QString &identifier, int lineNumber)
{
    Q_UNUSED(lineNumber)
    if (identifier == QLatin1StringView("else") || identifier == QLatin1StringView("break") || identifier == QLatin1StringView("require")
        || identifier == QLatin1StringView("foreverypart") || identifier == QLatin1StringView("if") || identifier == QLatin1StringView("elsif")) {
        mStream->writeStartElement(u"control"_s);
        mStream->writeAttribute(u"name"_s, identifier);
    } else {
        mStream->writeStartElement(u"action"_s);
        mStream->writeAttribute(u"name"_s, identifier);
    }
}

void XMLPrintingScriptBuilder::commandEnd(int lineNumber)
{
    Q_UNUSED(lineNumber)
    mStream->writeEndElement();
}

void XMLPrintingScriptBuilder::testStart(const QString &identifier)
{
    mStream->writeStartElement(u"test"_s);
    mStream->writeAttribute(u"name"_s, identifier);
}

void XMLPrintingScriptBuilder::testEnd()
{
    mStream->writeEndElement();
}

void XMLPrintingScriptBuilder::testListStart()
{
    mStream->writeStartElement(u"testlist"_s);
}

void XMLPrintingScriptBuilder::testListEnd()
{
    mStream->writeEndElement();
}

void XMLPrintingScriptBuilder::blockStart(int lineNumber)
{
    Q_UNUSED(lineNumber)
    mStream->writeStartElement(u"block"_s);
}

void XMLPrintingScriptBuilder::blockEnd(int lineNumber)
{
    Q_UNUSED(lineNumber)
    mStream->writeEndElement();
}

void XMLPrintingScriptBuilder::stringListArgumentStart()
{
    mStream->writeStartElement(u"list"_s);
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
    write(u"comment"_s, u"type"_s, QStringLiteral("hash"), comment);
}

void XMLPrintingScriptBuilder::bracketComment(const QString &comment)
{
    write(u"comment"_s, u"type"_s, QStringLiteral("bracket"), comment);
}

void XMLPrintingScriptBuilder::lineFeed()
{
    mStream->writeEmptyElement(u"crlf"_s);
}

void XMLPrintingScriptBuilder::error(const KSieve::Error &error)
{
    mError = QLatin1StringView("Error: ") + error.asString();
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
