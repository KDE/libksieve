/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "sieveaction.h"
#include <QStringList>
namespace KSieveUi
{
class SieveActionAbstractEditHeader : public SieveAction
{
    Q_OBJECT
public:
    SieveActionAbstractEditHeader(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget,
                                  const QString &name,
                                  const QString &label,
                                  QObject *parent = nullptr);

    Q_REQUIRED_RESULT QStringList needRequires(QWidget *parent) const override;

    Q_REQUIRED_RESULT bool needCheckIfServerHasCapability() const override;

    QString serverNeedsCapability() const override;
};
}

