/*
  Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef LEGACY_VACATIONSCRIPTEXTRACTOR_H
#define LEGACY_VACATIONSCRIPTEXTRACTOR_H

#include "sieve-vacation.h"
#include "../vacationscriptextractor.h"

#include <ksieve/error.h>
#include <ksieve/parser.h>
#include <ksieve/scriptbuilder.h>

#include "libksieve_debug.h"
#include <cassert>
#include <limits.h>
#include <map>
#include <set>
#include <vector>

namespace KSieveUi
{
namespace Legacy
{

/*

This file only contains legacy code, that can be removed if the lagacy code is not needed anymore.
See README for further information.

*/
static const GenericInformationExtractor::StateNode spamNodes[] = {
    { 0, GIE::CommandStart, "if",  1, 0, nullptr },              // 0
    { 0,   GIE::TestStart, "header", 2, 0, nullptr },            // 1
    { 0,     GIE::TaggedArgument, "contains", 3, 0, nullptr },   // 2

    // accept both string and string-list:
    { 0,     GIE::StringArgument, "x-spam-flag", 9, 4, "x-spam-flag" },    // 3
    { 0,     GIE::StringListArgumentStart, nullptr, 5, 0, nullptr },                   // 4
    { 0,       GIE::StringListEntry, "x-spam-flag", 6, 7, "x-spam-flag" }, // 5
    { 0,       GIE::StringListEntry, nullptr, 6, 8, nullptr },                         // 6
    { 0,     GIE::StringListArgumentEnd, nullptr, 0, 5, nullptr },                     // 7
    { 0,     GIE::StringListArgumentEnd, nullptr, 9, 0, nullptr },                     // 8

    // accept both string and string-list:
    { 0,     GIE::StringArgument, "yes", 15, 10, "spam-flag-yes" },    // 9
    { 0,     GIE::StringListArgumentStart, nullptr, 11, 0, nullptr },              // 10
    { 0,       GIE::StringListEntry, "yes", 12, 13, "spam-flag-yes" }, // 11
    { 0,       GIE::StringListEntry, nullptr, 12, 14, nullptr },                   // 12
    { 0,     GIE::StringListArgumentEnd, nullptr, 0, 11, nullptr },                // 13
    { 0,     GIE::StringListArgumentEnd, nullptr, 15, 0, nullptr },                // 14

    { 0,   GIE::TestEnd, nullptr, 16, 0, nullptr }, // 15

    // block of command, find "stop", take nested if's into account:
    { 0,   GIE::BlockStart, nullptr, 17, 0, nullptr },                // 16
    { 1,     GIE::CommandStart, "stop", 20, 19, "stop" }, // 17
    { -1,    GIE::Any, nullptr, 17, 0, nullptr },                     // 18
    { 0,   GIE::BlockEnd, nullptr, 0, 18, nullptr },                  // 19

    { -1, GIE::Any, nullptr, 20, 20, nullptr }, // 20 end state
};
static const unsigned int numSpamNodes = sizeof spamNodes / sizeof * spamNodes;

/*

This file only contains legacy code, that can be removed if the lagacy code is not needed anymore.
See README for further information.

*/
class __attribute__((__deprecated__)) SpamDataExtractor : public GenericInformationExtractor
{
public:
    SpamDataExtractor()
        : GenericInformationExtractor(std::vector<StateNode>(spamNodes, spamNodes + numSpamNodes))
    {

    }

    bool found() const
    {
        return mResults.count(QStringLiteral("x-spam-flag")) &&
               mResults.count(QStringLiteral("spam-flag-yes")) &&
               mResults.count(QStringLiteral("stop"));
    }
};

