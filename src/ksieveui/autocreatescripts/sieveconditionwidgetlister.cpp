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

#include "sieveconditionwidgetlister.h"
#include "autocreatescriptdialog.h"
#include "autocreatescriptutil_p.h"
#include "libksieve_debug.h"
#include "commonwidgets/sievehelpbutton.h"
#include "sieveeditorgraphicalmodewidget.h"
#include "sieveconditions/sieveconditionlist.h"
#include "sieveconditions/sievecondition.h"
#include <PimCommon/MinimumComboBox>

#include <QPushButton>
#include <KLocalizedString>
#include <QIcon>

#include <QGridLayout>
#include <QLabel>
#include <QWhatsThis>
#include "libksieve_debug.h"
#include "sievescriptdescriptiondialog.h"
#include <QDomElement>

using namespace KSieveUi;

static const int MINIMUMCONDITION = 1;
static const int MAXIMUMCONDITION = 8;

SieveConditionWidget::SieveConditionWidget(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent)
    : QWidget(parent)
    , mSieveGraphicalModeWidget(sieveGraphicalModeWidget)
{
    initWidget();
}

SieveConditionWidget::~SieveConditionWidget()
{
    qDeleteAll(mConditionList);
    mConditionList.clear();
}

void SieveConditionWidget::setFilterCondition(QWidget *widget)
{
    if (mLayout->itemAtPosition(1, 3)) {
        delete mLayout->itemAtPosition(1, 3)->widget();
    }

    if (widget) {
        mLayout->addWidget(widget, 1, 3);
    } else {
        mLayout->addWidget(new QLabel(i18n("Please select an condition."), this), 1, 3);
    }
}

void SieveConditionWidget::generatedScript(QString &script, QStringList &requires, bool inForEveryPartLoop)
{
    Q_UNUSED(inForEveryPartLoop);
    const int index = mComboBox->currentIndex();
    if (index != mComboBox->count() - 1) {
        KSieveUi::SieveCondition *widgetCondition = mConditionList.at(mComboBox->currentIndex());
        QWidget *currentWidget = mLayout->itemAtPosition(1, 3)->widget();
        const QStringList lstRequires = widgetCondition->needRequires(currentWidget);
        for (const QString &r : lstRequires) {
            if (!requires.contains(r)) {
                requires.append(r);
            }
        }
        script += mConditionList.at(mComboBox->currentIndex())->code(currentWidget) + QLatin1Char('\n');
    }
}

void SieveConditionWidget::initWidget()
{
    mLayout = new QGridLayout(this);
    mLayout->setContentsMargins(0, 0, 0, 0);

    mComboBox = new PimCommon::MinimumComboBox;
    mComboBox->setEditable(false);

    const QList<KSieveUi::SieveCondition *> list = KSieveUi::SieveConditionList::conditionList(mSieveGraphicalModeWidget);
    QList<KSieveUi::SieveCondition *>::const_iterator it;
    QList<KSieveUi::SieveCondition *>::const_iterator end(list.constEnd());
    int index = 0;
    for (index = 0, it = list.constBegin(); it != end; ++it, ++index) {
        if ((*it)->needCheckIfServerHasCapability()) {
            if (mSieveGraphicalModeWidget->sieveCapabilities().contains((*it)->serverNeedsCapability())) {
                // append to the list of actions:
                mConditionList.append(*it);
                connect(*it, &SieveCondition::valueChanged, this, &SieveConditionWidget::valueChanged);
                // add (i18n-ized) name to combo box
                mComboBox->addItem((*it)->label(), (*it)->name());
            } else {
                delete(*it);
            }
        } else {
            // append to the list of actions:
            mConditionList.append(*it);
            connect(*it, &SieveCondition::valueChanged, this, &SieveConditionWidget::valueChanged);
            // add (i18n-ized) name to combo box
            mComboBox->addItem((*it)->label(), (*it)->name());
        }
    }

    mHelpButton = new SieveHelpButton(this);
    mLayout->addWidget(mHelpButton, 1, 0);
    connect(mHelpButton, &SieveHelpButton::clicked, this, &SieveConditionWidget::slotHelp);

    mCommentButton = new QToolButton(this);
    mCommentButton->setToolTip(i18n("Add comment"));
    mLayout->addWidget(mCommentButton, 1, 1);
    mCommentButton->setIcon(QIcon::fromTheme(QStringLiteral("view-pim-notes")));
    connect(mCommentButton, &QToolButton::clicked, this, &SieveConditionWidget::slotAddComment);

    mComboBox->addItem(QLatin1String(""));
    mLayout->addWidget(mComboBox, 1, 2);
    connect(mComboBox, static_cast<void (PimCommon::MinimumComboBox::*)(int)>(&PimCommon::MinimumComboBox::activated), this, &SieveConditionWidget::slotConditionChanged);

    mComboBox->setMaxCount(mComboBox->count());
    mComboBox->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    mComboBox->adjustSize();

    mAdd = new QPushButton(this);
    mAdd->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    mAdd->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    mRemove = new QPushButton(this);
    mRemove->setIcon(QIcon::fromTheme(QStringLiteral("list-remove")));
    mRemove->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    mLayout->addWidget(mAdd, 1, 4);
    mLayout->addWidget(mRemove, 1, 5);

    // redirect focus to the filter action combo box
    setFocusProxy(mComboBox);

    connect(mAdd, &QPushButton::clicked, this, &SieveConditionWidget::slotAddWidget);
    connect(mRemove, &QPushButton::clicked, this, &SieveConditionWidget::slotRemoveWidget);

    clear();
}

