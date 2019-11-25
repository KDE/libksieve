/*
  Copyright (C) 2016-2019 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "sieveeditorwebengineview.h"
#include <qtwebenginewidgetsversion.h>

#include <KLocalizedString>
#include <QContextMenuEvent>
#include <QMenu>
#include <QFileDialog>
#include <QWebEngineDownloadItem>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QtWebEngineWidgets>
using namespace KSieveUi;

SieveEditorWebEngineView::SieveEditorWebEngineView(QWidget *parent)
    : QWebEngineView(parent)
{
    QWebEngineProfile *profile = new QWebEngineProfile(this);
    QWebEnginePage *page = new QWebEnginePage(profile, this);
    page->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, false);
    page->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, false);
    page->settings()->setAttribute(QWebEngineSettings::AutoLoadImages, false);
    page->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, false);
    page->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, false);
    page->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, false);
    page->settings()->setAttribute(QWebEngineSettings::XSSAuditingEnabled, false);
    page->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, false);
    page->settings()->setAttribute(QWebEngineSettings::ErrorPageEnabled, false);
    page->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, false);
    page->settings()->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, false);
    page->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, false);
    page->settings()->setAttribute(QWebEngineSettings::WebGLEnabled, false);
#if QTWEBENGINEWIDGETS_VERSION >= QT_VERSION_CHECK(5, 13, 0)
    page->settings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, false);
#endif

    connect(profile, &QWebEngineProfile::downloadRequested, this, &SieveEditorWebEngineView::downloadRequested);
    setPage(page);
}

SieveEditorWebEngineView::~SieveEditorWebEngineView()
{
}

void SieveEditorWebEngineView::downloadRequested(QWebEngineDownloadItem *download)
{
    const QString filename = QFileDialog::getSaveFileName(this, i18n("Save Web Page"));
    if (!filename.isEmpty()) {
        download->setSavePageFormat(QWebEngineDownloadItem::SingleHtmlSaveFormat);
#if QTWEBENGINEWIDGETS_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        download->setDownloadDirectory(QFileInfo(filename).path());
        download->setDownloadFileName(QFileInfo(filename).fileName());
#else
        download->setPath(filename);
#endif
        download->accept();
    } else {
        download->cancel();
    }
}

void SieveEditorWebEngineView::contextMenuEvent(QContextMenuEvent *ev)
{
    QMenu menu;
    QAction *act = pageAction(QWebEnginePage::Back);
    if (act->isEnabled()) {
        menu.addAction(act);
    }
    act = pageAction(QWebEnginePage::Forward);
    if (act->isEnabled()) {
        menu.addAction(act);
    }

    if (!menu.actions().isEmpty()) {
        QAction *separator = new QAction(&menu);
        separator->setSeparator(true);
        menu.addAction(separator);
    }

    act = pageAction(QWebEnginePage::Copy);
    if (act->isEnabled() && hasSelection()) {
        menu.addAction(act);
    }

    if (!menu.actions().isEmpty()) {
        QAction *separator = new QAction(&menu);
        separator->setSeparator(true);
        menu.addAction(separator);
    }
    act = pageAction(QWebEnginePage::Reload);
    if (act->isEnabled()) {
        menu.addAction(act);
    }

    act = pageAction(QWebEnginePage::SelectAll);
    if (act->isEnabled()) {
        menu.addAction(act);
    }
    act = pageAction(QWebEnginePage::CopyLinkToClipboard);
    if (act->isEnabled()) {
        menu.addAction(act);
    }
    act = pageAction(QWebEnginePage::SavePage);
    if (act->isEnabled()) {
        QAction *separator = new QAction(&menu);
        separator->setSeparator(true);
        menu.addAction(separator);
        menu.addAction(act);
    }
    menu.exec(ev->globalPos());
}

QWebEngineView *SieveEditorWebEngineView::createWindow(QWebEnginePage::WebWindowType type)
{
    Q_UNUSED(type);
    return this;
}
