/*
  Copyright (c) 2016 Montel Laurent <montel@kde.org>

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
#include <KLocalizedString>
#include <QContextMenuEvent>
#include <QMenu>
#include <QFileDialog>
#include <QWebEngineDownloadItem>
#include <QWebEngineProfile>

using namespace KSieveUi;

SieveEditorWebEngineView::SieveEditorWebEngineView(QWidget *parent)
    : QWebEngineView(parent)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    QWebEngineProfile *profile = new QWebEngineProfile(this);
    QWebEnginePage *page = new QWebEnginePage(profile, this);
    setPage(page);
    connect(profile, &QWebEngineProfile::downloadRequested, this, &SieveEditorWebEngineView::downloadRequested);
#endif
}

SieveEditorWebEngineView::~SieveEditorWebEngineView()
{

}

void SieveEditorWebEngineView::downloadRequested(QWebEngineDownloadItem *download)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    const QString filename = QFileDialog::getSaveFileName(this, i18n("Save Web Page"));
    if (!filename.isEmpty()) {
        download->setSavePageFormat(QWebEngineDownloadItem::SingleHtmlSaveFormat);
        download->setPath(filename);
        download->accept();
    } else {
        download->cancel();
    }
#else
    Q_UNUSED(download);
#endif
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
    if (act->isEnabled()) {
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
#if 0
    act = pageAction(QWebEnginePage::CopyLinkToClipboard);
    if (act->isEnabled()) {
        menu.addAction(act);
    }
#endif
#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    act = pageAction(QWebEnginePage::SavePage);
    if (act->isEnabled()) {
        QAction *separator = new QAction(&menu);
        separator->setSeparator(true);
        menu.addAction(separator);
        menu.addAction(act);
    }
#endif
    menu.exec(ev->globalPos());
}

