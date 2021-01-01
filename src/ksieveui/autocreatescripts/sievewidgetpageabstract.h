/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVEWIDGETPAGEABSTRACT_H
#define SIEVEWIDGETPAGEABSTRACT_H

#include <QWidget>

namespace KSieveUi {
class SieveWidgetPageAbstract : public QWidget
{
    Q_OBJECT
public:
    enum PageType {
        BlockIf = 0,
        BlockElsIf = 1,
        BlockElse = 2,
        Include = 3,
        ForEveryPart = 4,
        GlobalVariable = 5
    };

    explicit SieveWidgetPageAbstract(QWidget *parent = nullptr);
    ~SieveWidgetPageAbstract() override;

    virtual void generatedScript(QString &script, QStringList &required, bool inForEveryPartLoop) = 0;

    virtual void setPageType(PageType type);
    Q_REQUIRED_RESULT PageType pageType() const;

Q_SIGNALS:
    void valueChanged();

private:
    PageType mType = BlockIf;
};
}

#endif // SIEVEWIDGETPAGEABSTRACT_H
