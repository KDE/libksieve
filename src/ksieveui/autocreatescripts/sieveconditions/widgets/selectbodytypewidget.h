/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SELECTBODYTYPEWIDGET_H
#define SELECTBODYTYPEWIDGET_H

#include <QWidget>

class QComboBox;
class QLineEdit;

namespace KSieveUi {
class SelectBodyTypeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectBodyTypeWidget(QWidget *parent = nullptr);
    ~SelectBodyTypeWidget();

    Q_REQUIRED_RESULT QString code() const;
    void setCode(const QString &type, const QString &content, const QString &name, QString &error);

Q_SIGNALS:
    void valueChanged();
private:
    void slotBodyTypeChanged(int);
    void initialize();
    QComboBox *mBodyCombobox = nullptr;
    QLineEdit *mBodyLineEdit = nullptr;
};
}

#endif // SELECTBODYTYPEWIDGET_H
