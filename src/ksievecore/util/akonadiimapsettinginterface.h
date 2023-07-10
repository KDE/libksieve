/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractakonadiimapsettinginterface.h"
#include <memory>
class OrgKdeAkonadiImapSettingsInterface;
namespace KSieveCore
{
class AkonadiImapSettingInterface : public KSieveCore::AbstractAkonadiImapSettingInterface
{
public:
    AkonadiImapSettingInterface(std::unique_ptr<OrgKdeAkonadiImapSettingsInterface> &interface);
    ~AkonadiImapSettingInterface() override;

    Q_REQUIRED_RESULT bool sieveSupport() const override;
    Q_REQUIRED_RESULT bool sieveReuseConfig() const override;
    Q_REQUIRED_RESULT QString imapServer() const override;
    Q_REQUIRED_RESULT QString userName() const override;
    Q_REQUIRED_RESULT int sievePort() const override;
    Q_REQUIRED_RESULT QString sieveCustomUsername() const override;
    Q_REQUIRED_RESULT QString sieveCustomAuthentification() const override;
    Q_REQUIRED_RESULT QString sieveVacationFilename() const override;
    Q_REQUIRED_RESULT QString safety() const override;
    Q_REQUIRED_RESULT int alternateAuthentication() const override;
    Q_REQUIRED_RESULT int authentication() const override;
    Q_REQUIRED_RESULT QString sieveAlternateUrl() const override;
    Q_REQUIRED_RESULT int imapPort() const override;

private:
    std::unique_ptr<OrgKdeAkonadiImapSettingsInterface> mInterface;
};
}
