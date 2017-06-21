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

//#define USE_QXMLSTREAMWRITER 1
using namespace KSieveUi;
XMLPrintingScriptBuilder::XMLPrintingScriptBuilder()
    : KSieve::ScriptBuilder(),
      mStream(nullptr),
      mIsAction(false)
{
#ifdef USE_QXMLSTREAMWRITER
    mStream = new QXmlStreamWriter(&mResult);
    mStream->setAutoFormatting(false);
    mStream->writeStartDocument();
    mStream->writeStartElement(QStringLiteral("script"));
#else
    write(QStringLiteral("<?xml version='1.0'?>"));
    write(QStringLiteral("<script>"));
#endif
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
#ifdef USE_QXMLSTREAMWRITER
        mStream->writeStartElement(QStringLiteral("control"));
        mStream->writeAttribute(QStringLiteral("name"), identifier);
#else
        write(QStringLiteral("<control name=\"%1\">").arg(identifier));
#endif
        mIsAction = false;
    } else {
#ifdef USE_QXMLSTREAMWRITER
        mStream->writeStartElement(QStringLiteral("action"));
        mStream->writeAttribute(QStringLiteral("name"), identifier);
#else
        write(QStringLiteral("<action name=\"%1\">").arg(identifier));
#endif
        mIsAction = true;
    }
}

void XMLPrintingScriptBuilder::commandEnd(int lineNumber)
{
    Q_UNUSED(lineNumber);
#ifdef USE_QXMLSTREAMWRITER
    mStream->writeEndElement();
#else
    if (mIsAction) {
        write(QStringLiteral("</action>"));
    } else {
        write(QStringLiteral("</control>"));
    }
#endif
    mIsAction = false;
}

void XMLPrintingScriptBuilder::testStart(const QString &identifier)
{
#ifdef USE_QXMLSTREAMWRITER
    mStream->writeStartElement(QStringLiteral("test"));
    mStream->writeAttribute(QStringLiteral("name"), identifier);
#else
    write(QStringLiteral("<test name=\"%1\">").arg(identifier));
#endif
}

void XMLPrintingScriptBuilder::testEnd()
{
#ifdef USE_QXMLSTREAMWRITER
    mStream->writeEndElement();
#else
    write(QStringLiteral("</test>"));
#endif
}

void XMLPrintingScriptBuilder::testListStart()
{
#ifdef USE_QXMLSTREAMWRITER
    mStream->writeStartElement(QStringLiteral("testlist"));
#else
    write(QStringLiteral("<testlist>"));
#endif
}

void XMLPrintingScriptBuilder::testListEnd()
{
#ifdef USE_QXMLSTREAMWRITER
    mStream->writeEndElement();
#else
    write(QStringLiteral("</testlist>"));
#endif
}

void XMLPrintingScriptBuilder::blockStart(int lineNumber)
{
    Q_UNUSED(lineNumber);
#ifdef USE_QXMLSTREAMWRITER
    mStream->writeStartElement(QStringLiteral("block"));
#else
    write(QStringLiteral("<block>"));
#endif
}

void XMLPrintingScriptBuilder::blockEnd(int lineNumber)
{
    Q_UNUSED(lineNumber);
#ifdef USE_QXMLSTREAMWRITER
    mStream->writeEndElement();
#else
    write(QStringLiteral("</block>"));
#endif
}

void XMLPrintingScriptBuilder::stringListArgumentStart()
{
#ifdef USE_QXMLSTREAMWRITER
    mStream->writeStartElement(QStringLiteral("list"));
#else
    write(QStringLiteral("<list>"));
#endif
}

void XMLPrintingScriptBuilder::stringListArgumentEnd()
{
#ifdef USE_QXMLSTREAMWRITER
    mStream->writeEndElement();
#else
    write(QStringLiteral("</list>"));
#endif
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
#ifdef USE_QXMLSTREAMWRITER
    mStream->writeEmptyElement(QStringLiteral("crlf"));
#else
    write(QStringLiteral("<crlf/>"));
#endif
}

void XMLPrintingScriptBuilder::error(const KSieve::Error &error)
{
    mError = QStringLiteral("Error: ") + error.asString();
    write(mError);
}

void XMLPrintingScriptBuilder::finished()
{
#ifdef USE_QXMLSTREAMWRITER
    mStream->writeEndElement();
#else
    write(QStringLiteral("</script>"));
#endif
}

void XMLPrintingScriptBuilder::write(const QString &msg)
{
    mResult += msg;
}

void XMLPrintingScriptBuilder::write(const QString &key, const QString &value)
{
    if (value.isEmpty()) {
#ifdef  USE_QXMLSTREAMWRITER
        mStream->writeEmptyElement(key);
#else
        write(QStringLiteral("<%1>").arg(key));
#endif
        return;
    }
#ifdef USE_QXMLSTREAMWRITER
    mStream->writeStartElement(key);
    //mStream->writeAttribute(value);
    mStream->writeEndElement();
#else
    write(QStringLiteral("<%1>").arg(key));
    write(value);
    write(QStringLiteral("</%1>").arg(key));
#endif
}

void XMLPrintingScriptBuilder::write(const QString &key, const QString &qualifiedName, const QString &attribute, const QString &value)
{
    if (value.isEmpty()) {
#ifdef USE_QXMLSTREAMWRITER
        mStream->writeEmptyElement(key);
#else
        write(QStringLiteral("<%1/>").arg(key));
#endif
        return;
    }
#ifdef USE_QXMLSTREAMWRITER
    if (attribute.isEmpty()) {
        mStream->writeStartElement(key);
    } else {
        mStream->writeStartElement(key);
        //TODO mStream->writeAttribute();
    }
    mStream->writeCharacters(value);
#else
    if (attribute.isEmpty()) {
        write(QStringLiteral("<%1>").arg(key));
    } else {
        write(QStringLiteral("<%1 %2=\"%3\">").arg(key, qualifiedName, attribute));
    }
    QString tmpValue = value;
    tmpValue.replace(QLatin1Char('<'), QLatin1String("&lt;"));
    tmpValue.replace(QLatin1Char('>'), QLatin1String("&gt;"));
    write(tmpValue);
    write(QStringLiteral("</%1>").arg(key));
#endif
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
