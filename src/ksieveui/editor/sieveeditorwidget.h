/*
  SPDX-FileCopyrightText: 2014-2024 Laurent Montel <montel@kde.org>

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
/**
 * @brief The SieveEditorWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveEditorWidget(bool useMenuBar, QWidget *parent = nullptr);
    ~SieveEditorWidget() override;
    enum EditorMode {
        Unknown = -1,
        TextMode = 0,
        GraphicMode = 1
    };

    void setReadOnly(bool b);
    [[nodiscard]] QString script() const;
    [[nodiscard]] QString originalScript() const;
    void setScript(const QString &script, bool clearUndoRedo = false);
    void setDebugScript(const QString &debug);
    void setScriptName(const QString &name);

    void resultDone();

    void setSieveCapabilities(const QStringList &capabilities);
    void setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &sieveImapAccountSettings);
    void setListOfIncludeFile(const QStringList &listOfIncludeFile);

    void addFailedMessage(const QString &err);
    void addOkMessage(const QString &msg);
    void addNormalMessage(const QString &msg);

    void setModified(bool b);
    [[nodiscard]] bool isModified() const;

    [[nodiscard]] EditorMode mode() const;

    [[nodiscard]] bool isRedoAvailable() const;
    [[nodiscard]] bool isUndoAvailable() const;
    [[nodiscard]] bool hasSelection() const;

    void checkSpelling();
    void import();
    void lowerCase();
    void upperCase();
    void sentenceCase();
    void reverseCase();
    void zoomOut();
    void zoomIn();
    [[nodiscard]] QString currentHelpTitle() const;
    [[nodiscard]] QUrl currentHelpUrl() const;
    void openBookmarkUrl(const QUrl &url);
    void debugSieveScript();
    void zoomReset();
    bool isWordWrap() const;

    void updateOriginalScript();
    void print();
    void printPreview();

    bool printSupportEnabled() const;
    bool isTextEditor() const;
public Q_SLOTS:
    void find();
    void replace();
    void undo();
    void redo();
    void paste();
    void copy();
    void cut();
    void selectAll();
    void goToLine();
    void slotSaveAs();
    void slotImport();
    void slotCreateRulesGraphically();
    void slotCheckSyntax();
    void comment();
    void uncomment();
    void setWordWrap(bool state);

Q_SIGNALS:
    void checkSyntax();
    void enableButtonOk(bool b);
    void valueChanged(bool b);
    void modeEditorChanged(KSieveUi::SieveEditorWidget::EditorMode);
    void undoAvailable(bool);
    void redoAvailable(bool);
    void copyAvailable(bool);
    void changeModeEditor(bool);
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
