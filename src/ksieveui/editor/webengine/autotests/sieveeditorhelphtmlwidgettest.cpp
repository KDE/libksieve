/*
   Copyright (C) 2016-2017 Laurent Montel <montel@kde.org>

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

#include "sieveeditorhelphtmlwidgettest.h"
#include "../sieveeditorhelphtmlwidget.h"

#include <QTest>
#include "../findbar/findbarwebengineview.h"
#include "../sieveeditorloadprogressindicator.h"
#include "../sieveeditorwebengineview.h"

#include <kpimtextedit/slidecontainer.h>

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

    KSieveUi::SieveEditorLoadProgressIndicator *mProgressIndicator = w.findChild<KSieveUi::SieveEditorLoadProgressIndicator *>(QStringLiteral("progressindicator"));
    QVERIFY(mProgressIndicator);

    KSieveUi::SieveEditorWebEngineView *mWebView = w.findChild<KSieveUi::SieveEditorWebEngineView *>(QStringLiteral("webview"));
    QVERIFY(mWebView);

    KPIMTextEdit::SlideContainer *mSliderContainer = w.findChild<KPIMTextEdit::SlideContainer *>(QStringLiteral("slidercontainer"));
    QVERIFY(mSliderContainer);

    KSieveUi::FindBarWebEngineView *mFindBar = w.findChild<KSieveUi::FindBarWebEngineView *>(QStringLiteral("findbar"));
    QVERIFY(mFindBar);
}

QTEST_MAIN(SieveEditorHelpHtmlWidgetTest)
