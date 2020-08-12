/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVECONDITIONHEADER_H
#define SIEVECONDITIONHEADER_H

#include "sievecondition.h"

namespace KSieveUi {
class SieveConditionHeader : public SieveCondition
{
    Q_OBJECT
public:
    explicit SieveConditionHeader(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);

    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;

    Q_REQUIRED_RESULT QString code(QWidget *parent) const override;

    Q_REQUIRED_RESULT QString help() const override;
    Q_REQUIRED_RESULT QStringList needRequires(QWidget *parent) const override;

    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, bool notCondition, QString &error) override;
};
}

#endif // SIEVECONDITIONHEADER_H
