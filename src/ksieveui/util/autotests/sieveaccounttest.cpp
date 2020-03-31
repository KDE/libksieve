/*
   Copyright (C) 2016-2020 Laurent Montel <montel@kde.org>

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
#include "../sieveimapaccountsettings.h"

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
    KSieveUi::SieveImapAccountSettings account;
    QVERIFY(account.password().isEmpty());
    QVERIFY(account.serverName().isEmpty());
    QVERIFY(account.userName().isEmpty());
    QCOMPARE(account.port(), -1);
    QCOMPARE(account.authenticationType(), KSieveUi::SieveImapAccountSettings::Plain);
    QCOMPARE(account.encryptionMode(), KSieveUi::SieveImapAccountSettings::Unencrypted);
}

void SieveAccountTest::shouldAssignValue()
{
    KSieveUi::SieveImapAccountSettings account;
    QString pwd = QStringLiteral("foo");
    QString server = QStringLiteral("kde");
    QString user = QStringLiteral("bla");
    int port = 42;
    KSieveUi::SieveImapAccountSettings::AuthenticationMode type = KSieveUi::SieveImapAccountSettings::DigestMD5;
    KSieveUi::SieveImapAccountSettings::EncryptionMode mode = KSieveUi::SieveImapAccountSettings::SSLorTLS;
    account.setPassword(pwd);
    account.setServerName(server);
    account.setUserName(user);
    account.setPort(port);
    account.setAuthenticationType(type);
    account.setEncryptionMode(mode);
    QCOMPARE(account.password(), pwd);
    QCOMPARE(account.serverName(), server);
    QCOMPARE(account.userName(), user);
    QCOMPARE(account.port(), port);
    QCOMPARE(account.authenticationType(), type);
    QCOMPARE(account.encryptionMode(), mode);
}

void SieveAccountTest::shouldBeEqual()
{
    KSieveUi::SieveImapAccountSettings account;
    QString pwd = QStringLiteral("foo");
    QString server = QStringLiteral("kde");
    QString user = QStringLiteral("bla");
    int port = 42;
    KSieveUi::SieveImapAccountSettings::AuthenticationMode type = KSieveUi::SieveImapAccountSettings::DigestMD5;
    KSieveUi::SieveImapAccountSettings::EncryptionMode mode = KSieveUi::SieveImapAccountSettings::SSLorTLS;
    account.setPassword(pwd);
    account.setServerName(server);
    account.setUserName(user);
    account.setPort(port);
    account.setAuthenticationType(type);
    account.setEncryptionMode(mode);

    KSieveUi::SieveImapAccountSettings accountB;
    accountB = account;
    QCOMPARE(account, accountB);
}

void SieveAccountTest::shouldCreateIdentifier_data()
{
    QTest::addColumn<QString>("username");
    QTest::addColumn<QString>("servername");
    QTest::addColumn<QString>("output");

    QTest::newRow("empty") << QString() << QString() << QStringLiteral("_");
    QTest::newRow("test1") << QStringLiteral("foo") << QStringLiteral("bla") << QStringLiteral("foo_bla");
}

void SieveAccountTest::shouldCreateIdentifier()
{
    QFETCH(QString, username);
    QFETCH(QString, servername);
    QFETCH(QString, output);
    KSieveUi::SieveImapAccountSettings account;
    account.setServerName(servername);
    account.setUserName(username);

    //Identifier doesn't depend against theses settings
    KSieveUi::SieveImapAccountSettings::AuthenticationMode type = KSieveUi::SieveImapAccountSettings::DigestMD5;
    KSieveUi::SieveImapAccountSettings::EncryptionMode mode = KSieveUi::SieveImapAccountSettings::SSLorTLS;
    account.setPort(42);
    account.setAuthenticationType(type);
    account.setEncryptionMode(mode);

    QCOMPARE(account.identifier(), output);
}

QTEST_MAIN(SieveAccountTest)