void SieveConditionWidget::slotAddComment()
{
    const int index = mComboBox->currentIndex();
    if (index < mConditionList.count()) {
        KSieveUi::SieveCondition *condition = mConditionList.at(index);
        const QString comment = condition->comment();
        QPointer<SieveScriptDescriptionDialog> dlg = new SieveScriptDescriptionDialog;
        dlg->setDescription(comment);
        if (dlg->exec()) {
            condition->setComment(dlg->description());
            Q_EMIT valueChanged();
        }
        delete dlg;
    }
}

void SieveConditionWidget::slotHelp()
{
    const int index = mComboBox->currentIndex();
    if (index < mConditionList.count()) {
        KSieveUi::SieveCondition *condition = mConditionList.at(index);
        const QString help = condition->help();
        const QUrl href = condition->href();
        const QString fullWhatsThis = AutoCreateScriptUtil::createFullWhatsThis(help, href.toString());
        QWhatsThis::showText(QCursor::pos(), fullWhatsThis, mHelpButton);
    }
}

void SieveConditionWidget::slotConditionChanged(int index)
{
    if (index < mConditionList.count()) {
        KSieveUi::SieveCondition *condition = mConditionList.at(index);
        mHelpButton->setEnabled(!condition->help().isEmpty());
        setFilterCondition(condition->createParamWidget(this));
        mCommentButton->setEnabled(true);
    } else {
        setFilterCondition(nullptr);
        mHelpButton->setEnabled(false);
        mCommentButton->setEnabled(false);
    }
    Q_EMIT valueChanged();
}

void SieveConditionWidget::slotAddWidget()
{
    Q_EMIT addWidget(this);
    Q_EMIT valueChanged();
}

void SieveConditionWidget::slotRemoveWidget()
{
    Q_EMIT removeWidget(this);
    Q_EMIT valueChanged();
}

void SieveConditionWidget::clear()
{
    mComboBox->setCurrentIndex(mComboBox->count() - 1);
    setFilterCondition(nullptr);
    mHelpButton->setEnabled(false);
    mCommentButton->setEnabled(false);
}

void SieveConditionWidget::updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled)
{
    mAdd->setEnabled(addButtonEnabled);
    mRemove->setEnabled(removeButtonEnabled);
}

void SieveConditionWidget::setCondition(const QString &conditionName, QXmlStreamReader &element, bool notCondition, QString &error)
{
    const int index = mComboBox->findData(conditionName);
    if (index != -1) {
        mComboBox->setCurrentIndex(index);
        slotConditionChanged(index);
        KSieveUi::SieveCondition *condition = mConditionList.at(index);
        condition->setParamWidgetValue(element, this, notCondition, error);
    } else {
        error += i18n("Script contains unsupported feature \"%1\"", conditionName) + QLatin1Char('\n');
        qCDebug(LIBKSIEVE_LOG) << "Condition " << conditionName << " not supported";
    }
}

