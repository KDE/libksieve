/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectheadersdialogtest.h"
#include "../sieveconditions/widgets/selectheadertypecombobox.h"
#include <QTest>
#include <QStandardPaths>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

SelectHeadersDialogTest::SelectHeadersDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

SelectHeadersDialogTest::~SelectHeadersDialogTest()
{
}

void SelectHeadersDialogTest::shouldHaveDefaultValue()
{
    KSieveUi::SelectHeadersDialog dlg;
    QVERIFY(!dlg.windowTitle().isEmpty());

    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    QVBoxLayout *lay = dlg.findChild<QVBoxLayout *>(QStringLiteral("widgetlayout"));
    QVERIFY(lay);
    QCOMPARE(lay->contentsMargins(), QMargins(0, 0, 0, 0));

    KSieveUi::SelectHeadersWidget *mListWidget = dlg.findChild<KSieveUi::SelectHeadersWidget *>(QStringLiteral("listwidget"));
    QVERIFY(mListWidget);

    QLabel *lab = dlg.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(lab);
    QVERIFY(!lab->text().isEmpty());

    QLineEdit *mNewHeader = dlg.findChild<QLineEdit *>(QStringLiteral("newheader"));
    QVERIFY(mNewHeader);
    QVERIFY(mNewHeader->isClearButtonEnabled());

    QPushButton *mAddNewHeader = dlg.findChild<QPushButton *>(QStringLiteral("addnewheader"));
    QVERIFY(mAddNewHeader);
    QVERIFY(!mAddNewHeader->isEnabled());
}

QTEST_MAIN(SelectHeadersDialogTest)
