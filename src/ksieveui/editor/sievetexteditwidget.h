/*
  SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieveui_private_export.h"
#include <QWidget>
#include <memory>
namespace KPIMTextEdit
{
class SlideContainer;
class PlainTextEditFindBar;
}

namespace KSieveUi
{
class SieveTextEdit;
class SieveTextEditWidgetPrivate;
class KSIEVEUI_TESTS_EXPORT SieveTextEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveTextEditWidget(QWidget *parent = nullptr);
    SieveTextEditWidget(KSieveUi::SieveTextEdit *customTextEdit, QWidget *parent);
    ~SieveTextEditWidget() override;
    void setReadOnly(bool readOnly);

    Q_REQUIRED_RESULT KSieveUi::SieveTextEdit *textEdit() const;

public Q_SLOTS:
    void slotReplace();
    void slotFind();

private:
    void initialize(KSieveUi::SieveTextEdit *custom = nullptr);
    std::unique_ptr<SieveTextEditWidgetPrivate> const d;
    KSieveUi::SieveTextEdit *mTextEdit = nullptr;
    KPIMTextEdit::SlideContainer *mSliderContainer = nullptr;
    KPIMTextEdit::PlainTextEditFindBar *mFindBar = nullptr;
};
}
