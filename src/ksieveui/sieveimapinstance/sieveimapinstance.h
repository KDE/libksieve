/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVEIMAPINSTANCE_H
#define SIEVEIMAPINSTANCE_H

#include "ksieveui_export.h"
#include <QObject>

namespace KSieveUi {
/**
 * @brief The SieveImapInstance class
 * @author Laurent Montel <montel@kde.org>
 */
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
    Status mStatus = Idle;
};
}
Q_DECLARE_TYPEINFO(KSieveUi::SieveImapInstance, Q_MOVABLE_TYPE);

#endif // SIEVEIMAPINSTANCE_H
