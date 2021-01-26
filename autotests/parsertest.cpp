/*  -*- c++ -*-
    tests/parsertest.cpp

    This file is part of the testsuite of KSieve,
    the KDE internet mail/usenet news message filtering library.
    SPDX-FileCopyrightText: 2003 Marc Mutz <mutz@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#include <../src/ksieve/parser.h>
#include <config-libksieve.h> // SIZEOF_UNSIGNED_LONG
using KSieve::Parser;

#include <../src/ksieve/error.h>
#include <../src/ksieve/scriptbuilder.h>

#include <QString>

#include <cstdlib>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

#include <cassert>

enum BuilderMethod {
    TaggedArgument,
    StringArgument,
    NumberArgument,
    CommandStart,
    CommandEnd,
    TestStart,
    TestEnd,
    TestListStart,
    TestListEnd,
    BlockStart,
    BlockEnd,
    StringListArgumentStart,
    StringListEntry,
    StringListArgumentEnd,
    HashComment,
    BracketComment,
    Error,
    Finished
};

static const unsigned int MAX_RESPONSES = 100;

static struct TestCase {
    const char *name;
    const char *script;
    struct Response {
        BuilderMethod method;
        const char *string;
        bool boolean;
    } responses[MAX_RESPONSES];
} testCases[] = {
    //
    // single commands:
    //

    {"Null script", nullptr, {{Finished, nullptr, false}}},

    {"Empty script", "", {{Finished, nullptr, false}}},

    {"WS-only script", " \t\n\r\n", {{Finished, nullptr, false}}},

    {"Bare hash comment", "#comment", {{HashComment, "comment", false}, {Finished, nullptr, false}}},

    {"Bare bracket comment", "/*comment*/", {{BracketComment, "comment", false}, {Finished, nullptr, false}}},

    {"Bare command", "command;", {{CommandStart, "command", false}, {CommandEnd, nullptr, false}, {Finished, nullptr, false}}},

    {"Bare command - missing semicolon", "command", {{CommandStart, "command", false}, {Error, "MissingSemicolonOrBlock", false}}},

    {"surrounded by bracket comments",
     "/*comment*/command/*comment*/;/*comment*/",
     {{BracketComment, "comment", false},
      {CommandStart, "command", false},
      {BracketComment, "comment", false},
      {CommandEnd, nullptr, false},
      {BracketComment, "comment", false},
      {Finished, nullptr, false}}},

    {"surrounded by hash comments",
     "#comment\ncommand#comment\n;#comment",
     {{HashComment, "comment", false},
      {CommandStart, "command", false},
      {HashComment, "comment", false},
      {CommandEnd, nullptr, false},
      {HashComment, "comment", false},
      {Finished, nullptr, false}}},

    {"single tagged argument",
     "command :tag;",
     {{CommandStart, "command", false}, {TaggedArgument, "tag", false}, {CommandEnd, nullptr, false}, {Finished, nullptr, false}}},

    {"single tagged argument - missing semicolon",
     "command :tag",
     {{CommandStart, "command", false}, {TaggedArgument, "tag", false}, {Error, "MissingSemicolonOrBlock", false}}},

    {"single string argument - quoted string",
     "command \"string\";",
     {{CommandStart, "command", false}, {StringArgument, "string", false /*quoted*/}, {CommandEnd, nullptr, false}, {Finished, nullptr, false}}},

    {"single string argument - multi-line string",
     "command text:\nstring\n.\n;",
     {{CommandStart, "command", false}, {StringArgument, "string", true /*multiline*/}, {CommandEnd, nullptr, false}, {Finished, nullptr, false}}},

    {"single number argument - 100",
     "command 100;",
     {{CommandStart, "command", false}, {NumberArgument, "100 ", false}, {CommandEnd, nullptr, false}, {Finished, nullptr, false}}},

    {"single number argument - 100k",
     "command 100k;",
     {{CommandStart, "command", false}, {NumberArgument, "102400k", false}, {CommandEnd, nullptr, false}, {Finished, nullptr, false}}},

    {"single number argument - 100M",
     "command 100M;",
     {{CommandStart, "command", false}, {NumberArgument, "104857600M", false}, {CommandEnd, nullptr, false}, {Finished, nullptr, false}}},

    {"single number argument - 2G",
     "command 2G;",
     {{CommandStart, "command", false}, {NumberArgument, "2147483648G", false}, {CommandEnd, nullptr, false}, {Finished, nullptr, false}}},

