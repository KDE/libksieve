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
class CheckScriptJobPrivate;
/*!
 * \class KSieveCore::VacationCreateScriptJob
 * \inmodule KSieveCore
 * \inheaderfile KSieveCore/CheckScriptJob
 *
 * \brief The CheckScriptJob class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT CheckScriptJob : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a CheckScriptJob with the given parent object.
     * \param parent The parent object.
     */
    explicit CheckScriptJob(QObject *parent = nullptr);
    /*!
     * Destroys the CheckScriptJob.
     */
    ~CheckScriptJob() override;

    /*!
     * Sets the original script content for comparison.
     * \param script The original script.
     */
    void setOriginalScript(const QString &script);
    /*!
     * Sets the current script to check.
     * \param script The script to check.
     */
    void setCurrentScript(const QString &script);
    /*!
     * Sets the Sieve server URL.
     * \param url The server URL.
     */
    void setUrl(const QUrl &url);
    /*!
     * Sets whether the script is currently active.
     * \param active true if the script is active.
     */
    void setIsActive(bool active);

    /*!
     * Checks if the job has all required information to start.
     * \return true if the job can be started.
     */
    [[nodiscard]] bool canStart() const;
    /*!
     * Starts the job to check the script syntax.
     */
    void start();

Q_SIGNALS:
    /*!
     * Emitted when the script check is complete.
     * \param msg A message describing the check result.
     * \param success true if the script is valid, false otherwise.
     */
    void finished(const QString &msg, bool success);

private:
    KSIEVECORE_NO_EXPORT void slotPutCheckSyntaxResult(KManageSieve::SieveJob *job, bool success);
    KSIEVECORE_NO_EXPORT void slotRestoreFile(KManageSieve::SieveJob *job, bool success);
    std::unique_ptr<CheckScriptJobPrivate> const d;
};
}
