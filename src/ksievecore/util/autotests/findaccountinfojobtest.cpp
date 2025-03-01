/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "findaccountinfojobtest.h"
#include "../abstractakonadiimapsettinginterface.h"
#include "../sieveimappasswordprovider.h"
#include "../util.h"
#include "akonadiimapsettinginterfacetest.h"
#include "util/findaccountinfojob.h"
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(FindAccountInfoJobTest)

class DataImapPasswordProvider : public KSieveCore::SieveImapPasswordProvider
{
public:
    DataImapPasswordProvider(AkonadiImapSettingInterfaceDataTest *data)
        : mData(data)
    {
    }

    void passwords(const QString &identifier) override
    {
        Q_UNUSED(identifier)
        Q_EMIT passwordsRequested(mData->password, mData->sieveCustomPassword);
    }

    AkonadiImapSettingInterfaceDataTest *const mData;
};

FindAccountInfoJobTest::FindAccountInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void FindAccountInfoJobTest::shouldReturnEmptyInfo()
{
    auto job = new KSieveCore::FindAccountInfoJob;
    job->setIdentifier(QStringLiteral("dummy"));
    job->setProvider({});
    QSignalSpy spy(job, &KSieveCore::FindAccountInfoJob::findAccountInfoFinished);
    job->start();
    spy.wait(1000);
    QCOMPARE(spy.count(), 1);
    const auto info = spy.at(0).at(0).value<KSieveCore::Util::AccountInfo>();
    QVERIFY(!info.sieveImapAccountSettings.isValid());
}

