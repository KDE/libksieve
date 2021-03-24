/*  -*- c++ -*-
    ksieve/lexer.h

    This file is part of KSieve,
    the KDE internet mail/usenet news message filtering library.
    SPDX-FileCopyrightText: 2003 Marc Mutz <mutz@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieve_export.h"

class QString;

namespace KSieve
{
class Error;

class KSIEVE_EXPORT Lexer
{
public:
    enum Options { IncludeComments = 0, IgnoreComments = 1, IncludeLineFeeds = 0, IgnoreLineFeeds = 2 };

    Lexer(const char *scursor, const char *send, int options = 0);
    ~Lexer();

    /** Return whether comments are returned by @ref
        nextToken. Default is to not ignore comments. Ignoring them
        can speed up script parsing a bit, and can be used when the
        internal representation of the script won't be serialized into
        string form again (or if you simply want to delete all
        comments)
    **/
    bool ignoreComments() const;

    /** Return whether line feeds are returned by @ref
        nextToken. Default is to not ignore line feeds. Ignoring them
        can speed up script parsing a bit, and can be used when the
        internal representation of the script won't be serialized into
        string form again.
    **/
    bool ignoreLineFeeds() const;

    const Error &error() const;

    bool atEnd() const;
    int column() const;
    int line() const;

    enum Token {
        None = 0,
        Number, // 1, 100, 1M, 10k, 1G, 2g, 3m
        Identifier, // atom
        Tag, // :tag
        Special, // {} [] () ,;
        QuotedString, // "foo\"bar" -> foo"bar
        MultiLineString, // text: \nfoo\n. -> foo
        HashComment, // # foo
        BracketComment, // /* foo */
        LineFeeds // the number of line feeds encountered
    };

    /** Parse the next token and return it's type. @p result will contain
        the value of the token. */
    Token nextToken(QString &result);

    void save();
    void restore();

    class Impl;

private:
    Impl *i = nullptr;

private:
    const Lexer &operator=(const Lexer &);
    Lexer(const Lexer &);
};
} // namespace KSieve

