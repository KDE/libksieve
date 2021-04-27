/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include <QGroupBox>
#include <QListWidgetItem>

class QListWidget;
class QPushButton;
class QXmlStreamReader;

namespace KSieveUi
{
class SieveEditorGraphicalModeWidget;
class SieveScriptPage;
class SieveScriptListItem : public QListWidgetItem
{
public:
    SieveScriptListItem(const QString &text, QListWidget *parent);
    ~SieveScriptListItem() override;

    void setDescription(const QString &desc);
    Q_REQUIRED_RESULT QString description() const;

    Q_REQUIRED_RESULT SieveScriptPage *scriptPage() const;
    void setScriptPage(SieveScriptPage *page);

    Q_REQUIRED_RESULT QString generatedScript(QStringList &required) const;

private:
    QString mDescription;
    SieveScriptPage *mScriptPage = nullptr;
};

class KSIEVEUI_TESTS_EXPORT SieveScriptListBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit SieveScriptListBox(const QString &title, QWidget *parent = nullptr);
    ~SieveScriptListBox() override;

    void setSieveEditorGraphicalModeWidget(SieveEditorGraphicalModeWidget *graphicalModeWidget);
    Q_REQUIRED_RESULT QString generatedScript(QStringList &required) const;
    void loadScript(const QString &doc, QString &error);

Q_SIGNALS:
    void addNewPage(KSieveUi::SieveScriptPage *);
    void removePage(QWidget *);
    void activatePage(QWidget *);
    void enableButtonOk(bool);
    void valueChanged();

private:
    void slotCustomMenuRequested(const QPoint &pos);
    void slotNew();
    void slotDelete();
    void slotRename();
    void updateButtons();
    void slotEditDescription();
    void slotItemActived(QListWidgetItem *);
    void slotTop();
    void slotBottom();
    void slotDown();
    void slotUp();

    enum ParseSieveScriptTypeBlock {
        TypeUnknown = 0,
        TypeBlockIf,
        TypeBlockElsif,
        TypeBlockElse,
        TypeBlockInclude,
        TypeBlockGlobal,
        TypeBlockAction,
        TypeBlockForeachBlock
    };

    void loadBlock(QXmlStreamReader &n, SieveScriptPage *currentPage, ParseSieveScriptTypeBlock typeBlock, QString &error);
    void clear();
    Q_REQUIRED_RESULT SieveScriptPage *createNewScript(const QString &newName, const QString &description = QString());
    Q_REQUIRED_RESULT QString createUniqName();
    SieveEditorGraphicalModeWidget *mSieveGraphicalModeWidget = nullptr;
    QListWidget *const mSieveListScript;
    QPushButton *mBtnNew = nullptr;
    QPushButton *mBtnDelete = nullptr;
    QPushButton *mBtnRename = nullptr;
    QPushButton *mBtnDescription = nullptr;
    QPushButton *mBtnTop = nullptr;
    QPushButton *mBtnUp = nullptr;
    QPushButton *mBtnDown = nullptr;
    QPushButton *mBtnBottom = nullptr;
    int mScriptNumber = 0;
};
}

