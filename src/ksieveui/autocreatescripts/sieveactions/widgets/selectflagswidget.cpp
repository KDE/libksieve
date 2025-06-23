/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectflagswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KConfigGroup>
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QLineEdit>

#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QPointer>
#include <QPushButton>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWindow>

using namespace KSieveUi;
namespace
{
static const char mySelectFlagsListDialogGroupName[] = "SelectFlagsListDialog";
}
SelectFlagsListDialog::SelectFlagsListDialog(QWidget *parent)
    : QDialog(parent)
    , mListWidget(new SelectFlagsListWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Flags"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mListWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SelectFlagsListDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SelectFlagsListDialog::reject);
    mainLayout->addWidget(buttonBox);
    okButton->setFocus();
    readConfig();
}

SelectFlagsListDialog::~SelectFlagsListDialog()
{
    writeConfig();
}

void SelectFlagsListDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(300, 200));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySelectFlagsListDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SelectFlagsListDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySelectFlagsListDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

void SelectFlagsListDialog::setFlags(const QStringList &list)
{
    mListWidget->setFlags(list);
}

QStringList SelectFlagsListDialog::flags() const
{
    return mListWidget->flags();
}

SelectFlagsListWidget::SelectFlagsListWidget(QWidget *parent)
    : QListWidget(parent)
{
    init();
}

SelectFlagsListWidget::~SelectFlagsListWidget() = default;

void SelectFlagsListWidget::init()
{
    auto item = new QListWidgetItem(i18n("Deleted"), this);
    item->setData(FlagsRealName, u"\\\\Deleted"_s);
    item->setCheckState(Qt::Unchecked);
    item = new QListWidgetItem(i18n("Answered"), this);
    item->setData(FlagsRealName, u"\\\\Answered"_s);
    item->setCheckState(Qt::Unchecked);
    item = new QListWidgetItem(i18n("Flagged"), this);
    item->setData(FlagsRealName, u"\\\\Flagged"_s);
    item->setCheckState(Qt::Unchecked);
    item = new QListWidgetItem(i18n("Seen"), this);
    item->setData(FlagsRealName, u"\\\\Seen"_s);
    item->setCheckState(Qt::Unchecked);
    // item = new QListWidgetItem(u"\\\\Recent"_s, this);
    // item->setCheckState(Qt::Unchecked);
    item = new QListWidgetItem(i18n("Draft"), this);
    item->setData(FlagsRealName, u"\\\\Draft"_s);
    item->setCheckState(Qt::Unchecked);
}

void SelectFlagsListWidget::setFlags(const QStringList &list)
{
    const int numberOfItem = count();
    for (int i = 0; i < numberOfItem; ++i) {
        QListWidgetItem *it = item(i);
        if (list.contains(it->data(FlagsRealName).toString())) {
            it->setCheckState(Qt::Checked);
        }
    }
}

QStringList SelectFlagsListWidget::flags() const
{
    QStringList result;
    const int numberOfItem = count();
    for (int i = 0; i < numberOfItem; ++i) {
        QListWidgetItem *it = item(i);
        if (it->checkState() == Qt::Checked) {
            result << it->data(FlagsRealName).toString();
        }
    }
    return result;
}

SelectFlagsWidget::SelectFlagsWidget(QWidget *parent)
    : QWidget(parent)
{
    auto lay = new QHBoxLayout(this);
    lay->setContentsMargins({});
    mEdit = new QLineEdit(this);
    KLineEditEventHandler::catchReturnKey(mEdit);
    mEdit->setReadOnly(true);
    mEdit->setPlaceholderText(i18nc("@info:placeholder", "Click on button for selecting flags…"));
    connect(mEdit, &QLineEdit::textChanged, this, &SelectFlagsWidget::valueChanged);
    lay->addWidget(mEdit);
    auto selectFlags = new QToolButton(this);
    selectFlags->setText(i18n("…"));
    selectFlags->setToolTip(i18nc("@info:tooltip", "Select Flags"));
    connect(selectFlags, &QPushButton::clicked, this, &SelectFlagsWidget::slotSelectFlags);
    lay->addWidget(selectFlags);
}

SelectFlagsWidget::~SelectFlagsWidget() = default;

void SelectFlagsWidget::slotSelectFlags()
{
    QPointer<SelectFlagsListDialog> dialog = new SelectFlagsListDialog(this);
    dialog->setFlags(AutoCreateScriptUtil::createListFromString(mEdit->text()));
    if (dialog->exec()) {
        const QStringList lstFlags = dialog->flags();
        QString flags;
        if (!lstFlags.isEmpty()) {
            flags = AutoCreateScriptUtil::createList(lstFlags);
        }
        mEdit->setText(flags);
    }
    delete dialog;
}

void SelectFlagsWidget::setFlags(const QStringList &flags)
{
    mEdit->setText(AutoCreateScriptUtil::createList(flags, true, true));
}

QString SelectFlagsWidget::code() const
{
    return mEdit->text();
}

#include "moc_selectflagswidget.cpp"
