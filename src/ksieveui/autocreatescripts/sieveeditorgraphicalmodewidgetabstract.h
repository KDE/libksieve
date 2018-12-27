/*
   Copyright (C) 2017-2019 Laurent Montel <montel@kde.org>

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

#ifndef SIEVEEDITORGRAPHICALMODEWIDGETABSTRACT_H
#define SIEVEEDITORGRAPHICALMODEWIDGETABSTRACT_H

#include <editor/sieveeditorabstractwidget.h>
#include "ksieveui_private_export.h"
namespace KSieveUi {
class KSIEVEUI_TESTS_EXPORT SieveEditorGraphicalModeWidgetAbstract : public KSieveUi::SieveEditorAbstractWidget
{
    Q_OBJECT
public:
    explicit SieveEditorGraphicalModeWidgetAbstract(QWidget *parent);
    ~SieveEditorGraphicalModeWidgetAbstract();

    Q_REQUIRED_RESULT virtual QString script(QString &requireModules) const = 0;

    virtual void setSieveCapabilities(const QStringList &capabilities) = 0;

    Q_REQUIRED_RESULT virtual QStringList sieveCapabilities() = 0;

    virtual void loadScript(const QString &doc, QString &error) = 0;

    virtual void setListOfIncludeFile(const QStringList &listOfIncludeFile) = 0;
    Q_REQUIRED_RESULT virtual QStringList listOfIncludeFile() const = 0;
};
}

#endif // SIEVEEDITORGRAPHICALMODEWIDGETABSTRACT_H
