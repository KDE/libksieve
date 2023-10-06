/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "ksieveui_export.h"
#include <QUrl>
#include <QWidget>

namespace TextAddonsWidgets
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
    [[nodiscard]] QString title() const;

    [[nodiscard]] QUrl currentUrl() const;

    void resetZoom();
    void zoomOut();
    void zoomIn();
    void copy();

    [[nodiscard]] bool hasSelection() const;
    void selectAll();

    void find();
Q_SIGNALS:
    void titleChanged(KSieveUi::SieveEditorHelpHtmlWidget *widget, const QString &title);
    void progressIndicatorPixmapChanged(KSieveUi::SieveEditorHelpHtmlWidget *widget, const QPixmap &);
    void loadFinished(KSieveUi::SieveEditorHelpHtmlWidget *widget, bool success);
    void copyAvailable(bool);

private:
    KSIEVEUI_NO_EXPORT void slotTitleChanged(const QString &title);
    KSIEVEUI_NO_EXPORT void slotFinished(bool b);
    KSIEVEUI_NO_EXPORT void slotLoadStarted();
    KSIEVEUI_NO_EXPORT void slotPixmapChanged(const QPixmap &pixmap);
    KSIEVEUI_NO_EXPORT void slotLoadFinished(bool success);
    KSIEVEUI_NO_EXPORT void slotSelectionChanged();
    QString mTitle;
    SieveEditorWebEngineView *mWebView = nullptr;
    SieveEditorLoadProgressIndicator *mProgressIndicator = nullptr;
    TextAddonsWidgets::SlideContainer *mSliderContainer = nullptr;
    KSieveUi::FindBarWebEngineView *mFindBar = nullptr;
    qreal mZoomFactor = 100;
};
}
