/*
  SPDX-FileCopyrightText: 2016-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QWebEngineView>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
class QWebEngineDownloadItem;
#else
class QWebEngineDownloadRequest;
#endif
namespace KSieveUi
{
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void downloadRequested(QWebEngineDownloadItem *);
#else
    void downloadRequested(QWebEngineDownloadRequest *);
#endif
};
}

