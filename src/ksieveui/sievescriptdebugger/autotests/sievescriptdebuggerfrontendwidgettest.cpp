/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdebuggerfrontendwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "../sievescriptdebuggerfrontendwidget.h"
#ifdef HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif
#include "sievescriptdebugger/sievescriptdebuggerwarning.h"
#include <KUrlRequester>
#include <QLabel>
#include <QLineEdit>
#include <QSplitter>
#include <QTest>

SieveScriptDebuggerFrontEndWidgetTest::SieveScriptDebuggerFrontEndWidgetTest(QObject *parent)
    : QObject(parent)
{
}

SieveScriptDebuggerFrontEndWidgetTest::~SieveScriptDebuggerFrontEndWidgetTest() = default;

void SieveScriptDebuggerFrontEndWidgetTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveScriptDebuggerFrontEndWidget w;
    QVERIFY(w.script().isEmpty());

    auto splitter = w.findChild<QSplitter *>(u"splitter"_s);
    QVERIFY(splitter);
    QCOMPARE(splitter->count(), 2);

    auto warning = w.findChild<KSieveUi::SieveScriptDebuggerWarning *>(u"sievescriptdebuggerwarning"_s);
    QVERIFY(warning);

    auto emailLab = w.findChild<QLabel *>(u"emaillab"_s);
    QVERIFY(emailLab);

    auto emailPath = w.findChild<KUrlRequester *>(u"emailpath"_s);
    QVERIFY(emailPath);

    auto extensionLab = w.findChild<QLabel *>(u"extensionlab"_s);
    QVERIFY(extensionLab);

    auto extension = w.findChild<QLineEdit *>(u"extension"_s);
    QVERIFY(extension);
    QVERIFY(extension->text().isEmpty());
    QVERIFY(extension->isClearButtonEnabled());
#ifdef HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
    auto textToSpeechWidget = w.findChild<TextEditTextToSpeech::TextToSpeechContainerWidget *>(u"texttospeechwidget"_s);
    QVERIFY(textToSpeechWidget);
#endif
}

void SieveScriptDebuggerFrontEndWidgetTest::shouldChangeButtonEnabledState()
{
    KSieveUi::SieveScriptDebuggerFrontEndWidget w;
    QVERIFY(w.script().isEmpty());

    auto emailPath = w.findChild<KUrlRequester *>(u"emailpath"_s);
    emailPath->setUrl(QUrl::fromLocalFile(u"/"_s));
    //    QPushButton *debugScriptButton = w.findChild<QPushButton *>(u"debugbutton"_s);

    //    QVERIFY(!debugScriptButton->isEnabled());

    //    w.setScript(u"foo"_s);
    //    QVERIFY(debugScriptButton->isEnabled());

    //    emailPath->setUrl(QUrl::fromLocalFile(u"    "_s));
    //    QVERIFY(!debugScriptButton->isEnabled());
}

void SieveScriptDebuggerFrontEndWidgetTest::shouldAddScript()
{
    KSieveUi::SieveScriptDebuggerFrontEndWidget w;
    const QString script = u"foo foo \n bla bla"_s;
    w.setScript(script);
    QCOMPARE(w.script(), script);
}

void SieveScriptDebuggerFrontEndWidgetTest::shouldNotAccept()
{
    KSieveUi::SieveScriptDebuggerFrontEndWidget w;
    w.show();
    w.setScript(u"debug_log"_s);
    auto warning = w.findChild<KSieveUi::SieveScriptDebuggerWarning *>(u"sievescriptdebuggerwarning"_s);

    QVERIFY(!w.canAccept());
    QVERIFY(warning->isVisible());
    w.setScript(u"foo foo \n bla bla"_s);
    QVERIFY(w.canAccept());
    QVERIFY(!warning->isVisible());

    w.setScript(u"require \"vnd.dovecot.debug\""_s);
    QVERIFY(!w.canAccept());
    QVERIFY(warning->isVisible());
}

QTEST_MAIN(SieveScriptDebuggerFrontEndWidgetTest)

#include "moc_sievescriptdebuggerfrontendwidgettest.cpp"
