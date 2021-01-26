/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdebuggerfrontendwidgettest.h"
#include "../sievescriptdebuggerfrontendwidget.h"
#include <KPIMTextEdit/kpimtextedit/texttospeechwidget.h>
#include <KUrlRequester>
#include <QLabel>
#include <QLineEdit>
#include <QSplitter>
#include <QTest>
#include <sievescriptdebugger/sievescriptdebuggerwarning.h>

SieveScriptDebuggerFrontEndWidgetTest::SieveScriptDebuggerFrontEndWidgetTest(QObject *parent)
    : QObject(parent)
{
}

SieveScriptDebuggerFrontEndWidgetTest::~SieveScriptDebuggerFrontEndWidgetTest()
{
}

void SieveScriptDebuggerFrontEndWidgetTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveScriptDebuggerFrontEndWidget w;
    QVERIFY(w.script().isEmpty());

    auto splitter = w.findChild<QSplitter *>(QStringLiteral("splitter"));
    QVERIFY(splitter);
    QCOMPARE(splitter->count(), 2);

    auto warning = w.findChild<KSieveUi::SieveScriptDebuggerWarning *>(QStringLiteral("sievescriptdebuggerwarning"));
    QVERIFY(warning);

    auto emailLab = w.findChild<QLabel *>(QStringLiteral("emaillab"));
    QVERIFY(emailLab);

    auto emailPath = w.findChild<KUrlRequester *>(QStringLiteral("emailpath"));
    QVERIFY(emailPath);

    auto extensionLab = w.findChild<QLabel *>(QStringLiteral("extensionlab"));
    QVERIFY(extensionLab);

    auto extension = w.findChild<QLineEdit *>(QStringLiteral("extension"));
    QVERIFY(extension);
    QVERIFY(extension->text().isEmpty());
    QVERIFY(extension->isClearButtonEnabled());

    auto textToSpeechWidget = w.findChild<KPIMTextEdit::TextToSpeechWidget *>(QStringLiteral("texttospeechwidget"));
    QVERIFY(textToSpeechWidget);
}

void SieveScriptDebuggerFrontEndWidgetTest::shouldChangeButtonEnabledState()
{
    KSieveUi::SieveScriptDebuggerFrontEndWidget w;
    QVERIFY(w.script().isEmpty());

    auto emailPath = w.findChild<KUrlRequester *>(QStringLiteral("emailpath"));
    emailPath->setUrl(QUrl::fromLocalFile(QStringLiteral("/")));
    //    QPushButton *debugScriptButton = w.findChild<QPushButton *>(QStringLiteral("debugbutton"));

    //    QVERIFY(!debugScriptButton->isEnabled());

    //    w.setScript(QStringLiteral("foo"));
    //    QVERIFY(debugScriptButton->isEnabled());

    //    emailPath->setUrl(QUrl::fromLocalFile(QStringLiteral("    ")));
    //    QVERIFY(!debugScriptButton->isEnabled());
}

void SieveScriptDebuggerFrontEndWidgetTest::shouldAddScript()
{
    KSieveUi::SieveScriptDebuggerFrontEndWidget w;
    const QString script = QStringLiteral("foo foo \n bla bla");
    w.setScript(script);
    QCOMPARE(w.script(), script);
}

void SieveScriptDebuggerFrontEndWidgetTest::shouldNotAccept()
{
    KSieveUi::SieveScriptDebuggerFrontEndWidget w;
    w.show();
    w.setScript(QStringLiteral("debug_log"));
    auto warning = w.findChild<KSieveUi::SieveScriptDebuggerWarning *>(QStringLiteral("sievescriptdebuggerwarning"));

    QVERIFY(!w.canAccept());
    QVERIFY(warning->isVisible());
    w.setScript(QStringLiteral("foo foo \n bla bla"));
    QVERIFY(w.canAccept());
    QVERIFY(!warning->isVisible());

    w.setScript(QStringLiteral("require \"vnd.dovecot.debug\""));
    QVERIFY(!w.canAccept());
    QVERIFY(warning->isVisible());
}

QTEST_MAIN(SieveScriptDebuggerFrontEndWidgetTest)
