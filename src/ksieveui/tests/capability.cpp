/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "capability.h"
using namespace Qt::Literals::StringLiterals;

QStringList KSieveUi::Test::fullCapabilitiesList()
{
    return QStringList() << u"mboxmetadata"_s << u"body"_s << QStringLiteral("extlists") << QStringLiteral("envelope") << u"redirect"_s << u"fileinto"_s
                         << QStringLiteral("editheader") << QStringLiteral("reject") << u"ereject"_s << u"imapflags"_s << QStringLiteral("imap4flags")
                         << QStringLiteral("enotify") << u"date"_s << u"copy"_s << QStringLiteral("mailbox") << QStringLiteral("spamtest") << u"spamtestplus"_s
                         << u"virustest"_s << QStringLiteral("vacation") << QStringLiteral("vacation-seconds") << u"ihave"_s << u"subaddress"_s
                         << QStringLiteral("environment") << QStringLiteral("enclose") << u"replace"_s << u"include"_s << QStringLiteral("extracttext")
                         << QStringLiteral("metadata") << u"convert"_s << u"foreverypart"_s << QStringLiteral("variables") << QStringLiteral("servermetadata")
                         << u"regex"_s << u"relational"_s << QStringLiteral("comparator-i;ascii-numeric") << u"comparator-i;unicode-casemap"_s
                         << u"encoded-character"_s;
}
