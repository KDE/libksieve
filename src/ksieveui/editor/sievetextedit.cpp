/* SPDX-FileCopyrightText: 2011-2020 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "sievetextedit.h"
#include "editor/sievelinenumberarea.h"
#include "editor/sieveeditorutil.h"
#include "editor/sievetexteditorspellcheckdecorator.h"

#include <KPIMTextEdit/PlainTextSyntaxSpellCheckingHighlighter>
#include <KPIMTextEdit/EditorUtil>
#include <KPIMTextEdit/TextEditorCompleter>

#include <KLocalizedString>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Theme>

#include <QAction>
#include <QIcon>
#include <QAbstractItemView>
#include <QKeyEvent>
#include <QPainter>
#include <QMenu>
#include <QFontDatabase>
#include <QCompleter>
#include <QTextDocumentFragment>
using namespace KSieveUi;

class KSieveUi::SieveTextEditPrivate
{
public:
    SieveTextEditPrivate()
    {
    }

    SieveLineNumberArea *m_sieveLineNumberArea = nullptr;
    KPIMTextEdit::TextEditorCompleter *mTextEditorCompleter = nullptr;
    KSyntaxHighlighting::Repository mSyntaxRepo;
    bool mShowHelpMenu = true;
};

SieveTextEdit::SieveTextEdit(QWidget *parent)
    : KPIMTextEdit::PlainTextEditor(parent)
    , d(new KSieveUi::SieveTextEditPrivate)
{
    setSpellCheckingConfigFileName(QStringLiteral("sieveeditorrc"));
    setWordWrapMode(QTextOption::NoWrap);
    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    d->m_sieveLineNumberArea = new SieveLineNumberArea(this);

    connect(this, &SieveTextEdit::blockCountChanged, this, &SieveTextEdit::slotUpdateLineNumberAreaWidth);
    connect(this, &SieveTextEdit::updateRequest, this, &SieveTextEdit::slotUpdateLineNumberArea);

    slotUpdateLineNumberAreaWidth(0);

    initCompleter();
    createHighlighter();
}

SieveTextEdit::~SieveTextEdit()
{
    // disconnect these manually as the destruction of KPIMTextEdit::PlainTextEditorPrivate will trigger them
    disconnect(this, &SieveTextEdit::blockCountChanged, this, &SieveTextEdit::slotUpdateLineNumberAreaWidth);
    disconnect(this, &SieveTextEdit::updateRequest, this, &SieveTextEdit::slotUpdateLineNumberArea);

    delete d;
}

void SieveTextEdit::updateHighLighter()
{
    auto hlighter = dynamic_cast<KPIMTextEdit::PlainTextSyntaxSpellCheckingHighlighter *>(highlighter());
    if (hlighter) {
        hlighter->toggleSpellHighlighting(checkSpellingEnabled());
    }
}

void SieveTextEdit::clearDecorator()
{
    //Nothing
}

void SieveTextEdit::createHighlighter()
{
    auto highlighter = new KPIMTextEdit::PlainTextSyntaxSpellCheckingHighlighter(this);
    highlighter->toggleSpellHighlighting(checkSpellingEnabled());
    highlighter->setCurrentLanguage(spellCheckingLanguage());
    highlighter->setDefinition(d->mSyntaxRepo.definitionForName(QStringLiteral("Sieve")));
    highlighter->setTheme((palette().color(QPalette::Base).lightness() < 128)
                          ? d->mSyntaxRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                          : d->mSyntaxRepo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    setHighlighter(highlighter);
}

void SieveTextEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    const QRect cr = contentsRect();
    d->m_sieveLineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

int SieveTextEdit::lineNumberAreaWidth() const
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    const int space = 2 + fontMetrics().boundingRect(QLatin1Char('9')).width() * digits;
    return space;
}

void SieveTextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(d->m_sieveLineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            const QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, d->m_sieveLineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void SieveTextEdit::slotUpdateLineNumberAreaWidth(int)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void SieveTextEdit::slotUpdateLineNumberArea(const QRect &rect, int dy)
{
    if (dy) {
        d->m_sieveLineNumberArea->scroll(0, dy);
    } else {
        d->m_sieveLineNumberArea->update(0, rect.y(), d->m_sieveLineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect())) {
        slotUpdateLineNumberAreaWidth(0);
    }
}

QStringList SieveTextEdit::completerList() const
{
    QStringList listWord;

    listWord << QStringLiteral("require") << QStringLiteral("stop");
    listWord << QStringLiteral(":contains") << QStringLiteral(":matches") << QStringLiteral(":is") << QStringLiteral(":over") << QStringLiteral(":under") << QStringLiteral(":all") << QStringLiteral(
        ":domain") << QStringLiteral(":localpart");
    listWord << QStringLiteral("if") << QStringLiteral("elsif") << QStringLiteral("else");
    listWord << QStringLiteral("keep") << QStringLiteral("reject") << QStringLiteral("discard") << QStringLiteral("redirect")  << QStringLiteral("addflag") << QStringLiteral("setflag");
    listWord << QStringLiteral("address") << QStringLiteral("allof") << QStringLiteral("anyof") << QStringLiteral("exists") << QStringLiteral("false") << QStringLiteral("header") << QStringLiteral(
        "not") << QStringLiteral("size") << QStringLiteral("true");
    listWord << QStringLiteral(":days") << QStringLiteral(":seconds") << QStringLiteral(":subject") << QStringLiteral(":addresses") << QStringLiteral(":text");
    listWord << QStringLiteral(":name") << QStringLiteral(":headers") << QStringLiteral(":first") << QStringLiteral(":importance");
    listWord << QStringLiteral(":message") << QStringLiteral(":from");

    return listWord;
}

void SieveTextEdit::setCompleterList(const QStringList &list)
{
    d->mTextEditorCompleter->setCompleterStringList(list);
}

void SieveTextEdit::initCompleter()
{
    const QStringList listWord = completerList();

    d->mTextEditorCompleter = new KPIMTextEdit::TextEditorCompleter(this, this);
    d->mTextEditorCompleter->setCompleterStringList(listWord);
}

bool SieveTextEdit::event(QEvent *ev)
{
    if (ev->type() == QEvent::ShortcutOverride) {
        auto *e = static_cast<QKeyEvent *>(ev);
        if (overrideShortcut(e)) {
            e->accept();
            return true;
        }
    }
    return KPIMTextEdit::PlainTextEditor::event(ev);
}

Sonnet::SpellCheckDecorator *SieveTextEdit::createSpellCheckDecorator()
{
    return new SieveTextEditorSpellCheckDecorator(this);
}

bool SieveTextEdit::overrideShortcut(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F1) {
        if (openVariableHelp()) {
            return true;
        }
    }
    return PlainTextEditor::overrideShortcut(event);
}

bool SieveTextEdit::openVariableHelp()
{
    if (!textCursor().hasSelection()) {
        const QString word = selectedWord();
        const KSieveUi::SieveEditorUtil::HelpVariableName type = KSieveUi::SieveEditorUtil::strToVariableName(word);
        if (type != KSieveUi::SieveEditorUtil::UnknownHelp) {
            const QUrl url = KSieveUi::SieveEditorUtil::helpUrl(type);
            if (!url.isEmpty()) {
                return true;
            }
        }
    }
    return false;
}

void SieveTextEdit::keyPressEvent(QKeyEvent *e)
{
    if (d->mTextEditorCompleter->completer()->popup()->isVisible()) {
        switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
        default:
            break;
        }
    } else if (handleShortcut(e)) {
        return;
    }
    KPIMTextEdit::PlainTextEditor::keyPressEvent(e);
    if (e->key() == Qt::Key_F1 && !textCursor().hasSelection()) {
        const QString word = selectedWord();
        const KSieveUi::SieveEditorUtil::HelpVariableName type = KSieveUi::SieveEditorUtil::strToVariableName(word);
        if (type != KSieveUi::SieveEditorUtil::UnknownHelp) {
            const QUrl url = KSieveUi::SieveEditorUtil::helpUrl(type);
            if (!url.isEmpty()) {
                Q_EMIT openHelp(url);
            }
        }
        return;
    }
    d->mTextEditorCompleter->completeText();
}

void SieveTextEdit::setSieveCapabilities(const QStringList &capabilities)
{
    setCompleterList(completerList() + capabilities);
}

void SieveTextEdit::setShowHelpMenu(bool b)
{
    d->mShowHelpMenu = b;
}

void SieveTextEdit::addExtraMenuEntry(QMenu *menu, QPoint pos)
{
    if (!d->mShowHelpMenu) {
        return;
    }

    if (!textCursor().hasSelection()) {
        if (!isReadOnly()) {
            QAction *insertRules = new QAction(i18n("Insert Rule"), menu);
            //editRules->setIcon(QIcon::fromTheme(QStringLiteral("help-hint")));
            connect(insertRules, &QAction::triggered, this, &SieveTextEdit::insertRule);
            QAction *act = menu->addSeparator();
            menu->insertActions(menu->actions().at(0), {insertRules, act});
        }

        const QString word = selectedWord(pos);
        const KSieveUi::SieveEditorUtil::HelpVariableName type = KSieveUi::SieveEditorUtil::strToVariableName(word);
        if (type != KSieveUi::SieveEditorUtil::UnknownHelp) {
            auto *separator = new QAction(menu);
            separator->setSeparator(true);
            menu->insertAction(menu->actions().at(0), separator);

            QAction *searchAction = new QAction(i18n("Help about: \'%1\'", word), menu);
            searchAction->setShortcut(Qt::Key_F1);
            searchAction->setIcon(QIcon::fromTheme(QStringLiteral("help-hint")));
            searchAction->setData(word);
            connect(searchAction, &QAction::triggered, this, &SieveTextEdit::slotHelp);
            menu->insertAction(menu->actions().at(0), searchAction);
        }
    } else {
        if (!isReadOnly()) {
            QAction *editRules = new QAction(i18n("Edit Rule"), menu);
            //editRules->setIcon(QIcon::fromTheme(QStringLiteral("help-hint")));
            connect(editRules, &QAction::triggered, this, &SieveTextEdit::slotEditRule);
            QAction *act = menu->addSeparator();
            menu->insertActions(menu->actions().at(0), {editRules, act});
        }
    }
}

QString SieveTextEdit::selectedWord(const QPoint &pos) const
{
    QTextCursor wordSelectCursor(pos.isNull() ? textCursor() : cursorForPosition(pos));
    wordSelectCursor.clearSelection();
    wordSelectCursor.select(QTextCursor::WordUnderCursor);
    const QString word = wordSelectCursor.selectedText();
    return word;
}

void SieveTextEdit::slotEditRule()
{
    QTextCursor textcursor = textCursor();
    Q_EMIT editRule(textcursor.selection().toPlainText());
}

void SieveTextEdit::slotHelp()
{
    auto *act = qobject_cast<QAction *>(sender());
    if (act) {
        const QString word = act->data().toString();
        const KSieveUi::SieveEditorUtil::HelpVariableName type = KSieveUi::SieveEditorUtil::strToVariableName(word);
        const QUrl url = KSieveUi::SieveEditorUtil::helpUrl(type);
        if (!url.isEmpty()) {
            Q_EMIT openHelp(url);
        }
    }
}

void SieveTextEdit::comment()
{
    QTextCursor textcursor = textCursor();
    if (textcursor.hasSelection()) {
        //Move start block
        textcursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
        QString text = textcursor.selectedText();
        text = QLatin1Char('#') + text;
        text.replace(QChar::ParagraphSeparator, QStringLiteral("\n#"));
        textcursor.insertText(text);
        setTextCursor(textcursor);
    } else {
        textcursor.movePosition(QTextCursor::StartOfBlock);
        textcursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        const QString s = textcursor.selectedText();
        const QString str = QLatin1Char('#') + s;
        textcursor.insertText(str);
        setTextCursor(textcursor);
    }
}

void SieveTextEdit::upperCase()
{
    KPIMTextEdit::EditorUtil editorUtil;
    QTextCursor cursorText = textCursor();
    editorUtil.upperCase(cursorText);
}

void SieveTextEdit::lowerCase()
{
    KPIMTextEdit::EditorUtil editorUtil;
    QTextCursor cursorText = textCursor();
    editorUtil.lowerCase(cursorText);
}

void SieveTextEdit::sentenceCase()
{
    KPIMTextEdit::EditorUtil editorUtil;
    QTextCursor cursorText = textCursor();
    editorUtil.sentenceCase(cursorText);
}

void SieveTextEdit::reverseCase()
{
    KPIMTextEdit::EditorUtil editorUtil;
    QTextCursor cursorText = textCursor();
    editorUtil.reverseCase(cursorText);
}

void SieveTextEdit::uncomment()
{
    QTextCursor textcursor = textCursor();
    if (textcursor.hasSelection()) {
        textcursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
        QString text = textcursor.selectedText();
        if (text.startsWith(QLatin1Char('#'))) {
            text.remove(0, 1);
        }
        QString newText = text;
        for (int i = 0; i < newText.length();) {
            if (newText.at(i) == QChar::ParagraphSeparator || newText.at(i) == QChar::LineSeparator) {
                ++i;
                if (i < newText.length()) {
                    if (newText.at(i) == QLatin1Char('#')) {
                        newText.remove(i, 1);
                    } else {
                        ++i;
                    }
                }
            } else {
                ++i;
            }
        }

        textcursor.insertText(newText);
        setTextCursor(textcursor);
    } else {
        textcursor.movePosition(QTextCursor::StartOfBlock);
        textcursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        QString text = textcursor.selectedText();
        if (text.startsWith(QLatin1Char('#'))) {
            text.remove(0, 1);
        }
        textcursor.insertText(text);
        setTextCursor(textcursor);
    }
}

bool SieveTextEdit::isWordWrap() const
{
    return wordWrapMode() == QTextOption::WordWrap;
}

void SieveTextEdit::wordWrap(bool state)
{
    setWordWrapMode(state ? QTextOption::WordWrap : QTextOption::NoWrap);
}
