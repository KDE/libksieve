/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
