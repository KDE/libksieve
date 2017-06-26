/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "sieveactionkeep.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectflagswidget.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"

#include <KLocalizedString>
#include <QLabel>
#include <QHBoxLayout>
#include <QXmlStreamReader>
#include "libksieve_debug.h"

using namespace KSieveUi;
SieveActionKeep::SieveActionKeep(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, QStringLiteral("keep"), i18n("Keep"), parent)
{
    mHasImapFlag4Support = sieveCapabilities().contains(QStringLiteral("imap4flags"));
    mHasFlagSupport = sieveCapabilities().contains(QStringLiteral("imapflags")) || mHasImapFlag4Support;
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
        "The \"keep\" action is whatever action is taken in lieu of all other actions, if no filtering happens at all; generally, this simply means to file the message into the user's main mailbox.");
}

QWidget *SieveActionKeep::createParamWidget(QWidget *parent) const
{
    if (mHasFlagSupport) {
        QWidget *w = new QWidget(parent);
        QHBoxLayout *lay = new QHBoxLayout;
        lay->setMargin(0);
        w->setLayout(lay);
        QLabel *addFlags = new QLabel(i18n("Add flags:"));
        lay->addWidget(addFlags);

        SelectFlagsWidget *flagsWidget = new SelectFlagsWidget;
        connect(flagsWidget, &SelectFlagsWidget::valueChanged, this, &SieveActionKeep::valueChanged);
        flagsWidget->setObjectName(QStringLiteral("flagswidget"));
        lay->addWidget(flagsWidget);
        return w;
    } else {
        return nullptr;
    }
}

bool SieveActionKeep::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    if (mHasFlagSupport) {
        while (element.readNextStartElement()) {
            const QStringRef tagName = element.name();
            if (tagName == QLatin1String("list")) {
                SelectFlagsWidget *flagsWidget = w->findChild<SelectFlagsWidget *>(QStringLiteral("flagswidget"));
                flagsWidget->setFlags(AutoCreateScriptUtil::listValue(element));
            } else if (tagName == QLatin1String("str")) {
                SelectFlagsWidget *flagsWidget = w->findChild<SelectFlagsWidget *>(QStringLiteral("flagswidget"));
                flagsWidget->setFlags(QStringList() << element.readElementText());
            } else if (tagName == QLatin1String("tag") && element.readElementText() == QLatin1String("flags")) {
                //nothing :)
                //Don't skip here.
            } else if (tagName == QLatin1String("crlf")) {
                element.skipCurrentElement();
                //nothing
            } else if (tagName == QLatin1String("comment")) {
                element.skipCurrentElement();
                //implement in the future ?
            } else {
                unknownTag(tagName, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveActionAbstractFlags::setParamWidgetValue unknown tag :" << tagName;
            }
        }
    } else {
        qCDebug(LIBKSIEVE_LOG) << " Server doesn't support imapflags";
    }

    return true;
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
