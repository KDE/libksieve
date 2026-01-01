/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "sievetextedittest.h"
using namespace Qt::Literals::StringLiterals;

#include "../sievetextedit.h"
#include <QTest>

SieveTextEditTest::SieveTextEditTest(QObject *parent)
    : QObject(parent)
{
}

SieveTextEditTest::~SieveTextEditTest() = default;

void SieveTextEditTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveTextEdit edit;
    QVERIFY(edit.toPlainText().isEmpty());
}

void SieveTextEditTest::comment_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("result");
    QTest::addColumn<int>("startcursorposition");
    QTest::addColumn<int>("endcursorposition");
    QTest::newRow("onelinewithoutselection") << u"foo"_s << u"#foo"_s << -1 << -1;
    // Comment first line
    QTest::newRow("multilinewithoutselection") << u"foo\nbla"_s << u"#foo\nbla"_s << -1 << -1;
    QTest::newRow("multilinewithoutselectionsecondline") << u"foo\nbla"_s << u"foo\n#bla"_s << 6 << 6;
    QTest::newRow("onelinewithselection") << u"foo"_s << u"#foo"_s << 0 << 2;
    QTest::newRow("multilinewithselection") << u"foo\nbla"_s << u"#foo\n#bla"_s << 0 << 5;
}

void SieveTextEditTest::comment()
{
    QFETCH(QString, input);
    QFETCH(QString, result);
    QFETCH(int, startcursorposition);
    QFETCH(int, endcursorposition);
    KSieveUi::SieveTextEdit edit;
    edit.setPlainText(input);
    if (startcursorposition != -1 && endcursorposition != -1) {
        QTextCursor cursor = edit.textCursor();
        cursor.setPosition(startcursorposition);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, endcursorposition - startcursorposition);
        edit.setTextCursor(cursor);
    }
    edit.comment();
    QCOMPARE(edit.toPlainText(), result);
}

void SieveTextEditTest::uncomment_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("result");
    QTest::addColumn<int>("startcursorposition");
    QTest::addColumn<int>("endcursorposition");
    QTest::newRow("commentedline") << u"#foo"_s << u"foo"_s << -1 << -1;
    QTest::newRow("nocommentedline") << u"foo"_s << u"foo"_s << -1 << -1;
    QTest::newRow("onelinewithselection") << u"#foo"_s << u"foo"_s << 0 << 2;
    // First line
    QTest::newRow("multilinewithoutselection") << u"#foo\n#bla"_s << u"foo\n#bla"_s << -1 << -1;
    QTest::newRow("multilinewithoutselectionsecondline") << u"#foo\n#bla"_s << u"#foo\nbla"_s << 6 << 6;
    QTest::newRow("multilinewithselection") << u"#foo\n#bla"_s << u"foo\nbla"_s << 0 << 6;
}

void SieveTextEditTest::uncomment()
{
    QFETCH(QString, input);
    QFETCH(QString, result);
    QFETCH(int, startcursorposition);
    QFETCH(int, endcursorposition);
    KSieveUi::SieveTextEdit edit;
    edit.setPlainText(input);
    if (startcursorposition != -1 && endcursorposition != -1) {
        QTextCursor cursor = edit.textCursor();
        cursor.setPosition(endcursorposition);
        cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, endcursorposition - startcursorposition);
        edit.setTextCursor(cursor);
    }

    edit.uncomment();
    QCOMPARE(edit.toPlainText(), result);
}

QTEST_MAIN(SieveTextEditTest)

#include "moc_sievetextedittest.cpp"
