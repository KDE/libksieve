/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionihave.h"
using namespace Qt::Literals::StringLiterals;

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
    : SieveCondition(sieveGraphicalModeWidget, u"ihave"_s, i18n("IHave"), parent)
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
    edit->setPlaceholderText(i18nc("@info:placeholder", "Use \",\" to separate capabilities"));
    edit->setClearButtonEnabled(true);
    lay->addWidget(edit);
    edit->setObjectName(QLatin1StringView("edit"));

    return w;
}

QString SieveConditionIhave::code(QWidget *w) const
{
    const QLineEdit *edit = w->findChild<QLineEdit *>(u"edit"_s);
    const QString editValue = edit->text();
    return u"ihave %1"_s.arg(AutoCreateScriptUtil::createList(editValue, u','))
        + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionIhave::needRequires(QWidget *) const
{
    return QStringList() << u"ihave"_s;
}

bool SieveConditionIhave::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionIhave::serverNeedsCapability() const
{
    return u"ihave"_s;
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
        if (tagName == QLatin1StringView("str")) {
            const QString tagValue = element.readElementText();
            auto edit = w->findChild<QLineEdit *>(u"edit"_s);
            edit->setText(tagValue);
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionIhave::setParamWidgetValue unknown tagName " << tagName;
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