// to understand this table, study the output of
// libksieve/tests/parsertest
//   'if not address :domain :contains ["from"] ["mydomain.org"] { keep; stop; }'
static const GenericInformationExtractor::StateNode domainNodes[] = {
    { 0, GIE::CommandStart, "if", 1, 0, nullptr },       // 0
    { 0,   GIE::TestStart, "not", 2, 0, nullptr, },      // 1
    { 0,     GIE::TestStart, "address", 3, 0, nullptr }, // 2

    // :domain and :contains in arbitrary order:
    { 0,       GIE::TaggedArgument, "domain", 4, 5, nullptr },     // 3
    { 0,       GIE::TaggedArgument, "contains", 7, 0, nullptr },   // 4
    { 0,       GIE::TaggedArgument, "contains", 6, 0, nullptr },   // 5
    { 0,       GIE::TaggedArgument, "domain", 7, 0, nullptr },     // 6

    // accept both string and string-list:
    { 0,       GIE::StringArgument, "from", 13, 8, "from" },     // 7
    { 0,       GIE::StringListArgumentStart, nullptr, 9, 0, nullptr },       // 8
    { 0,         GIE::StringListEntry, "from", 10, 11, "from" }, // 9
    { 0,         GIE::StringListEntry, nullptr, 10, 12, nullptr },           // 10
    { 0,       GIE::StringListArgumentEnd, nullptr, 0, 9, nullptr },         // 11
    { 0,       GIE::StringListArgumentEnd, nullptr, 13, 0, nullptr },        // 12

    // string: save, string-list: save last
    { 0,       GIE::StringArgument, nullptr, 17, 14, "domainName" },    // 13
    { 0,       GIE::StringListArgumentStart, nullptr, 15, 0, nullptr },       // 14
    { 0,         GIE::StringListEntry, nullptr, 15, 16, "domainName" }, // 15
    { 0,       GIE::StringListArgumentEnd, nullptr, 17, 0, nullptr },         // 16

    { 0,     GIE::TestEnd, nullptr, 18, 0, nullptr },  // 17
    { 0,   GIE::TestEnd, nullptr, 19, 0, nullptr },    // 18

    // block of commands, find "stop", take nested if's into account:
    { 0,   GIE::BlockStart, nullptr, 20, 0, nullptr },                 // 19
    { 1,     GIE::CommandStart, "stop", 23, 22, "stop" },  // 20
    { -1,    GIE::Any, nullptr, 20, 0, nullptr },                      // 21
    { 0,   GIE::BlockEnd, nullptr, 0, 21, nullptr },                   // 22

    { -1, GIE::Any, nullptr, 23, 23, nullptr }  // 23 end state
};
static const unsigned int numDomainNodes = sizeof domainNodes / sizeof * domainNodes;

/*

This file only contains legacy code, that can be removed if the lagacy code is not needed anymore.
See README for further information.

*/
class __attribute__((__deprecated__)) DomainRestrictionDataExtractor : public GenericInformationExtractor
{
public:
    DomainRestrictionDataExtractor()
        : GenericInformationExtractor(std::vector<StateNode>(domainNodes, domainNodes + numDomainNodes))
    {

    }

    QString domainName() /*not const, since map::op[] isn't const*/
    {
        return mResults.count(QStringLiteral("stop")) && mResults.count(QStringLiteral("from"))
               ? mResults[QStringLiteral("domainName")] : QString();
    }
};

// if not allof (currentDate :value "ge" date "YYYY-MM-DD",
//               currentDate :value "le" date "YYYY-MM-DD") { keep; stop; }
static const GenericInformationExtractor::StateNode datesNodes[] = {
    { 0, GIE::CommandStart, "if", 1, 0, nullptr },          // 0
    { 0,   GIE::TestStart, "not", 2, 0, nullptr },            // 1
    { 0,     GIE::TestStart, "allof", 3, 0, nullptr },        // 2

    // handle startDate and endDate in arbitrary order
    { 0,       GIE::TestListStart, nullptr, 4, 0, nullptr },                 // 3
    { 0,         GIE::TestStart, "currentdate", 5, 0, nullptr },         // 4
    { 0,           GIE::TaggedArgument, "value", 6, 0, nullptr },          // 5
    { 0,           GIE::StringArgument, "ge", 7, 9, nullptr },             // 6
    { 0,           GIE::StringArgument, "date", 8, 0, nullptr },           // 7
    { 0,           GIE::StringArgument, nullptr, 12, 0, "startDate" },      // 8
    { 0,           GIE::StringArgument, "le", 10, 0, nullptr },             // 9
    { 0,           GIE::StringArgument, "date", 11, 0, nullptr },          // 10
    { 0,           GIE::StringArgument, nullptr, 12, 0, "endDate" },       // 11
    { 0,         GIE::TestEnd, nullptr, 13, 0, nullptr },                      // 12

    { 0,         GIE::TestStart, "currentdate", 14, 0, nullptr },        // 13
    { 0,           GIE::TaggedArgument, "value", 15, 0, nullptr },         // 14
    { 0,           GIE::StringArgument, "le", 16, 18, nullptr },           // 15
    { 0,           GIE::StringArgument, "date", 17, 0, nullptr },          // 16
    { 0,           GIE::StringArgument, nullptr, 21, 0, "endDate" },       // 17
    { 0,           GIE::StringArgument, "ge", 19, 0, nullptr },            // 18
    { 0,           GIE::StringArgument, "date", 20, 0, nullptr },          // 19
    { 0,           GIE::StringArgument, nullptr, 21, 0, "startDate" },     // 20
    { 0,         GIE::TestEnd, nullptr, 22, 0, nullptr },                      // 21
    { 0,      GIE::TestListEnd, nullptr, 23, 0, nullptr },                   // 22

    { 0,     GIE::TestEnd, nullptr, 24, 0, nullptr },               // 23
    { 0,   GIE::TestEnd, nullptr, 25, 0, nullptr },                 // 24

    // block of commands, find "stop", take nested if's into account:
    { 0,   GIE::BlockStart, nullptr, 26, 0, nullptr },              // 25
    { 1,     GIE::CommandStart, "stop", 29, 28, "stop" },  // 26
    { -1,    GIE::Any, nullptr, 26, 0, nullptr },                      // 27
    { 0,   GIE::BlockEnd, nullptr, 0, 27, nullptr },                // 28

    { -1, GIE::Any, nullptr, 27, 27, nullptr }                   // 29 end state
};

