/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
