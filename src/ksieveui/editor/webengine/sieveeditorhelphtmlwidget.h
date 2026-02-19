/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

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
/*!
 * \brief The SieveEditorHelpHtmlWidget class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveEditorHelpHtmlWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs a SieveEditorHelpHtmlWidget with the given parent widget.
     * \param parent The parent widget.
     */
    explicit SieveEditorHelpHtmlWidget(QWidget *parent = nullptr);
    /*!
     * Destroys the SieveEditorHelpHtmlWidget.
     */
    ~SieveEditorHelpHtmlWidget() override;

    /*!
     * Opens the given URL in the help widget.
     * \param currentUrl The URL to load.
     */
    void openUrl(const QUrl &currentUrl);
    /*!
     * Returns the title of the currently loaded page.
     * \return The page title.
     */
    [[nodiscard]] QString title() const;

    /*!
     * Returns the current URL being displayed.
     * \return The current URL.
     */
    [[nodiscard]] QUrl currentUrl() const;

    /*!
     * Resets the zoom level to default.
     */
    void resetZoom();
    /*!
     * Zooms out the view.
     */
    void zoomOut();
    /*!
     * Zooms in the view.
     */
    void zoomIn();
    /*!
     * Copies the selected text to the clipboard.
     */
    void copy();

    /*!
     * Checks if there is selected text.
     * \return true if text is selected.
     */
    [[nodiscard]] bool hasSelection() const;
    /*!
     * Selects all text in the page.
     */
    void selectAll();

    /*!
     * Opens the find dialog.
     */
    void find();
Q_SIGNALS:
    /*!
     * Emitted when the page title changes.
     * \param widget This widget instance.
     * \param title The new title.
     */
    void titleChanged(KSieveUi::SieveEditorHelpHtmlWidget *widget, const QString &title);
    /*!
     * Emitted when the progress indicator pixmap changes.
     * \param widget This widget instance.
     * \param The new progress pixmap.
     */
    void progressIndicatorPixmapChanged(KSieveUi::SieveEditorHelpHtmlWidget *widget, const QPixmap &);
    /*!
     * Emitted when page loading finishes.
     * \param widget This widget instance.
     * \param success true if loading was successful.
     */
    void loadFinished(KSieveUi::SieveEditorHelpHtmlWidget *widget, bool success);
    /*!
     * Emitted when copy availability changes.
     * \param true if text can be copied.
     */
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
