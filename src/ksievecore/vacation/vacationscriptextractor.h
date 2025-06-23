/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "sieve-vacation.h"
using namespace Qt::Literals::StringLiterals;

#include "vacationutils.h"

#include "error.h"
#include "parser.h"
#include "scriptbuilder.h"

#include "libksievecore_debug.h"
#include <cassert>
#include <climits>
#include <map>
#include <set>
#include <vector>

namespace KSieveExt
{
class MultiScriptBuilder : public KSieve::ScriptBuilder
{
    std::vector<KSieve::ScriptBuilder *> mBuilders;

public:
    MultiScriptBuilder()
        : KSieve::ScriptBuilder()
    {
    }

    MultiScriptBuilder(KSieve::ScriptBuilder *sb1)
        : KSieve::ScriptBuilder()
        , mBuilders(1)
    {
        mBuilders[0] = sb1;
        assert(sb1);
    }

    MultiScriptBuilder(KSieve::ScriptBuilder *sb1, KSieve::ScriptBuilder *sb2)
        : KSieve::ScriptBuilder()
        , mBuilders(2)
    {
        mBuilders[0] = sb1;
        mBuilders[1] = sb2;
        assert(sb1);
        assert(sb2);
    }

    MultiScriptBuilder(KSieve::ScriptBuilder *sb1, KSieve::ScriptBuilder *sb2, KSieve::ScriptBuilder *sb3)
        : KSieve::ScriptBuilder()
        , mBuilders(3)
    {
        mBuilders[0] = sb1;
        mBuilders[1] = sb2;
        mBuilders[2] = sb3;
        assert(sb1);
        assert(sb2);
        assert(sb3);
    }

    MultiScriptBuilder(KSieve::ScriptBuilder *sb1, KSieve::ScriptBuilder *sb2, KSieve::ScriptBuilder *sb3, KSieve::ScriptBuilder *sb4)
        : KSieve::ScriptBuilder()
        , mBuilders(4)
    {
        mBuilders[0] = sb1;
        mBuilders[1] = sb2;
        mBuilders[2] = sb3;
        mBuilders[3] = sb4;
        assert(sb1);
        assert(sb2);
        assert(sb3);
        assert(sb4);
    }

    ~MultiScriptBuilder() override = default;

private:
#ifdef FOREACH
#undef FOREACH
#endif
#define FOREACH                                                                                                                                                \
    for (std::vector<KSieve::ScriptBuilder *>::const_iterator it = mBuilders.begin(), end = mBuilders.end(); it != end; ++it)                                  \
    (*it)->
    void commandStart(const QString &identifier, int lineNumber) override
    {
        FOREACH commandStart(identifier, lineNumber);
    }

    void commandEnd(int lineNumber) override
    {
        FOREACH commandEnd(lineNumber);
    }

    void testStart(const QString &identifier) override
    {
        FOREACH testStart(identifier);
    }

    void testEnd() override
    {
        FOREACH testEnd();
    }

    void testListStart() override
    {
        FOREACH testListStart();
    }

    void testListEnd() override
    {
        FOREACH testListEnd();
    }

    void blockStart(int lineNumber) override
    {
        FOREACH blockStart(lineNumber);
    }

    void blockEnd(int lineNumber) override
    {
        FOREACH blockEnd(lineNumber);
    }

    void hashComment(const QString &comment) override
    {
        FOREACH hashComment(comment);
    }

    void bracketComment(const QString &comment) override
    {
        FOREACH bracketComment(comment);
    }

    void lineFeed() override
    {
        FOREACH lineFeed();
    }

    void error(const KSieve::Error &e) override
    {
        FOREACH error(e);
    }

    void finished() override
    {
        FOREACH finished();
    }

    void taggedArgument(const QString &tag) override
    {
        FOREACH taggedArgument(tag);
    }

    void stringArgument(const QString &string, bool multiline, const QString &fixme) override
    {
        FOREACH stringArgument(string, multiline, fixme);
    }

    void numberArgument(unsigned long number, char quantifier) override
    {
        FOREACH numberArgument(number, quantifier);
    }

    void stringListArgumentStart() override
    {
        FOREACH stringListArgumentStart();
    }

