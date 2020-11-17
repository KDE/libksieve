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
using namespace KSieveUi;
findAccountInfoJob::findAccountInfoJob(QObject *parent)
    : QObject(parent)
{

}

findAccountInfoJob::~findAccountInfoJob()
{

}

bool findAccountInfoJob::canStart() const
{
    return !mIdentifier.isEmpty();
}

void findAccountInfoJob::start()
{
    KSieveUi::Util::AccountInfo accountInfo;
    if (canStart()) {
        qCWarning(LIBKSIEVE_LOG) << "Impossible to start findAccountInfoJob";
        deleteLater();
        Q_EMIT findAccountInfoFinished(accountInfo);
        return;
    }
    std::unique_ptr<OrgKdeAkonadiImapSettingsInterface> interfaceImap(PimCommon::Util::createImapSettingsInterface(mIdentifier));
    std::unique_ptr<KSieveUi::AbstractAkonadiImapSettingInterface> interface(new KSieveUi::AkonadiImapSettingInterface(interfaceImap));
    if (!interface) {
        deleteLater();
        Q_EMIT findAccountInfoFinished(accountInfo);
        return;
    }

    if (!interface->sieveSupport()) {
        deleteLater();
        Q_EMIT findAccountInfoFinished(accountInfo);
        return;
    }

#if 0

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
            return accountInfo;
        }
        const QString userName = interface->userName();
        accountInfo.sieveImapAccountSettings.setServerName(server);
        accountInfo.sieveImapAccountSettings.setUserName(userName);

        u.setHost(server);
        u.setUserName(userName);

        const QString pwd = provider->password(identifier);
        u.setPassword(pwd);
        accountInfo.sieveImapAccountSettings.setPassword(pwd);
        accountInfo.sieveImapAccountSettings.setPort(interface->imapPort());
        u.setPort(interface->sievePort());
        QString authStr;
        accountInfo.sieveImapAccountSettings.setAuthenticationType(static_cast<SieveImapAccountSettings::AuthenticationMode>((int)interface->authentication()));
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
            accountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::Unencrypted);
            query.addQueryItem(QStringLiteral("x-allow-unencrypted"), QStringLiteral("true"));
        } else if (resultSafety == QLatin1String("SSL")) {
            accountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::SSLorTLS);
        } else if (resultSafety == QLatin1String("STARTTLS")) {
            accountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::STARTTLS);
        } else {
            accountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::Unencrypted);
        }
        u.setQuery(query);
        u = u.adjusted(QUrl::RemoveFilename);
        if (withVacationFileName) {
            u.setPath(u.path() + QLatin1Char('/') + QString(interface->sieveVacationFilename()));
        }
        accountInfo.sieveUrl = u;
        return accountInfo;
    } else {
        QString server;
        const QString reply = interface->imapServer();
        if (!reply.isEmpty()) {
            server = reply;
            server = server.section(QLatin1Char(':'), 0, 0);
        } else {
            return accountInfo;
        }

        const QString userName = interface->userName();
        accountInfo.sieveImapAccountSettings.setServerName(server);
        accountInfo.sieveImapAccountSettings.setUserName(userName);
        accountInfo.sieveImapAccountSettings.setAuthenticationType(static_cast<SieveImapAccountSettings::AuthenticationMode>((int)interface->authentication()));
        const QString pwd = provider->password(identifier);
        accountInfo.sieveImapAccountSettings.setPassword(pwd);
        accountInfo.sieveImapAccountSettings.setPort(interface->imapPort());

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
            accountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::Unencrypted);
            query.addQueryItem(QStringLiteral("x-allow-unencrypted"), QStringLiteral("true"));
        } else if (resultSafety == QLatin1String("SSL")) {
            accountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::SSLorTLS);
        } else if (resultSafety == QLatin1String("STARTTLS")) {
            accountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::STARTTLS);
        } else {
            accountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::Unencrypted);
        }

        u.setQuery(query);

        const QString resultCustomAuthentication = interface->sieveCustomAuthentification();
        if (resultCustomAuthentication == QLatin1String("ImapUserPassword")) {
            u.setUserName(interface->userName());
            const QString imapPwd = provider->password(identifier);
            u.setPassword(imapPwd);
        } else if (resultCustomAuthentication == QLatin1String("CustomUserPassword")) {
            const QString customPwd = provider->sieveCustomPassword(identifier);
            u.setPassword(customPwd);
            u.setUserName(interface->sieveCustomUsername());
        } else {
            qCWarning(LIBKSIEVE_LOG) << "resultCustomAuthentication undefined " << resultCustomAuthentication;
        }
        u = u.adjusted(QUrl::RemoveFilename);
        if (withVacationFileName) {
            u.setPath(u.path() + QLatin1Char('/') + interface->sieveVacationFilename());
        }
        accountInfo.sieveUrl = u;
        return accountInfo;
    }

#endif
    deleteLater();
    Q_EMIT findAccountInfoFinished(accountInfo);
}

QString findAccountInfoJob::identifier() const
{
    return mIdentifier;
}

void findAccountInfoJob::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

bool findAccountInfoJob::withVacationFileName() const
{
    return mWithVacationFileName;
}

void findAccountInfoJob::setWithVacationFileName(bool newWithVacationFileName)
{
    mWithVacationFileName = newWithVacationFileName;
}

SieveImapPasswordProvider *findAccountInfoJob::provider() const
{
    return mProvider;
}

void findAccountInfoJob::setProvider(SieveImapPasswordProvider *newProvider)
{
    mProvider = newProvider;
}
