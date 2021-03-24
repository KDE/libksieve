/* SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "ksieveui_export.h"
#include "sieveeditorabstractwidget.h"
class QSplitter;
namespace KPIMTextEdit
{
class SlideContainer;
class TextGoToLineWidget;
class PlainTextEditFindBar;
class PlainTextEditor;
class PlainTextEditorWidget;
class TextToSpeechWidget;
}

namespace KSieveUi
{
class SieveTextEdit;
class SieveEditorWarning;
class SieveEditorParsingMissingFeatureWarning;
class SieveEditorTabWidget;
class SieveTemplateWidget;
/**
 * @brief The SieveEditorTextModeWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveEditorTextModeWidget : public SieveEditorAbstractWidget
{
    Q_OBJECT
public:
    explicit SieveEditorTextModeWidget(QWidget *parent = nullptr);
    ~SieveEditorTextModeWidget() override;

    void setSieveCapabilities(const QStringList &capabilities);
    void setReadOnly(bool b);

    Q_REQUIRED_RESULT QString script() const;
    void setScript(const QString &script, bool clearUndoRedo = false);

    void setDebugScript(const QString &debug);

    Q_REQUIRED_RESULT QString currentscript() override;
    void setImportScript(const QString &script) override;

    void createRulesGraphically();
    void generateXml();

    void showEditorWarning();
    void hideEditorWarning();

    void showParsingEditorWarning();

    void setParsingEditorWarningError(const QString &script, const QString &error);
    void replace();
    void find();
    void undo();
    void redo();
    void paste();
    void cut();
    void copy();
    void checkSpelling();
    void comment();
    void uncomment();

    bool isUndoAvailable() const;
    bool isRedoAvailable() const;
    bool hasSelection() const;

    void selectAll();
    void upperCase();
    void lowerCase();
    void sentenceCase();
    void reverseCase();
    void zoomIn();
    void zoomOut();
    Q_REQUIRED_RESULT QString currentHelpTitle() const;
    Q_REQUIRED_RESULT QUrl currentHelpUrl() const;
    void openBookmarkUrl(const QUrl &url);
    void debugSieveScript();
    void zoomReset();
    void wordWrap(bool state);
    Q_REQUIRED_RESULT bool isWordWrap() const;
    void print();
    void printPreview();
    Q_REQUIRED_RESULT bool printSupportEnabled() const;
    Q_REQUIRED_RESULT bool isTextEditor() const;
    Q_REQUIRED_RESULT SieveEditorTabWidget *tabWidget() const;

    void setListOfIncludeFile(const QStringList &listOfIncludeFile);
    Q_REQUIRED_RESULT QStringList sieveCapabilities() const;

Q_SIGNALS:
    void enableButtonOk(bool);
    void switchToGraphicalMode();
    void valueChanged();
    void undoAvailable(bool);
    void redoAvailable(bool);
    void copyAvailable(bool);
    void sieveEditorTabCurrentChanged();

public Q_SLOTS:
    void slotShowGoToLine();

private:
    void slotTextChanged();
    void slotFind();
    void slotReplace();
    void slotGoToLine(int line);
    void readConfig();
    void writeConfig();
    void slotEditRule(const QString &selectedText);
    void slotInsertRule();
    void insertRequires(const QStringList &needToAddRequire);
    Q_REQUIRED_RESULT QStringList insertNecessaryRequires(const QStringList &requireModules);
    QString mOriginalScript;
    QStringList mSieveCapabilities;
    QStringList mListOfIncludeFile;
    SieveTextEdit *mTextEdit = nullptr;
    KPIMTextEdit::PlainTextEditorWidget *mDebugTextEdit = nullptr;
    KPIMTextEdit::PlainTextEditFindBar *mFindBar = nullptr;
    KPIMTextEdit::TextToSpeechWidget *mTextToSpeechWidget = nullptr;
    QSplitter *mMainSplitter = nullptr;
    QSplitter *mTemplateSplitter = nullptr;
    SieveEditorWarning *mSieveEditorWarning = nullptr;
    SieveEditorParsingMissingFeatureWarning *mSieveParsingWarning = nullptr;
    SieveEditorTabWidget *mTabWidget = nullptr;
    KPIMTextEdit::TextGoToLineWidget *mGoToLine = nullptr;
    KPIMTextEdit::SlideContainer *mGotoLineSliderContainer = nullptr;
    KPIMTextEdit::SlideContainer *mSliderContainer = nullptr;
    SieveTemplateWidget *mSieveTemplateWidget = nullptr;
    QWidget *mEditorWidget = nullptr;
};
}

