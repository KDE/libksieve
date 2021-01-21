/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef MANAGESIEVETREEVIEW_H
#define MANAGESIEVETREEVIEW_H

#include <PimCommon/CustomTreeView>
#include "ksieveui_export.h"

namespace KSieveUi {
/**
 * @brief The ManageSieveTreeView class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT ManageSieveTreeView : public PimCommon::CustomTreeView
{
    Q_OBJECT
public:
    explicit ManageSieveTreeView(QWidget *parent = nullptr);
    ~ManageSieveTreeView() override;

    void setNoImapFound(bool found);
    void setNetworkDown(bool state);
};
}

#endif // MANAGESIEVETREEVIEW_H
