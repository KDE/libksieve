/*  -*- c++ -*-
    impl/lexer.h

    Internal header file. Subject to change without notice. DO NOT USE.

    This file is part of KSieve,
    the KDE internet mail/usenet news message filtering library.
    SPDX-FileCopyrightText: 2003 Marc Mutz <mutz@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <ksieve/error.h>
#include <ksieve/lexer.h>

#include <QStack>

namespace KSieve
{
class Lexer::Impl
{
public:
    Impl(const char *scursor, const char *send, int options);

    bool ignoreComments() const
    {
        return mIgnoreComments;
    }

    bool ignoreLineFeeds() const
    {
        return mIgnoreLF;
    }

    const Error &error() const
    {
        return mState.error;
    }

    bool atEnd() const
    {
        return mState.cursor >= mEnd;
    }

    int column() const
    {
        return mState.cursor - mState.beginOfLine;
    }

    int line() const
    {
        return mState.line;
    }

    void save()
    {
        mStateStack.push(mState);
    }

    void restore()
    {
        mState = mStateStack.pop();
    }

    Lexer::Token nextToken(QString &tokenValue);

private:
    /** Cursor must be positioned on the \r or the \n. */
    bool eatCRLF();

    /** Cursor must be positioned after the opening hash (#). If
        parsing is successful, cursor is positioned behind the CRLF
        that ended the comment's line (or past the end). */
    bool parseHashComment(QString &result, bool reallySave = false);

    /** Cursor must be positioned after the opening slash-asterisk */
    bool parseBracketComment(QString &result, bool reallySave = false);

    /** Cursor must be positioned on the opening '/'or '#' */
    bool parseComment(QString &result, bool reallySave = false);

    /** Eats whitespace, but not comments */
    bool eatWS();

    /** Eats comments and whitespace */
    bool eatCWS();

    /** Cursor must be positioned on the first character */
    bool parseIdentifier(QString &result);

    /** Cursor must be positioned after the initial ':' */
    bool parseTag(QString &result);

    /** Cursor must be positioned on the first digit */
    bool parseNumber(QString &result);

    /** Cursor must be positioned after the "text:" token. */
    bool parseMultiLine(QString &result);

    /** Cursor must be positioned after the initial " */
    bool parseQuotedString(QString &result);

    struct State {
        State(const char *s = nullptr)
            : cursor(s)
            , beginOfLine(s)
            , error()
        {
        }

        const char *cursor;
        int line = 0;
        const char *beginOfLine;
        Error error;
    } mState;

    const char *const mEnd;
    const bool mIgnoreComments : 1;
    const bool mIgnoreLF : 1;
    QStack<State> mStateStack;

    const char *beginOfLine() const
    {
        return mState.beginOfLine;
    }

    int _strnicmp(const char *left, const char *right, size_t len) const
    {
        return charsLeft() >= len ? qstrnicmp(left, right, len) : 1;
    }

    void clearErrors()
    {
        mState.error = Error();
    }

    unsigned int charsLeft() const
    {
        return mEnd - mState.cursor < 0 ? 0 : mEnd - mState.cursor;
    }

    void makeError(Error::Type e)
    {
        makeError(e, line(), column());
    }

    void makeError(Error::Type e, int errorLine, int errorCol)
    {
        mState.error = Error(e, errorLine, errorCol);
    }

    void makeIllegalCharError(char ch);
    void makeIllegalCharError()
    {
        makeIllegalCharError(*mState.cursor);
    }

    /** Defines the current char to end a line.
        Warning: increases @p mCursor!
    **/
    void newLine()
    {
        ++mState.line;
        mState.beginOfLine = ++mState.cursor;
    }

    bool skipTo(char c, bool acceptEnd = false)
    {
        while (!atEnd()) {
            if (*mState.cursor == '\n' || *mState.cursor == '\r') {
                if (!eatCRLF()) {
                    return false;
                }
            } else if (*mState.cursor == c) {
                return true;
            } else {
                ++mState.cursor;
            }
        }
        return acceptEnd;
    }

    bool skipToCRLF(bool acceptEnd = true)
    {
        for (; !atEnd(); ++mState.cursor) {
            if (*mState.cursor == '\n' || *mState.cursor == '\r') {
                return eatCRLF();
            }
        }
        return acceptEnd;
    }

    void skipTo8BitEnd()
    {
        while (!atEnd() && (signed char)*mState.cursor < 0) {
            ++mState.cursor;
        }
    }

    void skipToDelim();
};
}
