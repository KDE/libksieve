/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QComboBox>
#include <QDialog>

#include <QListWidget>

class QLineEdit;
class QPushButton;
namespace KSieveUi
{
class SelectHeadersWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit SelectHeadersWidget(QWidget *parent = nullptr);
    ~SelectHeadersWidget() override;

    Q_REQUIRED_RESULT QString headers() const;
    void setListHeaders(const QMap<QString, QString> &lst, const QStringList &selectedHeaders);
    void addNewHeader(const QString &header);

private:
    enum HeaderEnum { HeaderId = Qt::UserRole + 1 };

    void init();
};

class SelectHeadersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SelectHeadersDialog(QWidget *parent = nullptr);
    ~SelectHeadersDialog() override;

    Q_REQUIRED_RESULT QString headers() const;
    void setListHeaders(const QMap<QString, QString> &lst, const QStringList &selectedHeaders);

private:
    void slotNewHeaderTextChanged(const QString &text);
    void slotAddNewHeader();
    void readConfig();
    void writeConfig();
    SelectHeadersWidget *const mListWidget;
    QLineEdit *const mNewHeader;
    QPushButton *const mAddNewHeader;
};

class SelectHeaderTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SelectHeaderTypeComboBox(bool onlyEnvelopType = false, QWidget *parent = nullptr);
    ~SelectHeaderTypeComboBox() override;

    Q_REQUIRED_RESULT QString code() const;
    void setCode(const QString &code);

Q_SIGNALS:
    void valueChanged();

private Q_SLOTS:
    void slotSelectItem(const QString &str);

private:
    Q_REQUIRED_RESULT const QString getSelectMultipleHeadersTranslated() const;
    void initialize(bool onlyEnvelopType);
    void changeReadOnlyStatus();
    void headerMap(bool onlyEnvelopType);
    QMap<QString, QString> mHeaderMap;
    QString mCode;
};
}

