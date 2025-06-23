/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "includefilelineedittest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto completer = w.findChild<QCompleter *>(u"completer"_s);
    QVERIFY(completer);
    QVERIFY(w.completer());
    QCOMPARE(completer->caseSensitivity(), Qt::CaseInsensitive);
}

QTEST_MAIN(IncludeFileLineEditTest)

#include "moc_includefilelineedittest.cpp"
