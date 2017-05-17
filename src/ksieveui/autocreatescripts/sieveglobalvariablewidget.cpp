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

#include "sieveglobalvariablewidget.h"
#include "sievescriptblockwidget.h"
#include "autocreatescriptutil_p.h"
#include "commonwidgets/sievehelpbutton.h"
#include "editor/sieveeditorutil.h"

#include <QPushButton>
#include <KLocalizedString>
#include <QLineEdit>
#include <QIcon>

#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QWhatsThis>
#include "libksieve_debug.h"
#include <QDomNode>

using namespace KSieveUi;
static const int MINIMUMGLOBALVARIABLEACTION = 1;
static const int MAXIMUMGLOBALVARIABLEACTION = 15;

SieveGlobalVariableActionWidget::SieveGlobalVariableActionWidget(QWidget *parent)
    : QWidget(parent)
{
    initWidget();
}

SieveGlobalVariableActionWidget::~SieveGlobalVariableActionWidget()
{
}

void SieveGlobalVariableActionWidget::generatedScript(QString &script)
{
    const QString variableName = mVariableName->text();
    if (variableName.trimmed().isEmpty()) {
        return;
    }
    script += QLatin1String("global ");
    script += QStringLiteral("\"%1\";\n").arg(variableName);
    if (mSetValueTo->isChecked() && !mVariableValue->text().isEmpty()) {
        script += QStringLiteral("set \"%1\" \"%2\";\n").arg(variableName, mVariableValue->text());
    }
}

void SieveGlobalVariableActionWidget::initWidget()
{
    mLayout = new QGridLayout(this);
    mLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *lab = new QLabel(i18n("Variable name:"));
    mLayout->addWidget(lab, 1, 0);

    mVariableName = new QLineEdit;
    connect(mVariableName, &QLineEdit::textChanged, this, &SieveGlobalVariableActionWidget::valueChanged);
    mLayout->addWidget(mVariableName, 1, 1);

    mSetValueTo = new QCheckBox(i18n("Set value to:"));
    connect(mSetValueTo, &QCheckBox::toggled, this, &SieveGlobalVariableActionWidget::valueChanged);
    mLayout->addWidget(mSetValueTo, 1, 2);
    mSetValueTo->setChecked(false);

    mVariableValue = new QLineEdit;
    connect(mVariableValue, &QLineEdit::textChanged, this, &SieveGlobalVariableActionWidget::valueChanged);
    mVariableValue->setEnabled(false);
    mLayout->addWidget(mVariableValue, 1, 3);

    connect(mSetValueTo, &QCheckBox::clicked, mVariableValue, &QLineEdit::setEnabled);

    mAdd = new QPushButton(this);
    mAdd->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    mAdd->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    mRemove = new QPushButton(this);
    mRemove->setIcon(QIcon::fromTheme(QStringLiteral("list-remove")));
    mRemove->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    mLayout->addWidget(mAdd, 1, 4);
    mLayout->addWidget(mRemove, 1, 5);

    connect(mAdd, &QPushButton::clicked, this, &SieveGlobalVariableActionWidget::slotAddWidget);
    connect(mRemove, &QPushButton::clicked, this, &SieveGlobalVariableActionWidget::slotRemoveWidget);
}

bool SieveGlobalVariableActionWidget::isInitialized() const
{
    return !mVariableName->text().isEmpty();
}

QString SieveGlobalVariableActionWidget::variableName() const
{
    const QString varName = mVariableName->text();
    if (varName.trimmed().isEmpty()) {
        return {};
    }
    return varName;
}

void SieveGlobalVariableActionWidget::setVariableValue(const QString &name)
{
    mSetValueTo->setChecked(true);
    mVariableValue->setText(name);
    mVariableValue->setEnabled(true);
}

