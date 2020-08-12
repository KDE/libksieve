/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVEACTIONFILEINTO_H
#define SIEVEACTIONFILEINTO_H
#include "sieveaction.h"

namespace KSieveUi {
class SieveActionFileInto : public SieveAction
{
    Q_OBJECT
public:
    explicit SieveActionFileInto(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    Q_REQUIRED_RESULT QString code(QWidget *) const override;
    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    Q_REQUIRED_RESULT QStringList needRequires(QWidget *parent) const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error) override;
    Q_REQUIRED_RESULT bool needCheckIfServerHasCapability() const override;
    Q_REQUIRED_RESULT QString serverNeedsCapability() const override;
    Q_REQUIRED_RESULT QString help() const override;
    Q_REQUIRED_RESULT QUrl href() const override;

private:
    bool mHasCopySupport = false;
    bool mHasMailBoxSupport = false;
};
}

#endif // SIEVEACTIONFILEINTO_H
