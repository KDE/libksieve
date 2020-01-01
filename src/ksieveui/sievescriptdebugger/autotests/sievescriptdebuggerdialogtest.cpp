/*
   Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

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

#include "sievescriptdebuggerdialogtest.h"
#include "../sievescriptdebuggerdialog.h"
#include "../sievescriptdebuggerwidget.h"
#include "../sievescriptdebuggerfrontendwidget.h"
#include <KUrlRequester>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QTest>

SieveScriptDebuggerDialogTest::SieveScriptDebuggerDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

SieveScriptDebuggerDialogTest::~SieveScriptDebuggerDialogTest()
{
}

void SieveScriptDebuggerDialogTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveScriptDebuggerDialog dlg;
    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    KSieveUi::SieveScriptDebuggerWidget *widget = dlg.findChild<KSieveUi::SieveScriptDebuggerWidget *>(QStringLiteral("sievescriptdebuggerwidget"));
    QVERIFY(widget);

    QVERIFY(dlg.script().isEmpty());

    QPushButton *mOkButton = dlg.findChild<QPushButton *>(QStringLiteral("okbutton"));
    QVERIFY(mOkButton);
    QVERIFY(!mOkButton->isEnabled());

    QPushButton *mDebugScriptButton = dlg.findChild<QPushButton *>(QStringLiteral("debug_button"));
    QVERIFY(mDebugScriptButton);
    QVERIFY(!mDebugScriptButton->isEnabled());
}

void SieveScriptDebuggerDialogTest::shouldChangeDebugButtonEnabledState()
{
    KSieveUi::SieveScriptDebuggerDialog dlg;
    KSieveUi::SieveScriptDebuggerWidget *widget = dlg.findChild<KSieveUi::SieveScriptDebuggerWidget *>(QStringLiteral("sievescriptdebuggerwidget"));

    KSieveUi::SieveScriptDebuggerFrontEndWidget *mSieveScriptFrontEnd = widget->findChild<KSieveUi::SieveScriptDebuggerFrontEndWidget *>(QStringLiteral("sievescriptfrontend"));
    QVERIFY(mSieveScriptFrontEnd);

    KUrlRequester *emailPath = mSieveScriptFrontEnd->findChild<KUrlRequester *>(QStringLiteral("emailpath"));
    QVERIFY(emailPath);

    QPushButton *mDebugScriptButton = dlg.findChild<QPushButton *>(QStringLiteral("debug_button"));
    QVERIFY(mDebugScriptButton);
    QVERIFY(!mDebugScriptButton->isEnabled());

    emailPath->setUrl(QUrl::fromLocalFile(QStringLiteral("/")));
    QVERIFY(!mDebugScriptButton->isEnabled());

    mSieveScriptFrontEnd->setScript(QStringLiteral("foo"));
    QVERIFY(mDebugScriptButton->isEnabled());

    emailPath->setUrl(QUrl::fromLocalFile(QStringLiteral("    ")));
    QVERIFY(!mDebugScriptButton->isEnabled());
}

QTEST_MAIN(SieveScriptDebuggerDialogTest)
