/*
   Copyright (C) 2013-2016 Laurent Montel <montel@kde.org>

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

#include "xmlprintingsyntaxhighlighter.h"

using namespace KSieveUi;

XMLPrintingSyntaxHighLighter::XMLPrintingSyntaxHighLighter(QTextDocument *doc)
    : KPIMTextEdit::SyntaxHighlighterBase(doc)
{
    init();
}

XMLPrintingSyntaxHighLighter::~XMLPrintingSyntaxHighLighter()
{
}

void XMLPrintingSyntaxHighLighter::init()
{
    QTextCharFormat testFormat;
    testFormat.setForeground(Qt::gray);
    testFormat.setFontWeight(QFont::Bold);
    const QStringList testType = { QStringLiteral("require") };

    for (const QString &s : testType) {
        const QRegularExpression regex(s, QRegularExpression::CaseInsensitiveOption);
        m_rules.append(KPIMTextEdit::Rule(regex, testFormat));
    }

    QTextCharFormat quoteFormat;
    quoteFormat.setForeground(Qt::blue);
    quoteFormat.setFontWeight(QFont::Bold);
    const QStringList quoteType = { QStringLiteral("quoted"), QStringLiteral("hash"), QStringLiteral("bracket"), QStringLiteral("multiline")};
    for (const QString &s : quoteType) {
        const QRegularExpression regex(s, QRegularExpression::CaseInsensitiveOption);
        m_rules.append(KPIMTextEdit::Rule(regex, quoteFormat));
    }

    QTextCharFormat misc;
    misc.setForeground(Qt::red);
    misc.setFontWeight(QFont::Bold);
    QStringList miscType;
    miscType << QStringLiteral("control") << QStringLiteral("block") << QStringLiteral("script") << QStringLiteral("action") << QStringLiteral("comment");
    miscType << QStringLiteral("num") << QStringLiteral("tag") << QStringLiteral("list") << QStringLiteral("str") << QStringLiteral("test") << QStringLiteral("crlf/");
    Q_FOREACH (const QString &s, miscType) {
        const QRegularExpression regex(s, QRegularExpression::CaseInsensitiveOption);
        m_rules.append(KPIMTextEdit::Rule(regex, misc));
    }

    QTextCharFormat header;
    header.setForeground(Qt::black);
    header.setFontWeight(QFont::Bold);
    m_rules.append(KPIMTextEdit::Rule(QRegularExpression(QStringLiteral("<\\?xml.*"), QRegularExpression::CaseInsensitiveOption), header));
}

