/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionsize.h"
#include "widgets/selectsizewidget.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include <KLocalizedString>

#include <QHBoxLayout>
#include <QComboBox>
#include <QXmlStreamReader>
#include "libksieve_debug.h"

using namespace KSieveUi;

SieveConditionSize::SieveConditionSize(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("size"), i18n("Size"), parent)
{
}

QWidget *SieveConditionSize::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    QComboBox *combo = new QComboBox;
    combo->setObjectName(QStringLiteral("combosize"));
    combo->addItem(i18n("under"), QStringLiteral(":under"));
    combo->addItem(i18n("over"), QStringLiteral(":over"));
    lay->addWidget(combo);
    connect(combo, QOverload<int>::of(&QComboBox::activated), this, &SieveConditionSize::valueChanged);

    SelectSizeWidget *sizeWidget = new SelectSizeWidget;
    connect(sizeWidget, &SelectSizeWidget::valueChanged, this, &SieveConditionSize::valueChanged);
    sizeWidget->setObjectName(QStringLiteral("sizewidget"));
    lay->addWidget(sizeWidget);

    return w;
}

QString SieveConditionSize::code(QWidget *w) const
{
    const QComboBox *combo = w->findChild<QComboBox *>(QStringLiteral("combosize"));
    const QString comparaison = combo->itemData(combo->currentIndex()).toString();
    const SelectSizeWidget *sizeWidget = w->findChild<SelectSizeWidget *>(QStringLiteral("sizewidget"));
    return QStringLiteral("size %1 %2").arg(comparaison, sizeWidget->code()) + AutoCreateScriptUtil::generateConditionComment(comment());
}

QString SieveConditionSize::help() const
{
    return i18n("The \"size\" test deals with the size of a message.  It takes either a tagged argument of \":over\" or \":under\", followed by a number representing the size of the message.");
}

void SieveConditionSize::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool /*notCondition*/, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("tag")) {
            const QString tagValue = element.readElementText();
            QComboBox *combo = w->findChild<QComboBox *>(QStringLiteral("combosize"));
            const int index = combo->findData(AutoCreateScriptUtil::tagValue(tagValue));
            if (index != -1) {
                combo->setCurrentIndex(index);
            }
        } else if (tagName == QLatin1String("num")) {
            QString numIdentifier;
            if (element.attributes().hasAttribute(QLatin1String("quantifier"))) {
                numIdentifier = element.attributes().value(QLatin1String("quantifier")).toString();
            }
            const qlonglong tagValue = element.readElementText().toLongLong();
            SelectSizeWidget *sizeWidget = w->findChild<SelectSizeWidget *>(QStringLiteral("sizewidget"));
            sizeWidget->setCode(tagValue, numIdentifier, name(), error);
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionSize::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl SieveConditionSize::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
