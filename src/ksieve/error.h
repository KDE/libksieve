/*  -*- c++ -*-
    ksieve/error.h

    This file is part of KSieve,
    the KDE internet mail/usenet news message filtering library.
    SPDX-FileCopyrightText: 2002-2003 Marc Mutz <mutz@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieve_export.h"

#include <QString>

#ifdef None // X headers
#undef None
#endif

namespace KSieve
{
class KSIEVE_EXPORT Error
{
public:
    enum Type {
        None = 0,
        Custom,
        // parse (well-formedness in XML speak) errors:
        FirstParseError,

        CRWithoutLF = FirstParseError,
        SlashWithoutAsterisk,
        IllegalCharacter,
        UnexpectedCharacter,
        NoLeadingDigits,
        NonCWSAfterTextColon,

        NumberOutOfRange,
        InvalidUTF8,

        UnfinishedBracketComment,
        PrematureEndOfMultiLine,
        PrematureEndOfQuotedString,
        PrematureEndOfStringList,
        PrematureEndOfTestList,
        PrematureEndOfBlock,
        MissingWhitespace,
        MissingSemicolonOrBlock,

        ExpectedBlockOrSemicolon,
        ExpectedCommand,
        ConsecutiveCommasInStringList,
        ConsecutiveCommasInTestList,
        MissingCommaInTestList,
        MissingCommaInStringList,
        NonStringInStringList,
        NonCommandInCommandList,
        NonTestInTestList,
        LastParseError = NonTestInTestList,
        // validity errors:
        FirstValidityError,
        RequireNotFirst = FirstValidityError, // rfc3028, 3.2
        RequireMissingForCommand,
        RequireMissingForTest,
        RequireMissingForComparator,
        UnsupportedCommand,
        UnsupportedTest,
        UnsupportedComparator,
        TestNestingTooDeep, // site policy
        BlockNestingTooDeep, // site policy
        InvalidArgument,
        ConflictingArguments, // e.g. rfc3028, 2.7.{1,3}
        ArgumentsRepeated, // similar to ConflictingArguments, e.g. :is :is
        CommandOrderingConstraintViolation, // e.g. else w/o if, rfc3028, 3.1
        LastValidityError = CommandOrderingConstraintViolation,
        // runtime errors:
        FirstRuntimeError,
        IncompatibleActionsRequested = FirstRuntimeError,
        MailLoopDetected,
        TooManyActions,
        LastRuntimeError = TooManyActions
    };

    static const char *typeToString(Type type);

    Error(Type type = None, const QString &s1 = QString(), const QString &s2 = QString(), int line = -1, int col = -1)
        : mType(type)
        , mLine(line)
        , mCol(col)
        , mStringOne(s1)
        , mStringTwo(s2)
    {
    }

    Error(Type type, int line, int col)
        : mType(type)
        , mLine(line)
        , mCol(col)
    {
    }

    QString asString() const;

    /** So you can write <pre>if ( error() )</pre> with e.g. @ref Lexer */
    operator bool() const
    {
        return type() != None;
    }

    Type type() const
    {
        return mType;
    }

    int line() const
    {
        return mLine;
    }

    int column() const
    {
        return mCol;
    }

    QString firstString() const
    {
        return mStringOne;
    }

    QString secondString() const
    {
        return mStringTwo;
    }

protected:
    Type mType;
    int mLine;
    int mCol;
    QString mStringOne, mStringTwo;
};
} // namespace KSieve