    void stringListEntry(const QString &string, bool multiline, const QString &fixme) override
    {
        FOREACH stringListEntry(string, multiline, fixme);
    }

    void stringListArgumentEnd() override
    {
        FOREACH stringListArgumentEnd();
    }

#undef FOREACH
};
}

namespace KSieveCore
{
class GenericInformationExtractor : public KSieve::ScriptBuilder
{
public:
    enum BuilderMethod {
        Any,
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
        StringListArgumentEnd
    };

    struct StateNode {
        // expectation:
        int depth;
        BuilderMethod method;
        const char *string;
        // actions:
        int if_found;
        int if_not_found;
        const char *save_tag;
    };

    const std::vector<StateNode> mNodes;
    std::map<QString, QString> mResults;
    std::set<unsigned int> mRecursionGuard;
    unsigned int mState;
    int mNestingDepth;

    int mLineNumber;

public:
    GenericInformationExtractor(const std::vector<StateNode> &nodes)
        : KSieve::ScriptBuilder()
        , mNodes(nodes)
        , mState(0)
        , mNestingDepth(0)
        , mLineNumber(0)
    {
    }

    const std::map<QString, QString> &results() const
    {
        return mResults;
    }

private:
    void process(BuilderMethod method, const QString &string = QString())
    {
        doProcess(method, string);
        mRecursionGuard.clear();
    }

    void doProcess(BuilderMethod method, const QString &string)
    {
        mRecursionGuard.insert(mState);
        bool found = true;
        const StateNode &expected = mNodes[mState];
        if (expected.depth != -1 && mNestingDepth != expected.depth) {
            found = false;
        }
        if (expected.method != Any && method != expected.method) {
            found = false;
        }
        if (const char *str = expected.string) {
            if (string.toLower() != QString::fromUtf8(str).toLower()) {
                found = false;
            }
        }
        qCDebug(LIBKSIEVECORE_LOG) << (found ? "found:" : "not found:") << mState << "->" << (found ? expected.if_found : expected.if_not_found);
        mState = found ? expected.if_found : expected.if_not_found;
        assert(mState < mNodes.size());
        if (found) {
            if (const char *save_tag = expected.save_tag) {
                mResults[QString::fromLatin1(save_tag)] = string;
            }
        }
        if (!found && !mRecursionGuard.count(mState)) {
            doProcess(method, string);
        }
    }

    void commandStart(const QString &identifier, int lineNumber) override
    {
        Q_UNUSED(lineNumber)
        qCDebug(LIBKSIEVECORE_LOG);
        process(CommandStart, identifier);
    }

    void commandEnd(int lineNumber) override
    {
        Q_UNUSED(lineNumber)
        qCDebug(LIBKSIEVECORE_LOG);
        process(CommandEnd);
    }

    void testStart(const QString &identifier) override
    {
        qCDebug(LIBKSIEVECORE_LOG);
        process(TestStart, identifier);
    }

    void testEnd() override
    {
        qCDebug(LIBKSIEVECORE_LOG);
        process(TestEnd);
    }

    void testListStart() override
    {
        qCDebug(LIBKSIEVECORE_LOG);
        process(TestListStart);
    }

    void testListEnd() override
    {
        qCDebug(LIBKSIEVECORE_LOG);
        process(TestListEnd);
    }

    void blockStart(int lineNumber) override
    {
        Q_UNUSED(lineNumber)
        qCDebug(LIBKSIEVECORE_LOG);
        process(BlockStart);
        ++mNestingDepth;
    }

    void blockEnd(int lineNumber) override
    {
        Q_UNUSED(lineNumber)
        qCDebug(LIBKSIEVECORE_LOG);
        --mNestingDepth;
        process(BlockEnd);
    }

    void hashComment(const QString &) override
    {
        qCDebug(LIBKSIEVECORE_LOG);
    }

    void bracketComment(const QString &) override
    {
        qCDebug(LIBKSIEVECORE_LOG);
    }

    void lineFeed() override
    {
        qCDebug(LIBKSIEVECORE_LOG);
    }

    void error(const KSieve::Error &) override
    {
        qCDebug(LIBKSIEVECORE_LOG);
        mState = 0;
    }

