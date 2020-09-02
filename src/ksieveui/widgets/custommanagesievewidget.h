/*
  SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef CUSTOMMANAGESIEVEWIDGET_H
#define CUSTOMMANAGESIEVEWIDGET_H

#include <widgets/managesievewidget.h>

namespace KSieveUi {
class SieveImapPasswordProvider;
class CustomManageSieveWidget : public KSieveUi::ManageSieveWidget
{
    Q_OBJECT
public:
    explicit CustomManageSieveWidget(SieveImapPasswordProvider *passwordProvider, QWidget *parent = nullptr);
    ~CustomManageSieveWidget() override;

protected:
    bool refreshList() override;

private:
    SieveImapPasswordProvider *const mPasswordProvider;
};
}

#endif // CUSTOMMANAGESIEVEWIDGET_H
