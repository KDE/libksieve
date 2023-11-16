/*
  SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieveui_private_export.h"
#include <QWidget>
#include <memory>
namespace TextCustomEditor
{
class PlainTextEditFindBar;
}
namespace TextAddonsWidgets
{
class SlideContainer;
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

    [[nodiscard]] KSieveUi::SieveTextEdit *textEdit() const;

public Q_SLOTS:
    void slotReplace();
    void slotFind();

private:
    KSIEVEUI_NO_EXPORT void initialize(KSieveUi::SieveTextEdit *custom = nullptr);
    std::unique_ptr<SieveTextEditWidgetPrivate> const d;
};
}
