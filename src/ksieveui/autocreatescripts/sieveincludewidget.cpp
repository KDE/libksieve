/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveincludewidget.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescriptutil_p.h"
#include "commonwidgets/sievehelpbutton.h"
#include "sievescriptblockwidget.h"
#include "widgets/includefilelineedit.h"
#include <KLineEditEventHandler>

#include <KLocalizedString>
#include <QIcon>
#include <QPushButton>

#include "libksieveui_debug.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QWhatsThis>
#include <QXmlStreamReader>

using namespace KSieveUi;

static const int MINIMUMINCLUDEACTION = 1;
static const int MAXIMUMINCLUDEACTION = 20;

SieveIncludeLocation::SieveIncludeLocation(QWidget *parent)
    : QComboBox(parent)
{
    initialize();
    connect(this, &SieveIncludeLocation::activated, this, &SieveIncludeLocation::valueChanged);
}

SieveIncludeLocation::~SieveIncludeLocation() = default;

void SieveIncludeLocation::initialize()
{
    addItem(i18n("personal"), QStringLiteral(":personal"));
    addItem(i18n("global"), QStringLiteral(":global"));
}

QString SieveIncludeLocation::code() const
{
    return itemData(currentIndex()).toString();
}

void SieveIncludeLocation::setCode(const QString &code, QString &error)
{
    const int index = findData(code);
    if (index != -1) {
        setCurrentIndex(index);
    } else {
        error += i18n("Unknown location type \"%1\" during parsing includes", code) + QLatin1Char('\n');
        setCurrentIndex(0);
    }
}

SieveIncludeActionWidget::SieveIncludeActionWidget(QWidget *parent)
    : QWidget(parent)
{
    initWidget();
}

SieveIncludeActionWidget::~SieveIncludeActionWidget() = default;

void SieveIncludeActionWidget::clear()
{
    mOptional->setChecked(false);
    mOnce->setChecked(false);
    mLocation->setCurrentIndex(0);
    mIncludeFileName->clear();
}

void SieveIncludeActionWidget::setListOfIncludeFile(const QStringList &listOfIncludeFile)
{
    mIncludeFileName->setListOfIncludeFile(listOfIncludeFile);
}

void SieveIncludeActionWidget::loadScript(QXmlStreamReader &element, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1String("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1String("personal") || tagValue == QLatin1String("global")) {
                mLocation->setCode(AutoCreateScriptUtil::tagValue(tagValue), error);
            } else if (tagValue == QLatin1String("optional")) {
                mOptional->setChecked(true);
            } else if (tagValue == QLatin1String("once")) {
                mOnce->setChecked(true);
            } else {
                qCDebug(LIBKSIEVE_LOG) << " SieveIncludeActionWidget::loadScript unknown tagValue " << tagValue;
            }
        } else if (tagName == QLatin1String("str")) {
            mIncludeFileName->setText(element.readElementText());
        } else {
            qCDebug(LIBKSIEVE_LOG) << " SieveIncludeActionWidget::loadScript unknown tagName " << tagName;
        }
    }
}

void SieveIncludeActionWidget::generatedScript(QString &script)
{
    const QString includeName = mIncludeFileName->text().trimmed();
    if (includeName.isEmpty()) {
        return;
    }
    script += QLatin1String("include ");
    script += mLocation->code() + QLatin1Char(' ');
    if (mOptional->isChecked()) {
        script += QLatin1String(":optional ");
    }
    if (mOnce->isChecked()) {
        script += QLatin1String(":once ");
    }
    script += QStringLiteral("\"%1\";\n").arg(includeName);
}

