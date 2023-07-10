/*  -*- c++ -*-
    impl/parser.h

    Internal header file. Subject to change without notice. DO NOT USE.

    This file is part of KSieve,
    the KDE internet mail/usenet news message filtering library.
    SPDX-FileCopyrightText: 2003 Marc Mutz <mutz@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "parser.h"

#include "error.h"
#include "lexer.h"
#include "lexer_p.h"

#include "scriptbuilder.h"

namespace KSieve
{
class Parser::Impl
{
    friend class Parser;

private:
    Impl(const char *scursor, const char *const send, int options = 0);

    void setScriptBuilder(ScriptBuilder *builder)
    {
        mBuilder = builder;
    }

    ScriptBuilder *scriptBuilder() const
    {
        return mBuilder;
    }

    bool parse();

    const Error &error() const
    {
        return mError == Error::None ? lexer.error() : mError;
    }

    bool parseCommandList();

    bool parseCommand();

    bool parseArgumentList();

    bool parseArgument();

    bool parseTestList();

    bool parseTest();

    bool parseBlock();

    bool parseStringList();

    bool parseNumber();

    Lexer::Token token() const
    {
        return mToken;
    }

    QString tokenValue() const
    {
        return mTokenValue;
    }

    bool atEnd() const
    {
        return !mToken && lexer.atEnd();
    }

    bool obtainToken();
    void consumeToken()
    {
        mToken = Lexer::None;
        mTokenValue.clear();
    }

    void makeError(Error::Type e, int line, int col)
    {
        mError = Error(e, line, col);
        if (scriptBuilder()) {
            scriptBuilder()->error(mError);
        }
    }

    void makeError(Error::Type e)
    {
        makeError(e, lexer.line(), lexer.column());
    }

    void makeUnexpectedTokenError(Error::Type e)
    {
        makeError(e); // ### save wrong token...
    }

    bool isArgumentToken() const;
    bool isStringToken() const;

    Error mError;
    Lexer::Token mToken;
    QString mTokenValue;
    Lexer::Impl lexer;
    ScriptBuilder *mBuilder = nullptr;
};
}
