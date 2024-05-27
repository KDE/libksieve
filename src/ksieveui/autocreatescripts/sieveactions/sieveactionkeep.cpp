/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionkeep.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectflagswidget.h"

#include "libksieveui_debug.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveActionKeep::SieveActionKeep(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, QStringLiteral("keep"), i18n("Keep"), parent)
{
    mHasImapFlag4Support = sieveCapabilities().contains(QLatin1StringView("imap4flags"));
    mHasFlagSupport = sieveCapabilities().contains(QLatin1StringView("imapflags")) || mHasImapFlag4Support;
}

QString SieveActionKeep::code(QWidget *w) const
{
    if (mHasFlagSupport) {
        const SelectFlagsWidget *flagsWidget = w->findChild<SelectFlagsWidget *>(QStringLiteral("flagswidget"));
        const QString flagCode = flagsWidget->code();
        if (flagCode.isEmpty()) {
            return QStringLiteral("keep;");
        } else {
            return QStringLiteral("keep :flags") + QLatin1Char(' ') + flagCode;
        }
    } else {
        return QStringLiteral("keep;");
    }
}

QString SieveActionKeep::help() const
{
    return i18n(
        "The \"keep\" action is whatever action is taken in lieu of all other actions, if no filtering happens at all; generally, this simply means to file "
        "the message into the user's main mailbox.");
}

QWidget *SieveActionKeep::createParamWidget(QWidget *parent) const
{
    if (mHasFlagSupport) {
        auto w = new QWidget(parent);
        auto lay = new QHBoxLayout;
        lay->setContentsMargins({});
        w->setLayout(lay);
        auto addFlags = new QLabel(i18nc("@label:textbox", "Add flags:"));
        lay->addWidget(addFlags);

        auto flagsWidget = new SelectFlagsWidget;
        connect(flagsWidget, &SelectFlagsWidget::valueChanged, this, &SieveActionKeep::valueChanged);
        flagsWidget->setObjectName(QLatin1StringView("flagswidget"));
        lay->addWidget(flagsWidget);
        return w;
    } else {
        return nullptr;
    }
}

void SieveActionKeep::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    if (mHasFlagSupport) {
        while (element.readNextStartElement()) {
            const QStringView tagName = element.name();
            if (tagName == QLatin1StringView("list")) {
                auto flagsWidget = w->findChild<SelectFlagsWidget *>(QStringLiteral("flagswidget"));
                flagsWidget->setFlags(AutoCreateScriptUtil::listValue(element));
            } else if (tagName == QLatin1StringView("str")) {
                auto flagsWidget = w->findChild<SelectFlagsWidget *>(QStringLiteral("flagswidget"));
                flagsWidget->setFlags(QStringList() << element.readElementText());
            } else if (tagName == QLatin1StringView("tag") && element.readElementText() == QLatin1StringView("flags")) {
                // nothing :)
                // Don't skip here.
            } else if (tagName == QLatin1StringView("crlf")) {
                element.skipCurrentElement();
                // nothing
            } else if (tagName == QLatin1StringView("comment")) {
                element.skipCurrentElement();
                // implement in the future ?
            } else {
                unknownTag(tagName, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveActionAbstractFlags::setParamWidgetValue unknown tag :" << tagName;
            }
        }
    } else {
        qCDebug(LIBKSIEVEUI_LOG) << " Server doesn't support imapflags";
    }
}

QStringList SieveActionKeep::needRequires(QWidget *) const
{
    QStringList requiresLst;
    if (mHasImapFlag4Support) {
        requiresLst << QStringLiteral("imap4flags");
    } else if (mHasFlagSupport) {
        requiresLst << QStringLiteral("imapflags");
    }
    return requiresLst;
}

QUrl SieveActionKeep::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveactionkeep.cpp"
