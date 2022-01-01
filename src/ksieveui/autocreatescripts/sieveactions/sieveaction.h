/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "autocreatescripts/commonwidgets/sievecommonactioncondition.h"
#include <QObject>
class QXmlStreamReader;
namespace KSieveUi
{
class SieveEditorGraphicalModeWidget;
class SieveAction : public SieveCommonActionCondition
{
    Q_OBJECT
public:
    SieveAction(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, const QString &name, const QString &label, QObject *parent = nullptr);
    ~SieveAction() override;

    virtual void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error);

    void unknownTag(const QStringRef &tag, QString &error) override;
    void unknownTagValue(const QString &tagValue, QString &error) override;
    void tooManyArguments(const QStringRef &tagName, int index, int maxValue, QString &error) override;
    void serverDoesNotSupportFeatures(const QString &feature, QString &error) override;

protected:
    Q_REQUIRED_RESULT QStringList listOfIncludeFile() const;
};
}

