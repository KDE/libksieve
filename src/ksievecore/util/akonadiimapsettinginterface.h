/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] bool sieveSupport() const override;
    [[nodiscard]] bool sieveReuseConfig() const override;
    [[nodiscard]] QString imapServer() const override;
    [[nodiscard]] QString userName() const override;
    [[nodiscard]] int sievePort() const override;
    [[nodiscard]] QString sieveCustomUsername() const override;
    [[nodiscard]] QString sieveCustomAuthentification() const override;
    [[nodiscard]] QString sieveVacationFilename() const override;
    [[nodiscard]] QString safety() const override;
    [[nodiscard]] int alternateAuthentication() const override;
    [[nodiscard]] int authentication() const override;
    [[nodiscard]] QString sieveAlternateUrl() const override;
    [[nodiscard]] int imapPort() const override;

private:
    std::unique_ptr<OrgKdeAkonadiImapSettingsInterface> mInterface;
};
}
