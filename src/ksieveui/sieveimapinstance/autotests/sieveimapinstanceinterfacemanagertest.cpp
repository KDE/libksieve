/*
   Copyright (C) 2017-2020 Laurent Montel <montel@kde.org>

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

#include "sieveimapinstanceinterfacemanagertest.h"
#include <KSieveUi/SieveImapInstanceInterfaceManager>
#include <KSieveUi/SieveImapInstance>
#include <KSieveUi/SieveImapInstanceInterface>
#include <QTest>

SieveImapInstanceInterfaceManagerTest::SieveImapInstanceInterfaceManagerTest(QObject *parent)
    : QObject(parent)
{
}

void SieveImapInstanceInterfaceManagerTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveImapInstanceInterfaceManager manager;
    QVERIFY(!manager.sieveImapInstanceInterface());

    QVERIFY(!KSieveUi::SieveImapInstanceInterfaceManager::self()->sieveImapInstanceInterface());
    QVERIFY(KSieveUi::SieveImapInstanceInterfaceManager::self()->sieveImapInstanceList().isEmpty());
}

void SieveImapInstanceInterfaceManagerTest::shouldAssignSieveImapInstanceInterface()
{
    KSieveUi::SieveImapInstanceInterfaceManager manager;
    KSieveUi::SieveImapInstanceInterface *instance = new KSieveUi::SieveImapInstanceInterface();
    manager.setSieveImapInstanceInterface(instance);
    QVERIFY(manager.sieveImapInstanceInterface());
    QCOMPARE(manager.sieveImapInstanceInterface(), instance);
}

QTEST_MAIN(SieveImapInstanceInterfaceManagerTest)
