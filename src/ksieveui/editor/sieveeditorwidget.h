/*
  SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#ifndef SIEVEEDITORWIDGET_H
#define SIEVEEDITORWIDGET_H
#include "ksieveui_export.h"

#include <QWidget>
class QStackedWidget;
class QLineEdit;
class QAction;
namespace KSieveUi {
class SieveImapAccountSettings;
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
    ~SieveEditorWidget();
    enum EditorMode {
        Unknown = -1,
        TextMode = 0,
        GraphicMode = 1
    };

    void setReadOnly(bool b);
    Q_REQUIRED_RESULT QString script() const;
    Q_REQUIRED_RESULT QString originalScript() const;
    void setScript(const QString &script, bool clearUndoRedo = false);
    void setDebugScript(const QString &debug);
    void setScriptName(const QString &name);

    void resultDone();

    void setSieveCapabilities(const QStringList &capabilities);
    void setSieveImapAccountSettings(const SieveImapAccountSettings &sieveImapAccountSettings);
    void setListOfIncludeFile(const QStringList &listOfIncludeFile);

    void addFailedMessage(const QString &err);
    void addOkMessage(const QString &msg);
    void addNormalMessage(const QString &msg);

    void setModified(bool b);
    Q_REQUIRED_RESULT bool isModified() const;

    Q_REQUIRED_RESULT EditorMode mode() const;

    Q_REQUIRED_RESULT bool isRedoAvailable() const;
    Q_REQUIRED_RESULT bool isUndoAvailable() const;
    Q_REQUIRED_RESULT bool hasSelection() const;

    void checkSpelling();
    void import();
    void lowerCase();
    void upperCase();
    void sentenceCase();
    void reverseCase();
    void zoomOut();
    void zoomIn();
    Q_REQUIRED_RESULT QString currentHelpTitle() const;
    Q_REQUIRED_RESULT QUrl currentHelpUrl() const;
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
    void slotShareScript();
    void slotCreateRulesGraphically();
    void slotCheckSyntax();
    void comment();
    void uncomment();
    void wordWrap(bool state);

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
    void slotEnableButtonOk(bool b);
    void slotGenerateXml();
    void slotSwitchMode();
    void slotSwitchTextMode(const QString &script);
    void slotSwitchToGraphicalMode();
    void slotModified();
    void changeSwitchButtonText();
    void changeMode(EditorMode mode);
    void addMessageEntry(const QString &errorMsg, const QColor &color);
    void slotServerInfo();
    QString mOriginalScript;
    SieveEditorTextModeWidget *mTextModeWidget = nullptr;
    SieveEditorGraphicalModeWidget *mGraphicalModeWidget = nullptr;
    QStackedWidget *mStackedWidget = nullptr;
    QLineEdit *mScriptName = nullptr;
    QAction *mCheckSyntax = nullptr;
    QAction *mSwitchMode = nullptr;
    QAction *mCreateRulesGraphically = nullptr;
    QAction *mSaveAs = nullptr;
    QAction *mShareScript = nullptr;
    QAction *mGenerateXml = nullptr;
    QAction *mServerInfo = nullptr;
    EditorMode mMode = EditorMode::TextMode;
    bool mModified = false;
    bool mDebug = false;
};
}

#endif // SIEVEEDITORWIDGET_H
