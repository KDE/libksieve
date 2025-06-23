/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moveimapfolderwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "../moveimapfolderwidget.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTest>

MoveImapFolderWidgetTest::MoveImapFolderWidgetTest(QObject *parent)
    : QObject(parent)
{
}

MoveImapFolderWidgetTest::~MoveImapFolderWidgetTest() = default;

void MoveImapFolderWidgetTest::shouldHaveDefaultValue()
{
    KSieveUi::MoveImapFolderWidget w;

    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainlayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));
    auto mLineEdit = w.findChild<QLineEdit *>(u"lineedit"_s);
    QVERIFY(mLineEdit);
    QVERIFY(mLineEdit->text().isEmpty());
    QVERIFY(w.text().isEmpty());
    QVERIFY(mLineEdit->isClearButtonEnabled());
}

void MoveImapFolderWidgetTest::shouldAssignValue()
{
    KSieveUi::MoveImapFolderWidget w;
    auto mLineEdit = w.findChild<QLineEdit *>(u"lineedit"_s);
    QString str = u"foo"_s;
    w.setText(str);
    QCOMPARE(mLineEdit->text(), str);
    QCOMPARE(w.text(), str);

    w.setText(QString());
    QVERIFY(mLineEdit->text().isEmpty());
    QVERIFY(w.text().isEmpty());
}

QTEST_MAIN(MoveImapFolderWidgetTest)

#include "moc_moveimapfolderwidgettest.cpp"
