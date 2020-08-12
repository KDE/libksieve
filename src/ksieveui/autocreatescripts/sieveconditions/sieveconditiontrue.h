/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVECONDITIONTRUE_H
#define SIEVECONDITIONTRUE_H

#include "sievecondition.h"

namespace KSieveUi {
class SieveConditionTrue : public SieveCondition
{
    Q_OBJECT
public:
    explicit SieveConditionTrue(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);

    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;

    Q_REQUIRED_RESULT QString code(QWidget *w) const override;

    Q_REQUIRED_RESULT QString help() const override;

    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, bool notCondition, QString &error) override;
    Q_REQUIRED_RESULT QUrl href() const override;
};
}

#endif // SIEVECONDITIONTRUE_H
