/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVEPREVIEWGENERATEDCODEWIDGET_H
#define SIEVEPREVIEWGENERATEDCODEWIDGET_H
#include "ksieveui_private_export.h"
#include <QWidget>
class QTextEdit;
namespace KSieveUi {
class KSIEVEUI_TESTS_EXPORT SievePreviewGeneratedCodeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SievePreviewGeneratedCodeWidget(QWidget *parent = nullptr);
    ~SievePreviewGeneratedCodeWidget();

    void setCode(const QString &str);
private:
    QTextEdit *const mTextEdit;
};
}

#endif // SIEVEPREVIEWGENERATEDCODEWIDGET_H