void FindAccountInfoJobTest::shouldAssignValue_data()
{
    QTest::addColumn<AkonadiImapSettingInterfaceDataTest>("data");
    QTest::addColumn<KSieveCore::Util::AccountInfo>("accountInfo");
    QTest::addColumn<bool>("sieveImapAccountValid");
    QTest::addColumn<bool>("useVacationFile");
    QTest::newRow("default") << AkonadiImapSettingInterfaceDataTest() << KSieveCore::Util::AccountInfo() << false << false;

    {
        const int imapPort = 53;
        AkonadiImapSettingInterfaceDataTest data1;
        data1.sieveSupport = true;
        data1.sieveReuseConfig = true;
        data1.imapServer = QStringLiteral("foo.bla.com");
        data1.userName = QStringLiteral("kde");
        data1.sievePort = 543;
        data1.imapPort = imapPort;
        data1.sieveCustomUsername = QString();
        data1.sieveCustomAuthentification = QString();
        data1.sieveVacationFilename = QStringLiteral("vacation.siv");
        data1.safety = QStringLiteral("SAFETY");
        data1.alternateAuthentication = -1;
        // MailTransport::Transport::EnumAuthenticationType::CRAM_MD5 = 2
        data1.authentication = 2;
        data1.sieveAlternateUrl = QString();
        data1.password = QStringLiteral("password1");
        data1.sieveCustomPassword = QString();

        KSieveCore::Util::AccountInfo info1;
        const QString password = QStringLiteral("password1");
        const QString userName = QStringLiteral("kde");
        const int port = 543;
        info1.sieveUrl = QUrl::fromUserInput(QStringLiteral("sieve://foo.bla.com?x-mech=CRAM-MD5"));
        info1.sieveUrl.setPassword(password);
        info1.sieveUrl.setUserName(userName);
        info1.sieveUrl.setPort(port);
        info1.sieveImapAccountSettings.setServerName(QStringLiteral("foo.bla.com"));
        info1.sieveImapAccountSettings.setPort(imapPort);
        info1.sieveImapAccountSettings.setUserName(userName);
        info1.sieveImapAccountSettings.setPassword(password);

        info1.sieveImapAccountSettings.setAuthenticationType(KSieveCore::SieveImapAccountSettings::Plain);
        // TODO fix encryptionmode
        info1.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::Unencrypted);

        QTest::newRow("sievereuseconfig1") << data1 << info1 << true << false;
    }

    {
        const int imapPort = 53;
        AkonadiImapSettingInterfaceDataTest data1;
        data1.sieveSupport = true;
        data1.sieveReuseConfig = true;
        data1.imapServer = QStringLiteral("foo.bla.com");
        data1.userName = QStringLiteral("kde");
        data1.sievePort = 3;
        data1.imapPort = imapPort;
        data1.sieveCustomUsername = QString();
        data1.sieveCustomAuthentification = QString();
        data1.sieveVacationFilename = QStringLiteral("vacation.siv");
        data1.safety = QStringLiteral("SAFETY");
        data1.alternateAuthentication = -1;
        // MailTransport::Transport::EnumAuthenticationType::PLAIN = 0
        data1.authentication = 0;
        data1.sieveAlternateUrl = QString();
        data1.password = QStringLiteral("password2");
        data1.sieveCustomPassword = QString();

        KSieveCore::Util::AccountInfo info1;
        const QString password = QStringLiteral("password2");
        const QString userName = QStringLiteral("kde");
        const int port = 3;
        info1.sieveUrl = QUrl::fromUserInput(QStringLiteral("sieve://foo.bla.com/vacation.siv?x-mech=LOGIN"));
        info1.sieveUrl.setPassword(password);
        info1.sieveUrl.setUserName(userName);
        info1.sieveUrl.setPort(port);
        info1.sieveImapAccountSettings.setServerName(QStringLiteral("foo.bla.com"));
        info1.sieveImapAccountSettings.setPort(imapPort);
        info1.sieveImapAccountSettings.setUserName(userName);
        info1.sieveImapAccountSettings.setPassword(password);

        info1.sieveImapAccountSettings.setAuthenticationType(KSieveCore::SieveImapAccountSettings::ClearText);
        // TODO fix encryptionmode
        info1.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::Unencrypted);

        QTest::newRow("sievereuseconfig2") << data1 << info1 << true << true;
    }
    {
        const int imapPort = 53;
        AkonadiImapSettingInterfaceDataTest data1;
        data1.sieveSupport = true;
        data1.sieveReuseConfig = true;
        data1.imapServer = QStringLiteral("foo.bla.com");
        data1.userName = QStringLiteral("kde");
        data1.sievePort = 3;
        data1.imapPort = imapPort;
        data1.sieveCustomUsername = QString();
        data1.sieveCustomAuthentification = QString();
        data1.sieveVacationFilename = QStringLiteral("vacation.siv");
        data1.safety = QStringLiteral("SSL");
        data1.alternateAuthentication = -1;
        // MailTransport::Transport::EnumAuthenticationType::PLAIN = 0
        data1.authentication = 0;
        data1.sieveAlternateUrl = QString();
        data1.password = QStringLiteral("password2");
        data1.sieveCustomPassword = QString();

        KSieveCore::Util::AccountInfo info1;
        const QString password = QStringLiteral("password2");
        const QString userName = QStringLiteral("kde");
        const int port = 3;
        info1.sieveUrl = QUrl::fromUserInput(QStringLiteral("sieve://foo.bla.com/vacation.siv?x-mech=LOGIN"));
        info1.sieveUrl.setPassword(password);
        info1.sieveUrl.setUserName(userName);
        info1.sieveUrl.setPort(port);
        info1.sieveImapAccountSettings.setServerName(QStringLiteral("foo.bla.com"));
        info1.sieveImapAccountSettings.setPort(imapPort);
        info1.sieveImapAccountSettings.setUserName(userName);
        info1.sieveImapAccountSettings.setPassword(password);

        info1.sieveImapAccountSettings.setAuthenticationType(KSieveCore::SieveImapAccountSettings::ClearText);
        // TODO fix encryptionmode
        info1.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::SSLorTLS);

        QTest::newRow("sievereuseconfig2ssl") << data1 << info1 << true << true;
    }

    {
        const int imapPort = 53;
        AkonadiImapSettingInterfaceDataTest data1;
        data1.sieveSupport = true;
        data1.sieveReuseConfig = true;
        data1.imapServer = QStringLiteral("foo.bla.com");
        data1.userName = QStringLiteral("kde");
        data1.sievePort = 3;
        data1.imapPort = imapPort;
        data1.sieveCustomUsername = QString();
        data1.sieveCustomAuthentification = QString();
        data1.sieveVacationFilename = QStringLiteral("vacation.siv");
        data1.safety = QStringLiteral("STARTTLS");
        data1.alternateAuthentication = -1;
        // MailTransport::Transport::EnumAuthenticationType::PLAIN = 0
        data1.authentication = 0;
        data1.sieveAlternateUrl = QString();
        data1.password = QStringLiteral("password2");
        data1.sieveCustomPassword = QString();

        KSieveCore::Util::AccountInfo info1;
        const QString password = QStringLiteral("password2");
        const QString userName = QStringLiteral("kde");
        const int port = 3;
        info1.sieveUrl = QUrl::fromUserInput(QStringLiteral("sieve://foo.bla.com/vacation.siv?x-mech=LOGIN"));
        info1.sieveUrl.setPassword(password);
        info1.sieveUrl.setUserName(userName);
        info1.sieveUrl.setPort(port);
        info1.sieveImapAccountSettings.setServerName(QStringLiteral("foo.bla.com"));
        info1.sieveImapAccountSettings.setPort(imapPort);
        info1.sieveImapAccountSettings.setUserName(userName);
        info1.sieveImapAccountSettings.setPassword(password);

        info1.sieveImapAccountSettings.setAuthenticationType(KSieveCore::SieveImapAccountSettings::ClearText);
        // TODO fix encryptionmode
        info1.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::STARTTLS);

        QTest::newRow("sievereuseconfig2startls") << data1 << info1 << true << true;
    }

    {
        const int imapPort = 53;
        AkonadiImapSettingInterfaceDataTest data1;
        data1.sieveSupport = false;
        data1.sieveReuseConfig = true;
        data1.imapServer = QStringLiteral("foo.bla.com");
        data1.userName = QStringLiteral("kde");
        data1.sievePort = 3;
        data1.imapPort = imapPort;
        data1.sieveCustomUsername = QString();
        data1.sieveCustomAuthentification = QString();
        data1.sieveVacationFilename = QStringLiteral("vacation.siv");
        data1.safety = QStringLiteral("SAFETY");
        data1.alternateAuthentication = -1;
        // MailTransport::Transport::EnumAuthenticationType::PLAIN = 0
        data1.authentication = 0;
        data1.sieveAlternateUrl = QString();
        data1.password = QStringLiteral("password2");
        data1.sieveCustomPassword = QString();

        KSieveCore::Util::AccountInfo info1;
        info1.sieveImapAccountSettings.setAuthenticationType(KSieveCore::SieveImapAccountSettings::Plain);
        info1.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::Unencrypted);

        QTest::newRow("dontusesieve") << data1 << info1 << false << true;
    }

    {
        const QString sievePasswd = QStringLiteral("password3");
        const QString imapPasswd = QStringLiteral("password2");
        const QString imapUser = QStringLiteral("kde");
        const QString sieveUser = QStringLiteral("foo1");
        const int sievePort = 3;
        const int imapPort = 53;
        AkonadiImapSettingInterfaceDataTest data1;
        data1.sieveSupport = true;
        data1.sieveReuseConfig = false;
        data1.imapServer = QStringLiteral("foo.bla.com");
        data1.userName = imapUser;
        data1.sievePort = sievePort;
        data1.imapPort = imapPort;
        data1.sieveCustomUsername = sieveUser;
        data1.sieveCustomAuthentification = QStringLiteral("CustomUserPassword");
        data1.sieveVacationFilename = QStringLiteral("vacation.siv");
        data1.safety = QStringLiteral("SAFETY");
        data1.alternateAuthentication = 4;
        // MailTransport::Transport::EnumAuthenticationType::PLAIN = 0
        data1.authentication = 3;
        data1.sieveAlternateUrl = QStringLiteral("bla.bla.com");
        data1.password = imapPasswd;
        data1.sieveCustomPassword = sievePasswd;

        KSieveCore::Util::AccountInfo info1;
        info1.sieveUrl = QUrl::fromUserInput(QStringLiteral("sieve://bla.bla.com/vacation.siv?x-mech=GSSAPI"));
        info1.sieveUrl.setPassword(sievePasswd);
        info1.sieveUrl.setUserName(sieveUser);
        info1.sieveUrl.setPort(sievePort);
        info1.sieveImapAccountSettings.setServerName(QStringLiteral("foo.bla.com"));
        info1.sieveImapAccountSettings.setPort(imapPort);
        info1.sieveImapAccountSettings.setUserName(imapUser);
        info1.sieveImapAccountSettings.setPassword(imapPasswd);

        info1.sieveImapAccountSettings.setAuthenticationType(KSieveCore::SieveImapAccountSettings::CramMD5);

        info1.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::Unencrypted);

        QTest::newRow("sievereusecustomconfig1") << data1 << info1 << true << true;
    }
    {
        const QString sievePasswd = QStringLiteral("password3");
        const QString imapPasswd = QStringLiteral("password2");
        const QString imapUser = QStringLiteral("kde");
        const QString sieveUser = QStringLiteral("foo1");
        const int sievePort = 3;
        const int imapPort = 53;
        AkonadiImapSettingInterfaceDataTest data1;
        data1.sieveSupport = true;
        data1.sieveReuseConfig = false;
        data1.imapServer = QStringLiteral("foo.bla.com");
        data1.userName = imapUser;
        data1.sievePort = sievePort;
        data1.imapPort = imapPort;
        data1.sieveCustomUsername = sieveUser;
        data1.sieveCustomAuthentification = QStringLiteral("CustomUserPassword");
        data1.sieveVacationFilename = QStringLiteral("vacation.siv");
        data1.safety = QStringLiteral("SAFETY");
        data1.alternateAuthentication = 4;
        // MailTransport::Transport::EnumAuthenticationType::PLAIN = 0
        data1.authentication = 3;
        data1.sieveAlternateUrl = QStringLiteral("bla.bla.com");
        data1.password = imapPasswd;
        data1.sieveCustomPassword = sievePasswd;

        KSieveCore::Util::AccountInfo info1;
        info1.sieveUrl = QUrl::fromUserInput(QStringLiteral("sieve://bla.bla.com?x-mech=GSSAPI"));
        info1.sieveUrl.setPassword(sievePasswd);
        info1.sieveUrl.setUserName(sieveUser);
        info1.sieveUrl.setPort(sievePort);
        info1.sieveImapAccountSettings.setServerName(QStringLiteral("foo.bla.com"));
        info1.sieveImapAccountSettings.setPort(imapPort);
        info1.sieveImapAccountSettings.setUserName(imapUser);
        info1.sieveImapAccountSettings.setPassword(imapPasswd);

        info1.sieveImapAccountSettings.setAuthenticationType(KSieveCore::SieveImapAccountSettings::CramMD5);

        info1.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::Unencrypted);

        QTest::newRow("sievereusecustomconfig1withoutvacationfile") << data1 << info1 << true << false;
    }

    {
        const QString sievePasswd = QStringLiteral("password3");
        const QString imapPasswd = QStringLiteral("password2");
        const QString imapUser = QStringLiteral("kde");
        const QString sieveUser = QStringLiteral("foo1");
        const int sievePort = 3;
        const int imapPort = 53;
        AkonadiImapSettingInterfaceDataTest data1;
        data1.sieveSupport = true;
        data1.sieveReuseConfig = false;
        data1.imapServer = QStringLiteral("foo.bla.com");
        data1.userName = imapUser;
        data1.sievePort = sievePort;
        data1.imapPort = imapPort;
        data1.sieveCustomUsername = sieveUser;
        data1.sieveCustomAuthentification = QStringLiteral("CustomUserPassword");
        data1.sieveVacationFilename = QStringLiteral("vacation.siv");
        data1.safety = QStringLiteral("SSL");
        data1.alternateAuthentication = 4;
        // MailTransport::Transport::EnumAuthenticationType::PLAIN = 0
        data1.authentication = 3;
        data1.sieveAlternateUrl = QStringLiteral("bla.bla.com");
        data1.password = imapPasswd;
        data1.sieveCustomPassword = sievePasswd;

        KSieveCore::Util::AccountInfo info1;
        info1.sieveUrl = QUrl::fromUserInput(QStringLiteral("sieve://bla.bla.com?x-mech=GSSAPI"));
        info1.sieveUrl.setPassword(sievePasswd);
        info1.sieveUrl.setUserName(sieveUser);
        info1.sieveUrl.setPort(sievePort);
        info1.sieveImapAccountSettings.setServerName(QStringLiteral("foo.bla.com"));
        info1.sieveImapAccountSettings.setPort(imapPort);
        info1.sieveImapAccountSettings.setUserName(imapUser);
        info1.sieveImapAccountSettings.setPassword(imapPasswd);

        info1.sieveImapAccountSettings.setAuthenticationType(KSieveCore::SieveImapAccountSettings::CramMD5);

        info1.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::SSLorTLS);

        QTest::newRow("sievereusecustomconfig1withoutvacationfileSSL") << data1 << info1 << true << false;
    }

    {
        const QString sievePasswd = QStringLiteral("password3");
        const QString imapPasswd = QStringLiteral("password2");
        const QString imapUser = QStringLiteral("kde");
        const QString sieveUser = QStringLiteral("foo1");
        const int sievePort = 3;
        const int imapPort = 53;
        AkonadiImapSettingInterfaceDataTest data1;
        data1.sieveSupport = true;
        data1.sieveReuseConfig = false;
        data1.imapServer = QStringLiteral("foo.bla.com");
        data1.userName = imapUser;
        data1.sievePort = sievePort;
        data1.imapPort = imapPort;
        data1.sieveCustomUsername = sieveUser;
        data1.sieveCustomAuthentification = QStringLiteral("CustomUserPassword");
        data1.sieveVacationFilename = QStringLiteral("vacation.siv");
        data1.safety = QStringLiteral("STARTTLS");
        data1.alternateAuthentication = 4;
        // MailTransport::Transport::EnumAuthenticationType::DigestMD5 = 3
        data1.authentication = 3;
        data1.sieveAlternateUrl = QStringLiteral("bla.bla.com");
        data1.password = imapPasswd;
        data1.sieveCustomPassword = sievePasswd;

        KSieveCore::Util::AccountInfo info1;
        info1.sieveUrl = QUrl::fromUserInput(QStringLiteral("sieve://bla.bla.com?x-mech=GSSAPI"));
        info1.sieveUrl.setPassword(sievePasswd);
        info1.sieveUrl.setUserName(sieveUser);
        info1.sieveUrl.setPort(sievePort);
        info1.sieveImapAccountSettings.setServerName(QStringLiteral("foo.bla.com"));
        info1.sieveImapAccountSettings.setPort(imapPort);
        info1.sieveImapAccountSettings.setUserName(imapUser);
        info1.sieveImapAccountSettings.setPassword(imapPasswd);

        info1.sieveImapAccountSettings.setAuthenticationType(KSieveCore::SieveImapAccountSettings::CramMD5);

        info1.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::STARTTLS);

        QTest::newRow("sievereusecustomconfig1withoutvacationfileStarttls") << data1 << info1 << true << false;
    }

    {
        const QString sievePasswd = QStringLiteral("password3");
        const QString imapPasswd = QStringLiteral("password2");
        const QString imapUser = QStringLiteral("kde");
        const QString sieveUser = QStringLiteral("foo1");
        const int sievePort = 3;
        const int imapPort = 53;
        AkonadiImapSettingInterfaceDataTest data1;
        data1.sieveSupport = true;
        data1.sieveReuseConfig = false;
        data1.imapServer = QStringLiteral("foo.bla.com");
        data1.userName = imapUser;
        data1.sievePort = sievePort;
        data1.imapPort = imapPort;
        data1.sieveCustomUsername = sieveUser;
        data1.sieveCustomAuthentification = QStringLiteral("CustomUserPassword");
        data1.sieveVacationFilename = QStringLiteral("vacation.siv");
        data1.safety = QStringLiteral("STARTTLS");
        data1.alternateAuthentication = 4;
        // MailTransport::Transport::EnumAuthenticationType::DigestMD5 = 3
        data1.authentication = 4;
        data1.sieveAlternateUrl = QStringLiteral("bla.bla.com");
        data1.password = imapPasswd;
        data1.sieveCustomPassword = sievePasswd;

        KSieveCore::Util::AccountInfo info1;
        info1.sieveUrl = QUrl::fromUserInput(QStringLiteral("sieve://bla.bla.com?x-mech=GSSAPI"));
        info1.sieveUrl.setPassword(sievePasswd);
        info1.sieveUrl.setUserName(sieveUser);
        info1.sieveUrl.setPort(sievePort);
        info1.sieveImapAccountSettings.setServerName(QStringLiteral("foo.bla.com"));
        info1.sieveImapAccountSettings.setPort(imapPort);
        info1.sieveImapAccountSettings.setUserName(imapUser);
        info1.sieveImapAccountSettings.setPassword(imapPasswd);

        info1.sieveImapAccountSettings.setAuthenticationType(KSieveCore::SieveImapAccountSettings::DigestMD5);

        info1.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::STARTTLS);

        QTest::newRow("sievereusecustomconfig1withoutvacationfileStarttls2") << data1 << info1 << true << false;
    }
    {
        const QString sievePasswd = QStringLiteral("password3");
        const QString imapPasswd = QStringLiteral("password2");
        const QString imapUser = QStringLiteral("kde");
        const QString sieveUser = QStringLiteral("foo1");
        const int sievePort = 3;
        const int imapPort = 53;
        AkonadiImapSettingInterfaceDataTest data1;
        data1.sieveSupport = true;
        data1.sieveReuseConfig = false;
        data1.imapServer = QStringLiteral("foo.bla.com");
        data1.userName = imapUser;
        data1.sievePort = sievePort;
        data1.imapPort = imapPort;
        data1.sieveCustomUsername = sieveUser;
        data1.sieveCustomAuthentification = QStringLiteral("CustomUserPassword");
        data1.sieveVacationFilename = QStringLiteral("vacation.siv");
        data1.safety = QStringLiteral("STARTTLS");
        data1.alternateAuthentication = 4;
        // MailTransport::Transport::EnumAuthenticationType::DigestMD5 = 3
        data1.authentication = 4;
        data1.sieveAlternateUrl = QStringLiteral("bla.bla.com");
        data1.password = imapPasswd;
        data1.sieveCustomPassword = sievePasswd;

        KSieveCore::Util::AccountInfo info1;
        info1.sieveUrl = QUrl::fromUserInput(QStringLiteral("sieve://bla.bla.com?x-mech=GSSAPI"));
        info1.sieveUrl.setPassword(sievePasswd);
        info1.sieveUrl.setUserName(sieveUser);
        info1.sieveUrl.setPort(sievePort);
        info1.sieveImapAccountSettings.setServerName(QStringLiteral("foo.bla.com"));
        info1.sieveImapAccountSettings.setPort(imapPort);
        info1.sieveImapAccountSettings.setUserName(imapUser);
        info1.sieveImapAccountSettings.setPassword(imapPasswd);

        info1.sieveImapAccountSettings.setAuthenticationType(KSieveCore::SieveImapAccountSettings::DigestMD5);

        info1.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::STARTTLS);

        QTest::newRow("sievereusecustomconfig1withoutvacationfileStarttls3") << data1 << info1 << true << false;
    }

    {
        const QString sievePasswd = QStringLiteral("password3");
        const QString imapPasswd = QStringLiteral("password2");
        const QString imapUser = QStringLiteral("kde");
        const QString sieveUser = QStringLiteral("foo1");
        const int sievePort = 3;
        const int imapPort = 53;
        AkonadiImapSettingInterfaceDataTest data1;
        data1.sieveSupport = false;
        data1.sieveReuseConfig = false;
        data1.imapServer = QStringLiteral("foo.bla.com");
        data1.userName = imapUser;
        data1.sievePort = sievePort;
        data1.imapPort = imapPort;
        data1.sieveCustomUsername = sieveUser;
        data1.sieveCustomAuthentification = QStringLiteral("CustomUserPassword");
        data1.sieveVacationFilename = QStringLiteral("vacation.siv");
        data1.safety = QStringLiteral("STARTTLS");
        data1.alternateAuthentication = 4;
        // MailTransport::Transport::EnumAuthenticationType::PLAIN = 2
        data1.authentication = 2;
        data1.sieveAlternateUrl = QStringLiteral("bla.bla.com");
        data1.password = imapPasswd;
        data1.sieveCustomPassword = sievePasswd;

        KSieveCore::Util::AccountInfo info1;
        info1.sieveImapAccountSettings.setAuthenticationType(KSieveCore::SieveImapAccountSettings::Plain);
        info1.sieveImapAccountSettings.setEncryptionMode(KSieveCore::SieveImapAccountSettings::Unencrypted);
        QTest::newRow("sievereusecustomconfig1withoutvacationfileStarttlsNoSieveSupport") << data1 << info1 << false << false;
    }
}

