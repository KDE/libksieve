/*
   Copyright (C) 2017-2018 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef AKONADIIMAPSETTINGINTERFACE_H
#define AKONADIIMAPSETTINGINTERFACE_H

#include "abstractakonadiimapsettinginterface.h"
#include <memory>
class OrgKdeAkonadiImapSettingsInterface;
namespace KSieveUi {
class AkonadiImapSettingInterface : public KSieveUi::AbstractAkonadiImapSettingInterface
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
#endif // AKONADIIMAPSETTINGINTERFACE_H
