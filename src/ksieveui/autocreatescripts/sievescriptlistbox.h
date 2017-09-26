/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SIEVESCRIPTLISTBOX_H
#define SIEVESCRIPTLISTBOX_H

#include <QGroupBox>
#include <QListWidgetItem>
#include "ksieveui_private_export.h"

class QListWidget;
class QPushButton;
class QXmlStreamReader;

namespace KSieveUi {
class SieveEditorGraphicalModeWidget;
class SieveScriptPage;
class SieveScriptListItem : public QListWidgetItem
{
public:
    SieveScriptListItem(const QString &text, QListWidget *parent);
    ~SieveScriptListItem();

    void setDescription(const QString &desc);
    QString description() const;

    SieveScriptPage *scriptPage() const;
    void setScriptPage(SieveScriptPage *page);

    QString generatedScript(QStringList &requires) const;

private:
    QString mDescription;
    SieveScriptPage *mScriptPage = nullptr;
};

class KSIEVEUI_TESTS_EXPORT SieveScriptListBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit SieveScriptListBox(const QString &title, QWidget *parent = nullptr);
    ~SieveScriptListBox();

    void setSieveEditorGraphicalModeWidget(SieveEditorGraphicalModeWidget *graphicalModeWidget);
    QString generatedScript(QString &requires) const;
    void loadScript(const QString &doc, QString &error);

Q_SIGNALS:
    void addNewPage(KSieveUi::SieveScriptPage *);
    void removePage(QWidget *);
    void activatePage(QWidget *);
    void enableButtonOk(bool);
    void valueChanged();

private:
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
    SieveScriptPage *createNewScript(const QString &newName, const QString &description = QString());
    QString createUniqName();
    SieveEditorGraphicalModeWidget *mSieveGraphicalModeWidget = nullptr;
    QListWidget *mSieveListScript = nullptr;
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

#endif // SIEVESCRIPTLISTBOX_H