void FindAccountInfoJobTest::shouldAssignValue()
{
    QFETCH(AkonadiImapSettingInterfaceDataTest, data);
    QFETCH(KSieveCore::Util::AccountInfo, accountInfo);
    QFETCH(bool, sieveImapAccountValid);
    QFETCH(bool, useVacationFile);

    std::unique_ptr<KSieveCore::AbstractAkonadiImapSettingInterface> interface(new AkonadiImapSettingInterfaceTest(data));
    std::unique_ptr<DataImapPasswordProvider> provider(new DataImapPasswordProvider(&data));

    auto job = new KSieveCore::FindAccountInfoJob;
    job->setIdentifier(QStringLiteral("foo"));
    job->setProvider(provider.get());
    job->setCustomImapSettingsInterface(interface.get());
    job->setWithVacationFileName(useVacationFile);
    QSignalSpy spy(job, &KSieveCore::FindAccountInfoJob::findAccountInfoFinished);
    job->start();
    spy.wait(1000);
    QCOMPARE(spy.count(), 1);
    const auto info = spy.at(0).at(0).value<KSieveCore::Util::AccountInfo>();
    QCOMPARE(info.sieveImapAccountSettings.isValid(), sieveImapAccountValid);
    const bool equal = (info == accountInfo);
    if (!equal) {
        qDebug() << " info " << info;
        qDebug() << " accountInfo " << accountInfo;
    }
    QVERIFY(equal);
}

#include "moc_findaccountinfojobtest.cpp"
