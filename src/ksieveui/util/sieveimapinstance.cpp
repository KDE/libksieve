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

#include "sieveimapinstance.h"
using namespace KSieveUi;
SieveImapInstance::SieveImapInstance()
    : mStatus(Idle)
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

void SieveImapInstance::setStatus(const Status &status)
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
