/*
   Copyright (C) 2015-2019 Laurent Montel <montel@kde.org>

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

#include "sievescriptdebuggerwidgettest.h"
#include "../sievescriptdebuggerwidget.h"
#include <QTest>
#include <QStackedWidget>

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
    QStackedWidget *stackedwidget = w.findChild<QStackedWidget *>(QStringLiteral("stackedwidget"));
    QVERIFY(stackedwidget);

    const int numberOfStackWidget(2);
    QCOMPARE(stackedwidget->count(), numberOfStackWidget);
    for (int i = 0; i < numberOfStackWidget; ++i) {
        QWidget *w = stackedwidget->widget(i);
        QVERIFY((w->objectName() == QLatin1String("sievescriptfrontend")) || (w->objectName() == QLatin1String("sievenoexistingfrontend")));
    }
}

QTEST_MAIN(SieveScriptDebuggerWidgetTest)
