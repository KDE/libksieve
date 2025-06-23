/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveeditorhelphtmlwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "../sieveeditorhelphtmlwidget.h"

#include "../findbar/findbarwebengineview.h"
#include "../sieveeditorloadprogressindicator.h"
#include "../sieveeditorwebengineview.h"
#include <QTest>

#include <TextAddonsWidgets/SlideContainer>

SieveEditorHelpHtmlWidgetTest::SieveEditorHelpHtmlWidgetTest(QObject *parent)
    : QObject(parent)
{
}

SieveEditorHelpHtmlWidgetTest::~SieveEditorHelpHtmlWidgetTest() = default;

void SieveEditorHelpHtmlWidgetTest::shouldHaveDefaultValue()
{
#if defined(Q_OS_WIN)
    QSKIP("This test doesn't work on windows");
#endif
    KSieveUi::SieveEditorHelpHtmlWidget w;
    // widget is 0x0 size without any content, which results in show() not creating a window, at least in Qt 5.15
    // so set some dummy size to make window appear
    w.resize(100, 100);
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    // KSieveUi::SieveEditorLoadProgressIndicator *mProgressIndicator = w.findChild<KSieveUi::SieveEditorLoadProgressIndicator
    // *>(u"progressindicator"_s); QVERIFY(mProgressIndicator);

    // KSieveUi::SieveEditorWebEngineView *mWebView = w.findChild<KSieveUi::SieveEditorWebEngineView *>(u"webview"_s);
    // QVERIFY(mWebView);

    auto mSliderContainer = w.findChild<TextAddonsWidgets::SlideContainer *>(u"slidercontainer"_s);
    QVERIFY(mSliderContainer);

    auto mFindBar = w.findChild<KSieveUi::FindBarWebEngineView *>(u"findbar"_s);
    QVERIFY(mFindBar);
}

QTEST_MAIN(SieveEditorHelpHtmlWidgetTest)

#include "moc_sieveeditorhelphtmlwidgettest.cpp"
