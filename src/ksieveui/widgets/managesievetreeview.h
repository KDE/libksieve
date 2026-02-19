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
 * \class KSieveUi::ManageSieveTreeView
 * \brief The ManageSieveTreeView class
 * \author Laurent Montel <montel@kde.org>
 * \inmodule KSieveUi
 * \inheaderfile KSieveUi/ManageSieveTreeView
 */
class KSIEVEUI_EXPORT ManageSieveTreeView : public PimCommon::CustomTreeView
{
    Q_OBJECT
public:
    /*!
     * Constructs a ManageSieveTreeView with the given parent widget.
     * \param parent The parent widget.
     */
    explicit ManageSieveTreeView(QWidget *parent = nullptr);
    /*!
     * Destroys the ManageSieveTreeView.
     */
    ~ManageSieveTreeView() override;

    /*!
     */
    void setNoImapFound(bool found);
    /*!
     * Sets the network state.
     * \param state true if the network is down.
     */
    void setNetworkDown(bool state);
};
}
