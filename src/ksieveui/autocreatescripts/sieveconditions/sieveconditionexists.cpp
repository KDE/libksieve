/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionexists.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "widgets/selectheadertypecombobox.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QXmlStreamReader>
#include "libksieve_debug.h"

using namespace KSieveUi;

SieveConditionExists::SieveConditionExists(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("exists"), i18n("Exists"), parent)
{
}

QWidget *SieveConditionExists::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    QComboBox *combo = new QComboBox;
    combo->setObjectName(QStringLiteral("existscheck"));
    combo->addItem(i18n("exists"), QStringLiteral("exists"));
    combo->addItem(i18n("not exists"), QStringLiteral("not exists"));
    lay->addWidget(combo);
    connect(combo, QOverload<int>::of(&QComboBox::activated), this, &SieveConditionExists::valueChanged);

    QLabel *lab = new QLabel(i18n("headers:"));
    lay->addWidget(lab);

    SelectHeaderTypeComboBox *value = new SelectHeaderTypeComboBox;
    connect(value, &SelectHeaderTypeComboBox::valueChanged, this, &SieveConditionExists::valueChanged);
    value->setObjectName(QStringLiteral("headervalue"));

    lay->addWidget(value);
    return w;
}

QString SieveConditionExists::code(QWidget *w) const
{
    const QComboBox *combo = w->findChild<QComboBox *>(QStringLiteral("existscheck"));
    const QString comparaison = combo->itemData(combo->currentIndex()).toString();

    const SelectHeaderTypeComboBox *value = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headervalue"));
    return QStringLiteral("%1 %2").arg(comparaison, value->code()) + AutoCreateScriptUtil::generateConditionComment(comment());
}

QString SieveConditionExists::help() const
{
    return i18n("The \"exists\" test is true if the headers listed in the header-names argument exist within the message.  All of the headers must exist or the test is false.");
}

void SieveConditionExists::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool notCondition, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (notCondition) {
            QComboBox *combo = w->findChild<QComboBox *>(QStringLiteral("existscheck"));
            combo->setCurrentIndex(1);
        }
        if (tagName == QLatin1String("str")) {
            SelectHeaderTypeComboBox *value = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headervalue"));
            value->setCode(element.readElementText());
        } else if (tagName == QLatin1String("list")) {
            SelectHeaderTypeComboBox *selectHeaderType = w->findChild<SelectHeaderTypeComboBox *>(QStringLiteral("headervalue"));
            selectHeaderType->setCode(AutoCreateScriptUtil::listValueToStr(element));
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionExists::setParamWidgetValue unknown tagName " << tagName;
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
