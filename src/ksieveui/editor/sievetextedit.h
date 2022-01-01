/* SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "ksieveui_export.h"
#include <KPIMTextEdit/PlainTextEditor>
#include <memory>

class QMenu;
namespace KSieveUi
{
class SieveTextEditPrivate;
/**
 * @brief The SieveTextEdit class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveTextEdit : public KPIMTextEdit::PlainTextEditor
{
    Q_OBJECT

public:
    explicit SieveTextEdit(QWidget *parent = nullptr);
    ~SieveTextEdit() override;

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    Q_REQUIRED_RESULT int lineNumberAreaWidth() const;

    void setSieveCapabilities(const QStringList &capabilities);

    void setShowHelpMenu(bool b);

    void uncomment();
    void comment();
    void upperCase();
    void lowerCase();
    void sentenceCase();
    void reverseCase();
    void wordWrap(bool state);
    Q_REQUIRED_RESULT bool isWordWrap() const;

protected:
    void initCompleter();
    void keyPressEvent(QKeyEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;
    void addExtraMenuEntry(QMenu *menu, QPoint pos) override;
    bool event(QEvent *ev) override;
    Sonnet::SpellCheckDecorator *createSpellCheckDecorator() override;

    void updateHighLighter() override;
    void clearDecorator() override;
    void createHighlighter() override;
Q_SIGNALS:
    void openHelp(const QUrl &url);
    void editRule(const QString &selectedText);
    void insertRule();

private:
    void slotUpdateLineNumberAreaWidth(int newBlockCount);
    void slotUpdateLineNumberArea(const QRect &, int);
    void slotHelp();
    void slotEditRule();
    Q_REQUIRED_RESULT bool openVariableHelp();
    bool overrideShortcut(QKeyEvent *event);
    Q_REQUIRED_RESULT QStringList completerList() const;
    void setCompleterList(const QStringList &list);
    Q_REQUIRED_RESULT QString selectedWord(QPoint pos = QPoint()) const;

    std::unique_ptr<SieveTextEditPrivate> const d;
};
}
