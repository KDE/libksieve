/*
  SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksievecore_export.h"
#include <KSieveCore/Util>
#include <QMap>
#include <QObject>
namespace KSieveCore
{
class SieveImapPasswordProvider;
/*!
 * \class KSieveCore::SearchServerWithVacationSupportJob
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/SearchServerWithVacationSupportJob
 *
 * \brief The SearchServerWithVacationSupportJob class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT SearchServerWithVacationSupportJob : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a SearchServerWithVacationSupportJob with the given parent object.
     * \param parent The parent object.
     */
    explicit SearchServerWithVacationSupportJob(QObject *parent = nullptr);
    /*!
     * Destroys the SearchServerWithVacationSupportJob.
     */
    ~SearchServerWithVacationSupportJob() override;
    /*!
     * Starts the search for servers with vacation support.
     */
    void start();

    /*!
     * Checks if the job can be started.
     * \return true if all required information is set.
     */
    [[nodiscard]] bool canStart() const;

    /*!
     * Returns the password provider used by this job.
     * \return The SieveImapPasswordProvider instance.
     */
    [[nodiscard]] KSieveCore::SieveImapPasswordProvider *passwordProvider() const;
    /*!
     * Sets the password provider for this job.
     * \param newProvider The new password provider instance.
     */
    void setPasswordProvider(KSieveCore::SieveImapPasswordProvider *newProvider);

Q_SIGNALS:
    /*!
     * Emitted when the search for servers with vacation support is finished.
     * \param info A map of server names to account information.
     */
    void searchServerWithVacationSupportFinished(const QMap<QString, KSieveCore::Util::AccountInfo> &info);

private:
    KSIEVECORE_NO_EXPORT void slotFindAccountInfoFinished(const KSieveCore::Util::AccountInfo &info);
    KSIEVECORE_NO_EXPORT void sendAccountList();
    KSIEVECORE_NO_EXPORT void searchNextInfo();
    KSIEVECORE_NO_EXPORT void slotSearchSieveScript(const QString &name, const QString &identifier);
    KSIEVECORE_NO_EXPORT void searchNextServerSieve();

    QMap<QString, QString>::const_iterator mSieveServerMapIterator;
    // name, identifier
    QMap<QString, QString> mServerSieveInfos;

    // Result
    QMap<QString, KSieveCore::Util::AccountInfo> mAccountList;
    KSieveCore::SieveImapPasswordProvider *mPasswordProvider = nullptr;
};
}
