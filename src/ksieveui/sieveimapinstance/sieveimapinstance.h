/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"
#include <QObject>

namespace KSieveUi
{
/**
 * @brief The SieveImapInstance class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveImapInstance
{
public:
    SieveImapInstance();
    ~SieveImapInstance() = default;

    // Same enum
    enum Status {
        Idle = 0, ///< The agent instance does currently nothing.
        Running, ///< The agent instance is working on something.
        Broken, ///< The agent instance encountered an error state.
        NotConfigured ///< The agent is lacking required configuration
    };

    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &name);

    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &identifier);

    Q_REQUIRED_RESULT Status status() const;
    void setStatus(Status status);

    Q_REQUIRED_RESULT QStringList mimeTypes() const;
    void setMimeTypes(const QStringList &mimeTypes);

    Q_REQUIRED_RESULT QStringList capabilities() const;
    void setCapabilities(const QStringList &capabilities);

    Q_REQUIRED_RESULT bool operator==(const SieveImapInstance &other) const;

private:
    QStringList mMimeTypes;
    QStringList mCapabilities;
    QString mName;
    QString mIdentifier;
    Status mStatus = Idle;
};
}
Q_DECLARE_TYPEINFO(KSieveUi::SieveImapInstance, Q_MOVABLE_TYPE);

