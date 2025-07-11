/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionbreak.h"
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
SieveActionBreak::SieveActionBreak(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, u"break"_s, i18n("Break"), parent)
{
}

QWidget *SieveActionBreak::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto lab = new QLabel(i18nc("@label:textbox", "Name (optional):"));
    lay->addWidget(lab);

    auto subject = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(subject);
    subject->setObjectName(QLatin1StringView("name"));
    connect(subject, &QLineEdit::textChanged, this, &SieveActionBreak::valueChanged);
    lay->addWidget(subject);
    return w;
}

void SieveActionBreak::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1StringView("name")) {
                auto name = w->findChild<QLineEdit *>(u"name"_s);
                name->setText(AutoCreateScriptUtil::strValue(element));
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveActionBreak::setParamWidgetValue unknown tagValue " << tagValue;
            }
        } else if (tagName == QLatin1StringView("str")) {
            element.skipCurrentElement();
            // Nothing
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << "SieveActionBreak::setParamWidgetValue unknown tag " << tagName;
        }
    }
}

QUrl SieveActionBreak::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

QString SieveActionBreak::code(QWidget *w) const
{
    const QLineEdit *name = w->findChild<QLineEdit *>(u"name"_s);
    const QString nameStr = name->text();
    if (!nameStr.isEmpty()) {
        return u"break :name \"%1\";"_s.arg(nameStr);
    }
    return u"break;"_s;
}

QString SieveActionBreak::help() const
{
    return i18n("The break command terminates the closest enclosing loop.");
}

QStringList SieveActionBreak::needRequires(QWidget * /*parent*/) const
{
    return QStringList() << u"foreverypart"_s;
}

bool SieveActionBreak::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionBreak::serverNeedsCapability() const
{
    return u"foreverypart"_s;
}

#include "moc_sieveactionbreak.cpp"
