/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionabstractflags.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "widgets/selectflagswidget.h"

#include "libksieveui_debug.h"
#include <QHBoxLayout>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveActionAbstractFlags::SieveActionAbstractFlags(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget,
                                                   const QString &name,
                                                   const QString &label,
                                                   QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, name, label, parent)
{
}

QWidget *SieveActionAbstractFlags::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);
    auto flagsWidget = new SelectFlagsWidget;
    connect(flagsWidget, &SelectFlagsWidget::valueChanged, this, &SieveActionAbstractFlags::valueChanged);
    flagsWidget->setObjectName(QStringLiteral("flagswidget"));
    lay->addWidget(flagsWidget);
    return w;
}

void SieveActionAbstractFlags::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1String("list")) {
            auto flagsWidget = w->findChild<SelectFlagsWidget *>(QStringLiteral("flagswidget"));
            flagsWidget->setFlags(AutoCreateScriptUtil::listValue(element));
        } else if (tagName == QLatin1String("str")) {
            auto flagsWidget = w->findChild<SelectFlagsWidget *>(QStringLiteral("flagswidget"));
            flagsWidget->setFlags(QStringList() << element.readElementText());
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveActionAbstractFlags::setParamWidgetValue unknown tag :" << tagName;
        }
    }
}

QString SieveActionAbstractFlags::code(QWidget *w) const
{
    const SelectFlagsWidget *flagsWidget = w->findChild<SelectFlagsWidget *>(QStringLiteral("flagswidget"));
    const QString flagCode = flagsWidget->code();
    const QString str = flagsCode();
    return str + QLatin1Char(' ') + (flagCode.isEmpty() ? QStringLiteral(";") : flagCode);
}

QStringList SieveActionAbstractFlags::needRequires(QWidget *) const
{
    if (sieveCapabilities().contains(QLatin1String("imap4flags"))) {
        return QStringList() << QStringLiteral("imap4flags");
    } else {
        return QStringList() << QStringLiteral("imapflags");
    }
}

bool SieveActionAbstractFlags::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionAbstractFlags::serverNeedsCapability() const
{
    if (sieveCapabilities().contains(QLatin1String("imap4flags"))) {
        return QStringLiteral("imap4flags");
    } else {
        return QStringLiteral("imapflags");
    }
}

#include "moc_sieveactionabstractflags.cpp"
