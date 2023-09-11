/* SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

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
#ifdef HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
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
    void setWordWrap(bool state);
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
    KSIEVEUI_NO_EXPORT void slotTextChanged();
    KSIEVEUI_NO_EXPORT void slotFind();
    KSIEVEUI_NO_EXPORT void slotReplace();
    KSIEVEUI_NO_EXPORT void slotGoToLine(int line);
    KSIEVEUI_NO_EXPORT void readConfig();
    KSIEVEUI_NO_EXPORT void writeConfig();
    KSIEVEUI_NO_EXPORT void slotEditRule(const QString &selectedText);
    KSIEVEUI_NO_EXPORT void slotInsertRule();
    KSIEVEUI_NO_EXPORT void insertRequires(const QStringList &needToAddRequire);
    Q_REQUIRED_RESULT KSIEVEUI_NO_EXPORT QStringList insertNecessaryRequires(const QStringList &requireModules);
    QString mOriginalScript;
    QStringList mSieveCapabilities;
    QStringList mListOfIncludeFile;
    SieveTextEdit *mTextEdit = nullptr;
    TextCustomEditor::PlainTextEditorWidget *mDebugTextEdit = nullptr;
    TextCustomEditor::PlainTextEditFindBar *mFindBar = nullptr;
#ifdef HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
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
