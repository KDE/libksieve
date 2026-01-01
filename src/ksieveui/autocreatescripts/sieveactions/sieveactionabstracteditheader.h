/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QStringList needRequires(QWidget *parent) const override;

    [[nodiscard]] bool needCheckIfServerHasCapability() const override;

    QString serverNeedsCapability() const override;
};
}
