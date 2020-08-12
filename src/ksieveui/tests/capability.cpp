/*
  SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "capability.h"

QStringList KSieveUi::Test::fullCapabilitiesList()
{
    return QStringList()
           << QStringLiteral("mboxmetadata")
           << QStringLiteral("body")
           << QStringLiteral("extlists")
           << QStringLiteral("envelope")
           << QStringLiteral("redirect")
           << QStringLiteral("fileinto")
           << QStringLiteral("editheader")
           << QStringLiteral("reject")
           << QStringLiteral("ereject")
           << QStringLiteral("imapflags")
           << QStringLiteral("imap4flags")
           << QStringLiteral("enotify")
           << QStringLiteral("date")
           << QStringLiteral("copy")
           << QStringLiteral("mailbox")
           << QStringLiteral("spamtest")
           << QStringLiteral("spamtestplus")
           << QStringLiteral("virustest")
           << QStringLiteral("vacation")
           << QStringLiteral("vacation-seconds")
           << QStringLiteral("ihave")
           << QStringLiteral("subaddress")
           << QStringLiteral("environment")
           << QStringLiteral("enclose")
           << QStringLiteral("replace")
           << QStringLiteral("include")
           << QStringLiteral("extracttext")
           << QStringLiteral("metadata")
           << QStringLiteral("convert")
           << QStringLiteral("foreverypart")
           << QStringLiteral("variables")
           << QStringLiteral("servermetadata")
           << QStringLiteral("regex")
           << QStringLiteral("relational")
           << QStringLiteral("comparator-i;ascii-numeric")
           << QStringLiteral("comparator-i;unicode-casemap")
           << QStringLiteral("encoded-character");
}