SieveConditionWidgetLister::SieveConditionWidgetLister(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent)
    : KPIM::KWidgetLister(false, MINIMUMCONDITION, MAXIMUMCONDITION, parent)
    , mSieveGraphicalModeWidget(sieveGraphicalModeWidget)
{
    slotClear();
    updateAddRemoveButton();
}

SieveConditionWidgetLister::~SieveConditionWidgetLister()
{
}

void SieveConditionWidgetLister::slotAddWidget(QWidget *w)
{
    addWidgetAfterThisWidget(w);
    updateAddRemoveButton();
}

void SieveConditionWidgetLister::slotRemoveWidget(QWidget *w)
{
    removeWidget(w);
    updateAddRemoveButton();
}

void SieveConditionWidgetLister::updateAddRemoveButton()
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
        SieveConditionWidget *w = qobject_cast<SieveConditionWidget *>(*wIt);
        w->updateAddRemoveButton(addButtonEnabled, removeButtonEnabled);
    }
}

void SieveConditionWidgetLister::reconnectWidget(SieveConditionWidget *w)
{
    connect(w, &SieveConditionWidget::addWidget, this, &SieveConditionWidgetLister::slotAddWidget, Qt::UniqueConnection);
    connect(w, &SieveConditionWidget::removeWidget, this, &SieveConditionWidgetLister::slotRemoveWidget, Qt::UniqueConnection);
    connect(w, &SieveConditionWidget::valueChanged, this, &SieveConditionWidgetLister::valueChanged, Qt::UniqueConnection);
}

void SieveConditionWidgetLister::clearWidget(QWidget *aWidget)
{
    if (aWidget) {
        SieveConditionWidget *widget = static_cast<SieveConditionWidget *>(aWidget);
        widget->clear();
    }
    Q_EMIT valueChanged();
}

QWidget *SieveConditionWidgetLister::createWidget(QWidget *parent)
{
    SieveConditionWidget *w = new SieveConditionWidget(mSieveGraphicalModeWidget, parent);
    reconnectWidget(w);
    return w;
}

void SieveConditionWidgetLister::generatedScript(QString &script, int &numberOfCondition, QStringList &requires, bool inForEveryPartLoop)
{
    const QList<QWidget *> widgetList = widgets();
    QList<QWidget *>::ConstIterator wIt = widgetList.constBegin();
    QList<QWidget *>::ConstIterator wEnd = widgetList.constEnd();
    bool wasFirst = true;
    for (; wIt != wEnd; ++wIt) {
        QString condition;
        SieveConditionWidget *w = qobject_cast<SieveConditionWidget *>(*wIt);
        w->generatedScript(condition, requires, inForEveryPartLoop);
        if (!condition.isEmpty()) {
            if (!wasFirst) {
                if (inForEveryPartLoop) {
                    script += AutoCreateScriptUtil::indentation();
                }
                script += QLatin1String(", ");
            }
            script += condition;
            wasFirst = false;
            ++numberOfCondition;
        }
    }
}

int SieveConditionWidgetLister::conditionNumber() const
{
    return widgets().count();
}

void SieveConditionWidgetLister::loadTest(QXmlStreamReader &element, bool notCondition, QString &error)
{
    if (notCondition) {
        element.readNextStartElement();
    }
    if (element.attributes().hasAttribute(QStringLiteral("name"))) {
        const QString conditionName = element.attributes().value(QStringLiteral("name")).toString();
        SieveConditionWidget *w = qobject_cast<SieveConditionWidget *>(widgets().constLast());
        w->setCondition(conditionName, element, notCondition, error);
    }
#ifdef REMOVE_QDOMELEMENT
    QDomElement testElement = element;
    if (notCondition) {
        QDomNode node = element.firstChild();
        if (!node.isNull()) {
            testElement = node.toElement();
        }
    }
    if (testElement.hasAttribute(QStringLiteral("name"))) {
        const QString conditionName = testElement.attribute(QStringLiteral("name"));
        SieveConditionWidget *w = qobject_cast<SieveConditionWidget *>(widgets().constLast());
        w->setCondition(conditionName, testElement, notCondition, error);
    }
#endif
}

