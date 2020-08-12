/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
