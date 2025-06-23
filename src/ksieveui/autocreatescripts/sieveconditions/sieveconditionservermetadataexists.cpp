/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionservermetadataexists.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveConditionServerMetaDataExists::SieveConditionServerMetaDataExists(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, u"servermetadataexists"_s, i18n("Server Meta Data Exists"), parent)
{
}

QWidget *SieveConditionServerMetaDataExists::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto lab = new QLabel(i18nc("@label:textbox", "Annotation:"));
    lay->addWidget(lab);

    auto value = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(value);
    value->setObjectName(QLatin1StringView("value"));
    connect(value, &QLineEdit::textChanged, this, &SieveConditionServerMetaDataExists::valueChanged);
    lay->addWidget(value);

    return w;
}

QString SieveConditionServerMetaDataExists::code(QWidget *w) const
{
    const QLineEdit *value = w->findChild<QLineEdit *>(u"value"_s);
    const QString valueStr = value->text();
    return u"servermetadataexists \"%1\""_s.arg(valueStr) + AutoCreateScriptUtil::generateConditionComment(comment());
}

QStringList SieveConditionServerMetaDataExists::needRequires(QWidget *) const
{
    return QStringList() << u"servermetadata"_s;
}

bool SieveConditionServerMetaDataExists::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionServerMetaDataExists::serverNeedsCapability() const
{
    return u"servermetadata"_s;
}

QString SieveConditionServerMetaDataExists::help() const
{
    return i18n("The \"servermetadataexists\" test is true if all of the server annotations listed in the \"annotation-names\" argument exist.");
}

void SieveConditionServerMetaDataExists::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool /*notCondition*/, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("str")) {
            const QString tagValue = element.readElementText();
            auto value = w->findChild<QLineEdit *>(u"value"_s);
            value->setText(tagValue);
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionServerMetaDataExists::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl SieveConditionServerMetaDataExists::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditionservermetadataexists.cpp"
