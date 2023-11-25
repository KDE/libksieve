/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditiondate.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/commonwidgets/selectmatchtypecombobox.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectdatewidget.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <QHBoxLayout>
#include <QLabel>

using namespace KSieveUi;

SieveConditionDate::SieveConditionDate(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("date"), i18n("Date"), parent)
{
}

QWidget *SieveConditionDate::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto matchTypeCombo = new SelectMatchTypeComboBox(mSieveGraphicalModeWidget);
    matchTypeCombo->setObjectName(QLatin1StringView("matchtype"));
    connect(matchTypeCombo, &SelectMatchTypeComboBox::valueChanged, this, &SieveConditionDate::valueChanged);
    lay->addWidget(matchTypeCombo);

    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    lay->addLayout(grid);
    auto lab = new QLabel(i18n("header:"));
    grid->addWidget(lab, 0, 0);

    auto header = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(header);
    connect(header, &QLineEdit::textChanged, this, &SieveConditionDate::valueChanged);
    header->setObjectName(QLatin1StringView("header"));
    grid->addWidget(header, 0, 1);

    auto dateWidget = new SelectDateWidget;
    connect(dateWidget, &SelectDateWidget::valueChanged, this, &SieveConditionDate::valueChanged);
    dateWidget->setObjectName(QLatin1StringView("datewidget"));
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
        const QStringView tagName = element.name();
        if (tagName == QLatin1String("str")) {
            if (index == 0) {
                headerStr = element.readElementText();
            } else if (index == 1) {
                type = element.readElementText();
            } else if (index == 2) {
                value = element.readElementText();
            } else {
                tooManyArguments(tagName, index, 3, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionDate::setParamWidgetValue too many argument :" << index;
            }
            ++index;
        } else if (tagName == QLatin1String("tag")) {
            auto selectMatchCombobox = w->findChild<SelectMatchTypeComboBox *>(QStringLiteral("matchtype"));
            selectMatchCombobox->setCode(AutoCreateScriptUtil::tagValueWithCondition(element.readElementText(), notCondition), name(), error);
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << "SieveConditionDate::setParamWidgetValue unknown tag " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }

    auto dateWidget = w->findChild<SelectDateWidget *>(QStringLiteral("datewidget"));
    dateWidget->setCode(type, value);
    auto header = w->findChild<QLineEdit *>(QStringLiteral("header"));
    header->setText(headerStr);
}

QUrl SieveConditionDate::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditiondate.cpp"
