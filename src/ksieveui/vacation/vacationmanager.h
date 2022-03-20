/*
  SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieveui_export.h"
#include <QObject>
#include <memory>
class QWidget;
namespace KSieveUi
{
class VacationManagerPrivate;
class SieveImapPasswordProvider;
/**
 * @brief The VacationManager class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT VacationManager : public QObject
{
    Q_OBJECT
public:
    explicit VacationManager(SieveImapPasswordProvider *passwordProvider, QWidget *parent);
    ~VacationManager() override;

    void checkVacation();

public Q_SLOTS:
    void slotEditVacation(const QString &serverName);

Q_SIGNALS:
    void updateVacationScriptStatus(bool active, const QString &serverName);

private:
    void slotDialogCanceled();
    void slotDialogOk();
    void slotUpdateVacationScriptStatus(bool active, const QString &serverName);

private:
    Q_DISABLE_COPY(VacationManager)
    std::unique_ptr<VacationManagerPrivate> const d;
};
}
