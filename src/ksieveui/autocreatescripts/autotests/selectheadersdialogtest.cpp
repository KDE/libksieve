/*
   Copyright (C) 2016-2020 Laurent Montel <montel@kde.org>

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
