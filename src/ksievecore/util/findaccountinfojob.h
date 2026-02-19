/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"
#include "util.h"
#include <QObject>
class OrgKdeAkonadiImapSettingsInterface;
namespace KSieveCore
{
class SieveImapPasswordProvider;
class AbstractAkonadiImapSettingInterface;
/*!
 * \class KSieveCore::FindAccountInfoJob
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/FindAccountInfoJob
 *
 * \brief The FindAccountInfoJob class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT FindAccountInfoJob : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a FindAccountInfoJob with the given parent object.
     * \param parent The parent object.
     */
    explicit FindAccountInfoJob(QObject *parent = nullptr);
    /*!
     * Destroys the FindAccountInfoJob.
     */
    ~FindAccountInfoJob() override;

    /*!
     * Starts the job to find account information.
     */
    void start();

    /*!
     * Returns the account identifier.
     * \return The account identifier.
     */
    [[nodiscard]] QString identifier() const;
    /*!
     * Sets the account identifier to look up.
     * \param newIdentifier The identifier to set.
     */
    void setIdentifier(const QString &newIdentifier);

    /*!
     * Checks if the vacation filename should be included.
     * \return true if vacation filename information should be retrieved.
     */
    [[nodiscard]] bool withVacationFileName() const;
    /*!
     * Sets whether to include vacation filename information.
     * \param newWithVacationFileName true to include vacation filename.
     */
    void setWithVacationFileName(bool newWithVacationFileName);

    /*!
     * Returns the password provider used by this job.
     * \return The SieveImapPasswordProvider instance.
     */
    [[nodiscard]] SieveImapPasswordProvider *provider() const;
    /*!
     * Sets the password provider for this job.
     * \param newProvider The new password provider instance.
     */
    void setProvider(SieveImapPasswordProvider *newProvider);

    /*!
     * Checks if the job can be started.
     * \return true if all required information is set.
     */
    [[nodiscard]] bool canStart() const;

    /*!
     * Sets a custom IMAP settings interface.
     * \param newCustomImapSettingsInterface The custom interface.
     */
    void setCustomImapSettingsInterface(KSieveCore::AbstractAkonadiImapSettingInterface *newCustomImapSettingsInterface);

Q_SIGNALS:
    /*!
     * Emitted when account information has been found.
     * \param info The account information.
     */
    void findAccountInfoFinished(const KSieveCore::Util::AccountInfo &info);

private:
    KSIEVECORE_NO_EXPORT void sendAccountInfo();
    KSIEVECORE_NO_EXPORT void slotPasswordsRequested(const QString &sievePassword, const QString &sieveCustomPassword);
    KSieveCore::Util::AccountInfo mAccountInfo;
    QString mIdentifier;
    KSieveCore::AbstractAkonadiImapSettingInterface *mCustomImapSettingsInterface = nullptr;
    SieveImapPasswordProvider *mPasswordProvider = nullptr;
    bool mWithVacationFileName = true;
    std::unique_ptr<OrgKdeAkonadiImapSettingsInterface> mInterfaceImap;
    std::unique_ptr<KSieveCore::AbstractAkonadiImapSettingInterface> mInterface;
};
}
