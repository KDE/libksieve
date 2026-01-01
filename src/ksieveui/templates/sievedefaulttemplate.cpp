/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievedefaulttemplate.h"
#include <KSieveCore/VacationUtils>

#include <KLocalizedString>

QList<PimCommon::defaultTemplate> KSieveUi::SieveDefaultTemplate::defaultTemplates()
{
    QList<PimCommon::defaultTemplate> lst;
    PimCommon::defaultTemplate tmp;
    tmp.name = i18n("Filter on Mailing List-ID");
    tmp.text = QStringLiteral(
        "require \"fileinto\";\n"
        "if header :contains \"List-ID\" [ \"examples.com\", \"examples.mail.com\" ] {\n"
        "    fileinto \"list-example/examples\"; \n"
        "    stop;\n"
        "}\n");
    lst << tmp;

    tmp.name = i18n("Filter on Subject");
    tmp.text = QStringLiteral(
        "require \"fileinto\";\n"
        "if header :contains \"Subject\" \"Foo Foo\" { \n"
        "    fileinto \"INBOX.Foo\"; \n"
        "}\n");
    lst << tmp;

    tmp.name = i18n("Filter on Spamassassin");
    tmp.text = QStringLiteral(
        "require \"fileinto\";\n"
        "if header :contains \"X-Spam-Level\" \"*********\" { \n"
        "    fileinto \"Spam\";\n"
        "}\n");
    lst << tmp;

    tmp.name = i18n("Flag messages");
    tmp.text = QStringLiteral(
        "require \"imap4flags\";\n"
        "if address \"From\" \"someone@example.org\" { \n"
        "    setflag \"\\\\Seen\";\n"
        "}\n");
    lst << tmp;

    tmp.name = i18n("Forward Message");
    tmp.text = QStringLiteral(
        "require \"copy\";\n"
        "if header :contains \"Subject\" \"foo\" { \n"
        "    redirect :copy \"other@example.net\";\n"
        "}\n");
    lst << tmp;

    tmp.name = i18n("Forward Message and add copy");
    tmp.text = QStringLiteral(
        "require [\"copy\", \"fileinto\"];\n"
        "if header :contains \"Subject\" \"foo\" { \n"
        "    redirect :copy \"other@example.net\";\n"
        "    fileinto \"Forwarded Messages\"; \n"
        "}\n");
    lst << tmp;

    tmp.name = i18n("Destroy mail posted by…");
    tmp.text = QStringLiteral(
        "if header :contains [\"from\",\"cc\"]\n"
        "[\n"
        "\"from-foo@example.net\",\n"
        "\"pub@foo.com\"\n"
        "]\n"
        "{\n"
        "    discard;\n"
        "    stop;\n"
        "}\n");
    lst << tmp;

    tmp.name = i18n("Vacations");

    tmp.text = QStringLiteral(
                   "require \"vacation\";\n\n"
                   "if header :contains \"X-Spam-Flag\" \"YES\" { keep; stop; }\n"
                   "vacation :addresses [ \"me@example.net\", \"other@example.net\" ] :days 7 text: \n%1"
                   "\n.\n;\n")
                   .arg(KSieveCore::VacationUtils::defaultMessageText());
    lst << tmp;

    return lst;
}
