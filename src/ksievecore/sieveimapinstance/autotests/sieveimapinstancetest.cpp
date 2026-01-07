/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveimapinstancetest.h"
using namespace Qt::Literals::StringLiterals;

#include <KSieveCore/SieveImapInstance>

#include <QTest>

SieveImapInstanceTest::SieveImapInstanceTest(QObject *parent)
    : QObject(parent)
{
}

void SieveImapInstanceTest::shouldHaveDefaultValue()
{
    KSieveCore::SieveImapInstance instance;
    QVERIFY(instance.name().isEmpty());
    QVERIFY(instance.identifier().isEmpty());
    QVERIFY(instance.mimeTypes().isEmpty());
    QVERIFY(instance.capabilities().isEmpty());
    QCOMPARE(instance.status(), KSieveCore::SieveImapInstance::Status::Idle);
}

void SieveImapInstanceTest::shouldAssignValues()
{
    KSieveCore::SieveImapInstance instance;
    const QStringList mMimeTypes = {u"ff"_s, u"ff2"_s};
    const QStringList mCapabilities = {u"bb"_s, u"bb2"_s};
    const QString mName = {u"cc"_s};
    const QString mIdentifier = {u"ee"_s};
    const KSieveCore::SieveImapInstance::Status mStatus = KSieveCore::SieveImapInstance::Status::Broken;
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
    KSieveCore::SieveImapInstance instance;
    const QStringList mMimeTypes = {u"ff"_s, u"ff2"_s};
    const QStringList mCapabilities = {u"bb"_s, u"bb2"_s};
    const QString mName = {u"cc"_s};
    const QString mIdentifier = {u"ee"_s};
    const KSieveCore::SieveImapInstance::Status mStatus = KSieveCore::SieveImapInstance::Status::Broken;
    instance.setName(mName);
    instance.setIdentifier(mIdentifier);
    instance.setStatus(mStatus);
    instance.setMimeTypes(mMimeTypes);
    instance.setCapabilities(mCapabilities);

    KSieveCore::SieveImapInstance instance2 = instance;
    QCOMPARE(instance, instance2);
}

QTEST_MAIN(SieveImapInstanceTest)

#include "moc_sieveimapinstancetest.cpp"
