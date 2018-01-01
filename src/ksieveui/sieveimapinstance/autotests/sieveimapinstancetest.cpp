/*
   Copyright (C) 2017-2018 Laurent Montel <montel@kde.org>

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

#include "sieveimapinstancetest.h"
#include <KSieveUi/SieveImapInstance>

#include <QTest>

SieveImapInstanceTest::SieveImapInstanceTest(QObject *parent)
    : QObject(parent)
{
}

void SieveImapInstanceTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveImapInstance instance;
    QVERIFY(instance.name().isEmpty());
    QVERIFY(instance.identifier().isEmpty());
    QVERIFY(instance.mimeTypes().isEmpty());
    QVERIFY(instance.capabilities().isEmpty());
    QCOMPARE(instance.status(), KSieveUi::SieveImapInstance::Idle);
}

void SieveImapInstanceTest::shouldAssignValues()
{
    KSieveUi::SieveImapInstance instance;
    const QStringList mMimeTypes = {QStringLiteral("ff"), QStringLiteral("ff2")};
    const QStringList mCapabilities = {QStringLiteral("bb"), QStringLiteral("bb2")};
    const QString mName = {QStringLiteral("cc")};
    const QString mIdentifier = {QStringLiteral("ee")};
    const KSieveUi::SieveImapInstance::Status mStatus = KSieveUi::SieveImapInstance::Broken;
    instance.setName(mName);
    instance.setIdentifier(mIdentifier);
    instance.setStatus(mStatus);
    instance.setMimeTypes(mMimeTypes);
    instance.setCapabilities(mCapabilities);

    QCOMPARE(instance.name(), mName);
    QCOMPARE(instance.identifier(), mIdentifier);
    QCOMPARE(instance.mimeTypes(), mMimeTypes);
    QCOMPARE(instance.capabilities(), mCapabilities);
    QCOMPARE(instance.status(), mStatus);
}

void SieveImapInstanceTest::shouldEqualSieveImapInstance()
{
    KSieveUi::SieveImapInstance instance;
    const QStringList mMimeTypes = {QStringLiteral("ff"), QStringLiteral("ff2")};
    const QStringList mCapabilities = {QStringLiteral("bb"), QStringLiteral("bb2")};
    const QString mName = {QStringLiteral("cc")};
    const QString mIdentifier = {QStringLiteral("ee")};
    const KSieveUi::SieveImapInstance::Status mStatus = KSieveUi::SieveImapInstance::Broken;
    instance.setName(mName);
    instance.setIdentifier(mIdentifier);
    instance.setStatus(mStatus);
    instance.setMimeTypes(mMimeTypes);
    instance.setCapabilities(mCapabilities);

    KSieveUi::SieveImapInstance instance2 = instance;
    QCOMPARE(instance, instance2);
}

QTEST_MAIN(SieveImapInstanceTest)
