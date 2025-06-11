/*  -*- c++ -*-
    ksieve/interfaces/scriptbuilder.h

    This file is part of KSieve,
    the KDE internet mail/usenet news message filtering library.
    SPDX-FileCopyrightText: 2002-2003 Marc Mutz <mutz@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/
#pragma once

class QString;

namespace KSieve
{
class Error;

class ScriptBuilder
{
public:
    virtual ~ScriptBuilder() = default;

    virtual void taggedArgument(const QString &tag) = 0;
    virtual void stringArgument(const QString &string, bool multiLine, const QString &embeddedHashComment) = 0;
    virtual void numberArgument(unsigned long number, char quantifier) = 0;

    virtual void stringListArgumentStart() = 0;
    virtual void stringListEntry(const QString &string, bool multiLine, const QString &embeddedHashComment) = 0;
    virtual void stringListArgumentEnd() = 0;

    virtual void commandStart(const QString &identifier, int lineNumber) = 0;
    virtual void commandEnd(int lineNumber) = 0;

    virtual void testStart(const QString &identifier) = 0;
    virtual void testEnd() = 0;

    virtual void testListStart() = 0;
    virtual void testListEnd() = 0;

    virtual void blockStart(int lineNumber) = 0;
    virtual void blockEnd(int lineNumber) = 0;

    /** A hash comment always includes an implicit lineFeed() at it's end. */
    virtual void hashComment(const QString &comment) = 0;
    /** Bracket comments include explicit lineFeed()s in their content */
    virtual void bracketComment(const QString &comment) = 0;

    virtual void lineFeed() = 0;

    virtual void error(const Error &error) = 0;

    virtual void finished() = 0;
};

} // namespace KSieve
