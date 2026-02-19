/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "ksieveui_export.h"
#include <QTabWidget>

namespace KSieveUi
{
class SieveEditorHelpHtmlWidget;
/*!
 * \brief The SieveEditorTabWidget class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveEditorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs a SieveEditorTabWidget with the given parent widget.
     * \param parent The parent widget.
     */
    explicit SieveEditorTabWidget(QWidget *parent = nullptr);
    /*!
     * Destroys the SieveEditorTabWidget.
     */
    ~SieveEditorTabWidget() override;

    /*!
     * Returns the current help URL in the active tab.
     * \return The help URL.
     */
    [[nodiscard]] QUrl currentHelpUrl() const;
    /*!
     * Returns the current help title in the active tab.
     * \return The help title.
     */
    [[nodiscard]] QString currentHelpTitle() const;
    /*!
     * Checks if the current page is an HTML help page.
     * \return true if the current page is HTML.
     */
    bool currentPageIsHtmlPage() const;

public Q_SLOTS:
    /*!
     * Adds a new help page tab with the given URL.
     * \param url The URL of the help page to add.
     */
    void slotAddHelpPage(const QUrl &url);

protected:
    /*!
     * Handles tab removal.
     * \param index The index of the removed tab.
     */
    void tabRemoved(int index) override;
    /*!
     * Handles tab insertion.
     * \param index The index of the inserted tab.
     */
    void tabInserted(int index) override;

Q_SIGNALS:
    /*!
     * Emitted when copy availability changes.
     * \param true if text can be copied.
     */
    void copyAvailable(bool);

private:
    KSIEVEUI_NO_EXPORT void slotTitleChanged(KSieveUi::SieveEditorHelpHtmlWidget *widget, const QString &title);
    KSIEVEUI_NO_EXPORT void slotTabCloseRequested(int index);
    KSIEVEUI_NO_EXPORT void slotProgressIndicatorPixmapChanged(KSieveUi::SieveEditorHelpHtmlWidget *widget, const QPixmap &pixmap);
    KSIEVEUI_NO_EXPORT void slotLoadFinished(KSieveUi::SieveEditorHelpHtmlWidget *widget, bool success);
    KSIEVEUI_NO_EXPORT void slotTabContextMenuRequest(const QPoint &pos);

    KSIEVEUI_NO_EXPORT void slotCloseAllTabExcept(int index);
    KSIEVEUI_NO_EXPORT void slotCloseAllTab();
    KSIEVEUI_NO_EXPORT void slotCloseRequest(int index);
    KSIEVEUI_NO_EXPORT void closeAllTabExcept(int index = -1);
};
}
