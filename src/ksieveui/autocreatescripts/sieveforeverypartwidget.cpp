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

#include "sieveforeverypartwidget.h"
#include "sievescriptblockwidget.h"
#include "autocreatescriptutil_p.h"
#include "commonwidgets/sievehelpbutton.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>
#include <QLineEdit>

#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QWhatsThis>
#include <QXmlStreamReader>
#include "libksieve_debug.h"

using namespace KSieveUi;

SieveForEveryPartWidget::SieveForEveryPartWidget(QWidget *parent)
    : SieveWidgetPageAbstract(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *lay = new QHBoxLayout;
    mainLayout->addLayout(lay);
    mainLayout->addStretch(1);

    mHelpButton = new SieveHelpButton;
    lay->addWidget(mHelpButton);
    connect(mHelpButton, &SieveHelpButton::clicked, this, &SieveForEveryPartWidget::slotHelp);

    mForLoop = new QCheckBox(i18n("Add ForEveryPart loop"));
    connect(mForLoop, &QCheckBox::toggled, this, &SieveForEveryPartWidget::valueChanged);
    lay->addWidget(mForLoop);

    QLabel *lab = new QLabel(i18n("Name (optional):"));
    lay->addWidget(lab);

    mName = new QLineEdit;
    connect(mName, &QLineEdit::textChanged, this, &SieveForEveryPartWidget::valueChanged);
    mName->setEnabled(false);
    lay->addWidget(mName);

    connect(mForLoop, &QCheckBox::clicked, mName, &QLineEdit::setEnabled);
    setPageType(KSieveUi::SieveScriptBlockWidget::ForEveryPart);
}

SieveForEveryPartWidget::~SieveForEveryPartWidget()
{
}

void SieveForEveryPartWidget::slotHelp()
{
    const QString help = i18n(
        "\"foreverypart\", which is an iterator that walks though every MIME part of a message, including nested parts, depth first, and applies the commands in the specified block to each of them.");
    const QUrl href = KSieveUi::SieveEditorUtil::helpUrl(KSieveUi::SieveEditorUtil::ForEveryPart);
    const QString fullWhatsThis = AutoCreateScriptUtil::createFullWhatsThis(help, href.toString());
    QWhatsThis::showText(QCursor::pos(), fullWhatsThis, mHelpButton);
}

void SieveForEveryPartWidget::generatedScript(QString &script, QStringList &requires, bool inForEveryPartLoop)
{
    Q_UNUSED(inForEveryPartLoop);
    if (mForLoop->isChecked()) {
        requires << QStringLiteral("foreverypart");
        const QString loopName = mName->text();
        if (loopName.isEmpty()) {
            script += QStringLiteral("foreverypart {");
        } else {
            script += QStringLiteral("foreverypart :name \"%1\" {").arg(loopName);
        }
    }
}

void SieveForEveryPartWidget::loadScript(QXmlStreamReader &element, QString &error)
{
#ifdef REMOVE_QDOMELEMENT
    QDomNode node = element.firstChild();
    QDomElement e = node.toElement();
    if (!e.isNull()) {
        const QString tagName = e.tagName();
        if (tagName == QLatin1String("tag")) {
            const QString tagValue = e.text();
            if (tagValue == QLatin1String("name")) {
                mName->setText(AutoCreateScriptUtil::strValue(e));
            } else {
                error += i18n("Unknown tagValue \"%1\" during loading loop \"for\"", tagValue) + QLatin1Char('\n');
                qCDebug(LIBKSIEVE_LOG) << " SieveForEveryPartWidget::loadScript unknown tagValue " << tagValue;
            }
            mForLoop->setChecked(true);
            mName->setEnabled(true);
        } else if (tagName == QLatin1String("block")) {
            //Nothing
            //It's when name is empty
        } else if (tagName == QLatin1String("crlf")) {
            //Nothing
        } else {
            error += i18n("Unknown tag \"%1\" during loading loop \"for\"", tagName) + QLatin1Char('\n');
            qCDebug(LIBKSIEVE_LOG) << " SieveForEveryPartWidget::loadScript unknown tagName " << tagName;
        }
    }
#endif
}
