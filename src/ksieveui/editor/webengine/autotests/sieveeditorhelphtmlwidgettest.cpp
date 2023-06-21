/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveeditorhelphtmlwidgettest.h"
#include "../sieveeditorhelphtmlwidget.h"

#include "../findbar/findbarwebengineview.h"
#include "../sieveeditorloadprogressindicator.h"
#include "../sieveeditorwebengineview.h"
#include <QTest>

#include <KPIMTextEdit/SlideContainer>

SieveEditorHelpHtmlWidgetTest::SieveEditorHelpHtmlWidgetTest(QObject *parent)
    : QObject(parent)
{
}

SieveEditorHelpHtmlWidgetTest::~SieveEditorHelpHtmlWidgetTest() = default;

void SieveEditorHelpHtmlWidgetTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveEditorHelpHtmlWidget w;
    // widget is 0x0 size without any content, which results in show() not creating a window, at least in Qt 5.15
    // so set some dummy size to make window appear
    w.resize(100, 100);
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    // KSieveUi::SieveEditorLoadProgressIndicator *mProgressIndicator = w.findChild<KSieveUi::SieveEditorLoadProgressIndicator
    // *>(QStringLiteral("progressindicator")); QVERIFY(mProgressIndicator);

    // KSieveUi::SieveEditorWebEngineView *mWebView = w.findChild<KSieveUi::SieveEditorWebEngineView *>(QStringLiteral("webview"));
    // QVERIFY(mWebView);

    auto mSliderContainer = w.findChild<KPIMTextEdit::SlideContainer *>(QStringLiteral("slidercontainer"));
    QVERIFY(mSliderContainer);

    auto mFindBar = w.findChild<KSieveUi::FindBarWebEngineView *>(QStringLiteral("findbar"));
    QVERIFY(mFindBar);
}

QTEST_MAIN(SieveEditorHelpHtmlWidgetTest)

#include "moc_sieveeditorhelphtmlwidgettest.cpp"
