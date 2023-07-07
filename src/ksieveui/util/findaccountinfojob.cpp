/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "findaccountinfojob.h"
#include "abstractakonadiimapsettinginterface.h"
#include "akonadiimapsettinginterface.h"
#include "libksieveui_debug.h"
#include "sieveimappasswordprovider.h"
#include <MailTransport/Transport>
#include <PimCommon/PimUtil>
#include <QUrlQuery>
#include <memory>
#include <pimcommon/imapresourcesettings.h>

using namespace KSieveUi;
FindAccountInfoJob::FindAccountInfoJob(QObject *parent)
    : QObject(parent)
{
}

FindAccountInfoJob::~FindAccountInfoJob() = default;

bool FindAccountInfoJob::canStart() const
{
    return !mIdentifier.isEmpty();
}

void FindAccountInfoJob::setCustomImapSettingsInterface(AbstractAkonadiImapSettingInterface *newCustomImapSettingsInterface)
{
    mCustomImapSettingsInterface = newCustomImapSettingsInterface;
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
    if (!mPasswordProvider) {
        sendAccountInfo();
        return;
    }

    mInterfaceImap.reset(PimCommon::Util::createImapSettingsInterface(mIdentifier));
    mInterface = std::make_unique<KSieveUi::AkonadiImapSettingInterface>(mInterfaceImap);
    if (!mCustomImapSettingsInterface) {
        mCustomImapSettingsInterface = mInterface.get();
    }

    if (!mCustomImapSettingsInterface->sieveSupport()) {
        sendAccountInfo();
        return;
    }

    QString server;
    const QString reply = mCustomImapSettingsInterface->imapServer();
    if (!reply.isEmpty()) {
        server = reply;
        server = server.section(QLatin1Char(':'), 0, 0);
    } else {
        sendAccountInfo();
        return;
    }
    connect(mPasswordProvider, &SieveImapPasswordProvider::passwordsRequested, this, &FindAccountInfoJob::slotPasswordsRequested);
    mPasswordProvider->setProperty("server", server);
    mPasswordProvider->passwords(mIdentifier);
}

void FindAccountInfoJob::slotPasswordsRequested(const QString &sievePassword, const QString &sieveCustomPassword)
{
    const QString server = sender()->property("server").toString();
    QUrl sieveUrl;
    sieveUrl.setScheme(QStringLiteral("sieve"));

    if (mCustomImapSettingsInterface->sieveReuseConfig()) {
        // assemble Sieve url from the settings of the account:
        const QString userName = mCustomImapSettingsInterface->userName();
        mAccountInfo.sieveImapAccountSettings.setServerName(server);
        mAccountInfo.sieveImapAccountSettings.setUserName(userName);

        sieveUrl.setHost(server);
        sieveUrl.setUserName(userName);

        sieveUrl.setPassword(sievePassword);
        mAccountInfo.sieveImapAccountSettings.setPassword(sievePassword);
        mAccountInfo.sieveImapAccountSettings.setPort(mCustomImapSettingsInterface->imapPort());
        sieveUrl.setPort(mCustomImapSettingsInterface->sievePort());
        QString authStr;
        mAccountInfo.sieveImapAccountSettings.setAuthenticationType(
            static_cast<SieveImapAccountSettings::AuthenticationMode>((int)mCustomImapSettingsInterface->authentication()));
        switch (mCustomImapSettingsInterface->authentication()) {
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
        const QString resultSafety = mCustomImapSettingsInterface->safety();
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
        sieveUrl.setQuery(query);
    } else {
        const QString userName = mCustomImapSettingsInterface->userName();
        mAccountInfo.sieveImapAccountSettings.setServerName(server);
        mAccountInfo.sieveImapAccountSettings.setUserName(userName);
        mAccountInfo.sieveImapAccountSettings.setAuthenticationType(
            static_cast<SieveImapAccountSettings::AuthenticationMode>((int)mCustomImapSettingsInterface->authentication()));
        mAccountInfo.sieveImapAccountSettings.setPassword(sievePassword);
        mAccountInfo.sieveImapAccountSettings.setPort(mCustomImapSettingsInterface->imapPort());

        sieveUrl.setHost(mCustomImapSettingsInterface->sieveAlternateUrl());
        sieveUrl.setPort(mCustomImapSettingsInterface->sievePort());
        QString authStr;
        const QString resultSafety = mCustomImapSettingsInterface->safety();
        switch (mCustomImapSettingsInterface->alternateAuthentication()) {
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

        sieveUrl.setQuery(query);

        const QString resultCustomAuthentication = mCustomImapSettingsInterface->sieveCustomAuthentification();
        if (resultCustomAuthentication == QLatin1String("ImapUserPassword")) {
            sieveUrl.setUserName(mCustomImapSettingsInterface->userName());
            const QString imapPwd = sievePassword;
            sieveUrl.setPassword(imapPwd);
        } else if (resultCustomAuthentication == QLatin1String("CustomUserPassword")) {
            const QString customPwd = sieveCustomPassword;
            sieveUrl.setPassword(customPwd);
            sieveUrl.setUserName(mCustomImapSettingsInterface->sieveCustomUsername());
        } else {
            qCWarning(LIBKSIEVE_LOG) << "resultCustomAuthentication undefined " << resultCustomAuthentication;
        }
    }
    sieveUrl = sieveUrl.adjusted(QUrl::RemoveFilename);
    if (mWithVacationFileName) {
        sieveUrl.setPath(sieveUrl.path() + QLatin1Char('/') + mCustomImapSettingsInterface->sieveVacationFilename());
    }
    mAccountInfo.sieveUrl = sieveUrl;
    sendAccountInfo();
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
    return mPasswordProvider;
}

void FindAccountInfoJob::setProvider(SieveImapPasswordProvider *newProvider)
{
    mPasswordProvider = newProvider;
}

#include "moc_findaccountinfojob.cpp"
