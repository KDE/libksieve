/*
   Copyright (C) 2017-2020 Laurent Montel <montel@kde.org>

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

#ifndef ABSTRACTAKONADIIMAPSETTINGINTERFACE_H
#define ABSTRACTAKONADIIMAPSETTINGINTERFACE_H

#include "ksieveui_private_export.h"
#include <QString>
namespace KSieveUi {
class KSIEVEUI_TESTS_EXPORT AbstractAkonadiImapSettingInterface
{
public:
    AbstractAkonadiImapSettingInterface();
    virtual ~AbstractAkonadiImapSettingInterface();
    Q_REQUIRED_RESULT virtual bool sieveSupport() const;
    Q_REQUIRED_RESULT virtual bool sieveReuseConfig() const;
    Q_REQUIRED_RESULT virtual QString imapServer() const;
    Q_REQUIRED_RESULT virtual QString userName() const;
    Q_REQUIRED_RESULT virtual int sievePort() const;
    Q_REQUIRED_RESULT virtual QString sieveCustomUsername() const;
    Q_REQUIRED_RESULT virtual QString sieveCustomAuthentification() const;
    Q_REQUIRED_RESULT virtual QString sieveVacationFilename() const;
    Q_REQUIRED_RESULT virtual QString safety() const;
    Q_REQUIRED_RESULT virtual int alternateAuthentication() const;
    Q_REQUIRED_RESULT virtual int authentication() const;
    Q_REQUIRED_RESULT virtual QString sieveAlternateUrl() const;
    Q_REQUIRED_RESULT virtual int imapPort() const;
private:
    Q_DISABLE_COPY(AbstractAkonadiImapSettingInterface)
};
}

#endif // ABSTRACTAKONADIIMAPSETTINGINTERFACE_H