static const unsigned int numDatesNodes = sizeof datesNodes / sizeof * datesNodes;

/*

This file only contains legacy code, that can be removed if the lagacy code is not needed anymore.
See README for further information.

*/
class __attribute__((__deprecated__)) DateExtractor : public GenericInformationExtractor
{
public:
    DateExtractor()
        : GenericInformationExtractor(std::vector<StateNode>(datesNodes, datesNodes + numDatesNodes))
    {
    }

    QDate endDate() const
    {
        return date(QStringLiteral("endDate"));
    }

    QDate startDate() const
    {
        return date(QStringLiteral("startDate"));
    }

private:
    QDate date(const QString &name) const
    {
        if (mResults.count(name) == 0) {
            return QDate();
        } else {
            return QDate::fromString(mResults.at(name), Qt::ISODate);
        }
    }
};

/*

This file only contains legacy code, that can be removed if the lagacy code is not needed anymore.
See README for further information.

*/
class __attribute__((__deprecated__)) VacationDataExtractor : public KSieve::ScriptBuilder
{
    enum Context {
        None = 0,
        // command itself:
        VacationCommand,
        // tagged args:
        Days, Addresses, Subject,
        VacationEnd
    };
public:
    VacationDataExtractor();
    virtual ~VacationDataExtractor();
    bool commandFound() const
    {
        return mContext == VacationEnd;
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

    const QString &subject() const
    {
        return mSubject;
    }

private:
    void commandStart(const QString &identifier, int lineNumber) Q_DECL_OVERRIDE;

    void commandEnd(int lineNumber) Q_DECL_OVERRIDE;

    void testStart(const QString &) Q_DECL_OVERRIDE {}
    void testEnd() Q_DECL_OVERRIDE {}
    void testListStart() Q_DECL_OVERRIDE {}
    void testListEnd() Q_DECL_OVERRIDE {}
    void blockStart(int lineNumber) Q_DECL_OVERRIDE {
        Q_UNUSED(lineNumber)

    }
    void blockEnd(int lineNumber) Q_DECL_OVERRIDE {
        Q_UNUSED(lineNumber)
    }
    void hashComment(const QString &) Q_DECL_OVERRIDE {}
    void bracketComment(const QString &) Q_DECL_OVERRIDE {}
    void lineFeed() Q_DECL_OVERRIDE {}
    void error(const KSieve::Error &e) Q_DECL_OVERRIDE;
    void finished() Q_DECL_OVERRIDE;

    void taggedArgument(const QString &tag) Q_DECL_OVERRIDE;

    void stringArgument(const QString &string, bool, const QString &) Q_DECL_OVERRIDE;

    void numberArgument(unsigned long number, char) Q_DECL_OVERRIDE;

    void stringListArgumentStart() Q_DECL_OVERRIDE;
    void stringListEntry(const QString &string, bool, const QString &) Q_DECL_OVERRIDE;
    void stringListArgumentEnd() Q_DECL_OVERRIDE;

private:
    Context mContext;
    int mNotificationInterval;
    QString mMessageText;
    QString mSubject;
    QStringList mAliases;

    void reset();
};
}
}

#endif // LEGACY_VACATIONSCRIPTEXTRACTOR_H
