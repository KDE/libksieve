/*
   SPDX-FileCopyrightText: 2014-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "findbarbasetest.h"
using namespace Qt::Literals::StringLiterals;

#include "../findbarbase.h"

#include <PimCommon/LineEditWithCompleterNg>
#include <QLabel>
#include <QPushButton>
#include <QSignalSpy>
#include <QTest>
#include <QToolButton>

FindBarBaseTest::FindBarBaseTest(QObject *parent)
    : QObject(parent)
{
}

FindBarBaseTest::~FindBarBaseTest() = default;

void FindBarBaseTest::shouldHaveDefaultValue()
{
    KSieveUi::FindBarBase bar;
    auto status = bar.findChild<QLabel *>(u"status"_s);
    QVERIFY(status);
    QVERIFY(status->text().isEmpty());

    auto previous = bar.findChild<QPushButton *>(u"findprevious"_s);
    QVERIFY(previous);
    QVERIFY(!previous->isEnabled());

    auto next = bar.findChild<QPushButton *>(u"findnext"_s);
    QVERIFY(next);
    QVERIFY(!next->isEnabled());

    auto close = bar.findChild<QToolButton *>(u"close"_s);
    QVERIFY(close);

    auto lineedit = bar.findChild<PimCommon::LineEditWithCompleterNg *>(u"searchline"_s);
    QVERIFY(lineedit);
    QVERIFY(lineedit->text().isEmpty());
}

void FindBarBaseTest::shouldClearLineWhenClose()
{
#if defined(Q_OS_WIN)
    QSKIP("This test doesn't work on windows");
#endif
    KSieveUi::FindBarBase bar;
    bar.show();
    QSignalSpy spy(&bar, &KSieveUi::FindBarBase::hideFindBar);
    QVERIFY(QTest::qWaitForWindowExposed(&bar));
    QVERIFY(bar.isVisible());
    bar.focusAndSetCursor();
    auto lineedit = bar.findChild<PimCommon::LineEditWithCompleterNg *>(u"searchline"_s);
    lineedit->setText(u"FOO"_s);
    QVERIFY(!lineedit->text().isEmpty());
    QVERIFY(lineedit->hasFocus());
    bar.closeBar();
    QVERIFY(lineedit->text().isEmpty());
    QVERIFY(!lineedit->hasFocus());
    QCOMPARE(spy.count(), 1);
}

void FindBarBaseTest::shouldEnableDisableNextPreviousButton()
{
#if defined(Q_OS_WIN)
    QSKIP("This test doesn't work on windows");
#endif
    KSieveUi::FindBarBase bar;
    bar.show();
    QVERIFY(QTest::qWaitForWindowExposed(&bar));
    auto previous = bar.findChild<QPushButton *>(u"findprevious"_s);

    auto next = bar.findChild<QPushButton *>(u"findnext"_s);

    bar.autoSearch(u"FOO"_s);
    QVERIFY(next->isEnabled());
    QVERIFY(previous->isEnabled());

    bar.autoSearch(QString());
    QVERIFY(!next->isEnabled());
    QVERIFY(!previous->isEnabled());
}

void FindBarBaseTest::shouldClearAllWhenShowBar()
{
#if defined(Q_OS_WIN)
    QSKIP("This test doesn't work on windows");
#endif
    KSieveUi::FindBarBase bar;
    bar.show();
    QVERIFY(QTest::qWaitForWindowExposed(&bar));
    auto status = bar.findChild<QLabel *>(u"status"_s);
    status->setText(u"FOO"_s);
    bar.closeBar();

    bar.show();
    bar.focusAndSetCursor();
    auto lineedit = bar.findChild<PimCommon::LineEditWithCompleterNg *>(u"searchline"_s);
    QVERIFY(lineedit->hasFocus());
    QVERIFY(status->text().isEmpty());
}

QTEST_MAIN(FindBarBaseTest)

#include "moc_findbarbasetest.cpp"