void SieveConditionWidgetLister::loadScript(QXmlStreamReader &element, bool uniqTest, bool notCondition, QString &error)
{
    if (uniqTest) {
        loadTest(element, notCondition, error);
    } else {
        bool firstCondition = true;
        if (notCondition) {
            element.readNextStartElement();
        }
        while (element.readNextStartElement()) {
            const QStringRef tagName = element.name();
            if (tagName == QLatin1String("testlist")) {
                while (element.readNextStartElement()) {
                    const QStringRef testTagName = element.name();
                    if (testTagName == QLatin1String("test")) {
                        if (element.attributes().hasAttribute(QStringLiteral("name"))) {
                            QString conditionName = element.attributes().value(QStringLiteral("name")).toString();
                            if (firstCondition) {
                                firstCondition = false;
                            } else {
                                addWidgetAfterThisWidget(widgets().constLast());
                            }
                            SieveConditionWidget *w = qobject_cast<SieveConditionWidget *>(widgets().constLast());
                            if (conditionName == QLatin1String("not")) {
#ifdef QDOMELEMENT_FIXME
                                notCondition = true;
                                QDomNode notNode = testElement.firstChild();
                                QDomElement notElement = notNode.toElement();
                                if (notElement.hasAttribute(QStringLiteral("name"))) {
                                    conditionName = notElement.attribute(QStringLiteral("name"));
                                }
                                w->setCondition(conditionName, notElement, notCondition, error);
#endif
                            } else {
                                notCondition = false;
                                w->setCondition(conditionName, element, notCondition, error);
                            }
                        }
                    } else if (testTagName == QLatin1String("crlf")) {
                        //nothing
                    } else if (testTagName == QLatin1String("comment")) {
                        qDebug() << "Need to implement comment here ";
                        //nothing
                        //implement in the future ?
                    } else {
                        qCDebug(LIBKSIEVE_LOG) << " SieveConditionWidgetLister::loadScript unknown condition tag: " << testTagName;
                    }
                }
            }
        }
    }

#ifdef REMOVE_QDOMELEMENT
    if (uniqTest) {
        loadTest(e, notCondition, error);
    } else {
        bool firstCondition = true;
        QDomElement element = e;
        if (notCondition) {
            QDomNode node = e.firstChild();
            if (!node.isNull()) {
                element = node.toElement();
            }
        }
        QDomNode node = element.firstChild();
        while (!node.isNull()) {
            QDomElement e = node.toElement();
            if (!e.isNull()) {
                const QString tagName = e.tagName();
                if (tagName == QLatin1String("testlist")) {
                    QDomNode testNode = e.firstChild();
                    while (!testNode.isNull()) {
                        QDomElement testElement = testNode.toElement();
                        if (!testElement.isNull()) {
                            const QString testTagName = testElement.tagName();
                            if (testTagName == QLatin1String("test")) {
                                if (testElement.hasAttribute(QStringLiteral("name"))) {
                                    QString conditionName = testElement.attribute(QStringLiteral("name"));
                                    if (firstCondition) {
                                        firstCondition = false;
                                    } else {
                                        addWidgetAfterThisWidget(widgets().constLast());
                                    }
                                    SieveConditionWidget *w = qobject_cast<SieveConditionWidget *>(widgets().constLast());
                                    if (conditionName == QLatin1String("not")) {
                                        notCondition = true;
                                        QDomNode notNode = testElement.firstChild();
                                        QDomElement notElement = notNode.toElement();
                                        if (notElement.hasAttribute(QStringLiteral("name"))) {
                                            conditionName = notElement.attribute(QStringLiteral("name"));
                                        }
                                        w->setCondition(conditionName, notElement, notCondition, error);
                                    } else {
                                        notCondition = false;
                                        w->setCondition(conditionName, testElement, notCondition, error);
                                    }
                                }
                            } else if (testTagName == QLatin1String("crlf")) {
                                //nothing
                            } else if (testTagName == QLatin1String("comment")) {
                                qDebug() << "Need to implement comment here ";
                                //nothing
                                //implement in the future ?
                            } else {
                                qCDebug(LIBKSIEVE_LOG) << " SieveConditionWidgetLister::loadScript unknown condition tag: " << testTagName;
                            }
                        }
                        testNode = testNode.nextSibling();
                    }
                }
            }
            node = node.nextSibling();
        }
    }
#endif
}
