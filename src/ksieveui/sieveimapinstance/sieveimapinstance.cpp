/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveimapinstance.h"
using namespace KSieveUi;
SieveImapInstance::SieveImapInstance()
{
}

QString SieveImapInstance::name() const
{
    return mName;
}

void SieveImapInstance::setName(const QString &name)
{
    mName = name;
}

QString SieveImapInstance::identifier() const
{
    return mIdentifier;
}

void SieveImapInstance::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

KSieveUi::SieveImapInstance::Status SieveImapInstance::status() const
{
    return mStatus;
}

void SieveImapInstance::setStatus(Status status)
{
    mStatus = status;
}

QStringList SieveImapInstance::mimeTypes() const
{
    return mMimeTypes;
}

void SieveImapInstance::setMimeTypes(const QStringList &mimeTypes)
{
    mMimeTypes = mimeTypes;
}

QStringList SieveImapInstance::capabilities() const
{
    return mCapabilities;
}

void SieveImapInstance::setCapabilities(const QStringList &capabilities)
{
    mCapabilities = capabilities;
}

bool SieveImapInstance::operator==(const SieveImapInstance &other) const
{
    return (name() == other.name()) && (identifier() == other.identifier()) && (status() == other.status()) && (mimeTypes() == other.mimeTypes())
        && (capabilities() == other.capabilities());
}
