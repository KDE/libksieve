/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdebuggerdialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "../sievescriptdebuggerdialog.h"
#include "../sievescriptdebuggerfrontendwidget.h"
#include "../sievescriptdebuggerwidget.h"
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

SieveScriptDebuggerDialogTest::~SieveScriptDebuggerDialogTest() = default;

void SieveScriptDebuggerDialogTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveScriptDebuggerDialog dlg;
    auto buttonBox = dlg.findChild<QDialogButtonBox *>(u"buttonbox"_s);
    QVERIFY(buttonBox);

    auto widget = dlg.findChild<KSieveUi::SieveScriptDebuggerWidget *>(u"sievescriptdebuggerwidget"_s);
    QVERIFY(widget);

    QVERIFY(dlg.script().isEmpty());

    auto mOkButton = dlg.findChild<QPushButton *>(u"okbutton"_s);
    QVERIFY(mOkButton);
    QVERIFY(!mOkButton->isEnabled());

    auto mDebugScriptButton = dlg.findChild<QPushButton *>(u"debug_button"_s);
    QVERIFY(mDebugScriptButton);
    QVERIFY(!mDebugScriptButton->isEnabled());
}

void SieveScriptDebuggerDialogTest::shouldChangeDebugButtonEnabledState()
{
    KSieveUi::SieveScriptDebuggerDialog dlg;
    auto widget = dlg.findChild<KSieveUi::SieveScriptDebuggerWidget *>(u"sievescriptdebuggerwidget"_s);

    auto mSieveScriptFrontEnd = widget->findChild<KSieveUi::SieveScriptDebuggerFrontEndWidget *>(u"sievescriptfrontend"_s);
    QVERIFY(mSieveScriptFrontEnd);

    auto emailPath = mSieveScriptFrontEnd->findChild<KUrlRequester *>(u"emailpath"_s);
    QVERIFY(emailPath);

    auto mDebugScriptButton = dlg.findChild<QPushButton *>(u"debug_button"_s);
    QVERIFY(mDebugScriptButton);
    QVERIFY(!mDebugScriptButton->isEnabled());

    emailPath->setUrl(QUrl::fromLocalFile(u"/"_s));
    QVERIFY(!mDebugScriptButton->isEnabled());

    mSieveScriptFrontEnd->setScript(u"foo"_s);
    QVERIFY(mDebugScriptButton->isEnabled());

    emailPath->setUrl(QUrl::fromLocalFile(u"    "_s));
    QVERIFY(!mDebugScriptButton->isEnabled());
}

QTEST_MAIN(SieveScriptDebuggerDialogTest)

#include "moc_sievescriptdebuggerdialogtest.cpp"
