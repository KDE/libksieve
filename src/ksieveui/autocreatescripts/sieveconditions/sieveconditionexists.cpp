/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionexists.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectheadertypecombobox.h"

#include <KLocalizedString>

#include "libksieveui_debug.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveConditionExists::SieveConditionExists(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("exists"), i18n("Exists"), parent)
{
}

QWidget *SieveConditionExists::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto combo = new QComboBox;
    combo->setObjectName(QStringLiteral("existscheck"));
    combo->addItem(i18n("exists"), QStringLiteral("exists"));
    combo->addItem(i18n("not exists"), QStringLiteral("not exists"));
    lay->addWidget(combo);
    connect(combo, &QComboBox::activated, this, &SieveConditionExists::valueChanged);

    auto lab = new QLabel(i18n("headers:"));
    lay->addWidget(lab);

    auto value = new SelectHeaderTypeComboBox;
    connect(value, &SelectHeaderTypeComboBox::valueChanged, this, &SieveConditionExists::valueChanged);
    value->setObjectName(QStringLiteral("headervalue"));

    lay->addWidget(value);
    return w;
}

QString SieveConditionExists::code(QWidget *w) const
{
    const QComboBox *combo = w->findChild<QComboBox *>(QStringLiteral("existscheck"));
    const QString comparison = combo->itemData(combo->currentIndex()).toString();

    const SelectHeaderTypeComboBox *value = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headervalue"));
    return QStringLiteral("%1 %2").arg(comparison, value->code()) + AutoCreateScriptUtil::generateConditionComment(comment());
}

QString SieveConditionExists::help() const
{
    return i18n(
        "The \"exists\" test is true if the headers listed in the header-names argument exist within the message.  All of the headers must exist or the test "
        "is false.");
}

void SieveConditionExists::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool notCondition, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (notCondition) {
            auto combo = w->findChild<QComboBox *>(QStringLiteral("existscheck"));
            combo->setCurrentIndex(1);
        }
        if (tagName == QLatin1String("str")) {
            auto value = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headervalue"));
            value->setCode(element.readElementText());
        } else if (tagName == QLatin1String("list")) {
            auto selectHeaderType = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headervalue"));
            selectHeaderType->setCode(AutoCreateScriptUtil::listValueToStr(element));
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionExists::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl SieveConditionExists::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditionexists.cpp"
