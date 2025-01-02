/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactiondeleteheader.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "editor/sieveeditorutil.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
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
    matchType->setObjectName(QLatin1StringView("matchtype"));
    connect(matchType, &SelectMatchTypeComboBox::valueChanged, this, &SieveActionDeleteHeader::valueChanged);
    grid->addWidget(matchType, 0, 0);

    auto lab = new QLabel(i18nc("@label:textbox", "header:"));
    grid->addWidget(lab, 0, 1);

    auto headerEdit = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(headerEdit);
    headerEdit->setObjectName(QLatin1StringView("headeredit"));
    connect(headerEdit, &QLineEdit::textChanged, this, &SieveActionDeleteHeader::valueChanged);
    grid->addWidget(headerEdit, 0, 2);

    lab = new QLabel(i18nc("@label:textbox", "value:"));
    grid->addWidget(lab, 1, 1);

    auto valueEdit = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(valueEdit);
    valueEdit->setObjectName(QLatin1StringView("valueedit"));
    connect(valueEdit, &QLineEdit::textChanged, this, &SieveActionDeleteHeader::valueChanged);
    grid->addWidget(valueEdit, 1, 2);
    return w;
}

void SieveActionDeleteHeader::parseValue(QXmlStreamReader &element, QWidget *w, QString &error, bool isNegative)
{
    int index = 0;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("test")) {
            QString nameValue;
            if (element.attributes().hasAttribute(QLatin1StringView("name"))) {
                nameValue = element.attributes().value(QLatin1StringView("name")).toString();
            }
            const bool isNot = (nameValue == QLatin1StringView("not"));
            parseValue(element, w, error, isNot);
            element.skipCurrentElement();
            return;
        } else if (tagName == QLatin1StringView("tag")) {
            auto combo = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
            combo->setCode(AutoCreateScriptUtil::tagValueWithCondition(element.readElementText(), isNegative), name(), error);
        } else if (tagName == QLatin1StringView("str")) {
            if (index == 0) {
                auto edit = w->findChild<QLineEdit *>(QStringLiteral("headeredit"));
                edit->setText(element.readElementText());
            } else if (index == 1) {
                auto value = w->findChild<QLineEdit *>(QStringLiteral("valueedit"));
                value->setText(element.readElementText());
            } else {
                tooManyArguments(tagName, index, 2, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveActionAddHeader::setParamWidgetValue too many argument :" << index;
            }
            ++index;
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << "SieveActionAddHeader::setParamWidgetValue unknown tag " << tagName;
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

    return QStringLiteral("deleteheader %1 \"%2\" \"%3\";").arg((isNegative ? QLatin1StringView("not ") + matchTypeStr : matchTypeStr), headerStr, valueStr);
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

#include "moc_sieveactiondeleteheader.cpp"
