/* SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef WEBENGINEVIEWER_FINDBAR_WEBENGINEVIEW_H
#define WEBENGINEVIEWER_FINDBAR_WEBENGINEVIEW_H

#include "findbarbase.h"
#include "ksieveui_export.h"

class QWebEngineView;

namespace KSieveUi {
class FindBarWebEngineViewPrivate;
class KSIEVEUI_EXPORT FindBarWebEngineView : public FindBarBase
{
    Q_OBJECT
public:
    explicit FindBarWebEngineView(QWebEngineView *view, QWidget *parent = nullptr);
    ~FindBarWebEngineView() override;

private:
    void clearSelections() override;
    void searchText(bool backward, bool isAutoSearch) override;
    void updateSensitivity(bool sensitivity) override;

private:
    FindBarWebEngineViewPrivate *const d;
};
}

#endif
