/*
   Copyright (C) 2013-2019 Laurent Montel <montel@kde.org>

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
#include "sieveactionconvert.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/commonwidgets/selectconvertparameterwidget.h"
#include "autocreatescripts/commonwidgets/selectmimetypecombobox.h"

#include <KLocalizedString>

#include <QLabel>
#include "libksieve_debug.h"
#include <QXmlStreamReader>
#include <QGridLayout>

using namespace KSieveUi;
SieveActionConvert::SieveActionConvert(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, QStringLiteral("convert"), i18n("Convert"), parent)
{
}

QWidget *SieveActionConvert::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QGridLayout *lay = new QGridLayout;
    lay->setContentsMargins(0, 0, 0, 0);
    w->setLayout(lay);

    QLabel *lab = new QLabel(i18n("From:"));
    lay->addWidget(lab, 0, 0);

    SelectMimeTypeComboBox *fromMimeType = new SelectMimeTypeComboBox;
    connect(fromMimeType, &SelectMimeTypeComboBox::valueChanged, this, &SieveActionConvert::valueChanged);
    fromMimeType->setObjectName(QStringLiteral("from"));
    lay->addWidget(fromMimeType, 0, 1);

    lab = new QLabel(i18n("To:"));
    lay->addWidget(lab, 0, 2);

    SelectMimeTypeComboBox *toMimeType = new SelectMimeTypeComboBox;
    connect(toMimeType, &SelectMimeTypeComboBox::valueChanged, this, &SieveActionConvert::valueChanged);
    toMimeType->setObjectName(QStringLiteral("to"));
    lay->addWidget(toMimeType, 0, 3);

    lab = new QLabel(i18n("Parameters:"));
    lay->addWidget(lab, 1, 0);

    SelectConvertParameterWidget *params = new SelectConvertParameterWidget;
    connect(params, &SelectConvertParameterWidget::valueChanged, this, &SieveActionConvert::valueChanged);
    params->setObjectName(QStringLiteral("params"));
    lay->addWidget(params, 1, 1, 2, 3);

    return w;
}

bool SieveActionConvert::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    int index = 0;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("str")) {
            if (index == 0) {
                SelectMimeTypeComboBox *fromMimeType = w->findChild<SelectMimeTypeComboBox *>(QStringLiteral("from"));
                fromMimeType->setCode(element.readElementText(), name(), error);
            } else if (index == 1) {
                SelectMimeTypeComboBox *toMimeType = w->findChild<SelectMimeTypeComboBox *>(QStringLiteral("to"));
                toMimeType->setCode(element.readElementText(), name(), error);
            } else {
                tooManyArgument(tagName, index, 2, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveActionConvert::setParamWidgetValue too many argument :" << index;
            }
            ++index;
        } else if (tagName == QLatin1String("list")) {
            SelectConvertParameterWidget *params = w->findChild<SelectConvertParameterWidget *>(QStringLiteral("params"));
            params->setCode(AutoCreateScriptUtil::listValue(element), error);
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            element.skipCurrentElement();
            //implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << "SieveActionConvert::setParamWidgetValue unknown tag " << tagName;
        }
    }

    return true;
}

QString SieveActionConvert::code(QWidget *w) const
{
    QString result = QStringLiteral("convert ");
    const SelectMimeTypeComboBox *fromMimeType = w->findChild<SelectMimeTypeComboBox *>(QStringLiteral("from"));
    const QString fromMimeTypeStr = fromMimeType->code();
    result += QStringLiteral("%1 ").arg(fromMimeTypeStr);

    const SelectMimeTypeComboBox *toMimeType = w->findChild<SelectMimeTypeComboBox *>(QStringLiteral("to"));
    const QString toMimeTypeStr = toMimeType->code();
    result += QStringLiteral("%1 ").arg(toMimeTypeStr);

    const SelectConvertParameterWidget *params = w->findChild<SelectConvertParameterWidget *>(QStringLiteral("params"));
    const QString paramsStr = params->code();
    if (!paramsStr.isEmpty()) {
        result += paramsStr;
    }
    result += QLatin1Char(';');
    return result;
}

QStringList SieveActionConvert::needRequires(QWidget *) const
{
    return QStringList() << QStringLiteral("convert");
}

bool SieveActionConvert::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionConvert::serverNeedsCapability() const
{
    return QStringLiteral("convert");
}

QString SieveActionConvert::help() const
{
    return i18n("The \"convert\" action specifies that all body parts with a media type equal to \"media-type\" be converted to the media type in \"media-type\" using conversion parameters.");
}

QUrl SieveActionConvert::href() const
{
    return QUrl(QStringLiteral("https://tools.ietf.org/html/draft-ietf-sieve-convert-06"));
}
