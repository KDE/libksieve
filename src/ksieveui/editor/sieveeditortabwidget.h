/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "ksieveui_export.h"
#include <QTabWidget>

namespace KSieveUi
{
class SieveEditorHelpHtmlWidget;
/**
 * @brief The SieveEditorTabWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveEditorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit SieveEditorTabWidget(QWidget *parent = nullptr);
    ~SieveEditorTabWidget() override;

    [[nodiscard]] QUrl currentHelpUrl() const;
    [[nodiscard]] QString currentHelpTitle() const;
    bool currentPageIsHtmlPage() const;

public Q_SLOTS:
    void slotAddHelpPage(const QUrl &url);

protected:
    void tabRemoved(int index) override;
    void tabInserted(int index) override;

Q_SIGNALS:
    void copyAvailable(bool);

private:
    KSIEVEUI_NO_EXPORT void slotTitleChanged(KSieveUi::SieveEditorHelpHtmlWidget *widget, const QString &title);
    KSIEVEUI_NO_EXPORT void slotTabCloseRequested(int index);
    KSIEVEUI_NO_EXPORT void slotProgressIndicatorPixmapChanged(KSieveUi::SieveEditorHelpHtmlWidget *widget, const QPixmap &pixmap);
    KSIEVEUI_NO_EXPORT void slotLoadFinished(KSieveUi::SieveEditorHelpHtmlWidget *widget, bool success);
    KSIEVEUI_NO_EXPORT void slotTabContextMenuRequest(const QPoint &pos);

    KSIEVEUI_NO_EXPORT void slotCloseAllTabExcept(int index);
    KSIEVEUI_NO_EXPORT void slotCloseAllTab();
    KSIEVEUI_NO_EXPORT void slotCloseRequest(int index);
    KSIEVEUI_NO_EXPORT void closeAllTabExcept(int index = -1);
};
}
