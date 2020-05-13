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

#ifndef SIEVECONDITION_H
#define SIEVECONDITION_H

#include <QObject>
#include <QUrl>
#include "autocreatescripts/commonwidgets/sievecommonactioncondition.h"
class QXmlStreamReader;
namespace KSieveUi {
class SieveEditorGraphicalModeWidget;
class SieveCondition : public SieveCommonActionCondition
{
    Q_OBJECT
public:
    SieveCondition(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, const QString &name, const QString &label, QObject *parent = nullptr);
    ~SieveCondition() override;

    virtual void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, bool notCondition, QString &error);

    void unknownTag(const QStringRef &tag, QString &error) override;
    void unknownTagValue(const QString &tagValue, QString &error) override;
    void tooManyArguments(const QStringRef &tagName, int index, int maxValue, QString &error) override;
    void tooManyArguments(const QString &tagName, int index, int maxValue, QString &error);
    void serverDoesNotSupportFeatures(const QString &feature, QString &error) override;
};
}

#endif // SIEVECONDITION_H
