/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"
#include <QObject>

namespace KSieveCore
{
/**
 * @brief The SieveImapInstance class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT SieveImapInstance
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

    [[nodiscard]] QString name() const;
    void setName(const QString &name);

    [[nodiscard]] QString identifier() const;
    void setIdentifier(const QString &identifier);

    [[nodiscard]] Status status() const;
    void setStatus(Status status);

    [[nodiscard]] QStringList mimeTypes() const;
    void setMimeTypes(const QStringList &mimeTypes);

    [[nodiscard]] QStringList capabilities() const;
    void setCapabilities(const QStringList &capabilities);

    [[nodiscard]] bool operator==(const SieveImapInstance &other) const;

private:
    QStringList mMimeTypes;
    QStringList mCapabilities;
    QString mName;
    QString mIdentifier;
    Status mStatus = Idle;
};
}
Q_DECLARE_TYPEINFO(KSieveCore::SieveImapInstance, Q_RELOCATABLE_TYPE);
