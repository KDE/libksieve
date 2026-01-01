/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <PimCommon/PurposeMenuWidget>
namespace KSieveUi
{
class SieveEditorWidget;
class SievePurposeMenuWidget : public PimCommon::PurposeMenuWidget
{
    Q_OBJECT
public:
    explicit SievePurposeMenuWidget(QWidget *parentWidget, QObject *parent = nullptr);
    ~SievePurposeMenuWidget() override;

    [[nodiscard]] QByteArray text() override;
    void setEditorWidget(SieveEditorWidget *editor);

private:
    SieveEditorWidget *mSieveEditorWidget = nullptr;
};
}