    void finished() override
    {
        qCDebug(LIBKSIEVECORE_LOG);
    }

    void taggedArgument(const QString &tag) override
    {
        qCDebug(LIBKSIEVECORE_LOG);
        process(TaggedArgument, tag);
    }

    void stringArgument(const QString &string, bool, const QString &) override
    {
        qCDebug(LIBKSIEVECORE_LOG);
        process(StringArgument, string);
    }

    void numberArgument(unsigned long number, char) override
    {
        qCDebug(LIBKSIEVECORE_LOG);
        process(NumberArgument, QString::number(number));
    }

    void stringListArgumentStart() override
    {
        qCDebug(LIBKSIEVECORE_LOG);
        process(StringListArgumentStart);
    }

    void stringListEntry(const QString &string, bool, const QString &) override
    {
        qCDebug(LIBKSIEVECORE_LOG);
        process(StringListEntry, string);
    }

    void stringListArgumentEnd() override
    {
        qCDebug(LIBKSIEVECORE_LOG);
        process(StringListArgumentEnd);
    }
};

using GIE = GenericInformationExtractor;
static const GenericInformationExtractor::StateNode spamNodes[] = {
    {0, GIE::CommandStart, "if", 1, 0, nullptr}, // 0
    {0, GIE::TestStart, "allof", 2, 3, nullptr}, // 1
    {0, GIE::TestListStart, nullptr, 3, 0, nullptr}, // 2
    {0, GIE::TestStart, "not", 4, 3, nullptr}, // 3
    {0, GIE::TestStart, "header", 5, 3, nullptr}, // 4
    {0, GIE::TaggedArgument, "contains", 6, 0, nullptr}, // 5

    // accept both string and string-list:
    {0, GIE::StringArgument, "x-spam-flag", 12, 7, "x-spam-flag"}, // 6
    {0, GIE::StringListArgumentStart, nullptr, 8, 0, nullptr}, // 7
    {0, GIE::StringListEntry, "x-spam-flag", 9, 10, "x-spam-flag"}, // 8
    {0, GIE::StringListEntry, nullptr, 9, 11, nullptr}, // 9
    {0, GIE::StringListArgumentEnd, nullptr, 0, 8, nullptr}, // 10
    {0, GIE::StringListArgumentEnd, nullptr, 12, 0, nullptr}, // 11

    // accept both string and string-list:
    {0, GIE::StringArgument, "yes", 18, 13, "spam-flag-yes"}, // 12
    {0, GIE::StringListArgumentStart, nullptr, 14, 0, nullptr}, // 13
    {0, GIE::StringListEntry, "yes", 15, 16, "spam-flag-yes"}, // 14
    {0, GIE::StringListEntry, nullptr, 15, 17, nullptr}, // 15
    {0, GIE::StringListArgumentEnd, nullptr, 0, 14, nullptr}, // 16
    {0, GIE::StringListArgumentEnd, nullptr, 18, 0, nullptr}, // 17

    {0, GIE::TestEnd, nullptr, 21, 20, nullptr}, // 18
    {0, GIE::Any, nullptr, 21, 0, nullptr}, // 19
    {0, GIE::TestListEnd, nullptr, 21, 19, nullptr}, // 20

    // block of command, find "stop", take nested if's into account:
    {0, GIE::BlockStart, nullptr, 22, 18, nullptr}, // 21
    {1, GIE::CommandStart, "vacation", 24, 22, "vacation"}, // 22
    {1, GIE::Any, nullptr, 24, 0, nullptr}, // 23
    {0, GIE::BlockEnd, nullptr, 25, 23, nullptr}, // 24

    {-1, GIE::Any, nullptr, 25, 25, nullptr}, // 25 end state
};
static const unsigned int numSpamNodes = sizeof spamNodes / sizeof *spamNodes;

class SpamDataExtractor : public GenericInformationExtractor
{
public:
    SpamDataExtractor()
        : GenericInformationExtractor(std::vector<StateNode>(spamNodes, spamNodes + numSpamNodes))
    {
    }

