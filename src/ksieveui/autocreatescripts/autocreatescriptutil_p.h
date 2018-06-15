/*
   Copyright (C) 2013-2018 Laurent Montel <montel@kde.org>

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

#ifndef AUTOCREATESCRIPTUTIL_H
#define AUTOCREATESCRIPTUTIL_H

#include <QString>
#include <QStringList>
#include <QXmlStreamReader>
#include <KSieveUi/AbstractRegexpEditorLineEdit>
#include "ksieveui_private_export.h"
namespace KSieveUi {
class AbstractMoveImapFolderWidget;
class AbstractSelectEmailLineEdit;
namespace AutoCreateScriptUtil {
Q_REQUIRED_RESULT QString createMultiLine(const QString &str);
Q_REQUIRED_RESULT QString createList(const QString &str, QChar separator, bool addEndSemiColon = true);
Q_REQUIRED_RESULT QString createList(const QStringList &lst, bool addSemiColon = true, bool protectSlash = false);
Q_REQUIRED_RESULT QStringList createListFromString(QString str);
Q_REQUIRED_RESULT QString createAddressList(const QString &str, bool addSemiColon = true);
Q_REQUIRED_RESULT QString negativeString(bool isNegative);
Q_REQUIRED_RESULT QString tagValueWithCondition(const QString &tag, bool notCondition);
Q_REQUIRED_RESULT QString tagValue(const QString &tag);
Q_REQUIRED_RESULT QString strValue(QXmlStreamReader &element);
Q_REQUIRED_RESULT QStringList listValue(QXmlStreamReader &element);
Q_REQUIRED_RESULT QString listValueToStr(QXmlStreamReader &element);
Q_REQUIRED_RESULT QString fixListValue(QString valueStr);
Q_REQUIRED_RESULT QString quoteStr(const QString &str, bool protectSlash = true);
void comboboxItemNotFound(const QString &searchItem, const QString &name, QString &error);
Q_REQUIRED_RESULT QString createFullWhatsThis(const QString &help, const QString &href);
Q_REQUIRED_RESULT QString protectSlash(QString str);
Q_REQUIRED_RESULT QString indentation();
Q_REQUIRED_RESULT AbstractRegexpEditorLineEdit *createRegexpEditorLineEdit(QWidget *parent = nullptr);
Q_REQUIRED_RESULT QString generateConditionComment(const QString &comment);
Q_REQUIRED_RESULT QString loadConditionComment(QString originalComment, const QString &comment);
KSIEVEUI_TESTS_EXPORT KSieveUi::AbstractMoveImapFolderWidget *createImapFolderWidget();
KSIEVEUI_TESTS_EXPORT KSieveUi::AbstractSelectEmailLineEdit *createSelectEmailsWidget();
}
}
#endif // AUTOCREATESCRIPTUTIL_H
