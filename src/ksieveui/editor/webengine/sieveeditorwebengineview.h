/*
  Copyright (C) 2016-2020 Laurent Montel <montel@kde.org>

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

#ifndef SIEVEEDITORWEBENGINEVIEW_H
#define SIEVEEDITORWEBENGINEVIEW_H

#include <QWebEngineView>
class QWebEngineDownloadItem;
namespace KSieveUi {
class SieveEditorWebEngineView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit SieveEditorWebEngineView(QWidget *parent = nullptr);
    ~SieveEditorWebEngineView() override;

protected:
    void contextMenuEvent(QContextMenuEvent *ev) override;
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;
private:
    void downloadRequested(QWebEngineDownloadItem *);
};
}

#endif // SIEVEEDITORWEBENGINEVIEW_H