void SieveGlobalVariableActionWidget::loadScript(const QDomElement &element, QString &error)
{
    QDomNode node = element.firstChild();
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        if (!e.isNull()) {
            const QString tagName = e.tagName();
            if (tagName == QLatin1String("str")) {
                mVariableName->setText(e.text());
            } else {
                error += i18n("Unknown tag \"%1\" during loading of variables.") + QLatin1Char('\n');
                qCDebug(LIBKSIEVE_LOG) << " SieveGlobalVariableActionWidget::loadScript unknown tagName " << tagName;
            }
        }
        node = node.nextSibling();
    }
}

void SieveGlobalVariableActionWidget::slotAddWidget()
{
    Q_EMIT addWidget(this);
    Q_EMIT valueChanged();
}

void SieveGlobalVariableActionWidget::slotRemoveWidget()
{
    Q_EMIT removeWidget(this);
    Q_EMIT valueChanged();
}

void SieveGlobalVariableActionWidget::updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled)
{
    mAdd->setEnabled(addButtonEnabled);
    mRemove->setEnabled(removeButtonEnabled);
}

SieveGlobalVariableWidget::SieveGlobalVariableWidget(QWidget *parent)
    : SieveWidgetPageAbstract(parent)
{
    QVBoxLayout *lay = new QVBoxLayout(this);
    mHelpButton = new SieveHelpButton(this);
    lay->addWidget(mHelpButton);
    connect(mHelpButton, &SieveHelpButton::clicked, this, &SieveGlobalVariableWidget::slotHelp);

    mIncludeLister = new SieveGlobalVariableLister(this);
    connect(mIncludeLister, &SieveGlobalVariableLister::valueChanged, this, &SieveGlobalVariableWidget::valueChanged);
    lay->addWidget(mIncludeLister, 0, Qt::AlignTop);
    setPageType(KSieveUi::SieveScriptBlockWidget::GlobalVariable);
}

SieveGlobalVariableWidget::~SieveGlobalVariableWidget()
{
}

void SieveGlobalVariableWidget::slotHelp()
{
    const QString help = i18n(
        "A variable has global scope in all scripts that have declared it with the \"global\" command.  If a script uses that variable name without declaring it global, the name specifies a separate, non-global variable within that script.");
    const QUrl href = KSieveUi::SieveEditorUtil::helpUrl(KSieveUi::SieveEditorUtil::GlobalVariable);
    const QString fullWhatsThis = AutoCreateScriptUtil::createFullWhatsThis(help, href.toString());
    QWhatsThis::showText(QCursor::pos(), fullWhatsThis, mHelpButton);
}

void SieveGlobalVariableWidget::generatedScript(QString &script, QStringList &requires, bool inForEveryPartLoop)
{
    Q_UNUSED(inForEveryPartLoop)
    QString result;
    QStringList lst;
    mIncludeLister->generatedScript(result, lst);
    if (!result.isEmpty()) {
        script += result;
        requires << lst;
    }
}

void SieveGlobalVariableWidget::loadScript(const QDomElement &element, QString &error)
{
    mIncludeLister->loadScript(element, error);
}

bool SieveGlobalVariableWidget::loadSetVariable(const QDomElement &element, QString &error)
{
    return mIncludeLister->loadSetVariable(element, error);
}

SieveGlobalVariableLister::SieveGlobalVariableLister(QWidget *parent)
    : KPIM::KWidgetLister(false, MINIMUMGLOBALVARIABLEACTION, MAXIMUMGLOBALVARIABLEACTION, parent)
{
    slotClear();
    updateAddRemoveButton();
}

SieveGlobalVariableLister::~SieveGlobalVariableLister()
{
}

void SieveGlobalVariableLister::slotAddWidget(QWidget *w)
{
    addWidgetAfterThisWidget(w);
    updateAddRemoveButton();
}

void SieveGlobalVariableLister::slotRemoveWidget(QWidget *w)
{
    removeWidget(w);
    updateAddRemoveButton();
}

