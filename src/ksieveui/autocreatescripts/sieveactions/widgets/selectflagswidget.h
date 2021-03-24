/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QDialog>
#include <QListWidget>
class QLineEdit;

namespace KSieveUi
{
class SelectFlagsListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit SelectFlagsListWidget(QWidget *parent = nullptr);
    ~SelectFlagsListWidget() override;

    void setFlags(const QStringList &list);
    Q_REQUIRED_RESULT QStringList flags() const;

private:
    enum Type { FlagsRealName = Qt::UserRole + 1 };
    void init();
};

class SelectFlagsListDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SelectFlagsListDialog(QWidget *parent = nullptr);
    ~SelectFlagsListDialog();

    void setFlags(const QStringList &list);
    Q_REQUIRED_RESULT QStringList flags() const;

private:
    void readConfig();
    void writeConfig();
    SelectFlagsListWidget *const mListWidget;
};

class SelectFlagsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectFlagsWidget(QWidget *parent = nullptr);
    ~SelectFlagsWidget();

    Q_REQUIRED_RESULT QString code() const;
    void setFlags(const QStringList &flags);

Q_SIGNALS:
    void valueChanged();

private:
    void slotSelectFlags();
    QLineEdit *mEdit = nullptr;
};
}

