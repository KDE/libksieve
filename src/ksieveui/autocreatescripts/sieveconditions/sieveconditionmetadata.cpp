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
#include "sieveconditionmetadata.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "editor/sieveeditorutil.h"
#include <KLocalizedString>
#include <QLineEdit>

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include "libksieve_debug.h"
#include <QXmlStreamReader>
#include <KSieveUi/AbstractMoveImapFolderWidget>

using namespace KSieveUi;
SieveConditionMetaData::SieveConditionMetaData(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("metadata"), i18n("Meta Data"), parent)
{
}

QWidget *SieveConditionMetaData::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setMargin(0);
    w->setLayout(lay);

    SelectMatchTypeComboBox *selectType = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    selectType->setObjectName(QStringLiteral("selecttype"));
    connect(selectType, &SelectMatchTypeComboBox::valueChanged, this, &SieveConditionMetaData::valueChanged);
    lay->addWidget(selectType);

    QGridLayout *grid = new QGridLayout;
    grid->setMargin(0);
    lay->addLayout(grid);

    QLabel *lab = new QLabel(i18n("Mailbox:"));
    grid->addWidget(lab, 0, 0);

    KSieveUi::AbstractMoveImapFolderWidget *mailbox = AutoCreateScriptUtil::createImapFolderWidget();

    connect(mailbox, &KSieveUi::AbstractMoveImapFolderWidget::textChanged, this, &SieveConditionMetaData::valueChanged);
    mailbox->setObjectName(QStringLiteral("mailbox"));
    grid->addWidget(mailbox, 0, 1);

    lab = new QLabel(i18n("Annotations:"));
    grid->addWidget(lab, 1, 0);

    QLineEdit *annotation = new QLineEdit;
    connect(annotation, &QLineEdit::textChanged, this, &SieveConditionMetaData::valueChanged);
    annotation->setObjectName(QStringLiteral("annotation"));
    grid->addWidget(annotation, 1, 1);

    lab = new QLabel(i18n("Value:"));
    grid->addWidget(lab, 2, 0);

    AbstractRegexpEditorLineEdit *value = AutoCreateScriptUtil::createRegexpEditorLineEdit();
    connect(value, &AbstractRegexpEditorLineEdit::textChanged, this, &SieveConditionMetaData::valueChanged);
    connect(selectType, &SelectMatchTypeComboBox::switchToRegexp, value, &AbstractRegexpEditorLineEdit::switchToRegexpEditorLineEdit);
    value->setObjectName(QStringLiteral("value"));
    grid->addWidget(value, 2, 1);

    return w;
}

QString SieveConditionMetaData::code(QWidget *w) const
{
    const SelectMatchTypeComboBox *selectType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("selecttype"));
    bool isNegative = false;
    const QString matchString = selectType->code(isNegative);

    QString result = AutoCreateScriptUtil::negativeString(isNegative) + QStringLiteral("metadata %1 ").arg(matchString);

    const KSieveUi::AbstractMoveImapFolderWidget *mailbox = w->findChild<KSieveUi::AbstractMoveImapFolderWidget *>(QStringLiteral("mailbox"));
    const QString mailboxStr = mailbox->text();

    result += QStringLiteral("\"%1\" ").arg(mailboxStr);

    const QLineEdit *annotation = w->findChild<QLineEdit *>(QStringLiteral("annotation"));
    const QString annotationStr = annotation->text();

    result += QStringLiteral("\"%1\" ").arg(annotationStr);

    const AbstractRegexpEditorLineEdit *value = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("value"));
    const QString valueStr = value->code();

    result += QStringLiteral("\"%1\"").arg(valueStr);
    return result + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionMetaData::needRequires(QWidget *w) const
{
    const SelectMatchTypeComboBox *selectType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("selecttype"));
    return QStringList() << QStringLiteral("mboxmetadata") << selectType->needRequires();
}

bool SieveConditionMetaData::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionMetaData::serverNeedsCapability() const
{
    return QStringLiteral("mboxmetadata");
}

QString SieveConditionMetaData::help() const
{
    return i18n(
        "This test retrieves the value of the mailbox annotation \"annotation-name\" for the mailbox \"mailbox\". The retrieved value is compared to the \"key-list\". The test returns true if the annotation exists and its value matches any of the keys.");
}

bool SieveConditionMetaData::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool notCondition, QString &error)
{
    int index = 0;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("str")) {
            const QString tagValue = element.readElementText();
            switch (index) {
            case 0:
            {
                KSieveUi::AbstractMoveImapFolderWidget *mailbox = w->findChild<KSieveUi::AbstractMoveImapFolderWidget *>(QStringLiteral("mailbox"));
                mailbox->setText(tagValue);
                break;
            }
            case 1:
            {
                QLineEdit *annotation = w->findChild<QLineEdit *>(QStringLiteral("annotation"));
                annotation->setText(AutoCreateScriptUtil::quoteStr(tagValue));
                break;
            }
            case 2:
            {
                AbstractRegexpEditorLineEdit *value = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("value"));
                value->setCode(AutoCreateScriptUtil::quoteStr(tagValue));
                break;
            }
            default:
                tooManyArgument(tagName, index, 3, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionMetaData::setParamWidgetValue too many argument " << index;
                break;
            }
            ++index;
        } else if (tagName == QLatin1String("tag")) {
            SelectMatchTypeComboBox *selectType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("selecttype"));
            selectType->setCode(AutoCreateScriptUtil::tagValueWithCondition(element.readElementText(), notCondition), name(), error);
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionMetaData::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }

    return true;
}

QUrl SieveConditionMetaData::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
