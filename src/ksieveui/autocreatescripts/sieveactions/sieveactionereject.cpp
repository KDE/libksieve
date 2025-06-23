/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionereject.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include "widgets/multilineedit.h"

#include <KLocalizedString>

#include "libksieveui_debug.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveActionEReject::SieveActionEReject(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, u"ereject"_s, i18n("E-Reject"), parent)
{
}

QWidget *SieveActionEReject::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);
    auto lab = new QLabel(i18nc("@label:textbox", "text:"));
    lay->addWidget(lab);

    auto edit = new MultiLineEdit;
    connect(edit, &MultiLineEdit::textChanged, this, &SieveActionEReject::valueChanged);
    edit->setObjectName(QLatin1StringView("rejectmessage"));
    lay->addWidget(edit);
    return w;
}

void SieveActionEReject::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("str")) {
            const QString tagValue = element.readElementText();
            auto edit = w->findChild<MultiLineEdit *>(u"rejectmessage"_s);
            edit->setPlainText(tagValue);
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveActionEReject::setParamWidgetValue unknown tagName " << tagName;
        }
    }
}

QString SieveActionEReject::code(QWidget *w) const
{
    const MultiLineEdit *edit = w->findChild<MultiLineEdit *>(u"rejectmessage"_s);
    const QString text = edit->toPlainText();

    return u"ereject text:%1"_s.arg(AutoCreateScriptUtil::createMultiLine(text));
}

QStringList SieveActionEReject::needRequires(QWidget *) const
{
    return QStringList() << u"ereject"_s;
}

QString SieveActionEReject::serverNeedsCapability() const
{
    return u"ereject"_s;
}

bool SieveActionEReject::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionEReject::help() const
{
    return i18n(" The \"ereject\" action cancels the implicit keep and refuses delivery of a message.");
}

QUrl SieveActionEReject::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveactionereject.cpp"
