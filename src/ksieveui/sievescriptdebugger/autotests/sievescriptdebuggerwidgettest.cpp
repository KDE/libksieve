/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdebuggerwidgettest.h"
#include "../sievescriptdebuggerwidget.h"
#include <QStackedWidget>
#include <QTest>

SieveScriptDebuggerWidgetTest::SieveScriptDebuggerWidgetTest(QObject *parent)
    : QObject(parent)
{
}

SieveScriptDebuggerWidgetTest::~SieveScriptDebuggerWidgetTest()
{
}

void SieveScriptDebuggerWidgetTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveScriptDebuggerWidget w;
    auto stackedwidget = w.findChild<QStackedWidget *>(QStringLiteral("stackedwidget"));
    QVERIFY(stackedwidget);

    const int numberOfStackWidget(2);
    QCOMPARE(stackedwidget->count(), numberOfStackWidget);
    for (int i = 0; i < numberOfStackWidget; ++i) {
        QWidget *w = stackedwidget->widget(i);
        QVERIFY((w->objectName() == QLatin1String("sievescriptfrontend")) || (w->objectName() == QLatin1String("sievenoexistingfrontend")));
    }
}

QTEST_MAIN(SieveScriptDebuggerWidgetTest)
