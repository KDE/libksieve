/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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

#ifndef SIEVEFOREVERYPARTWIDGET_H
#define SIEVEFOREVERYPARTWIDGET_H

#include "sievewidgetpageabstract.h"
class QCheckBox;
class QLineEdit;
class QXmlStreamReader;

namespace KSieveUi {
class SieveHelpButton;
class SieveForEveryPartWidget : public SieveWidgetPageAbstract
{
    Q_OBJECT
public:
    explicit SieveForEveryPartWidget(QWidget *parent = nullptr);
    ~SieveForEveryPartWidget() override;

    void generatedScript(QString &script, QStringList &required, bool inForEveryPartLoop) override;
    void loadScript(QXmlStreamReader &element, QString &error);

private:
    void slotHelp();
    QCheckBox *mForLoop = nullptr;
    QLineEdit *mName = nullptr;
    SieveHelpButton *mHelpButton = nullptr;
};
}

#endif // SIEVEFOREVERYPARTWIDGET_H
