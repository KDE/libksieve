/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

SieveEditorTabWidget::~SieveEditorTabWidget() = default;

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

    const bool hasMoreThanTwoTab = (count() > 1);

    QAction *closeTab = nullptr;
    if ((indexBar != 0) && hasMoreThanTwoTab) {
        closeTab = menu.addAction(i18nc("@action:inmenu", "Close Tab"));
        closeTab->setIcon(QIcon::fromTheme(QStringLiteral("tab-close")));
    }

    QAction *allOther = nullptr;
    if ((indexBar == 0) || (count() > 2)) {
        allOther = menu.addAction(i18nc("@action:inmenu", "Close All Other Tabs"));
        allOther->setIcon(QIcon::fromTheme(QStringLiteral("tab-close-other")));
    }

    QAction *allTab = nullptr;
    if (hasMoreThanTwoTab) {
        allTab = menu.addAction(i18nc("@action:inmenu", "Close All Tabs"));
        allTab->setEnabled(hasMoreThanTwoTab);
        allTab->setIcon(QIcon::fromTheme(QStringLiteral("tab-close")));
    }

    QAction *action = menu.exec(mapToGlobal(pos));

    if (action) {
        if (action == allOther) { // Close all other tabs
            slotCloseAllTabExcept(indexBar);
        } else if (action == closeTab) {
            slotCloseRequest(indexBar);
        } else if (action == allTab) {
            slotCloseAllTab();
        }
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
    // Don't close first tab
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
    // Don't remove first tab.
    if (index > 0) {
        removeTab(index);
    }
}

bool SieveEditorTabWidget::currentPageIsHtmlPage() const
{
    auto page = qobject_cast<SieveEditorHelpHtmlWidget *>(currentWidget());
    return page ? true : false;
}

QUrl SieveEditorTabWidget::currentHelpUrl() const
{
    auto page = qobject_cast<SieveEditorHelpHtmlWidget *>(currentWidget());
    if (page) {
        return page->currentUrl();
    }
    return {};
}

QString SieveEditorTabWidget::currentHelpTitle() const
{
    auto page = qobject_cast<SieveEditorHelpHtmlWidget *>(currentWidget());
    if (page) {
        return page->title();
    }
    return {};
}

void SieveEditorTabWidget::slotAddHelpPage(const QUrl &url)
{
    for (int i = 0; i < count(); ++i) {
        auto page = qobject_cast<SieveEditorHelpHtmlWidget *>(widget(i));
        if (page && page->currentUrl() == url) {
            setCurrentIndex(i);
            return;
        }
    }
    auto htmlPage = new SieveEditorHelpHtmlWidget;
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
