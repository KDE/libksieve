/*
   SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
