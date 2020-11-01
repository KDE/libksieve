/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    auto *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    auto *widget = dlg.findChild<KSieveUi::SieveScriptDebuggerWidget *>(QStringLiteral("sievescriptdebuggerwidget"));
    QVERIFY(widget);

    QVERIFY(dlg.script().isEmpty());

    auto *mOkButton = dlg.findChild<QPushButton *>(QStringLiteral("okbutton"));
    QVERIFY(mOkButton);
    QVERIFY(!mOkButton->isEnabled());

    auto *mDebugScriptButton = dlg.findChild<QPushButton *>(QStringLiteral("debug_button"));
    QVERIFY(mDebugScriptButton);
    QVERIFY(!mDebugScriptButton->isEnabled());
}

void SieveScriptDebuggerDialogTest::shouldChangeDebugButtonEnabledState()
{
    KSieveUi::SieveScriptDebuggerDialog dlg;
    auto *widget = dlg.findChild<KSieveUi::SieveScriptDebuggerWidget *>(QStringLiteral("sievescriptdebuggerwidget"));

    auto *mSieveScriptFrontEnd = widget->findChild<KSieveUi::SieveScriptDebuggerFrontEndWidget *>(QStringLiteral("sievescriptfrontend"));
    QVERIFY(mSieveScriptFrontEnd);

    auto *emailPath = mSieveScriptFrontEnd->findChild<KUrlRequester *>(QStringLiteral("emailpath"));
    QVERIFY(emailPath);

    auto *mDebugScriptButton = dlg.findChild<QPushButton *>(QStringLiteral("debug_button"));
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
