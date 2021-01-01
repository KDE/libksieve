/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SELECTCONVERTPARAMETERWIDGET_H
#define SELECTCONVERTPARAMETERWIDGET_H

#include <QWidget>
class QSpinBox;
namespace KSieveUi {
class SelectConvertParameterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectConvertParameterWidget(QWidget *parent = nullptr);
    ~SelectConvertParameterWidget();

    Q_REQUIRED_RESULT QString code() const;
    void setCode(const QStringList &code, QString &error);

Q_SIGNALS:
    void valueChanged();

private:
    void initialize();
    QSpinBox *mWidth = nullptr;
    QSpinBox *mHeight = nullptr;
};
}

#endif // SELECTCONVERTPARAMETERWIDGET_H