    bool found() const
    {
        return mResults.count(u"x-spam-flag"_s) && mResults.count(u"spam-flag-yes"_s) && mResults.count(QStringLiteral("vacation"));
    }
};

// to understand this table, study the output of
// libksieve/tests/parsertest
//   'if not address :domain :contains ["from"] ["mydomain.org"] { keep; stop; }'
static const GenericInformationExtractor::StateNode domainNodes[] = {
    {0, GIE::CommandStart, "if", 1, 0, nullptr}, // 0
    {0, GIE::TestStart, "allof", 2, 3, nullptr}, // 1
    {0, GIE::TestListStart, nullptr, 3, 0, nullptr}, // 2
    {0, GIE::TestStart, "address", 4, 3, nullptr}, // 3

    // :domain and :contains in arbitrary order:
    {0, GIE::TaggedArgument, "domain", 5, 6, nullptr}, // 4
    {0, GIE::TaggedArgument, "contains", 8, 0, nullptr}, // 5
    {0, GIE::TaggedArgument, "contains", 7, 0, nullptr}, // 6
    {0, GIE::TaggedArgument, "domain", 8, 0, nullptr}, // 7

    // accept both string and string-list:
    {0, GIE::StringArgument, "from", 14, 9, "from"}, // 8
    {0, GIE::StringListArgumentStart, nullptr, 10, 0, nullptr}, // 9
    {0, GIE::StringListEntry, "from", 11, 12, "from"}, // 10
    {0, GIE::StringListEntry, nullptr, 11, 13, nullptr}, // 11
    {0, GIE::StringListArgumentEnd, nullptr, 0, 10, nullptr}, // 12
    {0, GIE::StringListArgumentEnd, nullptr, 14, 0, nullptr}, // 13

    // string: save, string-list: save last
    {0, GIE::StringArgument, nullptr, 18, 15, "domainName"}, // 14
    {0, GIE::StringListArgumentStart, nullptr, 16, 0, nullptr}, // 15
    {0, GIE::StringListEntry, nullptr, 16, 17, "domainName"}, // 16
    {0, GIE::StringListArgumentEnd, nullptr, 18, 0, nullptr}, // 17

    {0, GIE::TestEnd, nullptr, 18, 20, nullptr}, // 18
    {0, GIE::Any, nullptr, 18, 0, nullptr}, // 19

    // block of commands, find "stop", take nested if's into account:
    {0, GIE::BlockStart, nullptr, 21, 19, nullptr}, // 20
    {1, GIE::CommandStart, "vacation", 23, 21, "vacation"}, // 21
    {1, GIE::Any, nullptr, 23, 0, nullptr}, // 22
    {0, GIE::BlockEnd, nullptr, 24, 22, nullptr}, // 23

    {-1, GIE::Any, nullptr, 24, 24, nullptr} // 24 end state
};
static const unsigned int numDomainNodes = sizeof domainNodes / sizeof *domainNodes;

class DomainRestrictionDataExtractor : public GenericInformationExtractor
{
public:
    DomainRestrictionDataExtractor()
        : GenericInformationExtractor(std::vector<StateNode>(domainNodes, domainNodes + numDomainNodes))
    {
    }

