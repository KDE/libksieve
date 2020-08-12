/* SPDX-FileCopyrightText: 2012-2020 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef KSIEVE_KSIEVEUI_SIEVELINENUMBERAREA_H
#define KSIEVE_KSIEVEUI_SIEVELINENUMBERAREA_H

#include <QWidget>

namespace KSieveUi {
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
    SieveTextEdit *m_sieveTextEdit = nullptr;
};
}

#endif
