/* SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "findbarbase.h"
#include "ksieveui_export.h"
#include <memory>

class QWebEngineView;

namespace KSieveUi
{
class FindBarWebEngineViewPrivate;
class KSIEVEUI_EXPORT FindBarWebEngineView : public FindBarBase
{
    Q_OBJECT
public:
    explicit FindBarWebEngineView(QWebEngineView *view, QWidget *parent = nullptr);
    ~FindBarWebEngineView() override;

private:
    KSIEVEUI_NO_EXPORT void clearSelections() override;
    KSIEVEUI_NO_EXPORT void searchText(bool backward, bool isAutoSearch) override;
    KSIEVEUI_NO_EXPORT void updateSensitivity(bool sensitivity) override;

private:
    std::unique_ptr<FindBarWebEngineViewPrivate> const d;
};
}
