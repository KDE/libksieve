/*
  SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "sievetexteditorspellcheckdecorator.h"
using namespace Qt::Literals::StringLiterals;

#include "sievetextedit.h"
using namespace KSieveUi;

SieveTextEditorSpellCheckDecorator::SieveTextEditorSpellCheckDecorator(SieveTextEdit *plainTextEdit)
    : Sonnet::SpellCheckDecorator(plainTextEdit)
    , mPlainTextEdit(plainTextEdit)
{
}

bool SieveTextEditorSpellCheckDecorator::isSpellCheckingEnabledForBlock(const QString &textBlock) const
{
    // allow to spell check comment
    return mPlainTextEdit->checkSpellingEnabled() && textBlock.startsWith(u'#');
}

#include "moc_sievetexteditorspellcheckdecorator.cpp"
