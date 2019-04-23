/*
   Copyright (C) 2017-2019 Laurent Montel <montel@kde.org>

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

#ifndef SIEVEIMAPINSTANCE_H
#define SIEVEIMAPINSTANCE_H

#include "ksieveui_export.h"
#include <QObject>

namespace KSieveUi {
class KSIEVEUI_EXPORT SieveImapInstance
{
public:
    SieveImapInstance();
    ~SieveImapInstance() = default;

    //Same enum
    enum Status {
        Idle = 0, ///< The agent instance does currently nothing.
        Running,  ///< The agent instance is working on something.
        Broken,    ///< The agent instance encountered an error state.
        NotConfigured  ///< The agent is lacking required configuration
    };

    QString name() const;
    void setName(const QString &name);

    QString identifier() const;
    void setIdentifier(const QString &identifier);

    Status status() const;
    void setStatus(Status status);

    QStringList mimeTypes() const;
    void setMimeTypes(const QStringList &mimeTypes);

    QStringList capabilities() const;
    void setCapabilities(const QStringList &capabilities);

    bool operator==(const SieveImapInstance &other) const;
private:
    QStringList mMimeTypes;
    QStringList mCapabilities;
    QString mName;
    QString mIdentifier;
    Status mStatus;
};
}
Q_DECLARE_TYPEINFO(KSieveUi::SieveImapInstance, Q_MOVABLE_TYPE);

#endif // SIEVEIMAPINSTANCE_H
