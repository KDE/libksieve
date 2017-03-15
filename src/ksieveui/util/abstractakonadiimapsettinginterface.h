/*
   Copyright (C) 2017 Laurent Montel <montel@kde.org>

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
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT AbstractAkonadiImapSettingInterface
{
public:
    AbstractAkonadiImapSettingInterface();
    virtual ~AbstractAkonadiImapSettingInterface();
    virtual bool sieveSupport() const;
    virtual bool sieveReuseConfig() const;
    virtual QString imapServer() const;
    virtual QString userName() const;
    virtual int sievePort() const;
    virtual QString sieveCustomUsername() const;
    virtual QString sieveCustomAuthentification() const;
    virtual QString sieveVacationFilename() const;
    virtual QString safety() const;
    virtual int alternateAuthentication() const;
    virtual int authentication() const;
    virtual QString sieveAlternateUrl() const;
    virtual QString password(const QString &identifier) const;
    virtual QString sieveCustomPassword(const QString &identifier) const;
    virtual int imapPort() const;
};
}

#endif // ABSTRACTAKONADIIMAPSETTINGINTERFACE_H
