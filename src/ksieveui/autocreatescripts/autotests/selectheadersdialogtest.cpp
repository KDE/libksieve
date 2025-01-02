/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectheadersdialogtest.h"
#include "../sieveconditions/widgets/selectheadertypecombobox.h"
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

SelectHeadersDialogTest::SelectHeadersDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

SelectHeadersDialogTest::~SelectHeadersDialogTest() = default;

void SelectHeadersDialogTest::shouldHaveDefaultValue()
{
    KSieveUi::SelectHeadersDialog dlg;
    QVERIFY(!dlg.windowTitle().isEmpty());

    auto buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    auto lay = dlg.findChild<QVBoxLayout *>(QStringLiteral("widgetlayout"));
    QVERIFY(lay);
    QCOMPARE(lay->contentsMargins(), QMargins(0, 0, 0, 0));

    auto mListWidget = dlg.findChild<KSieveUi::SelectHeadersWidget *>(QStringLiteral("listwidget"));
    QVERIFY(mListWidget);

    auto lab = dlg.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(lab);
    QVERIFY(!lab->text().isEmpty());

    auto mNewHeader = dlg.findChild<QLineEdit *>(QStringLiteral("newheader"));
    QVERIFY(mNewHeader);
    QVERIFY(mNewHeader->isClearButtonEnabled());

    auto mAddNewHeader = dlg.findChild<QPushButton *>(QStringLiteral("addnewheader"));
    QVERIFY(mAddNewHeader);
    QVERIFY(!mAddNewHeader->isEnabled());
}

QTEST_MAIN(SelectHeadersDialogTest)

#include "moc_selectheadersdialogtest.cpp"
