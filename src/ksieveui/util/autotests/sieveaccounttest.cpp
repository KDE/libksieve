/*
   Copyright (C) 2016 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "sieveaccounttest.h"
#include "../sieveaccount.h"

#include <QTest>

SieveAccountTest::SieveAccountTest(QObject *parent)
    : QObject(parent)
{

}

SieveAccountTest::~SieveAccountTest()
{

}

void SieveAccountTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveAccount account;
    QVERIFY(account.password().isEmpty());
    QVERIFY(account.serverName().isEmpty());
    QVERIFY(account.userName().isEmpty());
    QCOMPARE(account.port(), -1);
    QCOMPARE(account.authenticationType(), MailTransport::Transport::EnumAuthenticationType::PLAIN);
}

void SieveAccountTest::shouldAssignValue()
{
    KSieveUi::SieveAccount account;
    QString pwd = QStringLiteral("foo");
    QString server = QStringLiteral("kde");
    QString user = QStringLiteral("bla");
    int port = 42;
    MailTransport::Transport::EnumAuthenticationType::type type = MailTransport::Transport::EnumAuthenticationType::CRAM_MD5;
    account.setPassword(pwd);
    account.setServerName(server);
    account.setUserName(user);
    account.setPort(port);
    account.setAuthenticationType(type);
    QCOMPARE(account.password(), pwd);
    QCOMPARE(account.serverName(), server);
    QCOMPARE(account.userName(), user);
    QCOMPARE(account.port(), port);
    QCOMPARE(account.authenticationType(), type);
}

void SieveAccountTest::shouldBeEqual()
{
    KSieveUi::SieveAccount account;
    QString pwd = QStringLiteral("foo");
    QString server = QStringLiteral("kde");
    QString user = QStringLiteral("bla");
    int port = 42;
    MailTransport::Transport::EnumAuthenticationType::type type = MailTransport::Transport::EnumAuthenticationType::CRAM_MD5;
    account.setPassword(pwd);
    account.setServerName(server);
    account.setUserName(user);
    account.setPort(port);
    account.setAuthenticationType(type);

    KSieveUi::SieveAccount accountB;
    accountB = account;
    QCOMPARE(account, accountB);
}

QTEST_MAIN(SieveAccountTest)
