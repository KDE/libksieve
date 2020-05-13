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
#include "sieveactionaddheader.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "widgets/selectaddheaderpositioncombobox.h"

#include <KLocalizedString>
#include <QLineEdit>

#include <QWidget>
#include <QLabel>
#include <QXmlStreamReader>
#include "libksieve_debug.h"
#include <QGridLayout>

using namespace KSieveUi;

SieveActionAddHeader::SieveActionAddHeader(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveActionAbstractEditHeader(sieveGraphicalModeWidget, QStringLiteral("addheader"), i18n("Add header"), parent)
{
}

QWidget *SieveActionAddHeader::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QGridLayout *grid = new QGridLayout;
    grid->setContentsMargins(0, 0, 0, 0);
    w->setLayout(grid);

    SelectAddHeaderPositionCombobox *combo = new SelectAddHeaderPositionCombobox;
    combo->setObjectName(QStringLiteral("selectposition"));
    connect(combo, &SelectAddHeaderPositionCombobox::valueChanged, this, &SieveActionAddHeader::valueChanged);
    grid->addWidget(combo, 0, 0);

    QLabel *lab = new QLabel(i18n("header:"));
    grid->addWidget(lab, 0, 1);

    QLineEdit *headerEdit = new QLineEdit;
    connect(headerEdit, &QLineEdit::textChanged, this, &SieveActionAddHeader::valueChanged);
    headerEdit->setObjectName(QStringLiteral("headeredit"));
    grid->addWidget(headerEdit, 0, 2);

    lab = new QLabel(i18n("value:"));
    grid->addWidget(lab, 1, 1);

    QLineEdit *valueEdit = new QLineEdit;
    connect(valueEdit, &QLineEdit::textChanged, this, &SieveActionAddHeader::valueChanged);
    valueEdit->setObjectName(QStringLiteral("valueedit"));
    grid->addWidget(valueEdit, 1, 2);

    return w;
}

void SieveActionAddHeader::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    int index = 0;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("tag")) {
            SelectAddHeaderPositionCombobox *combo = w->findChild<SelectAddHeaderPositionCombobox *>(QStringLiteral("selectposition"));
            combo->setCode(AutoCreateScriptUtil::tagValue(element.readElementText()), name(), error);
        } else if (tagName == QLatin1String("str")) {
            if (index == 0) {
                QLineEdit *edit = w->findChild<QLineEdit *>(QStringLiteral("headeredit"));
                edit->setText(element.readElementText());
            } else if (index == 1) {
                QLineEdit *value = w->findChild<QLineEdit *>(QStringLiteral("valueedit"));
                value->setText(AutoCreateScriptUtil::quoteStr(element.readElementText()));
            } else {
                tooManyArguments(tagName, index, 2, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveActionAddHeader::setParamWidgetValue too many argument :" << index;
            }
            ++index;
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            element.skipCurrentElement();
            //implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << "SieveActionAddHeader::setParamWidgetValue unknown tag " << tagName;
        }
    }
}

QString SieveActionAddHeader::code(QWidget *w) const
{
    const SelectAddHeaderPositionCombobox *combo = w->findChild<SelectAddHeaderPositionCombobox *>(QStringLiteral("selectposition"));
    const QString position = combo->code();

    const QLineEdit *edit = w->findChild<QLineEdit *>(QStringLiteral("headeredit"));
    const QString headerStr = edit->text();

    const QLineEdit *value = w->findChild<QLineEdit *>(QStringLiteral("valueedit"));
    const QString valueStr = value->text();

    if (position.isEmpty()) {
        return QStringLiteral("addheader \"%1\" \"%2\";").arg(headerStr, valueStr);
    } else {
        return QStringLiteral("addheader %1 \"%2\" \"%3\";").arg(position, headerStr, valueStr);
    }
}

QString SieveActionAddHeader::help() const
{
    return i18n("The addheader action adds a header field to the existing message header.");
}

QUrl SieveActionAddHeader::href() const
{
    return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5293"));
}
