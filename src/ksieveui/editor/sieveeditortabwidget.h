/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVEEDITORTABWIDGET_H
#define SIEVEEDITORTABWIDGET_H

#include <QTabWidget>
#include "ksieveui_export.h"

namespace KSieveUi {
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

    Q_REQUIRED_RESULT QUrl currentHelpUrl() const;
    Q_REQUIRED_RESULT QString currentHelpTitle() const;
    bool currentPageIsHtmlPage() const;

public Q_SLOTS:
    void slotAddHelpPage(const QUrl &url);

protected:
    void tabRemoved(int index) override;
    void tabInserted(int index) override;

Q_SIGNALS:
    void copyAvailable(bool);

private:
    void slotTitleChanged(KSieveUi::SieveEditorHelpHtmlWidget *widget, const QString &title);
    void slotTabCloseRequested(int index);
    void slotProgressIndicatorPixmapChanged(KSieveUi::SieveEditorHelpHtmlWidget *widget, const QPixmap &pixmap);
    void slotLoadFinished(KSieveUi::SieveEditorHelpHtmlWidget *widget, bool success);
    void slotTabContextMenuRequest(const QPoint &pos);

    void slotCloseAllTabExcept(int index);
    void slotCloseAllTab();
    void slotCloseRequest(int index);
    void closeAllTabExcept(int index = -1);
};
}

#endif // SIEVEEDITORTABWIDGET_H
