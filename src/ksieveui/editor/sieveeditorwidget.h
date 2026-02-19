/*
  SPDX-FileCopyrightText: 2014-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once
#include "ksieveui_export.h"

#include <QWidget>
class QStackedWidget;
class QLineEdit;
class QAction;
namespace KSieveCore
{
class SieveImapAccountSettings;
}
namespace KSieveUi
{
class SieveEditorTextModeWidget;
class SieveEditorGraphicalModeWidget;
/*!
 * \brief The SieveEditorWidget class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveEditorWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs a SieveEditorWidget with the given settings and parent widget.
     * \param useMenuBar true to display a menu bar.
     * \param parent The parent widget.
     */
    explicit SieveEditorWidget(bool useMenuBar, QWidget *parent = nullptr);
    /*!
     * Destroys the SieveEditorWidget.
     */
    ~SieveEditorWidget() override;
    enum EditorMode {
        Unknown = -1,
        TextMode = 0,
        GraphicMode = 1
    };

    /*!
     * Sets the editor to read-only mode.
     * \param b true to make the editor read-only.
     */
    void setReadOnly(bool b);
    /*!
     * Returns the current Sieve script.
     * \return The script content.
     */
    [[nodiscard]] QString script() const;
    /*!
     * Returns the original script before any modifications.
     * \return The original script content.
     */
    [[nodiscard]] QString originalScript() const;
    /*!
     * Sets the Sieve script content.
     * \param script The script to set.
     * \param clearUndoRedo true to clear undo/redo history.
     */
    void setScript(const QString &script, bool clearUndoRedo = false);
    /*!
     * Sets the debug script for debugging purposes.
     * \param debug The debug script content.
     */
    void setDebugScript(const QString &debug);
    /*!
     * Sets the name of the Sieve script.
     * \param name The script name.
     */
    void setScriptName(const QString &name);

    /*!
     * Marks the script as done (successful execution).
     */
    void resultDone();

    /*!
     * Sets the Sieve capabilities supported by the server.
     * \param capabilities A list of capability strings.
     */
    void setSieveCapabilities(const QStringList &capabilities);
    /*!
     * Sets the IMAP account settings for Sieve.
     * \param sieveImapAccountSettings The account settings.
     */
    void setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &sieveImapAccountSettings);
    /*!
     * Sets the list of include files available for Sieve scripts.
     * \param listOfIncludeFile The list of include file names.
     */
    void setListOfIncludeFile(const QStringList &listOfIncludeFile);

    /*!
     * Adds a failed message to the message display area.
     * \param err The error message text.
     */
    void addFailedMessage(const QString &err);
    /*!
     * Adds a success message to the message display area.
     * \param msg The success message text.
     */
    void addOkMessage(const QString &msg);
    /*!
     * Adds a normal message to the message display area.
     * \param msg The message text.
     */
    void addNormalMessage(const QString &msg);

    /*!
     * Sets the modified state of the script.
     * \param b true if the script has been modified.
     */
    void setModified(bool b);
    /*!
     * Checks if the script has been modified.
     * \return true if the script is modified.
     */
    [[nodiscard]] bool isModified() const;

    /*!
     * Returns the current editor mode.
     * \return The EditorMode enumeration value.
     */
    [[nodiscard]] EditorMode mode() const;

    /*!
     * Checks if redo is available.
     * \return true if redo can be performed.
     */
    [[nodiscard]] bool isRedoAvailable() const;
    /*!
     * Checks if undo is available.
     * \return true if undo can be performed.
     */
    [[nodiscard]] bool isUndoAvailable() const;
    /*!
     * Checks if text is currently selected.
     * \return true if there is a selection.
     */
    [[nodiscard]] bool hasSelection() const;

    /*!
     * Starts a spell check of the script.
     */
    void checkSpelling();
    /*!
     * Converts the selected text to lowercase.
     */
    void lowerCase();
    /*!
     * Converts the selected text to uppercase.
     */
    void upperCase();
    /*!
     * Converts the selected text to sentence case.
     */
    void sentenceCase();
    /*!
     * Reverses the case of the selected text.
     */
    void reverseCase();
    /*!
     * Zooms out the editor view.
     */
    void zoomOut();
    /*!
     * Zooms in the editor view.
     */
    void zoomIn();
    /*!
     * Returns the current help title.
     * \return The help title string.
     */
    [[nodiscard]] QString currentHelpTitle() const;
    /*!
     * Returns the current help URL.
     * \return The help URL.
     */
    [[nodiscard]] QUrl currentHelpUrl() const;
    /*!
     * Opens a bookmarked help URL.
     * \param url The URL to open.
     */
    void openBookmarkUrl(const QUrl &url);
    /*!
     * Starts debugging the Sieve script.
     */
    void debugSieveScript();
    /*!
     * Resets the zoom level to default.
     */
    void zoomReset();
    /*!
     * Checks if word wrap is enabled.
     * \return true if word wrap is active.
     */
    [[nodiscard]] bool isWordWrap() const;

    /*!
     * Updates the original script content (marks current as original).
     */
    void updateOriginalScript();
    /*!
     * Prints the script.
     */
    void print();
    /*!
     * Shows the print preview dialog.
     */
    void printPreview();

    /*!
     * Checks if print support is enabled.
     * \return true if printing is supported.
     */
    [[nodiscard]] bool printSupportEnabled() const;
    /*!
     * Checks if the editor is in text editor mode.
     * \return true if in text mode.
     */
    [[nodiscard]] bool isTextEditor() const;
