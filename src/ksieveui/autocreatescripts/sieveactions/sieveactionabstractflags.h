/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVEACTIONABSTRACTFLAGS_H
#define SIEVEACTIONABSTRACTFLAGS_H

#include "sieveaction.h"
namespace KSieveUi {
class SieveActionAbstractFlags : public SieveAction
{
    Q_OBJECT
public:
    SieveActionAbstractFlags(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, const QString &name, const QString &label, QObject *parent = nullptr);

    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error) override;
    Q_REQUIRED_RESULT QString code(QWidget *) const override;
    Q_REQUIRED_RESULT QStringList needRequires(QWidget *parent) const override;

    virtual QString flagsCode() const = 0;

    Q_REQUIRED_RESULT bool needCheckIfServerHasCapability() const override;

    QString serverNeedsCapability() const override;
};
}

#endif // SIEVEACTIONABSTRACTFLAGS_H