void SieveIncludeActionWidget::initWidget()
{
    mLayout = new QGridLayout(this);
    mLayout->setContentsMargins({});

    auto lab = new QLabel(i18n("Include:"), this);
    mLayout->addWidget(lab, 1, 0);
    mLocation = new SieveIncludeLocation(this);
    connect(mLocation, &SieveIncludeLocation::valueChanged, this, &SieveIncludeActionWidget::valueChanged);
    mLayout->addWidget(mLocation, 1, 1);

    lab = new QLabel(i18n("Name:"), this);
    mLayout->addWidget(lab, 1, 2);

    mIncludeFileName = new IncludeFileLineEdit(this);
    KLineEditEventHandler::catchReturnKey(mIncludeFileName);
    mIncludeFileName->setClearButtonEnabled(true);
    connect(mIncludeFileName, &QLineEdit::textChanged, this, &SieveIncludeActionWidget::valueChanged);
    mLayout->addWidget(mIncludeFileName, 1, 3);

    mOptional = new QCheckBox(i18n("Optional"), this);
    connect(mOptional, &QCheckBox::toggled, this, &SieveIncludeActionWidget::valueChanged);
    mLayout->addWidget(mOptional, 1, 4);

    mOnce = new QCheckBox(i18n("Once"), this);
    connect(mOnce, &QCheckBox::toggled, this, &SieveIncludeActionWidget::valueChanged);
    mLayout->addWidget(mOnce, 1, 5);

    mAdd = new QPushButton(this);
    mAdd->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    mAdd->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    mRemove = new QPushButton(this);
    mRemove->setIcon(QIcon::fromTheme(QStringLiteral("list-remove")));
    mRemove->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    mLayout->addWidget(mAdd, 1, 6);
    mLayout->addWidget(mRemove, 1, 7);

    connect(mAdd, &QPushButton::clicked, this, &SieveIncludeActionWidget::slotAddWidget);
    connect(mRemove, &QPushButton::clicked, this, &SieveIncludeActionWidget::slotRemoveWidget);
}

void SieveIncludeActionWidget::slotAddWidget()
{
    Q_EMIT valueChanged();
    Q_EMIT addWidget(this);
}

void SieveIncludeActionWidget::slotRemoveWidget()
{
    Q_EMIT valueChanged();
    Q_EMIT removeWidget(this);
}

bool SieveIncludeActionWidget::isInitialized() const
{
    return !mIncludeFileName->text().isEmpty();
}

void SieveIncludeActionWidget::updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled)
{
    mAdd->setEnabled(addButtonEnabled);
    mRemove->setEnabled(removeButtonEnabled);
}

SieveIncludeWidget::SieveIncludeWidget(QWidget *parent)
    : SieveWidgetPageAbstract(parent)
{
    auto lay = new QVBoxLayout(this);
    mHelpButton = new SieveHelpButton(this);
    lay->addWidget(mHelpButton);
    connect(mHelpButton, &SieveHelpButton::clicked, this, &SieveIncludeWidget::slotHelp);

    mIncludeLister = new SieveIncludeWidgetLister(this);
    connect(mIncludeLister, &SieveIncludeWidgetLister::valueChanged, this, &SieveIncludeWidget::valueChanged);
    lay->addWidget(mIncludeLister, 0, Qt::AlignTop);
    setPageType(KSieveUi::SieveScriptBlockWidget::Include);
}

SieveIncludeWidget::~SieveIncludeWidget() = default;

void SieveIncludeWidget::setListOfIncludeFile(const QStringList &lst)
{
    mIncludeLister->setListOfIncludeFile(lst);
}

void SieveIncludeWidget::slotHelp()
{
    const QString help = i18n(
        "The \"include\" command takes an optional \"location\" parameter, an optional \":once\" parameter, an optional \":optional\" parameter, and a single "
        "string argument representing the name of the script to include for processing at that point.");
    const QString href = QStringLiteral("https://tools.ietf.org/html/rfc6609#page-4");
    const QString fullWhatsThis = AutoCreateScriptUtil::createFullWhatsThis(help, href);
    QWhatsThis::showText(QCursor::pos(), fullWhatsThis, mHelpButton);
}

void SieveIncludeWidget::generatedScript(QString &script, QStringList &requireModules, bool inForEveryPartLoop)
{
    Q_UNUSED(inForEveryPartLoop)
    QString result;
    QStringList lst;
    mIncludeLister->generatedScript(result, lst);
    if (!result.isEmpty()) {
        script += result;
        requireModules << lst;
    }
}

