/*
  SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksievecore_export.h"
#include <QObject>
#include <QUrl>
#include <memory>
namespace KManageSieve
{
class SieveJob;
}

namespace KSieveCore
{
class RenameScriptJobPrivate;
/*!
 * \class KSieveCore::RenameScriptJob
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/RenameScriptJob
 *
 * \brief The RenameScriptJob class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT RenameScriptJob : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a RenameScriptJob with the given parent object.
     * \param parent The parent object.
     */
    explicit RenameScriptJob(QObject *parent = nullptr);
    /*!
     * Destroys the RenameScriptJob.
     */
    ~RenameScriptJob() override;

    /*!
     * Sets the original URL of the script to rename.
     * \param url The old script URL.
     */
    void setOldUrl(const QUrl &url);
    /*!
     * Sets whether the script is currently active.
     * \param active true if the script is active.
     */
    void setIsActive(bool active);
    /*!
     * Sets the new name for the script.
     * \param newName The new script name.
     */
    void setNewName(const QString &newName);

    /*!
     * Checks if the job has all required information to start.
     * \return true if the job can be started.
     */
    [[nodiscard]] bool canStart() const;

    /*!
     * Starts the script rename job.
     */
    void start();

Q_SIGNALS:
    /*!
     * Emitted when the rename operation completes.
     * \param oldUrl The original script URL.
     * \param newUrl The new script URL after renaming.
     * \param error Error message if the operation failed.
     * \param success true if the rename succeeded.
     */
    void finished(const QUrl &oldUrl, const QUrl &newUrl, const QString &error, bool success);

private Q_SLOTS:
    KSIEVECORE_NO_EXPORT void slotDeleteResult(KManageSieve::SieveJob *job, bool success);
    KSIEVECORE_NO_EXPORT void slotPutScript(KManageSieve::SieveJob *job, bool success);
    KSIEVECORE_NO_EXPORT void slotGetResult(KManageSieve::SieveJob *job, bool success, const QString &script, bool isActive);

private:
    std::unique_ptr<RenameScriptJobPrivate> const d;
};
}
