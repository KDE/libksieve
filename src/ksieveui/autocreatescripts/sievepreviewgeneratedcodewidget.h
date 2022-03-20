/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "ksieveui_private_export.h"
#include <QWidget>
class QTextEdit;
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT SievePreviewGeneratedCodeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SievePreviewGeneratedCodeWidget(QWidget *parent = nullptr);
    ~SievePreviewGeneratedCodeWidget() override;

    void setCode(const QString &str);

private:
    QTextEdit *const mTextEdit;
};
}
