/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "findaccountinfojob.h"
#include "libksieve_debug.h"
#include <PimCommon/PimUtil>
#include "abstractakonadiimapsettinginterface.h"
#include "akonadiimapsettinginterface.h"
#include "imapresourcesettings.h"
#include "sieveimappasswordprovider.h"
#include <MailTransport/Transport>
#include <QUrlQuery>

using namespace KSieveUi;
FindAccountInfoJob::FindAccountInfoJob(QObject *parent)
    : QObject(parent)
{

}

FindAccountInfoJob::~FindAccountInfoJob()
{

}

bool FindAccountInfoJob::canStart() const
{
    return !mIdentifier.isEmpty() && mProvider;
}

void FindAccountInfoJob::sendAccountInfo()
{
    deleteLater();
    Q_EMIT findAccountInfoFinished(mAccountInfo);
}

void FindAccountInfoJob::start()
{
    if (!canStart()) {
        qCWarning(LIBKSIEVE_LOG) << "Impossible to start findAccountInfoJob";
        sendAccountInfo();
        return;
    }
    std::unique_ptr<OrgKdeAkonadiImapSettingsInterface> interfaceImap(PimCommon::Util::createImapSettingsInterface(mIdentifier));
    std::unique_ptr<KSieveUi::AbstractAkonadiImapSettingInterface> interface(new KSieveUi::AkonadiImapSettingInterface(interfaceImap));
    if (!interface) {
        sendAccountInfo();
        return;
    }

    if (!interface->sieveSupport()) {
        sendAccountInfo();
        return;
    }

    if (interface->sieveReuseConfig()) {
        // assemble Sieve url from the settings of the account:
        QUrl u;
        u.setScheme(QStringLiteral("sieve"));
        QString server;
        const QString reply = interface->imapServer();
        if (!reply.isEmpty()) {
            server = reply;
            server = server.section(QLatin1Char(':'), 0, 0);
        } else {
            sendAccountInfo();
            return;
        }
        const QString userName = interface->userName();
        mAccountInfo.sieveImapAccountSettings.setServerName(server);
        mAccountInfo.sieveImapAccountSettings.setUserName(userName);

        u.setHost(server);
        u.setUserName(userName);

        const QString pwd = mProvider->password(mIdentifier);
        u.setPassword(pwd);
        mAccountInfo.sieveImapAccountSettings.setPassword(pwd);
        mAccountInfo.sieveImapAccountSettings.setPort(interface->imapPort());
        u.setPort(interface->sievePort());
        QString authStr;
        mAccountInfo.sieveImapAccountSettings.setAuthenticationType(static_cast<SieveImapAccountSettings::AuthenticationMode>((int)interface->authentication()));
        switch (interface->authentication()) {
        case MailTransport::Transport::EnumAuthenticationType::CLEAR:
        case MailTransport::Transport::EnumAuthenticationType::PLAIN:
            authStr = QStringLiteral("PLAIN");
            break;
        case MailTransport::Transport::EnumAuthenticationType::LOGIN:
            authStr = QStringLiteral("LOGIN");
            break;
        case MailTransport::Transport::EnumAuthenticationType::CRAM_MD5:
            authStr = QStringLiteral("CRAM-MD5");
            break;
        case MailTransport::Transport::EnumAuthenticationType::DIGEST_MD5:
            authStr = QStringLiteral("DIGEST-MD5");
            break;
        case MailTransport::Transport::EnumAuthenticationType::GSSAPI:
            authStr = QStringLiteral("GSSAPI");
            break;
        case MailTransport::Transport::EnumAuthenticationType::ANONYMOUS:
            authStr = QStringLiteral("ANONYMOUS");
            break;
        default:
            authStr = QStringLiteral("PLAIN");
            break;
        }
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("x-mech"), authStr);
        const QString resultSafety = interface->safety();
        if (resultSafety == QLatin1String("None")) {
            mAccountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::Unencrypted);
            query.addQueryItem(QStringLiteral("x-allow-unencrypted"), QStringLiteral("true"));
        } else if (resultSafety == QLatin1String("SSL")) {
            mAccountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::SSLorTLS);
        } else if (resultSafety == QLatin1String("STARTTLS")) {
            mAccountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::STARTTLS);
        } else {
            mAccountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::Unencrypted);
        }
        u.setQuery(query);
        u = u.adjusted(QUrl::RemoveFilename);
        if (mWithVacationFileName) {
            u.setPath(u.path() + QLatin1Char('/') + QString(interface->sieveVacationFilename()));
        }
        mAccountInfo.sieveUrl = u;
        sendAccountInfo();
    } else {
        QString server;
        const QString reply = interface->imapServer();
        if (!reply.isEmpty()) {
            server = reply;
            server = server.section(QLatin1Char(':'), 0, 0);
        } else {
            sendAccountInfo();
            return;
        }

        const QString userName = interface->userName();
        mAccountInfo.sieveImapAccountSettings.setServerName(server);
        mAccountInfo.sieveImapAccountSettings.setUserName(userName);
        mAccountInfo.sieveImapAccountSettings.setAuthenticationType(static_cast<SieveImapAccountSettings::AuthenticationMode>((int)interface->authentication()));
        const QString pwd = mProvider->password(mIdentifier);
        mAccountInfo.sieveImapAccountSettings.setPassword(pwd);
        mAccountInfo.sieveImapAccountSettings.setPort(interface->imapPort());

        QUrl u;
        u.setHost(interface->sieveAlternateUrl());
        u.setScheme(QStringLiteral("sieve"));
        u.setPort(interface->sievePort());
        QString authStr;
        const QString resultSafety = interface->safety();
        switch (interface->alternateAuthentication()) {
        case MailTransport::Transport::EnumAuthenticationType::CLEAR:
        case MailTransport::Transport::EnumAuthenticationType::PLAIN:
            authStr = QStringLiteral("PLAIN");
            break;
        case MailTransport::Transport::EnumAuthenticationType::LOGIN:
            authStr = QStringLiteral("LOGIN");
            break;
        case MailTransport::Transport::EnumAuthenticationType::CRAM_MD5:
            authStr = QStringLiteral("CRAM-MD5");
            break;
        case MailTransport::Transport::EnumAuthenticationType::DIGEST_MD5:
            authStr = QStringLiteral("DIGEST-MD5");
            break;
        case MailTransport::Transport::EnumAuthenticationType::GSSAPI:
            authStr = QStringLiteral("GSSAPI");
            break;
        case MailTransport::Transport::EnumAuthenticationType::ANONYMOUS:
            authStr = QStringLiteral("ANONYMOUS");
            break;
        default:
            authStr = QStringLiteral("PLAIN");
            break;
        }
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("x-mech"), authStr);

        if (resultSafety == QLatin1String("None")) {
            mAccountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::Unencrypted);
            query.addQueryItem(QStringLiteral("x-allow-unencrypted"), QStringLiteral("true"));
        } else if (resultSafety == QLatin1String("SSL")) {
            mAccountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::SSLorTLS);
        } else if (resultSafety == QLatin1String("STARTTLS")) {
            mAccountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::STARTTLS);
        } else {
            mAccountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::Unencrypted);
        }

        u.setQuery(query);

        const QString resultCustomAuthentication = interface->sieveCustomAuthentification();
        if (resultCustomAuthentication == QLatin1String("ImapUserPassword")) {
            u.setUserName(interface->userName());
            const QString imapPwd = pwd;
            u.setPassword(imapPwd);
        } else if (resultCustomAuthentication == QLatin1String("CustomUserPassword")) {
            const QString customPwd = mProvider->sieveCustomPassword(mIdentifier);
            u.setPassword(customPwd);
            u.setUserName(interface->sieveCustomUsername());
        } else {
            qCWarning(LIBKSIEVE_LOG) << "resultCustomAuthentication undefined " << resultCustomAuthentication;
        }
        u = u.adjusted(QUrl::RemoveFilename);
        if (mWithVacationFileName) {
            u.setPath(u.path() + QLatin1Char('/') + interface->sieveVacationFilename());
        }
        mAccountInfo.sieveUrl = u;
        sendAccountInfo();
    }
}

QString FindAccountInfoJob::identifier() const
{
    return mIdentifier;
}

void FindAccountInfoJob::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

bool FindAccountInfoJob::withVacationFileName() const
{
    return mWithVacationFileName;
}

void FindAccountInfoJob::setWithVacationFileName(bool newWithVacationFileName)
{
    mWithVacationFileName = newWithVacationFileName;
}

SieveImapPasswordProvider *FindAccountInfoJob::provider() const
{
    return mProvider;
}

void FindAccountInfoJob::setProvider(SieveImapPasswordProvider *newProvider)
{
    mProvider = newProvider;
}
