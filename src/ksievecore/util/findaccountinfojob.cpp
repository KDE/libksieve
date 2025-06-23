/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "findaccountinfojob.h"
using namespace Qt::Literals::StringLiterals;

#include "abstractakonadiimapsettinginterface.h"
#include "akonadiimapsettinginterface.h"
#include "libksievecore_debug.h"
#include "sieveimappasswordprovider.h"
#include <MailTransport/Transport>
#include <PimCommon/PimUtil>
#include <QUrlQuery>
#include <memory>
#include <pimcommon/imapresourcesettings.h>

using namespace KSieveCore;
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
        qCWarning(LIBKSIEVECORE_LOG) << "Impossible to start findAccountInfoJob";
        sendAccountInfo();
        return;
    }
    if (!mPasswordProvider) {
        sendAccountInfo();
        return;
    }

    mInterfaceImap.reset(PimCommon::Util::createImapSettingsInterface(mIdentifier));
    mInterface = std::make_unique<KSieveCore::AkonadiImapSettingInterface>(mInterfaceImap);
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
        server = server.section(u':', 0, 0);
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
    sieveUrl.setScheme(u"sieve"_s);

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
            authStr = u"PLAIN"_s;
            break;
        case MailTransport::Transport::EnumAuthenticationType::LOGIN:
            authStr = u"LOGIN"_s;
            break;
        case MailTransport::Transport::EnumAuthenticationType::CRAM_MD5:
            authStr = u"CRAM-MD5"_s;
            break;
        case MailTransport::Transport::EnumAuthenticationType::DIGEST_MD5:
            authStr = u"DIGEST-MD5"_s;
            break;
        case MailTransport::Transport::EnumAuthenticationType::GSSAPI:
            authStr = u"GSSAPI"_s;
            break;
        case MailTransport::Transport::EnumAuthenticationType::ANONYMOUS:
            authStr = u"ANONYMOUS"_s;
            break;
        default:
            authStr = u"PLAIN"_s;
            break;
        }
        QUrlQuery query;
        query.addQueryItem(u"x-mech"_s, authStr);
        const QString resultSafety = mCustomImapSettingsInterface->safety();
        if (resultSafety == QLatin1StringView("None")) {
            mAccountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::Unencrypted);
            query.addQueryItem(u"x-allow-unencrypted"_s, u"true"_s);
        } else if (resultSafety == QLatin1StringView("SSL")) {
            mAccountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::SSLorTLS);
        } else if (resultSafety == QLatin1StringView("STARTTLS")) {
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
            authStr = u"PLAIN"_s;
            break;
        case MailTransport::Transport::EnumAuthenticationType::LOGIN:
            authStr = u"LOGIN"_s;
            break;
        case MailTransport::Transport::EnumAuthenticationType::CRAM_MD5:
            authStr = u"CRAM-MD5"_s;
            break;
        case MailTransport::Transport::EnumAuthenticationType::DIGEST_MD5:
            authStr = u"DIGEST-MD5"_s;
            break;
        case MailTransport::Transport::EnumAuthenticationType::GSSAPI:
            authStr = u"GSSAPI"_s;
            break;
        case MailTransport::Transport::EnumAuthenticationType::ANONYMOUS:
            authStr = u"ANONYMOUS"_s;
            break;
        default:
            authStr = u"PLAIN"_s;
            break;
        }
        QUrlQuery query;
        query.addQueryItem(u"x-mech"_s, authStr);

        if (resultSafety == QLatin1StringView("None")) {
            mAccountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::Unencrypted);
            query.addQueryItem(u"x-allow-unencrypted"_s, u"true"_s);
        } else if (resultSafety == QLatin1StringView("SSL")) {
            mAccountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::SSLorTLS);
        } else if (resultSafety == QLatin1StringView("STARTTLS")) {
            mAccountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::STARTTLS);
        } else {
            mAccountInfo.sieveImapAccountSettings.setEncryptionMode(SieveImapAccountSettings::Unencrypted);
        }

        sieveUrl.setQuery(query);

        const QString resultCustomAuthentication = mCustomImapSettingsInterface->sieveCustomAuthentification();
        if (resultCustomAuthentication == QLatin1StringView("ImapUserPassword")) {
            sieveUrl.setUserName(mCustomImapSettingsInterface->userName());
            const QString imapPwd = sievePassword;
            sieveUrl.setPassword(imapPwd);
        } else if (resultCustomAuthentication == QLatin1StringView("CustomUserPassword")) {
            const QString customPwd = sieveCustomPassword;
            sieveUrl.setPassword(customPwd);
            sieveUrl.setUserName(mCustomImapSettingsInterface->sieveCustomUsername());
        } else {
            qCWarning(LIBKSIEVECORE_LOG) << "resultCustomAuthentication undefined " << resultCustomAuthentication;
        }
    }
    sieveUrl = sieveUrl.adjusted(QUrl::RemoveFilename);
    if (mWithVacationFileName) {
        sieveUrl.setPath(sieveUrl.path() + u'/' + mCustomImapSettingsInterface->sieveVacationFilename());
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
