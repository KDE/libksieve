/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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
#include "sieveactionenclose.h"
#include "widgets/multilineedit.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>
#include <QLineEdit>

#include <QLabel>
#include <QXmlStreamReader>
#include "libksieve_debug.h"
#include <QGridLayout>

using namespace KSieveUi;
SieveActionEnclose::SieveActionEnclose(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, QStringLiteral("enclose"), i18n("Enclose"), parent)
{
}

QWidget *SieveActionEnclose::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QGridLayout *grid = new QGridLayout;
    grid->setContentsMargins(0, 0, 0, 0);
    w->setLayout(grid);

    QLabel *lab = new QLabel(i18n("Subject:"));
    grid->addWidget(lab, 0, 0);

    QLineEdit *subject = new QLineEdit;
    subject->setObjectName(QStringLiteral("subject"));
    connect(subject, &QLineEdit::textChanged, this, &SieveActionEnclose::valueChanged);
    grid->addWidget(subject, 0, 1);

    lab = new QLabel(i18n("headers:"));
    grid->addWidget(lab, 1, 0);

    QLineEdit *headers = new QLineEdit;
    headers->setObjectName(QStringLiteral("headers"));
    connect(headers, &QLineEdit::textChanged, this, &SieveActionEnclose::valueChanged);
    grid->addWidget(headers, 1, 1);

    lab = new QLabel(i18n("text:"));
    grid->addWidget(lab, 2, 0);

    MultiLineEdit *text = new MultiLineEdit;
    text->setObjectName(QStringLiteral("text"));
    connect(text, &MultiLineEdit::valueChanged, this, &SieveActionEnclose::valueChanged);
    grid->addWidget(text, 2, 1);

    return w;
}

bool SieveActionEnclose::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1String("headers")) {
                const QString strValue = AutoCreateScriptUtil::strValue(element);
                if (!strValue.isEmpty()) {
                    QLineEdit *subject = w->findChild<QLineEdit *>(QStringLiteral("headers"));
                    subject->setText(strValue);
                }
            } else if (tagValue == QLatin1String("subject")) {
                const QString strValue = AutoCreateScriptUtil::strValue(element);
                if (!strValue.isEmpty()) {
                    QLineEdit *headers = w->findChild<QLineEdit *>(QStringLiteral("subject"));
                    headers->setText(strValue);
                }
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveActionEnclose::setParamWidgetValue unknown tag value:" << tagValue;
            }
        } else if (tagName == QLatin1String("str")) {
            MultiLineEdit *edit = w->findChild<MultiLineEdit *>(QStringLiteral("text"));
            edit->setPlainText(element.readElementText());
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            element.skipCurrentElement();
            //implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveActionEnclose::setParamWidgetValue unknown tagName " << tagName;
        }
    }

    return true;
}

QString SieveActionEnclose::code(QWidget *w) const
{
    QString result = QStringLiteral("enclose ");
    const QLineEdit *subject = w->findChild<QLineEdit *>(QStringLiteral("subject"));
    const QString subjectStr = subject->text();
    if (!subjectStr.isEmpty()) {
        result += QStringLiteral(":subject \"%1\" ").arg(subjectStr);
    }

    const QLineEdit *headers = w->findChild<QLineEdit *>(QStringLiteral("headers"));
    const QString headersStr = headers->text();
    if (!headersStr.isEmpty()) {
        result += QStringLiteral(":headers \"%1\" ").arg(headersStr);
    }

    const MultiLineEdit *edit = w->findChild<MultiLineEdit *>(QStringLiteral("text"));
    const QString text = edit->toPlainText();
    if (!text.isEmpty()) {
        result += QStringLiteral("text:%1").arg(AutoCreateScriptUtil::createMultiLine(text));
    } else {
        result += QLatin1Char(';');
    }

    return result;
}

QStringList SieveActionEnclose::needRequires(QWidget * /*parent*/) const
{
    return QStringList() << QStringLiteral("enclose");
}

bool SieveActionEnclose::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionEnclose::serverNeedsCapability() const
{
    return QStringLiteral("enclose");
}

QString SieveActionEnclose::help() const
{
    return i18n("Enclose action command is defined to allow an entire message to be enclosed as an attachment to a new message.");
}

QUrl SieveActionEnclose::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
