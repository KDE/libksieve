/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

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
/*!
 * \brief The SieveScriptDebuggerWidget class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveScriptDebuggerWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     */
    explicit SieveScriptDebuggerWidget(QWidget *parent = nullptr);
    /*!
     */
    ~SieveScriptDebuggerWidget() override;

    /*!
     */
    void setScript(const QString &script);
    /*!
     */
    [[nodiscard]] QString script() const;

    /*!
     */
    [[nodiscard]] bool canAccept() const;

    /*!
     */
    [[nodiscard]] QList<int> splitterSizes() const;
    /*!
     */
    void setSplitterSizes(const QList<int> &sizes);

    /*!
     */
    [[nodiscard]] bool haveDebugApps() const;

Q_SIGNALS:
    /*!
     */
    void scriptTextChanged();
    /*!
     */
    void debugScriptButtonClicked();
    /*!
     */
    void debugButtonEnabled(bool state);
    /*!
     */
    void sieveTestNotFound();

private:
    KSIEVEUI_NO_EXPORT void checkSieveTestApplication();
    QStackedWidget *mStackedWidget = nullptr;
    SieveScriptDebuggerFrontEndWidget *mSieveScriptFrontEnd = nullptr;
    QLabel *mSieveNoExistingFrontEnd = nullptr;
    bool mHaveDebugApps = false;
};
}
