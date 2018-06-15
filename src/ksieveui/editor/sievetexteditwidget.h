/*
  Copyright (C) 2015-2018 Laurent Montel <montel@kde.org>

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

#ifndef SIEVETEXTEDITWIDGET_H
#define SIEVETEXTEDITWIDGET_H

#include <QWidget>
#include "ksieveui_private_export.h"
namespace KPIMTextEdit {
class SlideContainer;
class PlainTextEditFindBar;
}

namespace KSieveUi {
class SieveTextEdit;
class SieveTextEditWidgetPrivate;
class KSIEVEUI_TESTS_EXPORT SieveTextEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveTextEditWidget(QWidget *parent = nullptr);
    SieveTextEditWidget(KSieveUi::SieveTextEdit *customTextEdit, QWidget *parent);
    ~SieveTextEditWidget();
    void setReadOnly(bool readOnly);

    Q_REQUIRED_RESULT KSieveUi::SieveTextEdit *textEdit() const;

public Q_SLOTS:
    void slotReplace();
    void slotFind();

private:
    void initialize(KSieveUi::SieveTextEdit *custom = nullptr);
    SieveTextEditWidgetPrivate *const d;
    KSieveUi::SieveTextEdit *mTextEdit = nullptr;
    KPIMTextEdit::SlideContainer *mSliderContainer = nullptr;
    KPIMTextEdit::PlainTextEditFindBar *mFindBar = nullptr;
};
}
#endif // SIEVETEXTEDITWIDGET_H
