/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moveimapfolderwidgettest.h"
#include "../moveimapfolderwidget.h"
#include <QTest>
#include <QHBoxLayout>
#include <QLineEdit>

MoveImapFolderWidgetTest::MoveImapFolderWidgetTest(QObject *parent)
    : QObject(parent)
{
}

MoveImapFolderWidgetTest::~MoveImapFolderWidgetTest()
{
}

void MoveImapFolderWidgetTest::shouldHaveDefaultValue()
{
    KSieveUi::MoveImapFolderWidget w;

    QHBoxLayout *mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));
    QLineEdit *mLineEdit = w.findChild<QLineEdit *>(QStringLiteral("lineedit"));
    QVERIFY(mLineEdit);
    QVERIFY(mLineEdit->text().isEmpty());
    QVERIFY(w.text().isEmpty());
    QVERIFY(mLineEdit->isClearButtonEnabled());
}

void MoveImapFolderWidgetTest::shouldAssignValue()
{
    KSieveUi::MoveImapFolderWidget w;
    QLineEdit *mLineEdit = w.findChild<QLineEdit *>(QStringLiteral("lineedit"));
    QString str = QStringLiteral("foo");
    w.setText(str);
    QCOMPARE(mLineEdit->text(), str);
    QCOMPARE(w.text(), str);

    w.setText(QString());
    QVERIFY(mLineEdit->text().isEmpty());
    QVERIFY(w.text().isEmpty());
}

QTEST_MAIN(MoveImapFolderWidgetTest)
