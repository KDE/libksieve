/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionereject.h"
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
    : SieveAction(sieveGraphicalModeWidget, QStringLiteral("ereject"), i18n("E-Reject"), parent)
{
}

QWidget *SieveActionEReject::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);
    auto lab = new QLabel(i18n("text:"));
    lay->addWidget(lab);

    auto edit = new MultiLineEdit;
    connect(edit, &MultiLineEdit::textChanged, this, &SieveActionEReject::valueChanged);
    edit->setObjectName(QStringLiteral("rejectmessage"));
    lay->addWidget(edit);
    return w;
}

void SieveActionEReject::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1String("str")) {
            const QString tagValue = element.readElementText();
            auto edit = w->findChild<MultiLineEdit *>(QStringLiteral("rejectmessage"));
            edit->setPlainText(tagValue);
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
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
    const MultiLineEdit *edit = w->findChild<MultiLineEdit *>(QStringLiteral("rejectmessage"));
    const QString text = edit->toPlainText();

    return QStringLiteral("ereject text:%1").arg(AutoCreateScriptUtil::createMultiLine(text));
}

QStringList SieveActionEReject::needRequires(QWidget *) const
{
    return QStringList() << QStringLiteral("ereject");
}

QString SieveActionEReject::serverNeedsCapability() const
{
    return QStringLiteral("ereject");
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
