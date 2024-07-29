/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveforeverypartwidget.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescriptutil_p.h"
#include "commonwidgets/sievehelpbutton.h"
#include "editor/sieveeditorutil.h"
#include "sievescriptblockwidget.h"

#include "widgets/lineeditvalidator.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QWhatsThis>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveForEveryPartWidget::SieveForEveryPartWidget(QWidget *parent)
    : SieveWidgetPageAbstract(parent)
    , mForLoop(new QCheckBox(i18nc("@option:check", "Add ForEveryPart loop"), this))
    , mName(new LineEditValidator(this))
    , mHelpButton(new SieveHelpButton(this))
{
    auto mainLayout = new QVBoxLayout(this);
    auto lay = new QHBoxLayout;
    mainLayout->addLayout(lay);
    mainLayout->addStretch(1);

    lay->addWidget(mHelpButton);
    connect(mHelpButton, &SieveHelpButton::clicked, this, &SieveForEveryPartWidget::slotHelp);

    connect(mForLoop, &QCheckBox::toggled, this, &SieveForEveryPartWidget::valueChanged);
    lay->addWidget(mForLoop);

    auto lab = new QLabel(i18nc("@label:textbox", "Name (optional):"), this);
    lay->addWidget(lab);

    KLineEditEventHandler::catchReturnKey(mName);
    connect(mName, &QLineEdit::textChanged, this, &SieveForEveryPartWidget::valueChanged);
    mName->setEnabled(false);
    lay->addWidget(mName);

    connect(mForLoop, &QCheckBox::clicked, mName, &QLineEdit::setEnabled);
    setPageType(KSieveUi::SieveScriptBlockWidget::ForEveryPart);
}

SieveForEveryPartWidget::~SieveForEveryPartWidget() = default;

void SieveForEveryPartWidget::slotHelp()
{
    const QString help = i18n(
        "\"foreverypart\", which is an iterator that walks though every MIME part of a message, including nested parts, depth first, and applies the commands "
        "in the specified block to each of them.");
    const QUrl href = KSieveUi::SieveEditorUtil::helpUrl(KSieveUi::SieveEditorUtil::ForEveryPart);
    const QString fullWhatsThis = AutoCreateScriptUtil::createFullWhatsThis(help, href.toString());
    QWhatsThis::showText(QCursor::pos(), fullWhatsThis, mHelpButton);
}

void SieveForEveryPartWidget::generatedScript(QString &script, QStringList &required, bool inForEveryPartLoop)
{
    Q_UNUSED(inForEveryPartLoop)
    if (mForLoop->isChecked()) {
        required << QStringLiteral("foreverypart");
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
    if (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1StringView("name")) {
                mName->setText(AutoCreateScriptUtil::strValue(element));
            } else {
                error += i18n("Unknown tagValue \"%1\" during loading loop \"for\"", tagValue) + QLatin1Char('\n');
                qCDebug(LIBKSIEVEUI_LOG) << " SieveForEveryPartWidget::loadScript unknown tagValue " << tagValue;
            }
            mForLoop->setChecked(true);
            mName->setEnabled(true);
        } else if (tagName == QLatin1StringView("block")) {
            // Nothing
            // It's when name is empty
        } else if (tagName == QLatin1StringView("crlf")) {
            // Nothing
            element.skipCurrentElement();
        } else {
            error += i18n("Unknown tag \"%1\" during loading loop \"for\"", tagName.toString()) + QLatin1Char('\n');
            qCDebug(LIBKSIEVEUI_LOG) << " SieveForEveryPartWidget::loadScript unknown tagName " << tagName;
        }
    } else {
        qCDebug(LIBKSIEVEUI_LOG) << " SieveForEveryPartWidget::loadScript problem during loading";
    }
}

#include "moc_sieveforeverypartwidget.cpp"
