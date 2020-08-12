/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVECONDITIONCONVERT_H
#define SIEVECONDITIONCONVERT_H

#include "sievecondition.h"

namespace KSieveUi {
class SieveConditionConvert : public SieveCondition
{
    Q_OBJECT
public:
    explicit SieveConditionConvert(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    Q_REQUIRED_RESULT QString code(QWidget *) const override;
    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    Q_REQUIRED_RESULT QStringList needRequires(QWidget *) const override;
    Q_REQUIRED_RESULT bool needCheckIfServerHasCapability() const override;
    Q_REQUIRED_RESULT QString serverNeedsCapability() const override;
    Q_REQUIRED_RESULT QString help() const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, bool notCondition, QString &error) override;
};
}

#endif // SIEVECONDITIONCONVERT_H