#if SIZEOF_UNSIGNED_LONG == 8
#define ULONG_MAX_STRING "18446744073709551615"
#define ULONG_MAXP1_STRING "18446744073709551616"
#elif SIZEOF_UNSIGNED_LONG == 4
#define ULONG_MAX_STRING "4294967295"
#define ULONG_MAXP1_STRING "4G"
#else
#error sizeof( unsigned long ) != 4 && sizeof( unsigned long ) != 8 ???
#endif

    {"single number argument - ULONG_MAX + 1", "command " ULONG_MAXP1_STRING ";", {{CommandStart, "command", false}, {Error, "NumberOutOfRange", false}}},

    {"single number argument - ULONG_MAX",
     "command " ULONG_MAX_STRING ";",
     {{CommandStart, "command", false}, {NumberArgument, ULONG_MAX_STRING " ", false}, {CommandEnd, nullptr, false}, {Finished, nullptr, false}}},

    {"single one-element string list argument - quoted string",
     "command [\"string\"];",
     {{CommandStart, "command", false},
      {StringListArgumentStart, nullptr, false},
      {StringListEntry, "string", false /*quoted*/},
      {StringListArgumentEnd, nullptr, false},
      {CommandEnd, nullptr, false},
      {Finished, nullptr, false}}},

    {"single one-element string list argument - multi-line string",
     "command [text:\nstring\n.\n];",
     {{CommandStart, "command", false},
      {StringListArgumentStart, nullptr, false},
      {StringListEntry, "string", true /*multiline*/},
      {StringListArgumentEnd, nullptr, false},
      {CommandEnd, nullptr, false},
      {Finished, nullptr, false}}},

    {"single two-element string list argument - quoted strings",
     R"(command ["string","string"];)",
     {{CommandStart, "command", false},
      {StringListArgumentStart, nullptr, false},
      {StringListEntry, "string", false /*quoted*/},
      {StringListEntry, "string", false /*quoted*/},
      {StringListArgumentEnd, nullptr, false},
      {CommandEnd, nullptr, false},
      {Finished, nullptr, false}}},

    {"single two-element string list argument - multi-line strings",
     "command [text:\nstring\n.\n,text:\nstring\n.\n];",
     {{CommandStart, "command", false},
      {StringListArgumentStart, nullptr, false},
      {StringListEntry, "string", true /*multiline*/},
      {StringListEntry, "string", true /*multiline*/},
      {StringListArgumentEnd, nullptr, false},
      {CommandEnd, nullptr, false},
      {Finished, nullptr, false}}},

    {"single two-element string list argument - quoted + multi-line strings",
     "command [\"string\",text:\nstring\n.\n];",
     {{CommandStart, "command", false},
      {StringListArgumentStart, nullptr, false},
      {StringListEntry, "string", false /*quoted*/},
      {StringListEntry, "string", true /*multiline*/},
      {StringListArgumentEnd, nullptr, false},
      {CommandEnd, nullptr, false},
      {Finished, nullptr, false}}},

    {"single two-element string list argument - multi-line + quoted strings",
     "command [text:\nstring\n.\n,\"string\"];",
     {{CommandStart, "command", false},
      {StringListArgumentStart, nullptr, false},
      {StringListEntry, "string", true /*multiline*/},
      {StringListEntry, "string", false /*quoted*/},
      {StringListArgumentEnd, nullptr, false},
      {CommandEnd, nullptr, false},
      {Finished, nullptr, false}}},

    {"single bare test argument",
     "command test;",
     {{CommandStart, "command", false}, {TestStart, "test", false}, {TestEnd, nullptr, false}, {CommandEnd, nullptr, false}, {Finished, nullptr, false}}},

    {"one-element test list argument",
     "command(test);",
     {{CommandStart, "command", false},
      {TestListStart, nullptr, false},
      {TestStart, "test", false},
      {TestEnd, nullptr, false},
      {TestListEnd, nullptr, false},
      {CommandEnd, nullptr, false},
      {Finished, nullptr, false}}},

    {"two-element test list argument",
     "command(test,test);",
     {{CommandStart, "command", false},
      {TestListStart, nullptr, false},
      {TestStart, "test", false},
      {TestEnd, nullptr, false},
      {TestStart, "test", false},
      {TestEnd, nullptr, false},
      {TestListEnd, nullptr, false},
      {CommandEnd, nullptr, false},
      {Finished, nullptr, false}}},

    {"zero-element block",
     "command{}",
     {{CommandStart, "command", false}, {BlockStart, nullptr, false}, {BlockEnd, nullptr, false}, {CommandEnd, nullptr, false}, {Finished, nullptr, false}}},

    {"one-element block",
     "command{command;}",
     {{CommandStart, "command", false},
      {BlockStart, nullptr, false},
      {CommandStart, "command", false},
      {CommandEnd, nullptr, false},
      {BlockEnd, nullptr, false},
      {CommandEnd, nullptr, false},
      {Finished, nullptr, false}}},

    {"two-element block",
     "command{command;command;}",
     {{CommandStart, "command", false},
      {BlockStart, nullptr, false},
      {CommandStart, "command", false},
      {CommandEnd, nullptr, false},
      {CommandStart, "command", false},
      {CommandEnd, nullptr, false},
      {BlockEnd, nullptr, false},
      {CommandEnd, nullptr, false},
      {Finished, nullptr, false}}},

    {"command with a test with a test with a test",
     "command test test test;",
     {{CommandStart, "command", false},
      {TestStart, "test", false},
      {TestStart, "test", false},
      {TestStart, "test", false},
      {TestEnd, nullptr, false},
      {TestEnd, nullptr, false},
      {TestEnd, nullptr, false},
      {CommandEnd, nullptr, false},
      {Finished, nullptr, false}}},
};

