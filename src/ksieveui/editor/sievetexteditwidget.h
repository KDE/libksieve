/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
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
