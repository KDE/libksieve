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
#include "sieveconditionservermetadata.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>
#include <QLineEdit>

#include <QWidget>
#include <QHBoxLayout>
#include "libksieve_debug.h"
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveConditionServerMetaData::SieveConditionServerMetaData(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("servermetadata"), i18n("Server Meta Data"), parent)
{
}

QWidget *SieveConditionServerMetaData::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setMargin(0);
    w->setLayout(lay);

    SelectMatchTypeComboBox *selectType = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    selectType->setObjectName(QStringLiteral("selecttype"));
    connect(selectType, &SelectMatchTypeComboBox::valueChanged, this, &SieveConditionServerMetaData::valueChanged);
    lay->addWidget(selectType);

    QGridLayout *grid = new QGridLayout;
    grid->setMargin(0);
    lay->addLayout(grid);

    QLabel *lab = new QLabel(i18n("Mailbox:"));
    grid->addWidget(lab, 0, 0);

    QLineEdit *mailbox = new QLineEdit;
    mailbox->setObjectName(QStringLiteral("mailbox"));
    connect(mailbox, &QLineEdit::textChanged, this, &SieveConditionServerMetaData::valueChanged);
    grid->addWidget(mailbox, 0, 1);

    lab = new QLabel(i18n("Annotations:"));
    grid->addWidget(lab, 1, 0);

    QLineEdit *annotation = new QLineEdit;
    connect(annotation, &QLineEdit::textChanged, this, &SieveConditionServerMetaData::valueChanged);
    annotation->setObjectName(QStringLiteral("annotation"));
    grid->addWidget(annotation, 1, 1);

    lab = new QLabel(i18n("Value:"));
    grid->addWidget(lab, 2, 0);

    AbstractRegexpEditorLineEdit *value = AutoCreateScriptUtil::createRegexpEditorLineEdit();
    connect(value, &AbstractRegexpEditorLineEdit::textChanged, this, &SieveConditionServerMetaData::valueChanged);
    connect(selectType, &SelectMatchTypeComboBox::switchToRegexp, value, &AbstractRegexpEditorLineEdit::switchToRegexpEditorLineEdit);
    value->setObjectName(QStringLiteral("value"));
    grid->addWidget(value, 2, 1);

    return w;
}

QString SieveConditionServerMetaData::code(QWidget *w) const
{
    const SelectMatchTypeComboBox *selectType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("selecttype"));
    bool isNegative = false;
    const QString matchString = selectType->code(isNegative);

    QString result = (isNegative ? QStringLiteral("not ") : QString()) + QStringLiteral("servermetadata %1 ").arg(matchString);

    const QLineEdit *mailbox = w->findChild<QLineEdit *>(QStringLiteral("mailbox"));
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

QStringList SieveConditionServerMetaData::needRequires(QWidget *w) const
{
    const SelectMatchTypeComboBox *selectType = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("selecttype"));
    return QStringList() << QStringLiteral("servermetadata") << selectType->needRequires();
}

bool SieveConditionServerMetaData::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionServerMetaData::serverNeedsCapability() const
{
    return QStringLiteral("servermetadata");
}

QString SieveConditionServerMetaData::help() const
{
    return i18n(
        "This test retrieves the value of the server annotation \"annotation-name\".  The retrieved value is compared to the \"key-list\". The test returns true if the annotation exists and its value matches any of the keys.");
}

bool SieveConditionServerMetaData::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool notCondition, QString &error)
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
                QLineEdit *mailbox = w->findChild<QLineEdit *>(QStringLiteral("mailbox"));
                mailbox->setText(tagValue);
                break;
            }
            case 1:
            {
                QLineEdit *annotation = w->findChild<QLineEdit *>(QStringLiteral("annotation"));
                annotation->setText(tagValue);
                break;
            }
            case 2:
            {
                AbstractRegexpEditorLineEdit *value = w->findChild<AbstractRegexpEditorLineEdit *>(QStringLiteral("value"));
                value->setCode(tagValue);
                break;
            }
            default:
                tooManyArgument(tagName, index, 3, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionServerMetaData::setParamWidgetValue too many argument " << index;
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
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionServerMetaData::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }

    return true;
}

QUrl SieveConditionServerMetaData::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
