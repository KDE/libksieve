/*
  SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "sieveeditorwebengineview.h"

#include <KLocalizedString>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QMenu>
#include <QWebEngineDownloadRequest>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
using namespace KSieveUi;

SieveEditorWebEngineView::SieveEditorWebEngineView(QWidget *parent)
    : QWebEngineView(parent)
{
    auto profile = new QWebEngineProfile(this);
    auto page = new QWebEnginePage(profile, this);
    page->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, false);
    page->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, false);
    page->settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
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
    page->settings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, false);
    page->settings()->setAttribute(QWebEngineSettings::NavigateOnDropEnabled, false);

    connect(profile, &QWebEngineProfile::downloadRequested, this, &SieveEditorWebEngineView::downloadRequested);
    setPage(page);
}

SieveEditorWebEngineView::~SieveEditorWebEngineView() = default;

void SieveEditorWebEngineView::downloadRequested(QWebEngineDownloadRequest *download)
{
    const QString filename = QFileDialog::getSaveFileName(this, i18n("Save Web Page"));
    if (!filename.isEmpty()) {
        download->setSavePageFormat(QWebEngineDownloadRequest::SingleHtmlSaveFormat);
        download->setDownloadDirectory(QFileInfo(filename).path());
        download->setDownloadFileName(QFileInfo(filename).fileName());
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
        auto separator = new QAction(&menu);
        separator->setSeparator(true);
        menu.addAction(separator);
    }

    act = pageAction(QWebEnginePage::Copy);
    if (act->isEnabled() && hasSelection()) {
        act->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy")));
        menu.addAction(act);
    }

    if (!menu.actions().isEmpty()) {
        auto separator = new QAction(&menu);
        separator->setSeparator(true);
        menu.addAction(separator);
    }
    act = pageAction(QWebEnginePage::Reload);
    if (act->isEnabled()) {
        menu.addAction(act);
    }

    act = pageAction(QWebEnginePage::SelectAll);
    if (act->isEnabled()) {
        act->setIcon(QIcon::fromTheme(QStringLiteral("edit-select-all")));
        menu.addAction(act);
    }
    act = pageAction(QWebEnginePage::CopyLinkToClipboard);
    if (act->isEnabled()) {
        menu.addAction(act);
    }
    act = pageAction(QWebEnginePage::SavePage);
    if (act->isEnabled()) {
        auto separator = new QAction(&menu);
        separator->setSeparator(true);
        menu.addAction(separator);
        act->setIcon(QIcon::fromTheme(QStringLiteral("document-save")));
        menu.addAction(act);
    }
    menu.exec(ev->globalPos());
}

QWebEngineView *SieveEditorWebEngineView::createWindow(QWebEnginePage::WebWindowType type)
{
    Q_UNUSED(type)
    return this;
}

#include "moc_sieveeditorwebengineview.cpp"
