/* SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "ksieveui_export.h"
#include <QDialog>
#include <memory>
namespace KSieveCore
{
class SieveImapAccountSettings;
}
namespace KSieveUi
{
class SieveEditorPrivate;
/**
 * @brief The SieveEditor class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveEditor : public QDialog
{
    Q_OBJECT
public:
    explicit SieveEditor(QWidget *parent = nullptr);
    ~SieveEditor() override;

    [[nodiscard]] QString script() const;
    [[nodiscard]] QString originalScript() const;
    void setScript(const QString &script, bool clearUndoRedo = false);
    void setDebugScript(const QString &debug);
    void addFailedMessage(const QString &err);
    void addOkMessage(const QString &msg);
    void addNormalMessage(const QString &msg);
    void setScriptName(const QString &name);

    void resultDone();

    void setSieveCapabilities(const QStringList &capabilities);
    void setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &account);

    void setListOfIncludeFile(const QStringList &listOfIncludeFile);
    void setModified(bool b);

Q_SIGNALS:
    void checkSyntax();
    void valueChanged(bool);
    void okClicked();
    void cancelClicked();

protected:
    bool event(QEvent *e) override;
    void closeEvent(QCloseEvent *e) override;

private:
    KSIEVEUI_NO_EXPORT void slotEnableButtonOk(bool b);
    KSIEVEUI_NO_EXPORT void slotAccepted();
    KSIEVEUI_NO_EXPORT void slotCanceled();
    KSIEVEUI_NO_EXPORT void writeConfig();
    KSIEVEUI_NO_EXPORT void readConfig();
    std::unique_ptr<SieveEditorPrivate> const d;
};
}
