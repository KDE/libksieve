/*  -*- c++ -*-
    tests/lexertest.cpp

    This file is part of the testsuite of KSieve,
    the KDE internet mail/usenet news message filtering library.
    SPDX-FileCopyrightText: 2003 Marc Mutz <mutz@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#include "lexer.h"
using KSieve::Lexer;

#include "error.h"
using KSieve::Error;

#include <QString>

#include <cstdlib>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

static const char *token2string(Lexer::Token t)
{
    switch (t) {
#define CASE(x)                                                                                                                                                \
    case Lexer::x:                                                                                                                                             \
        return #x
        CASE(None);
        CASE(HashComment);
        CASE(BracketComment);
        CASE(Identifier);
        CASE(Tag);
        CASE(Number);
        CASE(MultiLineString);
        CASE(QuotedString);
        CASE(Special);
        CASE(LineFeeds);
    }
    return "";
#undef CASE
}

struct TestCase {
    const char *name;
    const char *string;
    struct {
        Lexer::Token token;
        const char *result;
    } expected[16]; // end with { None, 0 }
    Error::Type expectedError;
    int errorLine, errorCol;
};

static const TestCase testcases[] = {
    //
    // Whitespace:
    //

    {"Null script", nullptr, {{Lexer::None, nullptr}}, Error::None, 0, 0},

    {"Empty script", "", {{Lexer::None, nullptr}}, Error::None, 0, 0},

    {"Whitespace-only script", " \t\n\t \n", {{Lexer::LineFeeds, "2"}, {Lexer::None, nullptr}}, Error::None, 0, 0},

    {"Lone CR", "\r", {{Lexer::None, nullptr}}, Error::CRWithoutLF, 0, 1},

    {"CR+Space", "\r ", {{Lexer::None, nullptr}}, Error::CRWithoutLF, 0, 1},

    {"CRLF alone", "\r\n", {{Lexer::LineFeeds, "1"}, {Lexer::None, nullptr}}, Error::None, 0, 0},

    //
    // hash comments:
    //

    {"Basic hash comment (no newline)", "#comment", {{Lexer::HashComment, "comment"}, {Lexer::None, nullptr}}, Error::None, 0, 0},

    {"Basic hash comment (LF)", "#comment\n", {{Lexer::HashComment, "comment"}, {Lexer::None, nullptr}}, Error::None, 0, 0},

    {"Basic hash comment (CRLF)", "#comment\r\n", {{Lexer::HashComment, "comment"}, {Lexer::None, nullptr}}, Error::None, 0, 0},

    {"Basic hash comment (CR)", "#comment\r", {{Lexer::HashComment, nullptr}}, Error::CRWithoutLF, 0, 9},

    {"Non-UTF-8 in hash comment", "#\xA9 copyright", {{Lexer::HashComment, nullptr}}, Error::InvalidUTF8, 0, 12},

    //
    // bracket comments:
    //

    {"Basic bracket comment", "/* comment */", {{Lexer::BracketComment, " comment "}, {Lexer::None, nullptr}}, Error::None, 0, 0},

    {"Basic bracket comment - missing trailing slash", "/* comment *", {{Lexer::BracketComment, nullptr}}, Error::UnfinishedBracketComment, 0, 0},

    {"Basic bracket comment - missing trailing asterisk + slash", "/* comment ", {{Lexer::BracketComment, nullptr}}, Error::UnfinishedBracketComment, 0, 0},

    {"Basic bracket comment - missing leading slash", "* comment */", {{Lexer::None, nullptr}}, Error::IllegalCharacter, 0, 0},

    {"Basic bracket comment - missing leading asterisk + slash",
     "comment */",
     {{Lexer::Identifier, "comment"}, {Lexer::None, nullptr}},
     Error::IllegalCharacter,
     0,
     8},

    {"Basic multiline bracket comment (LF)",
     "/* comment\ncomment */",
     {{Lexer::BracketComment, " comment\ncomment "}, {Lexer::None, nullptr}},
     Error::None,
     0,
     0},

    {"Basic multiline bracket comment (CRLF)",
     "/* comment\r\ncomment */",
     {{Lexer::BracketComment, " comment\ncomment "}, {Lexer::None, nullptr}},
     Error::None,
     0,
     0},

    {"Basic multiline bracket comment (CR)", "/* comment\rcomment */", {{Lexer::BracketComment, nullptr}}, Error::CRWithoutLF, 0, 11},

    {"Non-UTF-8 in bracket comment", "/*\xA9 copyright*/", {{Lexer::BracketComment, nullptr}}, Error::InvalidUTF8, 0, 14},

    //
    // numbers:
    //
    {"Basic number 1", "1", {{Lexer::Number, "1"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Basic number 01", "01", {{Lexer::Number, "01"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Qualified number 1k", "1k", {{Lexer::Number, "1k"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Qualified number 1M", "1M", {{Lexer::Number, "1M"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Qualified number 1G", "1G", {{Lexer::Number, "1G"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    //
    // identifiers:
    //
    {"Basic identifier \"id\"", "id", {{Lexer::Identifier, "id"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Basic identifier \"_id\"", "_id", {{Lexer::Identifier, "_id"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    //
    // tags:
    //
    {"Basic tag \":tag\"", ":tag", {{Lexer::Tag, "tag"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Basic tag \":_tag\"", ":_tag", {{Lexer::Tag, "_tag"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    //
    // specials:
    //
    {"Basic special \"{}[]();,\"",
     "{}[]();,",
     {{Lexer::Special, "{"},
      {Lexer::Special, "}"},
      {Lexer::Special, "["},
      {Lexer::Special, "]"},
      {Lexer::Special, "("},
      {Lexer::Special, ")"},
      {Lexer::Special, ";"},
      {Lexer::Special, ","},
      {Lexer::None, nullptr}},
     Error::None,
     0,
     0},
    //
    // quoted-string:
    //
    {"Basic quoted string \"foo\"", "\"foo\"", {{Lexer::QuotedString, "foo"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Basic quoted string, UTF-8",
     "\"foo\xC3\xB1"
     "foo\"", // fooäfoo
     {{Lexer::QuotedString,
       "foo\xC3\xB1"
       "foo"},
      {Lexer::None, nullptr}},
     Error::None,
     0,
     0},
    {"Quoted string, escaped '\"'", R"("foo\"bar")", {{Lexer::QuotedString, "foo\"bar"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Quoted string, escaped '\\'", R"("foo\\bar")", {{Lexer::QuotedString, "foo\\bar"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Quoted string, excessive escapes", R"("\fo\o")", {{Lexer::QuotedString, "foo"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Quoted string across lines (LF)", "\"foo\nbar\"", {{Lexer::QuotedString, "foo\nbar"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Quoted string across lines (CRLF)", "\"foo\r\nbar\"", {{Lexer::QuotedString, "foo\nbar"}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    //
    // multiline strings:
    //
    {"Basic multiline string I (LF)", "text:\nfoo\n.", {{Lexer::MultiLineString, "foo" /* "foo\n" ? */}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Basic multiline string I (CRLF)", "text:\r\nfoo\r\n.", {{Lexer::MultiLineString, "foo" /* "foo\n" ? */}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Basic multiline string II (LF)", "text:\nfoo\n.\n", {{Lexer::MultiLineString, "foo" /* "foo\n" ? */}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Basic multiline string II (CRLF)", "text:\r\nfoo\r\n.\r\n", {{Lexer::MultiLineString, "foo" /* "foo\n" ? */}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Dotstuffed multiline string (LF)", "text:\n..foo\n.", {{Lexer::MultiLineString, ".foo" /* ".foo\n" ? */}, {Lexer::None, nullptr}}, Error::None, 0, 0},
    {"Dotstuffed multiline string (CRLF)",
     "text:\r\n..foo\r\n.",
     {{Lexer::MultiLineString, ".foo" /* ".foo\n" ? */}, {Lexer::None, nullptr}},
     Error::None,
     0,
     0},
    {"Incompletely dotstuffed multiline string (LF)",
     "text:\n.foo\n.",
     {{Lexer::MultiLineString, ".foo" /* ".foo\n" ? */}, {Lexer::None, nullptr}},
     Error::None,
     0,
     0},
    {"Incompletely dotstuffed multiline string (CRLF)",
     "text:\r\n.foo\r\n.",
     {{Lexer::MultiLineString, ".foo" /* ".foo\n" ? */}, {Lexer::None, nullptr}},
     Error::None,
     0,
     0},
    {"Multiline with a line with only one '.'",
     "text:\r\nfoo\r\n..\r\nbar\r\n.",
     {{Lexer::MultiLineString, "foo\n.\nbar"}, {Lexer::None, nullptr}},
     Error::None,
     0,
     0},

    //
    // Errors in single tokens:
    //

    //
    // numbers:
    //
    {"Number, unknown qualifier", "100f", {{Lexer::Number, "100"}}, Error::UnexpectedCharacter, 0, 3},
    {"Negative number", "-100", {{Lexer::None, nullptr}}, Error::IllegalCharacter, 0, 0},
    //
    // identifiers:
    //
    {"Identifier, leading digits", "0id", {{Lexer::Number, "0"}}, Error::UnexpectedCharacter, 0, 1},
    {"Identifier, embedded umlaut", "idäid", {{Lexer::Identifier, "id"}}, Error::IllegalCharacter, 0, 2},
    //
    // tags:
    //
    {"Lone ':' (at end)", ":", {{Lexer::Tag, nullptr}}, Error::UnexpectedCharacter, 0, 0},
    {"Lone ':' (in stream)", ": ", {{Lexer::Tag, nullptr}}, Error::UnexpectedCharacter, 0, 1},
    {"Tag, leading digits", ":0tag", {{Lexer::Tag, nullptr}}, Error::NoLeadingDigits, 0, 1},
    {"Tag, embedded umlaut", ":tagätag", {{Lexer::Tag, "tag"}}, Error::IllegalCharacter, 0, 4},
    //
    // specials: (none)
    // quoted string:
    //
    {"Premature end of quoted string", "\"foo", {{Lexer::QuotedString, "foo"}}, Error::PrematureEndOfQuotedString, 0, 0},
    {"Invalid UTF-8 in quoted string",
     "\"foo\xC0\xA0"
     "foo\"",
     {{Lexer::QuotedString, "foo"}},
     Error::InvalidUTF8,
     0,
     4},

    //
    // Whitespace / token separation: valid
    //

    {"Two identifiers with linebreaks",
     "foo\nbar\n",
     {{Lexer::Identifier, "foo"}, {Lexer::LineFeeds, "1"}, {Lexer::Identifier, "bar"}, {Lexer::LineFeeds, "1"}, {Lexer::None, nullptr}},
     Error::None,
     0,
     0},

    //
    // Whitespace / token separation: invalid
    //
};

static const int numTestCases = sizeof testcases / sizeof *testcases;

int main(int argc, char *argv[])
{
    if (argc == 2) { // manual test
        const char *scursor = argv[1];
        const char *const send = argv[1] + qstrlen(argv[1]);

        Lexer lexer(scursor, send);

        cout << "Begin" << endl;
        while (!lexer.atEnd()) {
            QString result;
            Lexer::Token token = lexer.nextToken(result);
            if (lexer.error()) {
                cout << "Error " << token2string(token) << ": \"" << lexer.error().asString().toLatin1().constData() << "\" at (" << lexer.error().line() << ","
                     << lexer.error().column() << ")" << endl;
                break;
            } else {
                cout << "Got " << token2string(token) << ": \"" << result.toUtf8().data() << "\" at (" << lexer.line() << "," << lexer.column() << ")" << endl;
            }
        }
        cout << "End" << endl;
    } else if (argc == 1) { // automated test
        bool success = true;
        for (int i = 0; i < numTestCases; ++i) {
            bool ok = true;
            const TestCase &t = testcases[i];
            const char *const send = t.string + qstrlen(t.string);
            Lexer lexer(t.string, send, Lexer::IncludeComments);
            cerr << t.name << ":";
            for (int j = 0; !lexer.atEnd(); ++j) {
                QString result;
                Lexer::Token token = lexer.nextToken(result);
                Error error = lexer.error();
                if (t.expected[j].token != token) {
                    ok = false;
                    cerr << " expected token " << token2string(t.expected[j].token) << ", got " << token2string(token);
                }
                if (QString::fromUtf8(t.expected[j].result) != result) {
                    ok = false;
                    if (t.expected[j].result) {
                        cerr << " expected string \"" << t.expected[j].result << "\"";
                    } else {
                        cerr << " expected null string";
                    }
                    if (!result.toUtf8().isNull()) {
                        cerr << ", got \"" << result.toUtf8().data() << "\"";
                    } else {
                        cerr << ", got null string";
                    }
                }
                if (error && error.type() != t.expectedError) {
                    ok = false;
                    cerr << " expected error #" << (int)t.expectedError << ", got #" << (int)error.type();
                }
                if (error && (error.line() != t.errorLine || error.column() != t.errorCol)) {
                    ok = false;
                    cerr << " expected position (" << t.errorLine << "," << t.errorCol << "), got (" << error.line() << "," << error.column() << ")";
                }
                if (error) {
                    goto ErrorOut;
                }
                if (t.expected[j].token == Lexer::None && t.expected[j].result == nullptr) {
                    break;
                }
            }
            if (!lexer.atEnd()) {
                ok = false;
                cerr << " premature end of expected token list";
            }
        ErrorOut:
            if (ok) {
                cerr << " ok";
            }
            cerr << endl;
            if (!ok) {
                success = false;
            }
        }
        if (!success) {
            return 1;
        }
    } else { // usage error
        cerr << "usage: lexertest [ <string> ]" << endl;
        exit(1);
    }

    return 0;
}