    QString domainName() /*not const, since map::op[] isn't const*/
    {
        return mResults.count(u"vacation"_s) && mResults.count(u"from"_s) ? mResults[QStringLiteral("domainName")] : QString();
    }
};

// if not allof (currentdate :value "ge" date "YYYY-MM-DD",
//               currentfate :value "le" date "YYYY-MM-DD) { keep; stop; }
static const GenericInformationExtractor::StateNode datesNodes[] = {
    {0, GIE::CommandStart, "if", 1, 0, nullptr}, // 0
    {0, GIE::TestStart, "allof", 2, 0, nullptr}, // 1

    // handle startDate and endDate in arbitrary order
    {0, GIE::TestListStart, nullptr, 3, 0, nullptr}, // 2
    {0, GIE::TestStart, "currentdate", 4, 3, nullptr}, // 3
    {0, GIE::TaggedArgument, "value", 5, 4, nullptr}, // 4
    {0, GIE::StringArgument, "ge", 6, 10, nullptr}, // 5
    {0, GIE::StringArgument, "date", 7, 8, nullptr}, // 6
    {0, GIE::StringArgument, nullptr, 15, 0, "startDate"}, // 7
    {0, GIE::StringArgument, "iso8601", 9, 0, nullptr}, // 8
    {0, GIE::StringArgument, nullptr, 15, 0, "startDateTime"}, // 9
    {0, GIE::StringArgument, "le", 11, 0, nullptr}, // 10
    {0, GIE::StringArgument, "date", 12, 13, nullptr}, // 11
    {0, GIE::StringArgument, nullptr, 15, 0, "endDate"}, // 12
    {0, GIE::StringArgument, "iso8601", 14, 0, nullptr}, // 13
    {0, GIE::StringArgument, nullptr, 15, 0, "endDateTime"}, // 14
    {0, GIE::TestEnd, nullptr, 16, 0, nullptr}, // 15

    {0, GIE::TestStart, "currentdate", 17, 16, nullptr}, // 16
    {0, GIE::TaggedArgument, "value", 18, 17, nullptr}, // 17
    {0, GIE::StringArgument, "le", 19, 23, nullptr}, // 18
    {0, GIE::StringArgument, "date", 20, 21, nullptr}, // 19
    {0, GIE::StringArgument, nullptr, 28, 0, "endDate"}, // 20
    {0, GIE::StringArgument, "iso8601", 22, 0, nullptr}, // 21
    {0, GIE::StringArgument, nullptr, 28, 0, "endDateTime"}, // 22
    {0, GIE::StringArgument, "ge", 24, 0, nullptr}, // 23
    {0, GIE::StringArgument, "date", 25, 26, nullptr}, // 24
    {0, GIE::StringArgument, nullptr, 28, 0, "startDate"}, // 25
    {0, GIE::StringArgument, "iso8601", 27, 0, nullptr}, // 26
    {0, GIE::StringArgument, nullptr, 28, 0, "startDateTime"}, // 27
    {0, GIE::TestEnd, nullptr, 32, 0, nullptr}, // 28
    {0, GIE::TestStart, nullptr, 31, 30, nullptr}, // 29
    {-1, GIE::Any, nullptr, 32, 0, nullptr}, // 30
    {0, GIE::TestEnd, nullptr, 32, 30, nullptr}, // 31
    {0, GIE::TestListEnd, nullptr, 33, 29, nullptr}, // 32

    {0, GIE::TestEnd, nullptr, 34, 0, nullptr}, // 33

    // block of commands, find "stop", take nested if's into account:
    {0, GIE::BlockStart, nullptr, 36, 33, nullptr}, // 34
    {-1, GIE::Any, nullptr, 36, 0, nullptr}, // 35
    {1, GIE::CommandStart, "vacation", 38, 35, "vacation"}, // 36
    {-1, GIE::Any, nullptr, 38, 0, nullptr}, // 37
    {0, GIE::BlockEnd, nullptr, 39, 37, nullptr}, // 38

    {-1, GIE::Any, nullptr, 39, 39, nullptr} // 39 end state
};

static const unsigned int numDatesNodes = sizeof datesNodes / sizeof *datesNodes;

class DateExtractor : public GenericInformationExtractor
{
public:
    DateExtractor()
        : GenericInformationExtractor(std::vector<StateNode>(datesNodes, datesNodes + numDatesNodes))
    {
    }

    QDate endDate() const
    {
        if (results().count(u"endDateTime"_s) == 1) {
            return datetime(u"endDateTime"_s).date();
        } else {
            return date(u"endDate"_s);
        }
    }

    QDate startDate() const
    {
        if (results().count(u"startDateTime"_s) == 1) {
            return datetime(u"startDateTime"_s).date();
        } else {
            return date(u"startDate"_s);
        }
    }

    QTime endTime() const
    {
        return datetime(u"endDateTime"_s).time();
    }

    QTime startTime() const
    {
        return datetime(u"startDateTime"_s).time();
    }

private:
    QDate date(const QString &name) const
    {
        if (results().count(name) == 0) {
            return {};
        } else {
            return QDate::fromString(results().at(name), Qt::ISODate);
        }
    }

