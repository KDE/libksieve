/*
  SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <sonnet/spellcheckdecorator.h>

namespace KSieveUi
{
class SieveTextEdit;
class SieveTextEditorSpellCheckDecorator : public Sonnet::SpellCheckDecorator
{
    Q_OBJECT
public:
    explicit SieveTextEditorSpellCheckDecorator(SieveTextEdit *plainTextEdit);
    Q_REQUIRED_RESULT bool isSpellCheckingEnabledForBlock(const QString &textBlock) const override;

private:
    SieveTextEdit *const mPlainTextEdit;
};
}
