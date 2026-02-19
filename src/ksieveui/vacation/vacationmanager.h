/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieveui_export.h"
#include <QObject>
#include <memory>
class QWidget;
namespace KSieveCore
{
class SieveImapPasswordProvider;
}
namespace KSieveUi
{
class VacationManagerPrivate;
/*!
 * \class KSieveUi::VacationManager
 * \brief The VacationManager class
 * \author Laurent Montel <montel@kde.org>
 * \inmodule KSieveUi
 * \inheaderfile KSieveUi/VacationManager
 */
class KSIEVEUI_EXPORT VacationManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a VacationManager with the given password provider and parent widget.
     */
    explicit VacationManager(KSieveCore::SieveImapPasswordProvider *passwordProvider, QWidget *parent);
    /*!
     * Destroys the VacationManager.
     */
    ~VacationManager() override;

    /*!
     * Checks the vacation status.
     */
    void checkVacation();

public Q_SLOTS:
    /*!
     * Edits the vacation message for the given server.
     * \param serverName The name of the server.
     */
    void slotEditVacation(const QString &serverName);

Q_SIGNALS:
    /*!
     * Emitted when the vacation script status is updated.
     * \param active true if vacation is active, false otherwise.
     * \param serverName The name of the server.
     */
    void updateVacationScriptStatus(bool active, const QString &serverName);

private:
    KSIEVEUI_NO_EXPORT void slotDialogCanceled();
    KSIEVEUI_NO_EXPORT void slotDialogOk();
    KSIEVEUI_NO_EXPORT void slotUpdateVacationScriptStatus(bool active, const QString &serverName);

private:
    Q_DISABLE_COPY(VacationManager)
    std::unique_ptr<VacationManagerPrivate> const d;
};
}