    QDateTime datetime(const QString &name) const
    {
        if (results().count(name) == 0) {
            return {};
        } else {
            return QDateTime::fromString(results().at(name), Qt::ISODate);
        }
    }
};

class VacationDataExtractor : public KSieve::ScriptBuilder
{
    enum Context {
        None = 0,
        // command itself:
        VacationCommand,
        // tagged args:
        Days,
        Addresses,
        Subject,
        VacationEnd,
        IfBlock,
        RedirectCommand
    };

public:
    VacationDataExtractor();
    ~VacationDataExtractor() override;

    bool commandFound() const
    {
        return mContext == VacationEnd;
    }

    bool active() const
    {
        return mActive;
    }

    int notificationInterval() const
    {
        return mNotificationInterval;
    }

    const QString &messageText() const
    {
        return mMessageText;
    }

    const QStringList &aliases() const
    {
        return mAliases;
    }

    const QString &ifComment() const
    {
        return mIfComment;
    }

    VacationUtils::MailAction mailAction() const
    {
        return mMailAction;
    }

    const QString &mailActionRecipient() const
    {
        return mMailActionRecipient;
    }

    const QString &subject() const
    {
        return mSubject;
    }

    int lineStart() const
    {
        return mLineStart;
    }

    int lineEnd() const
    {
        return mLineEnd;
    }

private:
    void commandStart(const QString &identifier, int lineNumber) override;

    void commandEnd(int lineNumber) override;

    void testStart(const QString &) override;
    void testEnd() override
    {
    }

    void testListStart() override
    {
    }

    void testListEnd() override
    {
    }

    void blockStart(int lineNumber) override;
    void blockEnd(int lineNumber) override;
    void hashComment(const QString &) override;
    void bracketComment(const QString &) override
    {
    }

    void lineFeed() override
    {
    }

    void error(const KSieve::Error &e) override;
    void finished() override;

    void taggedArgument(const QString &tag) override;

    void stringArgument(const QString &string, bool, const QString &) override;

    void numberArgument(unsigned long number, char) override;

    void stringListArgumentStart() override;
    void stringListEntry(const QString &string, bool, const QString &) override;
    void stringListArgumentEnd() override;

private:
    Context mContext = None;
    int mNotificationInterval = 0;
    QString mMessageText;
    QString mSubject;
    QStringList mAliases;
    bool mActive = true;
    bool mInIfBlock = false;
    bool mFoundInBlock = false;
    int mBlockLevel = 0;
    QString mIfComment;
    int mLineStart = 0;
    int mLineEnd = 0;

    VacationUtils::MailAction mMailAction = VacationUtils::Keep;
    Context mMailActionContext = None;
    QString mMailActionRecipient;

    void reset();
};

class RequireExtractor : public KSieve::ScriptBuilder
{
    enum Context {
        None = 0,
        // command itself:
        RequireCommand,
        EndState
    };

public:
    RequireExtractor();
    ~RequireExtractor() override;

    bool commandFound() const
    {
        return mContext == EndState;
    }

    const QStringList &requirements() const
    {
        return mRequirements;
    }

    int lineStart() const
    {
        return mLineStart;
    }

    int lineEnd() const
    {
        return mLineEnd;
    }

private:
    void commandStart(const QString &identifier, int lineNumber) override;

    void commandEnd(int lineNumber) override;

    void testStart(const QString &) override
    {
    }

    void testEnd() override
    {
    }

    void testListStart() override
    {
    }

    void testListEnd() override
    {
    }

    void blockStart(int lineNumber) override
    {
        Q_UNUSED(lineNumber)
    }

    void blockEnd(int lineNumber) override
    {
        Q_UNUSED(lineNumber)
    }

    void hashComment(const QString &) override
    {
    }

    void bracketComment(const QString &) override
    {
    }

    void lineFeed() override
    {
    }

    void error(const KSieve::Error &e) override;
    void finished() override;

    void taggedArgument(const QString &tag) override
    {
        Q_UNUSED(tag)
    }

    void numberArgument(unsigned long number, char) override
    {
        Q_UNUSED(number)
    }

    void stringArgument(const QString &string, bool, const QString &) override;

    void stringListArgumentStart() override
    {
    }

    void stringListEntry(const QString &string, bool, const QString &) override;
    void stringListArgumentEnd() override
    {
    }

private:
    Context mContext;
    QStringList mRequirements;
    int mLineStart;
    int mLineEnd;
};
}
