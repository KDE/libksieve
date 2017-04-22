/*
  Copyright (c) 2015-2017 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef SIEVESYNTAXSPELLCHECKINGHIGHLIGHTER_H
#define SIEVESYNTAXSPELLCHECKINGHIGHLIGHTER_H

#include <kpimtextedit/plaintextsyntaxspellcheckinghighlighter.h>

namespace KSieveUi {
class SieveTextEdit;
class SieveSyntaxSpellCheckingHighlighter : public KPIMTextEdit::PlainTextSyntaxSpellCheckingHighlighter
{
    Q_OBJECT
public:
    explicit SieveSyntaxSpellCheckingHighlighter(SieveTextEdit *plainText, const QColor &misspelledColor = Qt::red);
    ~SieveSyntaxSpellCheckingHighlighter();

protected:
    bool spellCheckBlock(const QString &text) Q_DECL_OVERRIDE;
};
}
#endif // SIEVESYNTAXSPELLCHECKINGHIGHLIGHTER_H
