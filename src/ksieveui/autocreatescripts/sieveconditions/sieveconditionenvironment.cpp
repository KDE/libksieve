/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionenvironment.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <QCompleter>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveConditionEnvironment::SieveConditionEnvironment(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, u"environment"_s, i18n("Environment"), parent)
{
}

QWidget *SieveConditionEnvironment::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    w->setLayout(grid);
    auto lab = new QLabel(i18nc("@label:textbox", "Item:"));
    grid->addWidget(lab, 0, 0);

    auto item = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(item);
    QStringList itemList;
    itemList << u"domain"_s << u"host"_s << QStringLiteral("location") << QStringLiteral("name") << QStringLiteral("phase") << u"remote-host"_s
             << u"remote-ip"_s << QStringLiteral("version");
    auto completer = new QCompleter(itemList, w);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    item->setCompleter(completer);
    connect(item, &QLineEdit::textChanged, this, &SieveConditionEnvironment::valueChanged);

    item->setObjectName(QLatin1StringView("item"));
    grid->addWidget(item, 0, 1);

    lab = new QLabel(i18nc("@label:textbox", "Value:"));
    grid->addWidget(lab, 1, 0);

    auto value = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(value);
    connect(value, &QLineEdit::textChanged, this, &SieveConditionEnvironment::valueChanged);
    value->setObjectName(QLatin1StringView("value"));
    grid->addWidget(value, 1, 1);

    return w;
}

QString SieveConditionEnvironment::code(QWidget *w) const
{
    const QLineEdit *item = w->findChild<QLineEdit *>(u"item"_s);
    const QString itemStr = item->text();

    const QLineEdit *value = w->findChild<QLineEdit *>(u"value"_s);
    const QString valueStr = value->text();

    return u"environment \"%1\" \"%2\""_s.arg(itemStr, valueStr) + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionEnvironment::needRequires(QWidget *) const
{
    return QStringList() << u"environment"_s;
}

bool SieveConditionEnvironment::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionEnvironment::serverNeedsCapability() const
{
    return u"environment"_s;
}

QString SieveConditionEnvironment::help() const
{
    return i18n(
        "The environment test retrieves the item of environment information specified by the name string and matches it to the values specified in the "
        "key-list argument.");
}

void SieveConditionEnvironment::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool /*notCondition*/, QString &error)
{
    int index = 0;
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("str")) {
            if (index == 0) {
                auto item = w->findChild<QLineEdit *>(u"item"_s);
                item->setText(AutoCreateScriptUtil::quoteStr(element.readElementText()));
            } else if (index == 1) {
                auto value = w->findChild<QLineEdit *>(u"value"_s);
                value->setText(AutoCreateScriptUtil::quoteStr(element.readElementText()));
            } else {
                tooManyArguments(tagName, index, 2, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionEnvironment::setParamWidgetValue to many argument " << index;
            }
            ++index;
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveActionSetVariable::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl KSieveUi::SieveConditionEnvironment::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditionenvironment.cpp"
