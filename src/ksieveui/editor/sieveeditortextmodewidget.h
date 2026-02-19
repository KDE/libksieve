/* SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "config-libksieve.h"
#include "ksieveui_export.h"
#include "sieveeditorabstractwidget.h"
class QSplitter;
namespace TextAddonsWidgets
{
class SlideContainer;
}
namespace TextCustomEditor
{
class TextGoToLineWidget;
class PlainTextEditFindBar;
class PlainTextEditor;
class PlainTextEditorWidget;
}
namespace PimCommon
{
class PurposeMenuMessageWidget;
}
#if HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}
#endif
namespace KSieveUi
{
class SieveTextEdit;
class SieveEditorWarning;
class SieveEditorParsingMissingFeatureWarning;
class SieveEditorTabWidget;
class SieveTemplateWidget;
/*!
 * \brief The SieveEditorTextModeWidget class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveEditorTextModeWidget : public SieveEditorAbstractWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs a SieveEditorTextModeWidget with the given parent widget.
     * \param parent The parent widget.
     */
    explicit SieveEditorTextModeWidget(QWidget *parent = nullptr);
    /*!
     * Destroys the SieveEditorTextModeWidget.
     */
    ~SieveEditorTextModeWidget() override;

    /*!
     * Sets the Sieve capabilities supported by the server.
     * \param capabilities A list of capability strings.
     */
    void setSieveCapabilities(const QStringList &capabilities);
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
     * Returns the current script being edited.
     * \return The script content.
     */
    [[nodiscard]] QString currentscript() override;
    /*!
     * Sets a script to be imported.
     * \param script The script to import.
     */
    void setImportScript(const QString &script) override;

    /*!
     * Creates rules graphically from the current script.
     */
    void createRulesGraphically();
    /*!
     * Generates XML representation of the script.
     */
    void generateXml();

    /*!
     * Shows an editor warning message.
     */
    void showEditorWarning();
    /*!
     * Hides the editor warning message.
     */
    void hideEditorWarning();

    /*!
     * Shows a parsing error warning.
     */
    void showParsingEditorWarning();

    /*!
     * Sets a parsing error warning with script and error details.
     * \param script The problematic script.
     * \param error The error message.
     */
    void setParsingEditorWarningError(const QString &script, const QString &error);
    /*!
     * Opens the find and replace dialog.
     */
    void replace();
    /*!
     * Opens the find dialog.
     */
    void find();
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
     * Cuts the selected text and places it on the clipboard.
     */
    void cut();
    /*!
     * Copies the selected text to the clipboard.
     */
    void copy();
    /*!
     * Starts a spell check of the script.
     */
    void checkSpelling();
    /*!
     * Comments out the selected lines.
     */
    void comment();
    /*!
     * Uncomments the selected lines.
     */
    void uncomment();

    /*!
     * Checks if undo is available.
     * \return true if undo can be performed.
     */
    [[nodiscard]] bool isUndoAvailable() const;
    /*!
     * Checks if redo is available.
     * \return true if redo can be performed.
     */
    [[nodiscard]] bool isRedoAvailable() const;
    /*!
     * Checks if text is currently selected.
     * \return true if there is a selection.
     */
    [[nodiscard]] bool hasSelection() const;

    /*!
     * Selects all text in the editor.
     */
    void selectAll();
    /*!
     * Converts the selected text to uppercase.
     */
    void upperCase();
    /*!
     * Converts the selected text to lowercase.
     */
    void lowerCase();
    /*!
     * Converts the selected text to sentence case.
     */
    void sentenceCase();
    /*!
     * Reverses the case of the selected text.
     */
    void reverseCase();
    /*!
     * Zooms in the editor view.
     */
    void zoomIn();
    /*!
     * Zooms out the editor view.
     */
    void zoomOut();
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
     * Enables or disables word wrap.
     * \param state true to enable word wrap.
     */
    void setWordWrap(bool state);
    /*!
     * Checks if word wrap is enabled.
     * \return true if word wrap is active.
     */
    [[nodiscard]] bool isWordWrap() const;
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
    /*!
     * Returns the editor tab widget.
     * \return The SieveEditorTabWidget instance.
     */
    [[nodiscard]] SieveEditorTabWidget *tabWidget() const;

    /*!
     * Sets the list of include files available for Sieve scripts.
     * \param listOfIncludeFile The list of include file names.
     */
    void setListOfIncludeFile(const QStringList &listOfIncludeFile);
    /*!
     * Returns the Sieve capabilities for this editor.
     * \return A list of capability strings.
     */
    [[nodiscard]] QStringList sieveCapabilities() const;

Q_SIGNALS:
    /*!
     */
    void enableButtonOk(bool);
    /*!
     */
    void switchToGraphicalMode();
    /*!
     */
    void valueChanged();
    /*!
     */
    void undoAvailable(bool);
    /*!
     */
    void redoAvailable(bool);
    /*!
     */
    void copyAvailable(bool);
    /*!
     */
    void sieveEditorTabCurrentChanged();

public Q_SLOTS:
    /*!
     */
    void slotShowGoToLine();
    /*!
     */
    void slotShareError(const QString &message);
    /*!
     */
    void slotShareSuccess(const QString &url);

private:
    KSIEVEUI_NO_EXPORT void slotTextChanged();
    KSIEVEUI_NO_EXPORT void slotFind();
    KSIEVEUI_NO_EXPORT void slotReplace();
    KSIEVEUI_NO_EXPORT void slotGoToLine(int line);
    KSIEVEUI_NO_EXPORT void readConfig();
    KSIEVEUI_NO_EXPORT void writeConfig();
    KSIEVEUI_NO_EXPORT void slotEditRule(const QString &selectedText);
    KSIEVEUI_NO_EXPORT void slotInsertRule();
    KSIEVEUI_NO_EXPORT void insertRequires(const QStringList &needToAddRequire);
    [[nodiscard]] KSIEVEUI_NO_EXPORT QStringList insertNecessaryRequires(const QStringList &requireModules);
    QString mOriginalScript;
    QStringList mSieveCapabilities;
    QStringList mListOfIncludeFile;
    SieveTextEdit *mTextEdit = nullptr;
    TextCustomEditor::PlainTextEditorWidget *mDebugTextEdit = nullptr;
    TextCustomEditor::PlainTextEditFindBar *mFindBar = nullptr;
#if HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
    TextEditTextToSpeech::TextToSpeechContainerWidget *mTextToSpeechWidget = nullptr;
#endif
    QSplitter *mMainSplitter = nullptr;
    QSplitter *mTemplateSplitter = nullptr;
    SieveEditorWarning *mSieveEditorWarning = nullptr;
    SieveEditorParsingMissingFeatureWarning *mSieveParsingWarning = nullptr;
    SieveEditorTabWidget *mTabWidget = nullptr;
    TextCustomEditor::TextGoToLineWidget *mGoToLine = nullptr;
    TextAddonsWidgets::SlideContainer *mGotoLineSliderContainer = nullptr;
    TextAddonsWidgets::SlideContainer *mSliderContainer = nullptr;
    SieveTemplateWidget *mSieveTemplateWidget = nullptr;
    QWidget *mEditorWidget = nullptr;
    PimCommon::PurposeMenuMessageWidget *const mPurposeMenuMessageWidget;
};
}
