/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveimappasswordprovider.h"
using namespace Qt::Literals::StringLiterals;


using namespace KSieveCore;
SieveImapPasswordProvider::SieveImapPasswordProvider(QObject *parent)
    : QObject(parent)
{
}

#include "moc_sieveimappasswordprovider.cpp"
