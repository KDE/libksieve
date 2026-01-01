/* SPDX-FileCopyrightText: 2011-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "sievetextedit.h"
using namespace Qt::Literals::StringLiterals;

#include "editor/sieveeditorutil.h"
#include "editor/sievelinenumberarea.h"
#include "editor/sievetexteditorspellcheckdecorator.h"

#include "config-libksieve.h"
#if HAVE_TEXT_AUTOGENERATE_TEXT
#include <QPointer>
#include <TextAutoGenerateText/TextAutoGenerateManager>
#include <TextAutoGenerateText/TextAutoGenerateMenuWidget>
#include <TextAutoGenerateText/TextAutoGenerateQuickAskDialog>
#endif

#include <TextCustomEditor/PlainTextSyntaxSpellCheckingHighlighter>
#include <TextCustomEditor/TextEditorCompleter>
#include <TextUtils/ConvertText>

#include <KLocalizedString>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Theme>

#include <QAbstractItemView>
#include <QAction>
#include <QCompleter>
#include <QFontDatabase>
#include <QIcon>
#include <QKeyEvent>
#include <QMenu>
#include <QPainter>
#include <QTextDocumentFragment>
using namespace KSieveUi;

class KSieveUi::SieveTextEditPrivate
{
public:
    SieveTextEditPrivate() = default;

    SieveLineNumberArea *m_sieveLineNumberArea = nullptr;
    TextCustomEditor::TextEditorCompleter *mTextEditorCompleter = nullptr;
#if HAVE_TEXT_AUTOGENERATE_TEXT
    TextAutoGenerateText::TextAutoGenerateMenuWidget *mTextAutoGenerateMenuWidget = nullptr;
    TextAutoGenerateText::TextAutoGenerateManager *mTextAutoGenerateManager = nullptr;
    QPointer<TextAutoGenerateText::TextAutoGenerateQuickAskDialog> mQuickDialog;
#endif
    KSyntaxHighlighting::Repository mSyntaxRepo;
    bool mShowHelpMenu = true;
};

SieveTextEdit::SieveTextEdit(QWidget *parent)
    : TextCustomEditor::PlainTextEditor(parent)
    , d(new KSieveUi::SieveTextEditPrivate)
{
    setSpellCheckingConfigFileName(u"sieveeditorrc"_s);
    setWordWrapMode(QTextOption::NoWrap);
    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    d->m_sieveLineNumberArea = new SieveLineNumberArea(this);
#if HAVE_TEXT_AUTOGENERATE_TEXT
    d->mTextAutoGenerateMenuWidget = new TextAutoGenerateText::TextAutoGenerateMenuWidget(this);
    d->mTextAutoGenerateManager = new TextAutoGenerateText::TextAutoGenerateManager(this);
#endif

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
}

void SieveTextEdit::updateHighLighter()
{
    auto hlighter = dynamic_cast<TextCustomEditor::PlainTextSyntaxSpellCheckingHighlighter *>(highlighter());
    if (hlighter) {
        hlighter->toggleSpellHighlighting(checkSpellingEnabled());
    }
}

void SieveTextEdit::clearDecorator()
{
    // Nothing
}

void SieveTextEdit::createHighlighter()
{
    auto highlighter = new TextCustomEditor::PlainTextSyntaxSpellCheckingHighlighter(this);
    highlighter->toggleSpellHighlighting(checkSpellingEnabled());
    highlighter->setCurrentLanguage(spellCheckingLanguage());
    highlighter->setDefinition(d->mSyntaxRepo.definitionForName(u"Sieve"_s));
    highlighter->setTheme((palette().color(QPalette::Base).lightness() < 128) ? d->mSyntaxRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
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

    const int space = 2 + fontMetrics().boundingRect(u'X').width() * digits;
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
            painter.drawText(0, top, d->m_sieveLineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
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

    listWord << u"require"_s << u"stop"_s;
    listWord << u":contains"_s << u":matches"_s << QStringLiteral(":is") << QStringLiteral(":over") << QStringLiteral(":under") << u":all"_s << u":domain"_s
             << QStringLiteral(":localpart");
    listWord << u"if"_s << u"elsif"_s << QStringLiteral("else");
    listWord << u"keep"_s << u"reject"_s << QStringLiteral("discard") << QStringLiteral("redirect") << QStringLiteral("addflag") << u"setflag"_s;
    listWord << u"address"_s << u"allof"_s << QStringLiteral("anyof") << QStringLiteral("exists") << QStringLiteral("false") << u"header"_s << u"not"_s
             << QStringLiteral("size") << QStringLiteral("true");
    listWord << u":days"_s << u":seconds"_s << QStringLiteral(":subject") << QStringLiteral(":addresses") << QStringLiteral(":text");
    listWord << u":name"_s << u":headers"_s << QStringLiteral(":first") << QStringLiteral(":importance");
    listWord << u":message"_s << u":from"_s;

    return listWord;
}

void SieveTextEdit::setCompleterList(const QStringList &list)
{
    d->mTextEditorCompleter->setCompleterStringList(list);
}

void SieveTextEdit::initCompleter()
{
    const QStringList listWord = completerList();

    d->mTextEditorCompleter = new TextCustomEditor::TextEditorCompleter(this, this);
    d->mTextEditorCompleter->setCompleterStringList(listWord);
}

bool SieveTextEdit::event(QEvent *ev)
{
    if (ev->type() == QEvent::ShortcutOverride) {
        auto e = static_cast<QKeyEvent *>(ev);
        if (overrideShortcut(e)) {
            e->accept();
            return true;
        }
    }
    return TextCustomEditor::PlainTextEditor::event(ev);
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
    TextCustomEditor::PlainTextEditor::keyPressEvent(e);
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

void SieveTextEdit::slotQuickAsk()
{
#if HAVE_TEXT_AUTOGENERATE_TEXT
    if (!d->mQuickDialog) {
        d->mQuickDialog = new TextAutoGenerateText::TextAutoGenerateQuickAskDialog(d->mTextAutoGenerateManager, this);
        d->mQuickDialog->setAttribute(Qt::WA_DeleteOnClose);
    }
    d->mQuickDialog->show();
#endif
}

void SieveTextEdit::addExtraMenuEntry(QMenu *menu, QPoint pos)
{
    const bool hasSelection = textCursor().hasSelection();
#if HAVE_TEXT_AUTOGENERATE_TEXT
    if (hasSelection) {
        QTextCursor textcursor = textCursor();
        d->mTextAutoGenerateMenuWidget->setSelectedText(textcursor.selection().toPlainText());
        menu->addSeparator();
        menu->addMenu(d->mTextAutoGenerateMenuWidget->menu());
    }
    menu->addSeparator();
    auto quickAskAction = new QAction(i18nc("@action", "Quick Ask"), menu);
    connect(quickAskAction, &QAction::triggered, this, &SieveTextEdit::slotQuickAsk);
    menu->addAction(quickAskAction);
#endif
    if (!d->mShowHelpMenu) {
        return;
    }

    if (!hasSelection) {
        if (!isReadOnly()) {
            auto insertRules = new QAction(i18nc("@action", "Insert Rule"), menu);
            // editRules->setIcon(QIcon::fromTheme(u"help-hint"_s));
            connect(insertRules, &QAction::triggered, this, &SieveTextEdit::insertRule);
            QAction *act = menu->addSeparator();
            menu->insertActions(menu->actions().at(0), {insertRules, act});
        }

        const QString word = selectedWord(pos);
        const KSieveUi::SieveEditorUtil::HelpVariableName type = KSieveUi::SieveEditorUtil::strToVariableName(word);
        if (type != KSieveUi::SieveEditorUtil::UnknownHelp) {
            auto separator = new QAction(menu);
            separator->setSeparator(true);
            menu->insertAction(menu->actions().at(0), separator);

            auto searchAction = new QAction(i18nc("@action", "Help about: \'%1\'", word), menu);
            searchAction->setShortcut(Qt::Key_F1);
            searchAction->setIcon(QIcon::fromTheme(u"help-hint"_s));
            searchAction->setData(word);
            connect(searchAction, &QAction::triggered, this, &SieveTextEdit::slotHelp);
            menu->insertAction(menu->actions().at(0), searchAction);
        }
    } else {
        if (!isReadOnly()) {
            auto editRules = new QAction(i18nc("@action", "Edit Rule"), menu);
            // editRules->setIcon(QIcon::fromTheme(u"help-hint"_s));
            connect(editRules, &QAction::triggered, this, &SieveTextEdit::slotEditRule);
            QAction *act = menu->addSeparator();
            menu->insertActions(menu->actions().at(0), {editRules, act});
        }
    }
}

QString SieveTextEdit::selectedWord(QPoint pos) const
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
    auto act = qobject_cast<QAction *>(sender());
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
        // Move start block
        textcursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
        QString text = textcursor.selectedText();
        text = u'#' + text;
        text.replace(QChar::ParagraphSeparator, u"\n#"_s);
        textcursor.insertText(text);
        setTextCursor(textcursor);
    } else {
        textcursor.movePosition(QTextCursor::StartOfBlock);
        textcursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        const QString s = textcursor.selectedText();
        const QString str = u'#' + s;
        textcursor.insertText(str);
        setTextCursor(textcursor);
    }
}

void SieveTextEdit::upperCase()
{
    QTextCursor cursorText = textCursor();
    TextUtils::ConvertText::upperCase(cursorText);
}

void SieveTextEdit::lowerCase()
{
    QTextCursor cursorText = textCursor();
    TextUtils::ConvertText::lowerCase(cursorText);
}

void SieveTextEdit::sentenceCase()
{
    QTextCursor cursorText = textCursor();
    TextUtils::ConvertText::sentenceCase(cursorText);
}

void SieveTextEdit::reverseCase()
{
    QTextCursor cursorText = textCursor();
    TextUtils::ConvertText::reverseCase(cursorText);
}

void SieveTextEdit::uncomment()
{
    QTextCursor textcursor = textCursor();
    if (textcursor.hasSelection()) {
        textcursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
        QString text = textcursor.selectedText();
        if (text.startsWith(u'#')) {
            text.remove(0, 1);
        }
        QString newText = text;
        for (int i = 0; i < newText.length();) {
            if (newText.at(i) == QChar::ParagraphSeparator || newText.at(i) == QChar::LineSeparator) {
                ++i;
                if (i < newText.length()) {
                    if (newText.at(i) == u'#') {
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
        if (text.startsWith(u'#')) {
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

void SieveTextEdit::setWordWrap(bool state)
{
    setWordWrapMode(state ? QTextOption::WordWrap : QTextOption::NoWrap);
}

#include "moc_sievetextedit.cpp"
