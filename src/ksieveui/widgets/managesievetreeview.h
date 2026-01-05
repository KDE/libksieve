/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieveui_export.h"
#include <PimCommon/CustomTreeView>

namespace KSieveUi
{
/*!
 * \brief The ManageSieveTreeView class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT ManageSieveTreeView : public PimCommon::CustomTreeView
{
    Q_OBJECT
public:
    /*!
     */
    explicit ManageSieveTreeView(QWidget *parent = nullptr);
    /*!
     */
    ~ManageSieveTreeView() override;

    /*!
     */
    void setNoImapFound(bool found);
    /*!
     */
    void setNetworkDown(bool state);
};
}