static const int numTestCases = sizeof testCases / sizeof *testCases;

// Prints out the parse tree in XML-like format. For visual inspection
// (manual tests).
class PrintingScriptBuilder : public KSieve::ScriptBuilder
{
public:
    PrintingScriptBuilder()
        : KSieve::ScriptBuilder()
        , indent(0)
    {
        write("<script type=\"application/sieve\">");
        ++indent;
    }

    ~PrintingScriptBuilder() override
    {
    }

    void taggedArgument(const QString &tag) override
    {
        write("tag", tag);
    }

    void stringArgument(const QString &string, bool multiLine, const QString & /*fixme*/) override
    {
        write(multiLine ? "string type=\"multiline\"" : "string type=\"quoted\"", string);
    }

    void numberArgument(unsigned long number, char quantifier) override
    {
        const QString txt = QLatin1String("number") + (quantifier ? QStringLiteral(" quantifier=\"%1\"").arg(quantifier) : QString());
        write(txt.toLatin1(), QString::number(number));
    }

    void commandStart(const QString &identifier, int lineNumber) override
    {
        Q_UNUSED(lineNumber)
        write("<command>");
        ++indent;
        write("identifier", identifier);
    }

    void commandEnd(int lineNumber) override
    {
        Q_UNUSED(lineNumber)
        --indent;
        write("</command>");
    }

    void testStart(const QString &identifier) override
    {
        write("<test>");
        ++indent;
        write("identifier", identifier);
    }

    void testEnd() override
    {
        --indent;
        write("</test>");
    }

    void testListStart() override
    {
        write("<testlist>");
        ++indent;
    }

    void testListEnd() override
    {
        --indent;
        write("</testlist>");
    }

    void blockStart(int lineNumber) override
    {
        Q_UNUSED(lineNumber)
        write("<block>");
        ++indent;
    }

    void blockEnd(int lineNumber) override
    {
        Q_UNUSED(lineNumber)
        --indent;
        write("</block>");
    }

    void stringListArgumentStart() override
    {
        write("<stringlist>");
        ++indent;
    }

    void stringListArgumentEnd() override
    {
        --indent;
        write("</stringlist>");
    }

    void stringListEntry(const QString &string, bool multiline, const QString &hashComment) override
    {
        stringArgument(string, multiline, hashComment);
    }

    void hashComment(const QString &comment) override
    {
        write("comment type=\"hash\"", comment);
    }

    void bracketComment(const QString &comment) override
    {
        write("comment type=\"bracket\"", comment);
    }

    void lineFeed() override
    {
        write("<crlf/>");
    }

    void error(const KSieve::Error &error) override
    {
        indent = 0;
        write((QStringLiteral("Error: ") + error.asString()).toLatin1().constData());
    }

    void finished() override
    {
        --indent;
        write("</script>");
    }

private:
    int indent;
    void write(const char *msg)
    {
        for (int i = 2 * indent; i > 0; --i) {
            cout << " ";
        }
        cout << msg << endl;
    }

    void write(const QByteArray &key, const QString &value)
    {
        if (value.isEmpty()) {
            write(QByteArray(QByteArray("<") + key + QByteArray("/>")).constData());
            return;
        }
        write(QByteArray(QByteArray("<") + key + QByteArray(">")).constData());
        ++indent;
        write(value.toUtf8().data());
        --indent;
        write(QByteArray(QByteArray("</") + key + QByteArray(">")).constData());
    }
};

// verifies that methods get called with expected arguments (and in
// expected sequence) as specified by the TestCase. For automated
// tests.
class VerifyingScriptBuilder : public KSieve::ScriptBuilder
{
public:
    VerifyingScriptBuilder(const TestCase &testCase)
        : KSieve::ScriptBuilder()
        , mNextResponse(0)
        , mTestCase(testCase)
        , mOk(true)
    {
    }

    ~VerifyingScriptBuilder() override
    {
    }

    bool ok() const
    {
        return mOk;
    }

    void taggedArgument(const QString &tag) override
    {
        checkIs(TaggedArgument);
        checkEquals(tag);
        ++mNextResponse;
    }

    void stringArgument(const QString &string, bool multiline, const QString & /*fixme*/) override
    {
        checkIs(StringArgument);
        checkEquals(string);
        checkEquals(multiline);
        ++mNextResponse;
    }

