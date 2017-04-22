/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "sieveeditortabwidget.h"
#include "webengine/sieveeditorhelphtmlwidget.h"

#include <KLocalizedString>
#include <KStringHandler>

#include <QMenu>
#include <QTabBar>
#include <QUrl>

using namespace KSieveUi;
SieveEditorTabWidget::SieveEditorTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setTabsClosable(true);
    connect(this, &SieveEditorTabWidget::tabCloseRequested, this, &SieveEditorTabWidget::slotTabCloseRequested);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &SieveEditorTabWidget::customContextMenuRequested, this, &SieveEditorTabWidget::slotTabContextMenuRequest);
}

SieveEditorTabWidget::~SieveEditorTabWidget()
{
}

void SieveEditorTabWidget::slotTabContextMenuRequest(const QPoint &pos)
{
    QTabBar *bar = tabBar();
    if (count() <= 1) {
        return;
    }

    const int indexBar = bar->tabAt(bar->mapFrom(this, pos));
    if (indexBar == -1) {
        return;
    }
    QMenu menu(this);

    const int countTab = (count() > 1);

    QAction *closeTab = menu.addAction(i18nc("@action:inmenu", "Close Tab"));
    closeTab->setEnabled((indexBar != 0) && countTab);
    closeTab->setIcon(QIcon::fromTheme(QStringLiteral("tab-close")));

    QAction *allOther = menu.addAction(i18nc("@action:inmenu", "Close All Other Tabs"));
    allOther->setEnabled(countTab);
    allOther->setIcon(QIcon::fromTheme(QStringLiteral("tab-close-other")));

    QAction *allTab = menu.addAction(i18nc("@action:inmenu", "Close All Tabs"));
    allTab->setEnabled(countTab);
    allTab->setIcon(QIcon::fromTheme(QStringLiteral("tab-close")));

    QAction *action = menu.exec(mapToGlobal(pos));

    if (action == allOther) {   // Close all other tabs
        slotCloseAllTabExcept(indexBar);
    } else if (action == closeTab) {
        slotCloseRequest(indexBar);
    } else if (action == allTab) {
        slotCloseAllTab();
    }
}

void SieveEditorTabWidget::slotCloseRequest(int index)
{
    if (index != 0) {
        removeTab(index);
    }
}

void SieveEditorTabWidget::closeAllTabExcept(int index)
{
    //Don't close first tab
    for (int i = count() - 1; i > 0; --i) {
        if (i == index) {
            continue;
        }
        removeTab(i);
    }
}

void SieveEditorTabWidget::slotCloseAllTabExcept(int index)
{
    closeAllTabExcept(index);
}

void SieveEditorTabWidget::slotCloseAllTab()
{
    closeAllTabExcept();
}

void SieveEditorTabWidget::slotTabCloseRequested(int index)
{
    //Don't remove first tab.
    if (index > 0) {
        removeTab(index);
    }
}

bool SieveEditorTabWidget::currentPageIsHtmlPage() const
{
    SieveEditorHelpHtmlWidget *page = qobject_cast<SieveEditorHelpHtmlWidget *>(currentWidget());
    return page ? true : false;
}

QUrl SieveEditorTabWidget::currentHelpUrl() const
{
    SieveEditorHelpHtmlWidget *page = qobject_cast<SieveEditorHelpHtmlWidget *>(currentWidget());
    if (page) {
        return page->currentUrl();
    }
    return QUrl();
}

QString SieveEditorTabWidget::currentHelpTitle() const
{
    SieveEditorHelpHtmlWidget *page = qobject_cast<SieveEditorHelpHtmlWidget *>(currentWidget());
    if (page) {
        return page->title();
    }
    return QString();
}

void SieveEditorTabWidget::slotAddHelpPage(const QUrl &url)
{
    for (int i = 0; i < count(); ++i) {
        SieveEditorHelpHtmlWidget *page = qobject_cast<SieveEditorHelpHtmlWidget *>(widget(i));
        if (page && page->currentUrl() == url) {
            setCurrentIndex(i);
            return;
        }
    }
    SieveEditorHelpHtmlWidget *htmlPage = new SieveEditorHelpHtmlWidget;
    connect(htmlPage, &SieveEditorHelpHtmlWidget::titleChanged, this, &SieveEditorTabWidget::slotTitleChanged);
    connect(htmlPage, &SieveEditorHelpHtmlWidget::progressIndicatorPixmapChanged, this, &SieveEditorTabWidget::slotProgressIndicatorPixmapChanged);
    connect(htmlPage, &SieveEditorHelpHtmlWidget::loadFinished, this, &SieveEditorTabWidget::slotLoadFinished);
    connect(htmlPage, &SieveEditorHelpHtmlWidget::copyAvailable, this, &SieveEditorTabWidget::copyAvailable);
    htmlPage->openUrl(url);
    const int index = addTab(htmlPage, i18n("Help"));
    setCurrentIndex(index);
}

void SieveEditorTabWidget::slotLoadFinished(KSieveUi::SieveEditorHelpHtmlWidget *widget, bool success)
{
    const int index = indexOf(widget);
    if (index != -1) {
        setTabIcon(index, QIcon());
    }
    if (!success) {
        setTabText(index, i18n("Error during load page about %1", widget->title()));
    }
}

void SieveEditorTabWidget::slotProgressIndicatorPixmapChanged(KSieveUi::SieveEditorHelpHtmlWidget *widget, const QPixmap &pixmap)
{
    const int index = indexOf(widget);
    if (index != -1) {
        setTabIcon(index, QIcon(pixmap));
    }
}

void SieveEditorTabWidget::slotTitleChanged(KSieveUi::SieveEditorHelpHtmlWidget *widget, const QString &title)
{
    const int index = indexOf(widget);
    if (index != -1) {
        const QString troncateTitle = KStringHandler::lsqueeze(title, 30);
        setTabText(index, i18n("Help about: %1", troncateTitle));
        setTabToolTip(index, title);
    }
}

void SieveEditorTabWidget::tabRemoved(int index)
{
    if (count() <= 1) {
        tabBar()->hide();
    }
    QTabWidget::tabRemoved(index);
}

void SieveEditorTabWidget::tabInserted(int index)
{
    if (count() > 1) {
        tabBar()->show();
    }
    QTabWidget::tabInserted(index);
    tabBar()->tabButton(0, QTabBar::RightSide)->setEnabled(false);
}
