/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"
#include <QObject>
namespace KSieveCore
{
class SieveImapInstanceInterface;
class SieveImapInstance;
/**
 * @brief The SieveImapInstanceInterfaceManager class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT SieveImapInstanceInterfaceManager : public QObject
{
    Q_OBJECT
public:
    explicit SieveImapInstanceInterfaceManager(QObject *parent = nullptr);
    ~SieveImapInstanceInterfaceManager() override;
    static SieveImapInstanceInterfaceManager *self();
    void setSieveImapInstanceInterface(SieveImapInstanceInterface *interface);
    SieveImapInstanceInterface *sieveImapInstanceInterface() const;
    [[nodiscard]] QList<KSieveCore::SieveImapInstance> sieveImapInstanceList() const;

private:
    SieveImapInstanceInterface *mInterface = nullptr;
};
}
