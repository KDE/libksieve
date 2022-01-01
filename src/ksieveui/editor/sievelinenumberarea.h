/* SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <QWidget>

namespace KSieveUi
{
class SieveTextEdit;

class SieveLineNumberArea : public QWidget
{
    Q_OBJECT
public:
    explicit SieveLineNumberArea(SieveTextEdit *editor);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    SieveTextEdit *const m_sieveTextEdit;
};
}

