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
#include "sieveactionbreak.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>
#include <QLineEdit>

#include <QHBoxLayout>
#include <QLabel>
#include <QXmlStreamReader>
#include "libksieve_debug.h"

using namespace KSieveUi;
SieveActionBreak::SieveActionBreak(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, QStringLiteral("break"), i18n("Break"), parent)
{
}

QWidget *SieveActionBreak::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setMargin(0);
    w->setLayout(lay);

    QLabel *lab = new QLabel(i18n("Name (optional):"));
    lay->addWidget(lab);

    QLineEdit *subject = new QLineEdit;
    subject->setObjectName(QStringLiteral("name"));
    connect(subject, &QLineEdit::textChanged, this, &SieveActionBreak::valueChanged);
    lay->addWidget(subject);
    return w;
}

bool SieveActionBreak::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    qDebug() << " bool SieveActionBreak::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)";
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        qDebug() << " tagName " << tagName;
        if (tagName == QLatin1String("tag")) {
            const QString tagValue = element.readElementText();
            qDebug() << " bool SieveActionBreak::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)"<<tagValue;
            if (tagValue == QLatin1String("name")) {
                QLineEdit *name = w->findChild<QLineEdit *>(QStringLiteral("name"));
                name->setText(AutoCreateScriptUtil::strValue(element));
                qDebug() << " NAME !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<name->text();
            } else {
                unknowTagValue(tagValue, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveActionBreak::setParamWidgetValue unknown tagValue " << tagValue;
            }
        } else if (tagName == QLatin1String("str")) {
            element.skipCurrentElement();
            //Nothing
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            element.skipCurrentElement();
            //implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << "SieveActionBreak::setParamWidgetValue unknown tag " << tagName;
        }
    }

#ifdef REMOVE_QDOMELEMENT
    QDomNode node = element.firstChild();
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        if (!e.isNull()) {
            const QString tagName = e.tagName();
            if (tagName == QLatin1String("tag")) {
                const QString tagValue = e.text();
                if (tagValue == QLatin1String("name")) {
                    QLineEdit *name = w->findChild<QLineEdit *>(QStringLiteral("name"));
                    name->setText(AutoCreateScriptUtil::strValue(e));
                } else {
                    unknowTagValue(tagValue, error);
                    qCDebug(LIBKSIEVE_LOG) << " SieveActionBreak::setParamWidgetValue unknown tagValue " << tagValue;
                }
            } else if (tagName == QLatin1String("str")) {
                //Nothing
            } else if (tagName == QLatin1String("crlf")) {
                //nothing
            } else if (tagName == QLatin1String("comment")) {
                //implement in the future ?
            } else {
                unknownTag(tagName, error);
                qCDebug(LIBKSIEVE_LOG) << "SieveActionBreak::setParamWidgetValue unknown tag " << tagName;
            }
        }
        node = node.nextSibling();
    }
#endif
    return true;
}

QUrl SieveActionBreak::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

QString SieveActionBreak::code(QWidget *w) const
{
    const QLineEdit *name = w->findChild<QLineEdit *>(QStringLiteral("name"));
    const QString nameStr = name->text();
    if (!nameStr.isEmpty()) {
        return QStringLiteral("break :name \"%1\";").arg(nameStr);
    }
    return QStringLiteral("break;");
}

QString SieveActionBreak::help() const
{
    return i18n("The break command terminates the closest enclosing loop.");
}

QStringList SieveActionBreak::needRequires(QWidget */*parent*/) const
{
    return QStringList() << QStringLiteral("foreverypart");
}

bool SieveActionBreak::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionBreak::serverNeedsCapability() const
{
    return QStringLiteral("foreverypart");
}
