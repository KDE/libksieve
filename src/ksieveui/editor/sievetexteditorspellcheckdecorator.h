/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef SIEVETEXTEDITORSPELLCHECKDECORATOR_H
#define SIEVETEXTEDITORSPELLCHECKDECORATOR_H

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
#endif // SIEVETEXTEDITORSPELLCHECKDECORATOR_H
