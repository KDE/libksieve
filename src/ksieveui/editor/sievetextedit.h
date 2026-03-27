/* SPDX-FileCopyrightText: 2011-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "config-libksieveui.h"
#include "ksieveui_export.h"
#include <TextCustomEditor/PlainTextEditor>
#include <memory>

class QMenu;
namespace TextAutoGenerateText
{
class TextAutoGenerateManager;
}
namespace KSieveUi
{
class SieveTextEditPrivate;
/*!
 * \brief The SieveTextEdit class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveTextEdit : public TextCustomEditor::PlainTextEditor
{
    Q_OBJECT

public:
    /*!
     * Constructs a SieveTextEdit with the given parent widget.
     * \param parent The parent widget.
     */
    explicit SieveTextEdit(QWidget *parent = nullptr);
#if HAVE_TEXT_AUTOGENERATE_TEXT
    /*!
     * Constructs a SieveTextEdit with the given parent widget.
     * \param parent The parent widget.
     * \param manager The text autogenerate manager.
     */
    explicit SieveTextEdit(TextAutoGenerateText::TextAutoGenerateManager *manager, QWidget *parent = nullptr);
#endif

    /*!
     * Destroys the SieveTextEdit.
     */
    ~SieveTextEdit() override;

    /*!
     * Paints the line number area.
     * \param event The paint event.
     */
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    /*!
     * Returns the width needed for the line number area.
     * \return The width in pixels.
     */
    [[nodiscard]] int lineNumberAreaWidth() const;

    /*!
     * Sets the Sieve capabilities supported by the server.
     * \param capabilities A list of capability strings.
     */
    void setSieveCapabilities(const QStringList &capabilities);
    /*!
     * Shows or hides the help menu.
     * \param b true to show the help menu.
     */
    void setShowHelpMenu(bool b);

    /*!
     * Uncomments the selected lines.
     */
    void uncomment();
    /*!
     * Comments out the selected lines.
     */
    void comment();
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
     * Enables or disables word wrap.
     * \param state true to enable word wrap.
     */
    void setWordWrap(bool state);
    /*!
     * Checks if word wrap is enabled.
     * \return true if word wrap is active.
     */
    [[nodiscard]] bool isWordWrap() const;

protected:
    /*!
     * Initializes the auto-completer.
     */
    void initCompleter();
    /*!
     * Handles key press events.
     * \param e The key event.
     */
    void keyPressEvent(QKeyEvent *e) override;
    /*!
     * Handles resize events.
     * \param event The resize event.
     */
    void resizeEvent(QResizeEvent *event) override;
    /*!
     * Adds extra menu entries to the context menu.
     * \param menu The context menu.
     * \param pos The position where the menu was requested.
     */
    void addExtraMenuEntry(QMenu *menu, QPoint pos) override;
    /*!
     * Handles events for the editor.
     * \param ev The event to handle.
     * \return true if the event was handled.
     */
    bool event(QEvent *ev) override;
    /*!
     * Creates a spell check decorator for the editor.
     * \return The SpellCheckDecorator instance.
     */
    Sonnet::SpellCheckDecorator *createSpellCheckDecorator() override;

    /*!
     * Updates the syntax highlighter.
     */
    void updateHighLighter() override;
    /*!
     * Clears the spell check decorator.
     */
    void clearDecorator() override;
    /*!
     * Creates the syntax highlighter.
     */
    void createHighlighter() override;
Q_SIGNALS:
    /*!
     * Emitted when a help URL should be opened.
     * \param url The help URL.
     */
    void openHelp(const QUrl &url);
    /*!
     * Emitted when a rule should be edited.
     * \param selectedText The selected text to edit as a rule.
     */
    void editRule(const QString &selectedText);
    /*!
     */
    void insertRule();

private:
    KSIEVEUI_NO_EXPORT void slotUpdateLineNumberAreaWidth(int newBlockCount);
    KSIEVEUI_NO_EXPORT void slotUpdateLineNumberArea(const QRect &, int);
    KSIEVEUI_NO_EXPORT void slotHelp();
    KSIEVEUI_NO_EXPORT void slotEditRule();
    [[nodiscard]] KSIEVEUI_NO_EXPORT bool openVariableHelp();
    KSIEVEUI_NO_EXPORT bool overrideShortcut(QKeyEvent *event);
    [[nodiscard]] KSIEVEUI_NO_EXPORT QStringList completerList() const;
    KSIEVEUI_NO_EXPORT void setCompleterList(const QStringList &list);
    [[nodiscard]] KSIEVEUI_NO_EXPORT QString selectedWord(QPoint pos = QPoint()) const;
    KSIEVEUI_NO_EXPORT void slotQuickAsk();
    KSIEVEUI_NO_EXPORT void initialize();

    std::unique_ptr<SieveTextEditPrivate> const d;
};
}
