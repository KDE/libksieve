/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "managesievetreeview.h"

#include <KLocalizedString>

using namespace KSieveUi;

ManageSieveTreeView::ManageSieveTreeView(QWidget *parent)
    : PimCommon::CustomTreeView(parent)
{
    setDefaultText(i18n("No IMAP server configured..."));
    setRootIsDecorated(true);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setHeaderLabel(i18n("Available Scripts"));
    setSortingEnabled(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
}

ManageSieveTreeView::~ManageSieveTreeView()
{
}

void ManageSieveTreeView::setNoImapFound(bool found)
{
    if (mShowDefaultText != found) {
        setDefaultText(i18n("No IMAP server configured..."));
        mShowDefaultText = found;
        update();
    }
}

void ManageSieveTreeView::setNetworkDown(bool state)
{
    if (!state) {
        setDefaultText(i18n("Network down."));
    }
    update();
}
