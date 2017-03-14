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
    QTest::newRow("default") << AkonadiImapSettingInterfaceDataTest() << KSieveUi::Util::AccountInfo() << false;
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
    data1.authentication = 2;
    data1.sieveAlternateUrl = QString();
    data1.password = QLatin1String("password1");
    data1.sieveCustomPassword = QString();

    KSieveUi::Util::AccountInfo info1;
    info1.sieveUrl = QUrl::fromUserInput(QStringLiteral("sieve://kde:password1@foo.bla.com:543?x-mech=CRAM-MD5"));
    info1.sieveImapAccountSettings.setServerName(QLatin1String("foo.bla.com"));
    info1.sieveImapAccountSettings.setPort(543);
    info1.sieveImapAccountSettings.setUserName(QLatin1String("kde"));
    info1.sieveImapAccountSettings.setPassword(QLatin1String("password1"));
    info1.sieveImapAccountSettings.setAuthenticationType(KSieveUi::SieveImapAccountSettings::Plain);
    //TODO fix encryptionmode
    info1.sieveImapAccountSettings.setEncryptionMode(KSieveUi::SieveImapAccountSettings::AnySslVersion);


    QTest::newRow("test1") << data1 << info1 << true;
}

void UtilMethodTest::shouldAssignValue()
{
    QFETCH(AkonadiImapSettingInterfaceDataTest, data);
    QFETCH(KSieveUi::Util::AccountInfo, accountInfo);
    QFETCH(bool, sieveImapAccountValid);

    std::unique_ptr<KSieveUi::AbstractAkonadiImapSettingInterface> interface(new AkonadiImapSettingInterfaceTest(data));
    const KSieveUi::Util::AccountInfo info = KSieveUi::Util::findAccountInfo(QStringLiteral("foo"), false, interface);
    //TODO
    QCOMPARE(info.sieveImapAccountSettings.isValid(), sieveImapAccountValid);
    QCOMPARE(info, accountInfo);
}

QTEST_MAIN(UtilMethodTest)