void SieveIncludeWidget::loadScript(QXmlStreamReader &element, QString &error)
{
    mIncludeLister->loadScript(element, error);
}

SieveIncludeWidgetLister::SieveIncludeWidgetLister(QWidget *parent)
    : KPIM::KWidgetLister(false, MINIMUMINCLUDEACTION, MAXIMUMINCLUDEACTION, parent)
{
    slotClear();
    updateAddRemoveButton();
}

SieveIncludeWidgetLister::~SieveIncludeWidgetLister() = default;

void SieveIncludeWidgetLister::slotAddWidget(QWidget *w)
{
    addWidgetAfterThisWidget(w);
    updateAddRemoveButton();
}

void SieveIncludeWidgetLister::slotRemoveWidget(QWidget *w)
{
    removeWidget(w);
    updateAddRemoveButton();
}

void SieveIncludeWidgetLister::updateAddRemoveButton()
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
        auto w = qobject_cast<SieveIncludeActionWidget *>(*wIt);
        w->updateAddRemoveButton(addButtonEnabled, removeButtonEnabled);
    }
}

void SieveIncludeWidgetLister::setListOfIncludeFile(const QStringList &listOfIncludeFile)
{
    mListOfIncludeFile = listOfIncludeFile;
    // Update all lineedit
    const QList<QWidget *> widgetList = widgets();
    QList<QWidget *>::ConstIterator wIt = widgetList.constBegin();
    QList<QWidget *>::ConstIterator wEnd = widgetList.constEnd();
    for (; wIt != wEnd; ++wIt) {
        auto w = qobject_cast<SieveIncludeActionWidget *>(*wIt);
        w->setListOfIncludeFile(mListOfIncludeFile);
    }
}

void SieveIncludeWidgetLister::generatedScript(QString &script, QStringList &requireModules)
{
    requireModules << QStringLiteral("include");
    const QList<QWidget *> widgetList = widgets();
    QList<QWidget *>::ConstIterator wIt = widgetList.constBegin();
    QList<QWidget *>::ConstIterator wEnd = widgetList.constEnd();
    for (; wIt != wEnd; ++wIt) {
        auto w = qobject_cast<SieveIncludeActionWidget *>(*wIt);
        w->generatedScript(script);
    }
}

void SieveIncludeWidgetLister::reconnectWidget(SieveIncludeActionWidget *w)
{
    connect(w, &SieveIncludeActionWidget::addWidget, this, &SieveIncludeWidgetLister::slotAddWidget, Qt::UniqueConnection);
    connect(w, &SieveIncludeActionWidget::removeWidget, this, &SieveIncludeWidgetLister::slotRemoveWidget, Qt::UniqueConnection);
    connect(w, &SieveIncludeActionWidget::valueChanged, this, &SieveIncludeWidgetLister::valueChanged, Qt::UniqueConnection);
}

void SieveIncludeWidgetLister::clearWidget(QWidget *aWidget)
{
    if (aWidget) {
        auto widget = static_cast<SieveIncludeActionWidget *>(aWidget);
        widget->clear();
    }
    Q_EMIT valueChanged();
}

QWidget *SieveIncludeWidgetLister::createWidget(QWidget *parent)
{
    auto w = new SieveIncludeActionWidget(parent);
    w->setListOfIncludeFile(mListOfIncludeFile);
    reconnectWidget(w);
    return w;
}

void SieveIncludeWidgetLister::loadScript(QXmlStreamReader &element, QString &error)
{
    if (widgets().count() == MAXIMUMINCLUDEACTION) {
        error += QLatin1Char('\n') + i18n("We can not add more includes elements.") + QLatin1Char('\n');
        return;
    }
    SieveIncludeActionWidget *w = static_cast<SieveIncludeActionWidget *>(widgets().constLast());
    if (w->isInitialized()) {
        addWidgetAfterThisWidget(widgets().constLast());
        w = static_cast<SieveIncludeActionWidget *>(widgets().constLast());
    }
    w->loadScript(element, error);
}

#include "moc_sieveincludewidget.cpp"
