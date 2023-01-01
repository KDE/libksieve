/*
   SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"
#include <QWidget>
class QStackedWidget;
class QLabel;
namespace KSieveUi
{
class SieveScriptDebuggerFrontEndWidget;
/**
 * @brief The SieveScriptDebuggerWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveScriptDebuggerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveScriptDebuggerWidget(QWidget *parent = nullptr);
    ~SieveScriptDebuggerWidget() override;

    void setScript(const QString &script);
    Q_REQUIRED_RESULT QString script() const;

    Q_REQUIRED_RESULT bool canAccept() const;

    Q_REQUIRED_RESULT QList<int> splitterSizes() const;
    void setSplitterSizes(const QList<int> &sizes);

    Q_REQUIRED_RESULT bool haveDebugApps() const;

Q_SIGNALS:
    void scriptTextChanged();
    void debugScriptButtonClicked();
    void debugButtonEnabled(bool state);
    void sieveTestNotFound();

private:
    void checkSieveTestApplication();
    QStackedWidget *mStackedWidget = nullptr;
    SieveScriptDebuggerFrontEndWidget *mSieveScriptFrontEnd = nullptr;
    QLabel *mSieveNoExistingFrontEnd = nullptr;
    bool mHaveDebugApps = false;
};
}
