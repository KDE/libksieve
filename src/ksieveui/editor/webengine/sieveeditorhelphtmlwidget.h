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
#ifndef SIEVEEDITORHELPHTMLWIDGET_H
#define SIEVEEDITORHELPHTMLWIDGET_H

#include "ksieveui_export.h"
#include <QWidget>
#include <QUrl>

namespace KPIMTextEdit {
class SlideContainer;
}

namespace KSieveUi {
class SieveEditorWebEngineView;
class SieveEditorLoadProgressIndicator;
class FindBarWebEngineView;
class KSIEVEUI_EXPORT SieveEditorHelpHtmlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveEditorHelpHtmlWidget(QWidget *parent = nullptr);
    ~SieveEditorHelpHtmlWidget();

    void openUrl(const QUrl &currentUrl);
    QString title() const;

    QUrl currentUrl() const;

    void resetZoom();
    void zoomOut();
    void zoomIn();
    void copy();

    bool hasSelection() const;
    void selectAll();

    void find();
Q_SIGNALS:
    void titleChanged(KSieveUi::SieveEditorHelpHtmlWidget *widget, const QString &title);
    void progressIndicatorPixmapChanged(KSieveUi::SieveEditorHelpHtmlWidget *widget, const QPixmap &);
    void loadFinished(KSieveUi::SieveEditorHelpHtmlWidget *widget, bool success);
    void copyAvailable(bool);

private:
    void slotTitleChanged(const QString &title);
    void slotFinished(bool b);
    void slotLoadStarted();
    void slotPixmapChanged(const QPixmap &pixmap);
    void slotLoadFinished(bool success);
    void slotSelectionChanged();
    QString mTitle;
    SieveEditorWebEngineView *mWebView = nullptr;
    SieveEditorLoadProgressIndicator *mProgressIndicator = nullptr;
    KPIMTextEdit::SlideContainer *mSliderContainer = nullptr;
    KSieveUi::FindBarWebEngineView *mFindBar = nullptr;
    qreal mZoomFactor;
};
}

#endif // SIEVEEDITORHELPHTMLWIDGET_H
