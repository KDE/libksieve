/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "includefilelineedittest.h"
#include "../includefilelineedit.h"
#include <QCompleter>
#include <QTest>

IncludeFileLineEditTest::IncludeFileLineEditTest(QObject *parent)
    : QObject(parent)
{
}

void IncludeFileLineEditTest::shouldHaveDefaultValue()
{
    KSieveUi::IncludeFileLineEdit w;
    auto completer = w.findChild<QCompleter *>(QStringLiteral("completer"));
    QVERIFY(completer);
    QVERIFY(w.completer());
    QCOMPARE(completer->caseSensitivity(), Qt::CaseInsensitive);
}

QTEST_MAIN(IncludeFileLineEditTest)
