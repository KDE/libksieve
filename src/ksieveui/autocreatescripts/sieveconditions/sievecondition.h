/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autocreatescripts/commonwidgets/sievecommonactioncondition.h"
class QXmlStreamReader;
namespace KSieveUi
{
class SieveEditorGraphicalModeWidget;
class SieveCondition : public SieveCommonActionCondition
{
    Q_OBJECT
public:
    SieveCondition(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, const QString &name, const QString &label, QObject *parent = nullptr);
    ~SieveCondition() override;

    virtual void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, bool notCondition, QString &error);

    void unknownTagValue(const QString &tagValue, QString &error) override;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void unknownTag(const QStringRef &tag, QString &error) override;
    void tooManyArguments(const QStringRef &tagName, int index, int maxValue, QString &error) override;
#else
    void unknownTag(const QStringView &tag, QString &error) override;
    void tooManyArguments(const QStringView &tagName, int index, int maxValue, QString &error) override;
#endif
    void tooManyArguments(const QString &tagName, int index, int maxValue, QString &error);
    void serverDoesNotSupportFeatures(const QString &feature, QString &error) override;
};
}

