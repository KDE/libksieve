/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
