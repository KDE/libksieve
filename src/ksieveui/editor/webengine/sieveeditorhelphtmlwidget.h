/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVEEDITORHELPHTMLWIDGET_H
#define SIEVEEDITORHELPHTMLWIDGET_H

#include "ksieveui_export.h"
#include <QUrl>
#include <QWidget>

namespace KPIMTextEdit
{
class SlideContainer;
}

namespace KSieveUi
{
class SieveEditorWebEngineView;
class SieveEditorLoadProgressIndicator;
class FindBarWebEngineView;
class KSIEVEUI_EXPORT SieveEditorHelpHtmlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveEditorHelpHtmlWidget(QWidget *parent = nullptr);
    ~SieveEditorHelpHtmlWidget() override;

    void openUrl(const QUrl &currentUrl);
    Q_REQUIRED_RESULT QString title() const;

    Q_REQUIRED_RESULT QUrl currentUrl() const;

    void resetZoom();
    void zoomOut();
    void zoomIn();
    void copy();

    Q_REQUIRED_RESULT bool hasSelection() const;
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
    qreal mZoomFactor = 100;
};
}

#endif // SIEVEEDITORHELPHTMLWIDGET_H
