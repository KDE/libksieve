/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionreject.h"
#include "editor/sieveeditorutil.h"
#include "widgets/multilineedit.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>

#include <QLabel>
#include <QHBoxLayout>
#include <QXmlStreamReader>
#include "libksieve_debug.h"

using namespace KSieveUi;
SieveActionReject::SieveActionReject(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, QStringLiteral("reject"), i18n("Reject"), parent)
{
}

QWidget *SieveActionReject::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setContentsMargins(0, 0, 0, 0);
    w->setLayout(lay);
    QLabel *lab = new QLabel(i18n("text:"));
    lay->addWidget(lab);

    MultiLineEdit *edit = new MultiLineEdit;
    connect(edit, &MultiLineEdit::textChanged, this, &SieveActionReject::valueChanged);
    edit->setObjectName(QStringLiteral("rejectmessage"));
    lay->addWidget(edit);
    return w;
}

void SieveActionReject::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("str")) {
            const QString tagValue = element.readElementText();
            MultiLineEdit *edit = w->findChild<MultiLineEdit *>(QStringLiteral("rejectmessage"));
            edit->setPlainText(tagValue);
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            element.skipCurrentElement();
            //implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveActionReject::setParamWidgetValue unknown tagName " << tagName;
        }
    }
}

QString SieveActionReject::code(QWidget *w) const
{
    const MultiLineEdit *edit = w->findChild<MultiLineEdit *>(QStringLiteral("rejectmessage"));
    const QString text = edit->toPlainText();

    return QStringLiteral("reject text:%1").arg(AutoCreateScriptUtil::createMultiLine(text));
}

QStringList SieveActionReject::needRequires(QWidget *) const
{
    return QStringList() << QStringLiteral("reject");
}

QString SieveActionReject::serverNeedsCapability() const
{
    return QStringLiteral("reject");
}

bool SieveActionReject::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionReject::help() const
{
    return i18n(" The \"reject\" action cancels the implicit keep and refuses delivery of a message.");
}

QUrl SieveActionReject::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
