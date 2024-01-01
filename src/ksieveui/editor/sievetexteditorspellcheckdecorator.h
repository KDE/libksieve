/*
  SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

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
    [[nodiscard]] bool isSpellCheckingEnabledForBlock(const QString &textBlock) const override;

private:
    SieveTextEdit *const mPlainTextEdit;
};
}
