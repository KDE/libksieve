/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionenvironment.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieve_debug.h"
#include <QCompleter>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveConditionEnvironment::SieveConditionEnvironment(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("environment"), i18n("Environment"), parent)
{
}

QWidget *SieveConditionEnvironment::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    w->setLayout(grid);
    auto lab = new QLabel(i18n("Item:"));
    grid->addWidget(lab, 0, 0);

    auto item = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(item);
    QStringList itemList;
    itemList << QStringLiteral("domain") << QStringLiteral("host") << QStringLiteral("location") << QStringLiteral("name") << QStringLiteral("phase")
             << QStringLiteral("remote-host") << QStringLiteral("remote-ip") << QStringLiteral("version");
    auto completer = new QCompleter(itemList, w);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    item->setCompleter(completer);
    connect(item, &QLineEdit::textChanged, this, &SieveConditionEnvironment::valueChanged);

    item->setObjectName(QStringLiteral("item"));
    grid->addWidget(item, 0, 1);

    lab = new QLabel(i18n("Value:"));
    grid->addWidget(lab, 1, 0);

    auto value = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(value);
    connect(value, &QLineEdit::textChanged, this, &SieveConditionEnvironment::valueChanged);
    value->setObjectName(QStringLiteral("value"));
    grid->addWidget(value, 1, 1);

    return w;
}

QString SieveConditionEnvironment::code(QWidget *w) const
{
    const QLineEdit *item = w->findChild<QLineEdit *>(QStringLiteral("item"));
    const QString itemStr = item->text();

    const QLineEdit *value = w->findChild<QLineEdit *>(QStringLiteral("value"));
    const QString valueStr = value->text();

    return QStringLiteral("environment \"%1\" \"%2\"").arg(itemStr, valueStr) + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionEnvironment::needRequires(QWidget *) const
{
    return QStringList() << QStringLiteral("environment");
}

bool SieveConditionEnvironment::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionEnvironment::serverNeedsCapability() const
{
    return QStringLiteral("environment");
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
        if (tagName == QLatin1String("str")) {
            if (index == 0) {
                auto item = w->findChild<QLineEdit *>(QStringLiteral("item"));
                item->setText(AutoCreateScriptUtil::quoteStr(element.readElementText()));
            } else if (index == 1) {
                auto value = w->findChild<QLineEdit *>(QStringLiteral("value"));
                value->setText(AutoCreateScriptUtil::quoteStr(element.readElementText()));
            } else {
                tooManyArguments(tagName, index, 2, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionEnvironment::setParamWidgetValue to many argument " << index;
            }
            ++index;
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveActionSetVariable::setParamWidgetValue unknown tagName " << tagName;
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
