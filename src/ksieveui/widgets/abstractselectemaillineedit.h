/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ABSTRACTSELECTEMAILLINEEDIT_H
#define ABSTRACTSELECTEMAILLINEEDIT_H

#include <QWidget>
#include "ksieveui_export.h"
namespace KSieveUi {
/**
 * @brief The AbstractSelectEmailLineEdit class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT AbstractSelectEmailLineEdit : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractSelectEmailLineEdit(QWidget *parent = nullptr);
    ~AbstractSelectEmailLineEdit();
    virtual void setText(const QString &str) = 0;
    virtual QString text() const = 0;

    Q_REQUIRED_RESULT bool multiSelection() const;
    void setMultiSelection(bool multiSelection);
    virtual Q_REQUIRED_RESULT bool isValid() const = 0;

Q_SIGNALS:
    void valueChanged();
private:
    bool mMultiSelection = false;
};
}

#endif // ABSTRACTSELECTEMAILLINEEDIT_H
