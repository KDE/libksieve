/*
   Copyright (C) 2013-2016 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SIEVEWIDGETPAGEABSTRACT_H
#define SIEVEWIDGETPAGEABSTRACT_H

#include <QWidget>

namespace KSieveUi
{
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
    ~SieveWidgetPageAbstract();

    virtual void generatedScript(QString &script, QStringList &requires) = 0;

    virtual void setPageType(PageType type);
    PageType pageType() const;

Q_SIGNALS:
    void valueChanged();

private:
    PageType mType;
};
}

#endif // SIEVEWIDGETPAGEABSTRACT_H
