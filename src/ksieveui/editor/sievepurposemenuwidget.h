/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVEPURPOSEMENUWIDGET_H
#define SIEVEPURPOSEMENUWIDGET_H

#include <PimCommon/PurposeMenuWidget>
namespace KSieveUi {
class SieveEditorWidget;
class SievePurposeMenuWidget : public PimCommon::PurposeMenuWidget
{
    Q_OBJECT
public:
    explicit SievePurposeMenuWidget(QWidget *parentWidget, QObject *parent = nullptr);
    ~SievePurposeMenuWidget() override;

    Q_REQUIRED_RESULT QByteArray text() override;
    void setEditorWidget(SieveEditorWidget *editor);
private:
    SieveEditorWidget *mSieveEditorWidget = nullptr;
};
}

#endif // SIEVEPURPOSEMENUWIDGET_H