public Q_SLOTS:
    /*!
     * Opens the find dialog.
     */
    void find();
    /*!
     * Opens the find and replace dialog.
     */
    void replace();
    /*!
     * Undoes the last action.
     */
    void undo();
    /*!
     * Redoes the last undone action.
     */
    void redo();
    /*!
     * Pastes content from the clipboard.
     */
    void paste();
    /*!
     * Copies the selected text to the clipboard.
     */
    void copy();
    /*!
     * Cuts the selected text and places it on the clipboard.
     */
    void cut();
    /*!
     * Selects all text in the editor.
     */
    void selectAll();
    /*!
     * Opens the go to line dialog.
     */
    void goToLine();
    /*!
     * Opens the save as dialog.
     */
    void slotSaveAs();
    /*!
     * Opens the import dialog.
     */
    void slotImport();
    /*!
     * Opens the graphical rule creator.
     */
    void slotCreateRulesGraphically();
    /*!
     * Checks the syntax of the script.
     */
    void slotCheckSyntax();
    /*!
     * Comments out the selected lines.
     */
    void comment();
    /*!
     * Uncomments the selected lines.
     */
    void uncomment();
    /*!
     * Enables or disables word wrap.
     * \param state true to enable word wrap.
     */
    void setWordWrap(bool state);

Q_SIGNALS:
    /*!
     * Emitted when syntax check is requested.
     */
    void checkSyntax();
    /*!
     * Emitted when the OK button should be enabled or disabled.
     * \param b true to enable the button.
     */
    void enableButtonOk(bool b);
    /*!
     * Emitted when the script value has changed.
     * \param b true if the script has been modified.
     */
    void valueChanged(bool b);
    /*!
     * Emitted when the editor mode has changed.
     * \param The new editor mode.
     */
    void modeEditorChanged(KSieveUi::SieveEditorWidget::EditorMode);
    /*!
     * Emitted when undo availability changes.
     * \param true if undo is now available.
     */
    void undoAvailable(bool);
    /*!
     * Emitted when redo availability changes.
     * \param true if redo is now available.
     */
    void redoAvailable(bool);
    /*!
     * Emitted when text copy availability changes.
     * \param true if text can be copied.
     */
    void copyAvailable(bool);
    /*!
     * Emitted when the editor mode is about to change.
     * \param true if changing mode.
     */
    void changeModeEditor(bool);
    /*!
     * Emitted when the editor tab has changed.
     */
    void sieveEditorTabCurrentChanged();

private:
    KSIEVEUI_NO_EXPORT void slotEnableButtonOk(bool b);
    KSIEVEUI_NO_EXPORT void slotGenerateXml();
    KSIEVEUI_NO_EXPORT void slotSwitchMode();
    KSIEVEUI_NO_EXPORT void slotSwitchTextMode(const QString &script);
    KSIEVEUI_NO_EXPORT void slotSwitchToGraphicalMode();
    KSIEVEUI_NO_EXPORT void slotModified();
    KSIEVEUI_NO_EXPORT void changeSwitchButtonText();
    KSIEVEUI_NO_EXPORT void changeMode(EditorMode mode);
    KSIEVEUI_NO_EXPORT void addMessageEntry(const QString &errorMsg, const QColor &color);
    KSIEVEUI_NO_EXPORT void slotServerInfo();
    QString mOriginalScript;
    SieveEditorTextModeWidget *mTextModeWidget = nullptr;
    SieveEditorGraphicalModeWidget *mGraphicalModeWidget = nullptr;
    QStackedWidget *mStackedWidget = nullptr;
    QLineEdit *mScriptName = nullptr;
    QAction *mCheckSyntax = nullptr;
    QAction *mSwitchMode = nullptr;
    QAction *mCreateRulesGraphically = nullptr;
    QAction *mSaveAs = nullptr;
    QAction *mGenerateXml = nullptr;
    QAction *mServerInfo = nullptr;
    EditorMode mMode = EditorMode::TextMode;
    bool mModified = false;
    bool mDebug = false;
};
}
