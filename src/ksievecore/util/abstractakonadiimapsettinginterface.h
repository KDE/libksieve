/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_private_export.h"
#include <QString>
namespace KSieveCore
{
class KSIEVECORE_TESTS_EXPORT AbstractAkonadiImapSettingInterface
{
public:
    AbstractAkonadiImapSettingInterface();
    virtual ~AbstractAkonadiImapSettingInterface();
    [[nodiscard]] virtual bool sieveSupport() const;
    [[nodiscard]] virtual bool sieveReuseConfig() const;
    [[nodiscard]] virtual QString imapServer() const;
    [[nodiscard]] virtual QString userName() const;
    [[nodiscard]] virtual int sievePort() const;
    [[nodiscard]] virtual QString sieveCustomUsername() const;
    [[nodiscard]] virtual QString sieveCustomAuthentification() const;
    [[nodiscard]] virtual QString sieveVacationFilename() const;
    [[nodiscard]] virtual QString safety() const;
    [[nodiscard]] virtual int alternateAuthentication() const;
    [[nodiscard]] virtual int authentication() const;
    [[nodiscard]] virtual QString sieveAlternateUrl() const;
    [[nodiscard]] virtual int imapPort() const;

private:
    Q_DISABLE_COPY(AbstractAkonadiImapSettingInterface)
};
}
