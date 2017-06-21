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

#include "xmlprintingscriptbuilder.h"
#include <ksieve/parser.h>
using KSieve::Parser;

#include <ksieve/error.h>
#include "libksieve_debug.h"
#include <QXmlStreamWriter>
#include <QDebug>

using namespace KSieveUi;
XMLPrintingScriptBuilder::XMLPrintingScriptBuilder(int indent)
    : KSieve::ScriptBuilder(),
      mStream(nullptr)
{
    initialize(indent);
}

XMLPrintingScriptBuilder::XMLPrintingScriptBuilder()
    : KSieve::ScriptBuilder(),
      mStream(nullptr)
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
    Q_UNUSED(lineNumber);
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
    Q_UNUSED(lineNumber);
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
    Q_UNUSED(lineNumber);
    mStream->writeStartElement(QStringLiteral("block"));
}

void XMLPrintingScriptBuilder::blockEnd(int lineNumber)
{
    Q_UNUSED(lineNumber);
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
    mError = QStringLiteral("Error: ") + error.asString();
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

QDomDocument XMLPrintingScriptBuilder::toDom() const
{
    QString errorMsg;
    int errorRow;
    int errorCol;
    QDomDocument doc;
    if (!doc.setContent(mResult, &errorMsg, &errorRow, &errorCol)) {
        qCDebug(LIBKSIEVE_LOG) << "Unable to load document.Parse error in line " << errorRow
                               << ", col " << errorCol << ": " << errorMsg;
        qCDebug(LIBKSIEVE_LOG) << " mResult" << mResult;

    }
    return doc;
}
