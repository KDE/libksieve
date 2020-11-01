/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionbreak.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>
#include <QLineEdit>

#include <QHBoxLayout>
#include <QLabel>
#include <QXmlStreamReader>
#include "libksieve_debug.h"

using namespace KSieveUi;
SieveActionBreak::SieveActionBreak(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, QStringLiteral("break"), i18n("Break"), parent)
{
}

QWidget *SieveActionBreak::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    auto *lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    QLabel *lab = new QLabel(i18n("Name (optional):"));
    lay->addWidget(lab);

    auto *subject = new QLineEdit;
    subject->setObjectName(QStringLiteral("name"));
    connect(subject, &QLineEdit::textChanged, this, &SieveActionBreak::valueChanged);
    lay->addWidget(subject);
    return w;
}

void SieveActionBreak::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1String("name")) {
                auto *name = w->findChild<QLineEdit *>(QStringLiteral("name"));
                name->setText(AutoCreateScriptUtil::strValue(element));
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveActionBreak::setParamWidgetValue unknown tagValue " << tagValue;
            }
        } else if (tagName == QLatin1String("str")) {
            element.skipCurrentElement();
            //Nothing
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            element.skipCurrentElement();
            //implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << "SieveActionBreak::setParamWidgetValue unknown tag " << tagName;
        }
    }
}

QUrl SieveActionBreak::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

QString SieveActionBreak::code(QWidget *w) const
{
    const QLineEdit *name = w->findChild<QLineEdit *>(QStringLiteral("name"));
    const QString nameStr = name->text();
    if (!nameStr.isEmpty()) {
        return QStringLiteral("break :name \"%1\";").arg(nameStr);
    }
    return QStringLiteral("break;");
}

QString SieveActionBreak::help() const
{
    return i18n("The break command terminates the closest enclosing loop.");
}

QStringList SieveActionBreak::needRequires(QWidget */*parent*/) const
{
    return QStringList() << QStringLiteral("foreverypart");
}

bool SieveActionBreak::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionBreak::serverNeedsCapability() const
{
    return QStringLiteral("foreverypart");
}
