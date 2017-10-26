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
    QCOMPARE(mainLayout->margin(), 0);
    QLineEdit *mLineEdit = w.findChild<QLineEdit *>(QStringLiteral("lineedit"));
    QVERIFY(mLineEdit);
    QVERIFY(mLineEdit->text().isEmpty());
    QVERIFY(w.text().isEmpty());
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