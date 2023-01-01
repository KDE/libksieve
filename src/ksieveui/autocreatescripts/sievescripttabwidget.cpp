/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescripttabwidget.h"
#include "sievewidgetpageabstract.h"

#include <KLocalizedString>
#include <QIcon>
#include <QMenu>

#include <QTabBar>

using namespace KSieveUi;

SieveScriptTabWidget::SieveScriptTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setElideMode(Qt::ElideRight);
    tabBar()->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
    setDocumentMode(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &SieveScriptTabWidget::customContextMenuRequested, this, &SieveScriptTabWidget::slotTabContextMenuRequest);
}

SieveScriptTabWidget::~SieveScriptTabWidget() = default;

void SieveScriptTabWidget::slotTabContextMenuRequest(const QPoint &pos)
{
    QTabBar *bar = tabBar();
    const int indexBar = bar->tabAt(bar->mapFrom(this, pos));
    QWidget *w = widget(indexBar);
    if (!w) {
        return;
    }

    auto page = qobject_cast<SieveWidgetPageAbstract *>(w);
    if (!page) {
        return;
    }
    if ((page->pageType() == SieveWidgetPageAbstract::BlockElsIf) || page->pageType() == SieveWidgetPageAbstract::BlockElse) {
        QMenu menu(this);
        QAction *closeTab = menu.addAction(i18nc("@action:inmenu", "Close Tab"));
        closeTab->setIcon(QIcon::fromTheme(QStringLiteral("tab-close")));

        QAction *action = menu.exec(mapToGlobal(pos));

        if (action == closeTab) {
            Q_EMIT tabCloseRequested(indexBar);
        }
    }
}
