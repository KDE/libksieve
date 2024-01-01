/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveimapinstanceinterfacemanagertest.h"
#include <KSieveCore/SieveImapInstance>
#include <KSieveCore/SieveImapInstanceInterface>
#include <KSieveCore/SieveImapInstanceInterfaceManager>
#include <QTest>

SieveImapInstanceInterfaceManagerTest::SieveImapInstanceInterfaceManagerTest(QObject *parent)
    : QObject(parent)
{
}

void SieveImapInstanceInterfaceManagerTest::shouldHaveDefaultValue()
{
    KSieveCore::SieveImapInstanceInterfaceManager manager;
    QVERIFY(!manager.sieveImapInstanceInterface());

    QVERIFY(!KSieveCore::SieveImapInstanceInterfaceManager::self()->sieveImapInstanceInterface());
    QVERIFY(KSieveCore::SieveImapInstanceInterfaceManager::self()->sieveImapInstanceList().isEmpty());
}

void SieveImapInstanceInterfaceManagerTest::shouldAssignSieveImapInstanceInterface()
{
    KSieveCore::SieveImapInstanceInterfaceManager manager;
    auto instance = new KSieveCore::SieveImapInstanceInterface();
    manager.setSieveImapInstanceInterface(instance);
    QVERIFY(manager.sieveImapInstanceInterface());
    QCOMPARE(manager.sieveImapInstanceInterface(), instance);
}

QTEST_MAIN(SieveImapInstanceInterfaceManagerTest)

#include "moc_sieveimapinstanceinterfacemanagertest.cpp"