    void numberArgument(unsigned long number, char quantifier) override
    {
        checkIs(NumberArgument);
        checkEquals(QString::number(number) + QLatin1Char(quantifier ? quantifier : ' '));
        ++mNextResponse;
    }

    void commandStart(const QString &identifier, int lineNumber) override
    {
        Q_UNUSED(lineNumber)
        checkIs(CommandStart);
        checkEquals(identifier);
        ++mNextResponse;
    }

    void commandEnd(int lineNumber) override
    {
        Q_UNUSED(lineNumber)
        checkIs(CommandEnd);
        ++mNextResponse;
    }

    void testStart(const QString &identifier) override
    {
        checkIs(TestStart);
        checkEquals(identifier);
        ++mNextResponse;
    }

    void testEnd() override
    {
        checkIs(TestEnd);
        ++mNextResponse;
    }

    void testListStart() override
    {
        checkIs(TestListStart);
        ++mNextResponse;
    }

    void testListEnd() override
    {
        checkIs(TestListEnd);
        ++mNextResponse;
    }

    void blockStart(int lineNumber) override
    {
        Q_UNUSED(lineNumber)
        checkIs(BlockStart);
        ++mNextResponse;
    }

    void blockEnd(int lineNumber) override
    {
        Q_UNUSED(lineNumber)
        checkIs(BlockEnd);
        ++mNextResponse;
    }

    void stringListArgumentStart() override
    {
        checkIs(StringListArgumentStart);
        ++mNextResponse;
    }

    void stringListEntry(const QString &string, bool multiLine, const QString & /*fixme*/) override
    {
        checkIs(StringListEntry);
        checkEquals(string);
        checkEquals(multiLine);
        ++mNextResponse;
    }

    void stringListArgumentEnd() override
    {
        checkIs(StringListArgumentEnd);
        ++mNextResponse;
    }

    void hashComment(const QString &comment) override
    {
        checkIs(HashComment);
        checkEquals(comment);
        ++mNextResponse;
    }

    void bracketComment(const QString &comment) override
    {
        checkIs(BracketComment);
        checkEquals(comment);
        ++mNextResponse;
    }

    void lineFeed() override
    {
        // FIXME
    }

    void error(const KSieve::Error &error) override
    {
        checkIs(Error);
        checkEquals(QString::fromLatin1(KSieve::Error::typeToString(error.type())));
        ++mNextResponse;
    }

    void finished() override
    {
        checkIs(Finished);
        //++mNextResponse (no!)
    }

private:
    Q_DISABLE_COPY(VerifyingScriptBuilder)
    const TestCase::Response &currentResponse() const
    {
        assert(mNextResponse <= MAX_RESPONSES);
        return mTestCase.responses[mNextResponse];
    }

    void checkIs(BuilderMethod m)
    {
        if (currentResponse().method != m) {
            cerr << " expected method " << (int)currentResponse().method << ", got " << (int)m;
            mOk = false;
        }
    }

    void checkEquals(const QString &s)
    {
        if (s != QString::fromUtf8(currentResponse().string)) {
            cerr << " expected string arg \"" << (currentResponse().string ? currentResponse().string : "<null>") << "\", got \""
                 << (s.isNull() ? "<null>" : s.toUtf8().data()) << "\"";
            mOk = false;
        }
    }

    void checkEquals(bool b)
    {
        if (b != currentResponse().boolean) {
            cerr << " expected boolean arg <" << currentResponse().boolean << ">, got <" << b << ">";
            mOk = false;
        }
    }

    unsigned int mNextResponse;
    const TestCase &mTestCase;
    bool mOk;
};

int main(int argc, char *argv[])
{
    if (argc == 2) { // manual test
        const char *scursor = argv[1];
        const char *const send = argv[1] + qstrlen(argv[1]);

        Parser parser(scursor, send);
        PrintingScriptBuilder psb;
        parser.setScriptBuilder(&psb);
        if (parser.parse()) {
            cout << "ok" << endl;
        } else {
            cout << "bad" << endl;
        }
    } else if (argc == 1) { // automated test
        bool success = true;
        for (int i = 0; i < numTestCases; ++i) {
            const TestCase &t = testCases[i];
            cerr << t.name << ":";
            VerifyingScriptBuilder v(t);
            Parser p(t.script, t.script + qstrlen(t.script));
            p.setScriptBuilder(&v);
            const bool ok = p.parse();
            if (v.ok()) {
                if (ok) {
                    cerr << " ok";
                } else {
                    cerr << " xfail";
                }
            } else {
                success = false;
            }
            cerr << endl;
        }
        if (!success) {
            exit(1);
        }
    } else { // usage error
        cerr << "usage: parsertest [ <string> ]" << endl;
        exit(1);
    }

    return 0;
}
