/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionextracttext.h"
#include "editor/sieveeditorutil.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveActionExtractText::SieveActionExtractText(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, QStringLiteral("extracttext"), i18n("Extract Text"), parent)
{
}

QWidget *SieveActionExtractText::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    w->setLayout(grid);

    auto lab = new QLabel(i18n("Number of characters:"));
    grid->addWidget(lab, 0, 0);

    auto nbCharacters = new QSpinBox;
    nbCharacters->setMinimum(1);
    nbCharacters->setMaximum(99999);
    nbCharacters->setObjectName(QStringLiteral("numberOfCharacters"));
    grid->addWidget(nbCharacters, 0, 1);
    connect(nbCharacters, &QSpinBox::valueChanged, this, &SieveActionExtractText::valueChanged);

    lab = new QLabel(i18n("Stored in variable name:"));
    grid->addWidget(lab, 1, 0);

    auto variableName = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(variableName);
    connect(variableName, &QLineEdit::textChanged, this, &SieveActionExtractText::valueChanged);
    variableName->setObjectName(QStringLiteral("variablename"));
    grid->addWidget(variableName, 1, 1);

    return w;
}

void SieveActionExtractText::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();

        if (tagName == QLatin1String("tag")) {
            element.skipCurrentElement();
            // TODO ?
        } else if (tagName == QLatin1String("num")) {
            auto numberOfCharacters = w->findChild<QSpinBox *>(QStringLiteral("numberOfCharacters"));
            numberOfCharacters->setValue(element.readElementText().toInt());
        } else if (tagName == QLatin1String("str")) {
            auto variableName = w->findChild<QLineEdit *>(QStringLiteral("variablename"));
            variableName->setText(element.readElementText());
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveActionExtractText::setParamWidgetValue unknown tagName " << tagName;
        }
    }
}

QString SieveActionExtractText::code(QWidget *w) const
{
    const QSpinBox *numberOfCharacters = w->findChild<QSpinBox *>(QStringLiteral("numberOfCharacters"));
    const QString numberOfCharactersStr = QString::number(numberOfCharacters->value());

    const QLineEdit *variableName = w->findChild<QLineEdit *>(QStringLiteral("variablename"));
    const QString variableNameStr = variableName->text();

    const QString result = QStringLiteral("extracttext :first %1 \"%2\";").arg(numberOfCharactersStr, variableNameStr);
    return result;
}

QStringList SieveActionExtractText::needRequires(QWidget * /*parent*/) const
{
    return QStringList() << QStringLiteral("extracttext");
}

bool SieveActionExtractText::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionExtractText::serverNeedsCapability() const
{
    return QStringLiteral("extracttext");
}

QString SieveActionExtractText::help() const
{
    return i18n("The \"extracttext\" action may be used within the context of a \"foreverypart\" loop and is used to store text into a variable");
}

QUrl SieveActionExtractText::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveactionextracttext.cpp"
