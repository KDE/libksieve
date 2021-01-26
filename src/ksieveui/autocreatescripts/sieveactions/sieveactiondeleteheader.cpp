/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactiondeleteheader.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>
#include <QLineEdit>

#include "libksieve_debug.h"
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveActionDeleteHeader::SieveActionDeleteHeader(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveActionAbstractEditHeader(sieveGraphicalModeWidget, QStringLiteral("deleteheader"), i18n("Delete header"), parent)
{
}

QWidget *SieveActionDeleteHeader::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    w->setLayout(grid);

    auto matchType = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    matchType->setObjectName(QStringLiteral("matchtype"));
    connect(matchType, &SelectMatchTypeComboBox::valueChanged, this, &SieveActionDeleteHeader::valueChanged);
    grid->addWidget(matchType, 0, 0);

    auto lab = new QLabel(i18n("header:"));
    grid->addWidget(lab, 0, 1);

    auto headerEdit = new QLineEdit;
    headerEdit->setObjectName(QStringLiteral("headeredit"));
    connect(headerEdit, &QLineEdit::textChanged, this, &SieveActionDeleteHeader::valueChanged);
    grid->addWidget(headerEdit, 0, 2);

    lab = new QLabel(i18n("value:"));
    grid->addWidget(lab, 1, 1);

    auto valueEdit = new QLineEdit;
    valueEdit->setObjectName(QStringLiteral("valueedit"));
    connect(valueEdit, &QLineEdit::textChanged, this, &SieveActionDeleteHeader::valueChanged);
    grid->addWidget(valueEdit, 1, 2);
    return w;
}

void SieveActionDeleteHeader::parseValue(QXmlStreamReader &element, QWidget *w, QString &error, bool isNegative)
{
    int index = 0;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("test")) {
            QString nameValue;
            if (element.attributes().hasAttribute(QLatin1String("name"))) {
                nameValue = element.attributes().value(QLatin1String("name")).toString();
            }
            const bool isNegative = (nameValue == QLatin1String("not"));
            parseValue(element, w, error, isNegative);
            element.skipCurrentElement();
            return;
        } else if (tagName == QLatin1String("tag")) {
            auto combo = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
            combo->setCode(AutoCreateScriptUtil::tagValueWithCondition(element.readElementText(), isNegative), name(), error);
        } else if (tagName == QLatin1String("str")) {
            if (index == 0) {
                auto edit = w->findChild<QLineEdit *>(QStringLiteral("headeredit"));
                edit->setText(element.readElementText());
            } else if (index == 1) {
                auto value = w->findChild<QLineEdit *>(QStringLiteral("valueedit"));
                value->setText(element.readElementText());
            } else {
                tooManyArguments(tagName, index, 2, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveActionAddHeader::setParamWidgetValue too many argument :" << index;
            }
            ++index;
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << "SieveActionAddHeader::setParamWidgetValue unknown tag " << tagName;
        }
    }
}

void SieveActionDeleteHeader::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    parseValue(element, w, error, false);
}

QString SieveActionDeleteHeader::code(QWidget *w) const
{
    const SelectMatchTypeComboBox *combo = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
    bool isNegative = false;
    const QString matchTypeStr = combo->code(isNegative);

    const QLineEdit *edit = w->findChild<QLineEdit *>(QStringLiteral("headeredit"));
    const QString headerStr = edit->text();

    const QLineEdit *value = w->findChild<QLineEdit *>(QStringLiteral("valueedit"));
    const QString valueStr = value->text();

    return QStringLiteral("deleteheader %1 \"%2\" \"%3\";").arg((isNegative ? QLatin1String("not ") + matchTypeStr : matchTypeStr), headerStr, valueStr);
}

QString SieveActionDeleteHeader::help() const
{
    return i18n("By default, the deleteheader action deletes all occurrences of the named header field.");
}

QUrl SieveActionDeleteHeader::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

QStringList SieveActionDeleteHeader::needRequires(QWidget *w) const
{
    const SelectMatchTypeComboBox *combo = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
    return SieveActionAbstractEditHeader::needRequires(w) + combo->needRequires();
}
