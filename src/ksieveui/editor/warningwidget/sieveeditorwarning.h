/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <KMessageWidget>

namespace KSieveUi
{
class SieveEditorWarning : public KMessageWidget
{
    Q_OBJECT
public:
    explicit SieveEditorWarning(QWidget *parent = nullptr);
    ~SieveEditorWarning() override;
};
}

