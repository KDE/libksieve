/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"
#include <QObject>
namespace KSieveUi
{
class SieveImapInstanceInterface;
class SieveImapInstance;
/**
 * @brief The SieveImapInstanceInterfaceManager class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveImapInstanceInterfaceManager : public QObject
{
    Q_OBJECT
public:
    explicit SieveImapInstanceInterfaceManager(QObject *parent = nullptr);
    ~SieveImapInstanceInterfaceManager() override;
    static SieveImapInstanceInterfaceManager *self();
    void setSieveImapInstanceInterface(SieveImapInstanceInterface *interface);
    SieveImapInstanceInterface *sieveImapInstanceInterface() const;
    Q_REQUIRED_RESULT QVector<KSieveUi::SieveImapInstance> sieveImapInstanceList() const;

private:
    SieveImapInstanceInterface *mInterface = nullptr;
};
}
