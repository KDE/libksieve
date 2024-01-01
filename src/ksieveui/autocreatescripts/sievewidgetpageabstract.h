/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

namespace KSieveUi
{
class SieveWidgetPageAbstract : public QWidget
{
    Q_OBJECT
public:
    enum PageType { BlockIf = 0, BlockElsIf = 1, BlockElse = 2, Include = 3, ForEveryPart = 4, GlobalVariable = 5 };

    explicit SieveWidgetPageAbstract(QWidget *parent = nullptr);
    ~SieveWidgetPageAbstract() override;

    virtual void generatedScript(QString &script, QStringList &required, bool inForEveryPartLoop) = 0;

    virtual void setPageType(PageType type);
    [[nodiscard]] PageType pageType() const;

Q_SIGNALS:
    void valueChanged();

private:
    PageType mType = BlockIf;
};
}
