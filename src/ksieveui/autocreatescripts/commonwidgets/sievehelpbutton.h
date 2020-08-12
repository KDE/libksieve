/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVEHELPBUTTON_H
#define SIEVEHELPBUTTON_H

#include <QToolButton>

namespace KSieveUi {
class SieveHelpButton : public QToolButton
{
    Q_OBJECT
public:
    explicit SieveHelpButton(QWidget *parent = nullptr);
    ~SieveHelpButton() override;

protected:
    bool event(QEvent *event) override;
};
}

#endif // SIEVEHELPBUTTON_H
