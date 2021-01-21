/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVEEDITORWARNING_H
#define SIEVEEDITORWARNING_H

#include <KMessageWidget>

namespace KSieveUi {
class SieveEditorWarning : public KMessageWidget
{
    Q_OBJECT
public:
    explicit SieveEditorWarning(QWidget *parent = nullptr);
    ~SieveEditorWarning() override;
};
}

#endif // SIEVEEDITORWARNING_H
