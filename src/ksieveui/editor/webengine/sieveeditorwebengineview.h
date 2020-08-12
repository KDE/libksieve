/*
  SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
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
