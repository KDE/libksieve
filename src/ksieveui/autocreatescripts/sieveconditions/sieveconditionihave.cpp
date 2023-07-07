/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionihave.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <QHBoxLayout>
#include <QWidget>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveConditionIhave::SieveConditionIhave(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("ihave"), i18n("IHave"), parent)
{
}

QWidget *SieveConditionIhave::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto edit = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(edit);
    connect(edit, &QLineEdit::textChanged, this, &SieveConditionIhave::valueChanged);
    edit->setPlaceholderText(i18n("Use \",\" to separate capabilities"));
    edit->setClearButtonEnabled(true);
    lay->addWidget(edit);
    edit->setObjectName(QStringLiteral("edit"));

    return w;
}

QString SieveConditionIhave::code(QWidget *w) const
{
    const QLineEdit *edit = w->findChild<QLineEdit *>(QStringLiteral("edit"));
    const QString editValue = edit->text();
    return QStringLiteral("ihave %1").arg(AutoCreateScriptUtil::createList(editValue, QLatin1Char(',')))
        + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionIhave::needRequires(QWidget *) const
{
    return QStringList() << QStringLiteral("ihave");
}

bool SieveConditionIhave::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionIhave::serverNeedsCapability() const
{
    return QStringLiteral("ihave");
}

QString SieveConditionIhave::help() const
{
    return i18n("The \"ihave\" test provides a means for Sieve scripts to test for the existence of a given extension prior to actually using it.");
}

void SieveConditionIhave::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1String("str")) {
            const QString tagValue = element.readElementText();
            auto edit = w->findChild<QLineEdit *>(QStringLiteral("edit"));
            edit->setText(tagValue);
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionIhave::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl SieveConditionIhave::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditionihave.cpp"
