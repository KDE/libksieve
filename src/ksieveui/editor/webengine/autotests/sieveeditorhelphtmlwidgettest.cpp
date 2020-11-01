/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveeditorhelphtmlwidgettest.h"
#include "../sieveeditorhelphtmlwidget.h"

#include <QTest>
#include "../findbar/findbarwebengineview.h"
#include "../sieveeditorloadprogressindicator.h"
#include "../sieveeditorwebengineview.h"

#include <KPIMTextEdit/SlideContainer>

SieveEditorHelpHtmlWidgetTest::SieveEditorHelpHtmlWidgetTest(QObject *parent)
    : QObject(parent)
{
}

SieveEditorHelpHtmlWidgetTest::~SieveEditorHelpHtmlWidgetTest()
{
}

void SieveEditorHelpHtmlWidgetTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveEditorHelpHtmlWidget w;
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    //KSieveUi::SieveEditorLoadProgressIndicator *mProgressIndicator = w.findChild<KSieveUi::SieveEditorLoadProgressIndicator *>(QStringLiteral("progressindicator"));
    //QVERIFY(mProgressIndicator);

    //KSieveUi::SieveEditorWebEngineView *mWebView = w.findChild<KSieveUi::SieveEditorWebEngineView *>(QStringLiteral("webview"));
    //QVERIFY(mWebView);

    auto *mSliderContainer = w.findChild<KPIMTextEdit::SlideContainer *>(QStringLiteral("slidercontainer"));
    QVERIFY(mSliderContainer);

    auto *mFindBar = w.findChild<KSieveUi::FindBarWebEngineView *>(QStringLiteral("findbar"));
    QVERIFY(mFindBar);
}

QTEST_MAIN(SieveEditorHelpHtmlWidgetTest)
