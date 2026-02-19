/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"
#include <QWidget>
class QStackedWidget;
class QLabel;
namespace KSieveUi
{
class SieveScriptDebuggerFrontEndWidget;
/*!
 * \class KSieveUi::SieveScriptDebuggerWidget
 * \brief The SieveScriptDebuggerWidget class
 * \author Laurent Montel <montel@kde.org>
 * \inmodule KSieveUi
 * \inheaderfile KSieveUi/SieveScriptDebuggerWidget
 */
class KSIEVEUI_EXPORT SieveScriptDebuggerWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs a SieveScriptDebuggerWidget with the given parent widget.
     */
    explicit SieveScriptDebuggerWidget(QWidget *parent = nullptr);
    /*!
     * Destroys the SieveScriptDebuggerWidget.
     */
    ~SieveScriptDebuggerWidget() override;

    /*!
     * Sets the Sieve script to be debugged.
     * \param script The Sieve script content.
     */
    void setScript(const QString &script);
    /*!
     * Returns the current Sieve script content.
     * \return The script text.
     */
    [[nodiscard]] QString script() const;

    /*!
     * Checks if the debugger can accept the current script.
     * \return true if the script is valid and can be debugged, false otherwise.
     */
    [[nodiscard]] bool canAccept() const;

    /*!
     * Returns the sizes of the widget's splitter sections.
     * \return A list of integer sizes for each splitter section.
     */
    [[nodiscard]] QList<int> splitterSizes() const;
    /*!
     * Sets the sizes of the widget's splitter sections.
     * \param sizes A list of integer sizes to apply to the splitter sections.
     */
    void setSplitterSizes(const QList<int> &sizes);

    /*!
     * Checks if debug applications are available on the system.
     * \return true if debug applications are available, false otherwise.
     */
    [[nodiscard]] bool haveDebugApps() const;

Q_SIGNALS:
    /*!
     * Emitted when the script text has been modified by the user.
     */
    void scriptTextChanged();
    /*!
     * Emitted when the debug script button is clicked.
     */
    void debugScriptButtonClicked();
    /*!
     * Emitted when the debug button's enabled state changes.
     * \param state true if the debug button is enabled, false otherwise.
     */
    void debugButtonEnabled(bool state);
    /*!
     * Emitted when the Sieve test application cannot be found on the system.
     */
    void sieveTestNotFound();

private:
    KSIEVEUI_NO_EXPORT void checkSieveTestApplication();
    QStackedWidget *mStackedWidget = nullptr;
    SieveScriptDebuggerFrontEndWidget *mSieveScriptFrontEnd = nullptr;
    QLabel *mSieveNoExistingFrontEnd = nullptr;
    bool mHaveDebugApps = false;
};
}
