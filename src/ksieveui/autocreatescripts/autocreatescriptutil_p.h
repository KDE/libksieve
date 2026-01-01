/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include <KSieveUi/AbstractRegexpEditorLineEdit>
#include <QString>
#include <QStringList>
#include <QXmlStreamReader>
namespace KSieveUi
{
class AbstractMoveImapFolderWidget;
class AbstractSelectEmailLineEdit;
namespace AutoCreateScriptUtil
{
[[nodiscard]] QString createMultiLine(const QString &str);
[[nodiscard]] QString createList(const QString &str, QChar separator, bool addEndSemiColon = true);
[[nodiscard]] QString createList(const QStringList &lst, bool addSemiColon = true, bool protectSlash = false);
[[nodiscard]] QStringList createListFromString(QString str);
[[nodiscard]] QString createAddressList(const QString &str, bool addSemiColon = true);
[[nodiscard]] QString negativeString(bool isNegative);
[[nodiscard]] QString tagValueWithCondition(const QString &tag, bool notCondition);
[[nodiscard]] QString tagValue(const QString &tag);
[[nodiscard]] QString strValue(QXmlStreamReader &element);
[[nodiscard]] QStringList listValue(QXmlStreamReader &element);
[[nodiscard]] QString listValueToStr(QXmlStreamReader &element);
[[nodiscard]] QString fixListValue(QString valueStr);
[[nodiscard]] QString quoteStr(const QString &str, bool protectSlash = true);
void comboboxItemNotFound(const QString &searchItem, const QString &name, QString &error);
[[nodiscard]] QString createFullWhatsThis(const QString &help, const QString &href);
[[nodiscard]] QString protectSlash(QString str);
[[nodiscard]] QString indentation();
[[nodiscard]] AbstractRegexpEditorLineEdit *createRegexpEditorLineEdit(QWidget *parent = nullptr);
[[nodiscard]] QString generateConditionComment(const QString &comment);
[[nodiscard]] QString loadConditionComment(QString originalComment, const QString &comment);
KSIEVEUI_TESTS_EXPORT KSieveUi::AbstractMoveImapFolderWidget *createImapFolderWidget();
KSIEVEUI_TESTS_EXPORT KSieveUi::AbstractSelectEmailLineEdit *createSelectEmailsWidget();
}
}
