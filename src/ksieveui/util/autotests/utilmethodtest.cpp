/*
   Copyright (C) 2016-2017 Laurent Montel <montel@kde.org>

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

#include "utilmethodtest.h"
#include "../util_p.h"
#include "imapresourcesettings.h"
#include "akonadiimapsettinginterfacetest.h"
#include "../abstractakonadiimapsettinginterface.h"
#include <KSieveUi/SieveImapAccountSettings>
#include <QTest>

UtilMethodTest::UtilMethodTest(QObject *parent)
    : QObject(parent)
{

}

void UtilMethodTest::shouldReturnEmptyInfo()
{
    std::unique_ptr<KSieveUi::AbstractAkonadiImapSettingInterface> interface(new KSieveUi::AbstractAkonadiImapSettingInterface);
    KSieveUi::Util::AccountInfo info = KSieveUi::Util::findAccountInfo(QStringLiteral("foo"), false, interface);
    QVERIFY(!info.sieveImapAccountSettings.isValid());
}

void UtilMethodTest::shouldAssignValue_data()
{
    QTest::addColumn<AkonadiImapSettingInterfaceDataTest>("data");
    QTest::addColumn<KSieveUi::Util::AccountInfo>("accountInfo");
    QTest::addColumn<bool>("sieveImapAccountValid");
    QTest::addColumn<bool>("useVacationFile");
    QTest::newRow("default") << AkonadiImapSettingInterfaceDataTest() << KSieveUi::Util::AccountInfo() << false << false;

    {
        AkonadiImapSettingInterfaceDataTest data1;
        data1.sieveSupport = true;
        data1.sieveReuseConfig = true;
        data1.imapServer = QLatin1String("foo.bla.com");
        data1.userName = QLatin1String("kde");
        data1.sievePort = 543;
        data1.sieveCustomUsername = QString();
        data1.sieveCustomAuthentification = QString();
        data1.sieveVacationFilename = QLatin1String("vacation.siv");
        data1.safety = QLatin1String("SAFETY");
        data1.alternateAuthentication = -1;
        //MailTransport::Transport::EnumAuthenticationType::CRAM_MD5 = 2
        data1.authentication = 2;
        data1.sieveAlternateUrl = QString();
        data1.password = QLatin1String("password1");
        data1.sieveCustomPassword = QString();

        KSieveUi::Util::AccountInfo info1;
        const QString password = QStringLiteral("password1");
        const QString userName = QStringLiteral("kde");
        const int port = 543;
        info1.sieveUrl = QUrl::fromUserInput(QStringLiteral("sieve://foo.bla.com?x-mech=CRAM-MD5"));
        info1.sieveUrl.setPassword(password);
        info1.sieveUrl.setUserName(userName);
        info1.sieveUrl.setPort(port);
        info1.sieveImapAccountSettings.setServerName(QLatin1String("foo.bla.com"));
        info1.sieveImapAccountSettings.setPort(port);
        info1.sieveImapAccountSettings.setUserName(userName);
        info1.sieveImapAccountSettings.setPassword(password);

        info1.sieveImapAccountSettings.setAuthenticationType(KSieveUi::SieveImapAccountSettings::CramMD5);
        //TODO fix encryptionmode
        info1.sieveImapAccountSettings.setEncryptionMode(KSieveUi::SieveImapAccountSettings::AnySslVersion);


        QTest::newRow("test1") << data1 << info1 << true << false;
    }

    {
        AkonadiImapSettingInterfaceDataTest data1;
        data1.sieveSupport = true;
        data1.sieveReuseConfig = true;
        data1.imapServer = QLatin1String("foo.bla.com");
        data1.userName = QLatin1String("kde");
        data1.sievePort = 3;
        data1.sieveCustomUsername = QString();
        data1.sieveCustomAuthentification = QString();
        data1.sieveVacationFilename = QLatin1String("vacation.siv");
        data1.safety = QLatin1String("SAFETY");
        data1.alternateAuthentication = -1;
        //MailTransport::Transport::EnumAuthenticationType::PLAIN = 0
        data1.authentication = 0;
        data1.sieveAlternateUrl = QString();
        data1.password = QLatin1String("password2");
        data1.sieveCustomPassword = QString();

        KSieveUi::Util::AccountInfo info1;
        const QString password = QStringLiteral("password2");
        const QString userName = QStringLiteral("kde");
        const int port = 3;
        info1.sieveUrl = QUrl::fromUserInput(QStringLiteral("sieve://foo.bla.com/vacation.siv?x-mech=LOGIN"));
        info1.sieveUrl.setPassword(password);
        info1.sieveUrl.setUserName(userName);
        info1.sieveUrl.setPort(port);
        info1.sieveImapAccountSettings.setServerName(QLatin1String("foo.bla.com"));
        info1.sieveImapAccountSettings.setPort(port);
        info1.sieveImapAccountSettings.setUserName(userName);
        info1.sieveImapAccountSettings.setPassword(password);

        info1.sieveImapAccountSettings.setAuthenticationType(KSieveUi::SieveImapAccountSettings::Login);
        //TODO fix encryptionmode
        info1.sieveImapAccountSettings.setEncryptionMode(KSieveUi::SieveImapAccountSettings::AnySslVersion);


        QTest::newRow("test2") << data1 << info1 << true << true;
    }
}

void UtilMethodTest::shouldAssignValue()
{
    QFETCH(AkonadiImapSettingInterfaceDataTest, data);
    QFETCH(KSieveUi::Util::AccountInfo, accountInfo);
    QFETCH(bool, sieveImapAccountValid);
    QFETCH(bool, useVacationFile);

    std::unique_ptr<KSieveUi::AbstractAkonadiImapSettingInterface> interface(new AkonadiImapSettingInterfaceTest(data));
    const KSieveUi::Util::AccountInfo info = KSieveUi::Util::findAccountInfo(QStringLiteral("foo"), useVacationFile, interface);
    QCOMPARE(info.sieveImapAccountSettings.isValid(), sieveImapAccountValid);
    QCOMPARE(info, accountInfo);
}

QTEST_MAIN(UtilMethodTest)
