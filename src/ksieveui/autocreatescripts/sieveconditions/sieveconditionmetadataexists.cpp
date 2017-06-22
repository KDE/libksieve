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
#include "sieveconditionmetadataexists.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include <KLocalizedString>
#include <QLineEdit>

#include <QLabel>
#include "libksieve_debug.h"
#include <QXmlStreamReader>
#include <QGridLayout>

using namespace KSieveUi;
SieveConditionMetaDataExists::SieveConditionMetaDataExists(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("metadataexists"), i18n("Metadata exists"), parent)
{
}

QWidget *SieveConditionMetaDataExists::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QGridLayout *grid = new QGridLayout;
    grid->setMargin(0);
    w->setLayout(grid);

    QLabel *lab = new QLabel(i18n("Mailbox:"));
    grid->addWidget(lab, 0, 0);

    QLineEdit *mailbox = new QLineEdit;
    connect(mailbox, &QLineEdit::textChanged, this, &SieveConditionMetaDataExists::valueChanged);
    mailbox->setObjectName(QStringLiteral("mailbox"));
    grid->addWidget(mailbox, 0, 1);

    lab = new QLabel(i18n("Annotation:"));
    grid->addWidget(lab, 1, 0);

    QLineEdit *value = new QLineEdit;
    connect(value, &QLineEdit::textChanged, this, &SieveConditionMetaDataExists::valueChanged);
    value->setObjectName(QStringLiteral("value"));
    grid->addWidget(value, 1, 1);

    return w;
}

QString SieveConditionMetaDataExists::code(QWidget *w) const
{
    const QLineEdit *mailbox = w->findChild<QLineEdit *>(QStringLiteral("mailbox"));
    const QString mailboxStr = mailbox->text();

    const QLineEdit *value = w->findChild<QLineEdit *>(QStringLiteral("value"));
    const QString valueStr = value->text();
    return QStringLiteral("metadataexists \"%1\" \"%2\"").arg(mailboxStr, valueStr) + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionMetaDataExists::needRequires(QWidget *) const
{
    return QStringList() << QStringLiteral("mboxmetadata");
}

bool SieveConditionMetaDataExists::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionMetaDataExists::serverNeedsCapability() const
{
    return QStringLiteral("mboxmetadata");
}

QString SieveConditionMetaDataExists::help() const
{
    return i18n("The \"metadataexists\" test is true if all of the annotations listed in the \"annotation-names\" argument exist for the specified mailbox.");
}

bool SieveConditionMetaDataExists::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool /*notCondition*/, QString &error)
{
    int index = 0;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("str")) {
            const QString tagValue = element.readElementText();
            if (index == 0) {
                QLineEdit *mailbox = w->findChild<QLineEdit *>(QStringLiteral("mailbox"));
                mailbox->setText(tagValue);
            } else if (index == 1) {
                QLineEdit *value = w->findChild<QLineEdit *>(QStringLiteral("value"));
                value->setText(AutoCreateScriptUtil::quoteStr(tagValue));
            } else {
                tooManyArgument(tagName, index, 2, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionServerMetaDataExists::setParamWidgetValue to many attribute " << index;
            }
            ++index;
        } else if (tagName == QLatin1String("crlf")) {
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionServerMetaDataExists::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }

#ifdef REMOVE_QDOMELEMENT
    int index = 0;
    QDomNode node = element.firstChild();
    QString commentStr;
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        if (!e.isNull()) {
            const QString tagName = e.tagName();
            if (tagName == QLatin1String("str")) {
                const QString tagValue = e.text();
                if (index == 0) {
                    QLineEdit *mailbox = w->findChild<QLineEdit *>(QStringLiteral("mailbox"));
                    mailbox->setText(tagValue);
                } else if (index == 1) {
                    QLineEdit *value = w->findChild<QLineEdit *>(QStringLiteral("value"));
                    value->setText(AutoCreateScriptUtil::quoteStr(tagValue));
                } else {
                    tooManyArgument(tagName, index, 2, error);
                    qCDebug(LIBKSIEVE_LOG) << " SieveConditionServerMetaDataExists::setParamWidgetValue to many attribute " << index;
                }
                ++index;
            } else if (tagName == QLatin1String("crlf")) {
                //nothing
            } else if (tagName == QLatin1String("comment")) {
                commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, e.text());
            } else {
                unknownTag(tagName, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionServerMetaDataExists::setParamWidgetValue unknown tagName " << tagName;
            }
        }
        node = node.nextSibling();
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
#endif
    return true;
}

QUrl SieveConditionMetaDataExists::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