void SieveGlobalVariableLister::updateAddRemoveButton()
{
    QList<QWidget *> widgetList = widgets();
    const int numberOfWidget(widgetList.count());
    bool addButtonEnabled = false;
    bool removeButtonEnabled = false;
    if (numberOfWidget <= widgetsMinimum()) {
        addButtonEnabled = true;
        removeButtonEnabled = false;
    } else if (numberOfWidget >= widgetsMaximum()) {
        addButtonEnabled = false;
        removeButtonEnabled = true;
    } else {
        addButtonEnabled = true;
        removeButtonEnabled = true;
    }
    QList<QWidget *>::ConstIterator wIt = widgetList.constBegin();
    QList<QWidget *>::ConstIterator wEnd = widgetList.constEnd();
    for (; wIt != wEnd; ++wIt) {
        SieveGlobalVariableActionWidget *w = qobject_cast<SieveGlobalVariableActionWidget *>(*wIt);
        w->updateAddRemoveButton(addButtonEnabled, removeButtonEnabled);
    }
}

void SieveGlobalVariableLister::generatedScript(QString &script, QStringList &requires)
{
    requires << QStringLiteral("include");
    const QList<QWidget *> widgetList = widgets();
    QList<QWidget *>::ConstIterator wIt = widgetList.constBegin();
    QList<QWidget *>::ConstIterator wEnd = widgetList.constEnd();
    for (; wIt != wEnd; ++wIt) {
        SieveGlobalVariableActionWidget *w = qobject_cast<SieveGlobalVariableActionWidget *>(*wIt);
        w->generatedScript(script);
    }
}

void SieveGlobalVariableLister::reconnectWidget(SieveGlobalVariableActionWidget *w)
{
    connect(w, &SieveGlobalVariableActionWidget::addWidget, this, &SieveGlobalVariableLister::slotAddWidget, Qt::UniqueConnection);
    connect(w, &SieveGlobalVariableActionWidget::removeWidget, this, &SieveGlobalVariableLister::slotRemoveWidget, Qt::UniqueConnection);
    connect(w, &SieveGlobalVariableActionWidget::valueChanged, this, &SieveGlobalVariableLister::valueChanged, Qt::UniqueConnection);
}

void SieveGlobalVariableLister::clearWidget(QWidget *aWidget)
{
    Q_UNUSED(aWidget);
    //TODO
    Q_EMIT valueChanged();
}

QWidget *SieveGlobalVariableLister::createWidget(QWidget *parent)
{
    SieveGlobalVariableActionWidget *w = new SieveGlobalVariableActionWidget(parent);
    reconnectWidget(w);
    return w;
}

void SieveGlobalVariableLister::loadScript(const QDomElement &element, QString &error)
{
    SieveGlobalVariableActionWidget *w = static_cast<SieveGlobalVariableActionWidget *>(widgets().constLast());
    if (w->isInitialized()) {
        addWidgetAfterThisWidget(widgets().constLast());
        w = static_cast<SieveGlobalVariableActionWidget *>(widgets().constLast());
    }
    w->loadScript(element, error);
}

bool SieveGlobalVariableLister::loadSetVariable(const QDomElement &element, QString & /*error*/)
{
    QString variableName;
    QString variableValue;
    int index = 0;
    QDomNode node = element.firstChild();
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        if (!e.isNull()) {
            const QString tagName = e.tagName();
            if (tagName == QLatin1String("str")) {
                if (index == 0) {
                    variableName = e.text();
                } else if (index == 1) {
                    variableValue = e.text();
                } else {
                    qCDebug(LIBKSIEVE_LOG) << " SieveGlobalVariableLister::loadSetVariable too many argument:" << index;
                }
                ++index;
            } else {
                qCDebug(LIBKSIEVE_LOG) << " SieveGlobalVariableLister::loadSetVariable unknown tagName " << tagName;
            }
        }
        node = node.nextSibling();
    }

    const QList<QWidget *> lstWidget = widgets();
    bool globalVariableFound = false;
    for (QWidget *widget : lstWidget) {
        SieveGlobalVariableActionWidget *w = static_cast<SieveGlobalVariableActionWidget *>(widget);
        if (w->variableName() == variableName) {
            w->setVariableValue(variableValue);
            globalVariableFound = true;
        }
    }
    return globalVariableFound;
}
