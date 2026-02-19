/* SPDX-FileCopyrightText: 2010 Torgny Nyblom <nyblom@kde.org>
 * SPDX-FileCopyrightText: 2010-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "ksieveui_export.h"
#include <QWidget>
class QAction;
class QPushButton;
class QMenu;
class QLabel;
namespace PimCommon
{
class LineEditWithCompleterNg;
}
namespace KSieveUi
{
/*!
 * \brief The FindBarBase class*
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT FindBarBase : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs a FindBarBase with the given parent widget.
     * \param parent The parent widget.
     */
    explicit FindBarBase(QWidget *parent = nullptr);
    /*!
     * Destroys the FindBarBase.
     */
    ~FindBarBase() override;

    /*!
     * Returns the current search text.
     * \return The text in the find field.
     */
    [[nodiscard]] QString text() const;
    /*!
     * Sets the search text.
     * \param text The text to search for.
     */
    void setText(const QString &text);
    /*!
     * Focuses the find bar and sets the cursor in the search field.
     */
    void focusAndSetCursor();

protected:
    /*!
     * Handles events for the find bar.
     * \param e The event to handle.
     * \return true if the event was handled.
     */
    bool event(QEvent *e) override;
    /*!
     * Clears search selections in the document.
     */
    virtual void clearSelections();
    /*!
     * Updates the highlight of search results.
     * \param true to highlight all matches.
     */
    virtual void updateHighLight(bool);
    /*!
     * Searches for text in the document.
     * \param backward true to search backward.
     * \param isAutoSearch true if this is an automatic search.
     */
    virtual void searchText(bool backward, bool isAutoSearch);
    /*!
     * Updates the sensitivity of search controls.
     * \param true if search is enabled.
     */
    virtual void updateSensitivity(bool);

    /*!
     * Sets whether a match was found.
     * \param match true if a match was found.
     */
    void setFoundMatch(bool match);
    /*!
     * Returns the options menu for the find bar.
     * \return The QMenu instance.
     */
    QMenu *optionsMenu();

public Q_SLOTS:
    /*!
     * Finds the next occurrence of the search text.
     */
    void findNext();
    /*!
     * Finds the previous occurrence of the search text.
     */
    void findPrev();
    /*!
     * Automatically searches as the user types.
     * \param str The text to search for.
     */
    void autoSearch(const QString &str);
    /*!
     * Performs a text search.
     * \param backward true to search backward.
     * \param isAutoSearch true if this is an automatic search.
     */
    void slotSearchText(bool backward = false, bool isAutoSearch = true);
    /*!
     * Closes the find bar.
     */
    void closeBar();

Q_SIGNALS:
    /*!
     * Emitted when the find bar should be hidden.
     */
    void hideFindBar();

protected Q_SLOTS:
    /*!
     * Handles case sensitivity setting changes.
     * \param true if case-sensitive search is enabled.
     */
    void caseSensitivityChanged(bool);
    /*!
     * Handles the highlight all setting changes.
     * \param true to highlight all matches.
     */
    void slotHighlightAllChanged(bool);

protected:
    QString mNotFoundString;
    QString mPositiveBackground;
    QString mNegativeBackground;
    QString mLastSearchStr;
    PimCommon::LineEditWithCompleterNg *mSearch = nullptr;
    QAction *mCaseSensitiveAct = nullptr;

    QPushButton *mFindPrevBtn = nullptr;
    QPushButton *mFindNextBtn = nullptr;
    QMenu *mOptionsMenu = nullptr;
    QLabel *mStatus = nullptr;
    void addToCompletion(const QString &text);
};
}
