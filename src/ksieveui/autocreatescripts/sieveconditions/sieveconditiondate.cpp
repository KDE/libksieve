/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditiondate.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "widgets/selectdatewidget.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>
#include <QLineEdit>

#include <QHBoxLayout>
#include <QLabel>
#include "libksieve_debug.h"

using namespace KSieveUi;

SieveConditionDate::SieveConditionDate(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("date"), i18n("Date"), parent)
{
}

QWidget *SieveConditionDate::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    auto *lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto *matchTypeCombo = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    matchTypeCombo->setObjectName(QStringLiteral("matchtype"));
    connect(matchTypeCombo, &SelectMatchTypeComboBox::valueChanged, this, &SieveConditionDate::valueChanged);
    lay->addWidget(matchTypeCombo);

    auto *grid = new QGridLayout;
    grid->setContentsMargins({});
    lay->addLayout(grid);
    QLabel *lab = new QLabel(i18n("header:"));
    grid->addWidget(lab, 0, 0);

    auto *header = new QLineEdit;
    connect(header, &QLineEdit::textChanged, this, &SieveConditionDate::valueChanged);
    header->setObjectName(QStringLiteral("header"));
    grid->addWidget(header, 0, 1);

    auto *dateWidget = new SelectDateWidget;
    connect(dateWidget, &SelectDateWidget::valueChanged, this, &SieveConditionDate::valueChanged);
    dateWidget->setObjectName(QStringLiteral("datewidget"));
    grid->addWidget(dateWidget, 1, 0, 1, 2);

    return w;
}

QString SieveConditionDate::code(QWidget *w) const
{
    const SelectMatchTypeComboBox *selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
    bool isNegative = false;
    const QString matchTypeStr = selectMatchCombobox->code(isNegative);

    const QLineEdit *header = w->findChild<QLineEdit *>(QStringLiteral("header"));
    const QString headerStr = header->text();

    const SelectDateWidget *dateWidget = w->findChild<SelectDateWidget *>(QStringLiteral("datewidget"));
    const QString dateWidgetStr = dateWidget->code();

    return AutoCreateScriptUtil::negativeString(isNegative) + QStringLiteral("date %1 \"%2\" %3").arg(matchTypeStr, headerStr, dateWidgetStr)
           + AutoCreateScriptUtil::generateConditionComment(comment());
}

bool SieveConditionDate::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionDate::serverNeedsCapability() const
{
    return QStringLiteral("date");
}

QStringList SieveConditionDate::needRequires(QWidget *w) const
{
    const SelectMatchTypeComboBox *selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
    return QStringList() << QStringLiteral("date") << selectMatchCombobox->needRequires();
}

QString SieveConditionDate::help() const
{
    return i18n("The date test matches date/time information derived from headers containing date-time values.");
}

void SieveConditionDate::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool notCondition, QString &error)
{
    int index = 0;
    QString type;
    QString value;
    QString headerStr;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("str")) {
            if (index == 0) {
                headerStr = element.readElementText();
            } else if (index == 1) {
                type = element.readElementText();
            } else if (index == 2) {
                value = element.readElementText();
            } else {
                tooManyArguments(tagName, index, 3, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionDate::setParamWidgetValue too many argument :" << index;
            }
            ++index;
        } else if (tagName == QLatin1String("tag")) {
            auto *selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
            selectMatchCombobox->setCode(AutoCreateScriptUtil::tagValueWithCondition(element.readElementText(), notCondition), name(), error);
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << "SieveConditionDate::setParamWidgetValue unknown tag " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }

    auto *dateWidget = w->findChild<SelectDateWidget *>(QStringLiteral("datewidget"));
    dateWidget->setCode(type, value);
    auto *header = w->findChild<QLineEdit *>(QStringLiteral("header"));
    header->setText(headerStr);
}

QUrl SieveConditionDate::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
