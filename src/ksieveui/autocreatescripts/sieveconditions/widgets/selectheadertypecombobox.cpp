/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectheadertypecombobox.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QIcon>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>

#include <KConfigGroup>
#include <KLazyLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QWindow>
using namespace KSieveUi;
namespace
{
static const char mySelectFlagsListDialogGroupName[] = "SelectHeadersDialog";
}
const KLazyLocalizedString selectMultipleHeaders = kli18n("Select multiple headers…");
SelectHeadersDialog::SelectHeadersDialog(QWidget *parent)
    : QDialog(parent)
    , mListWidget(new SelectHeadersWidget(this))
    , mNewHeader(new QLineEdit(this))
    , mAddNewHeader(new QPushButton(this))
{
    setWindowTitle(i18nc("@title:window", "Headers"));

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QLatin1StringView("buttonbox"));
    auto mainLayout = new QVBoxLayout(this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SelectHeadersDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SelectHeadersDialog::reject);
    okButton->setFocus();

    auto lay = new QVBoxLayout;
    lay->setObjectName(QLatin1StringView("widgetlayout"));
    lay->setContentsMargins({});
    mainLayout->addLayout(lay);
    mListWidget->setObjectName(QLatin1StringView("listwidget"));
    lay->addWidget(mListWidget);

    auto lab = new QLabel(i18nc("@label:textbox", "Add new header:"), this);
    lab->setObjectName(QLatin1StringView("label"));
    lay->addWidget(lab);

    auto hbox = new QHBoxLayout;

    KLineEditEventHandler::catchReturnKey(mNewHeader);
    mNewHeader->setObjectName(QLatin1StringView("newheader"));
    mNewHeader->setClearButtonEnabled(true);
    // mNewHeader->setTrapReturnKey(true);
    connect(mNewHeader, &QLineEdit::returnPressed, this, &SelectHeadersDialog::slotAddNewHeader);
    mNewHeader->setClearButtonEnabled(true);

    mAddNewHeader->setObjectName(QLatin1StringView("addnewheader"));
    mAddNewHeader->setEnabled(false);
    mAddNewHeader->setIcon(QIcon::fromTheme(u"list-add"_s));
    mAddNewHeader->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    connect(mAddNewHeader, &QPushButton::clicked, this, &SelectHeadersDialog::slotAddNewHeader);
    connect(mNewHeader, &QLineEdit::textChanged, this, &SelectHeadersDialog::slotNewHeaderTextChanged);
    hbox->addWidget(mNewHeader);
    hbox->addWidget(mAddNewHeader);

    lay->addLayout(hbox);

    mainLayout->addWidget(buttonBox);

    readConfig();
}

SelectHeadersDialog::~SelectHeadersDialog()
{
    writeConfig();
}

void SelectHeadersDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySelectFlagsListDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SelectHeadersDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySelectFlagsListDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

void SelectHeadersDialog::slotNewHeaderTextChanged(const QString &text)
{
    mAddNewHeader->setEnabled(!text.trimmed().isEmpty());
}

void SelectHeadersDialog::slotAddNewHeader()
{
    const QString headerText = mNewHeader->text().trimmed();
    if (!headerText.isEmpty()) {
        mListWidget->addNewHeader(headerText);
        mNewHeader->clear();
    }
}

void SelectHeadersDialog::setListHeaders(const QMap<QString, QString> &lst, const QStringList &selectedHeaders)
{
    mListWidget->setListHeaders(lst, selectedHeaders);
}

QString SelectHeadersDialog::headers() const
{
    return mListWidget->headers();
}

SelectHeadersWidget::SelectHeadersWidget(QWidget *parent)
    : QListWidget(parent)
{
}

SelectHeadersWidget::~SelectHeadersWidget() = default;

void SelectHeadersWidget::addNewHeader(const QString &header)
{
    const int numberOfItem = count();
    for (int i = 0; i < numberOfItem; ++i) {
        QListWidgetItem *it = item(i);
        if ((it->data(HeaderId).toString()) == header || (it->text() == header)) {
            return;
        }
    }

    auto item = new QListWidgetItem(header, this);
    item->setData(HeaderId, header);
    item->setCheckState(Qt::Checked);
    scrollToItem(item);
}

void SelectHeadersWidget::setListHeaders(const QMap<QString, QString> &lst, const QStringList &selectedHeaders)
{
    for (const auto &[key, value] : lst.asKeyValueRange()) {
        if (!value.isEmpty()) {
            auto item = new QListWidgetItem(value, this);
            item->setData(HeaderId, key);
            if (selectedHeaders.contains(key)) {
                item->setCheckState(Qt::Checked);
            } else {
                item->setCheckState(Qt::Unchecked);
            }
        }
    }
    for (const QString &header : selectedHeaders) {
        if (!lst.contains(header)) {
            auto item = new QListWidgetItem(header, this);
            item->setData(HeaderId, header);
            item->setCheckState(Qt::Checked);
        }
    }
}

QString SelectHeadersWidget::headers() const
{
    QString result;
    bool selected = false;
    const int numberOfItem = count();
    for (int i = 0; i < numberOfItem; ++i) {
        QListWidgetItem *it = item(i);
        if (it->checkState() == Qt::Checked) {
            if (selected) {
                result += QLatin1StringView(", ");
            }
            selected = true;
            result += QLatin1StringView("\"") + it->data(HeaderId).toString() + QLatin1StringView("\"");
        }
    }
    if (!result.isEmpty()) {
        result = QLatin1StringView("[ ") + result + QLatin1StringView(" ]");
    }
    return result;
}

SelectHeaderTypeComboBox::SelectHeaderTypeComboBox(bool onlyEnvelopType, QWidget *parent)
    : QComboBox(parent)
{
    setEditable(true);
    lineEdit()->setClearButtonEnabled(true);
    // TODO add completion
    initialize(onlyEnvelopType);
    connect(this, &SelectHeaderTypeComboBox::textActivated, this, &SelectHeaderTypeComboBox::slotSelectItem);
    connect(this, &SelectHeaderTypeComboBox::editTextChanged, this, &SelectHeaderTypeComboBox::valueChanged);
    connect(this, &SelectHeaderTypeComboBox::activated, this, &SelectHeaderTypeComboBox::valueChanged);
}

SelectHeaderTypeComboBox::~SelectHeaderTypeComboBox() = default;

void SelectHeaderTypeComboBox::changeReadOnlyStatus()
{
    const bool readOnly = (currentIndex() > 0);
    lineEdit()->setReadOnly(readOnly);
    lineEdit()->setClearButtonEnabled(!readOnly);
}

const QString SelectHeaderTypeComboBox::getSelectMultipleHeadersTranslated() const
{
    return selectMultipleHeaders.toString();
}

void SelectHeaderTypeComboBox::slotSelectItem(const QString &str)
{
    changeReadOnlyStatus();
    if (str == getSelectMultipleHeadersTranslated()) {
        QPointer<SelectHeadersDialog> dlg = new SelectHeadersDialog(this);
        dlg->setListHeaders(mHeaderMap, AutoCreateScriptUtil::createListFromString(mCode));
        if (dlg->exec()) {
            mCode = dlg->headers();
            lineEdit()->setText(dlg->headers());
            Q_EMIT valueChanged();
        } else {
            lineEdit()->setText(mCode);
        }
        delete dlg;
    } else {
        mCode = str;
    }
}

void SelectHeaderTypeComboBox::headerMap(bool onlyEnvelopType)
{
    mHeaderMap.insert(QString(), QString());
    mHeaderMap.insert(u"from"_s, i18n("From"));
    mHeaderMap.insert(u"to"_s, i18n("To"));
    mHeaderMap.insert(u"Reply-To"_s, i18n("Reply To"));
    mHeaderMap.insert(u"cc"_s, i18n("Cc"));
    mHeaderMap.insert(u"bcc"_s, i18n("Bcc"));
    mHeaderMap.insert(u"Resent-From"_s, i18n("Resent From"));
    mHeaderMap.insert(u"Resent-To"_s, i18n("Resent To"));
    mHeaderMap.insert(u"sender"_s, i18n("Sender"));
    if (!onlyEnvelopType) {
        mHeaderMap.insert(u"subject"_s, i18n("Subject"));
        mHeaderMap.insert(u"Date"_s, i18n("Date"));
        mHeaderMap.insert(u"Message-ID"_s, i18n("Message Id"));
        mHeaderMap.insert(u"Content-Type"_s, i18n("Content type"));
    }
}

void SelectHeaderTypeComboBox::initialize(bool onlyEnvelopType)
{
    headerMap(onlyEnvelopType);
    for (const auto &[key, value] : mHeaderMap.asKeyValueRange()) {
        addItem(value, key);
    }
    addItem(getSelectMultipleHeadersTranslated());
}

QString SelectHeaderTypeComboBox::code() const
{
    QString str = (currentIndex() > -1) ? itemData(currentIndex()).toString() : QString();
    if (str.isEmpty()) {
        str = currentText();
        if (str == getSelectMultipleHeadersTranslated()) {
            str = QString(); // return QString();
        }
    }
    if (!str.isEmpty() && !str.startsWith(u'[')) {
        str = QLatin1StringView("\"") + str + QLatin1StringView("\"");
    }
    return str;
}

void SelectHeaderTypeComboBox::setCode(const QString &code)
{
    bool foundHeaders = false;
    for (const auto &[key, value] : mHeaderMap.asKeyValueRange()) {
        if (key == code) {
            const int index = findData(key);
            setCurrentIndex(index);
            lineEdit()->setText(value);
            foundHeaders = true;
            break;
        }
    }
    // If not found select last combobox item
    if (!foundHeaders) {
        if (code.startsWith(u'[')) {
            setCurrentIndex(count() - 1);
        } else {
            setCurrentIndex(0);
        }
        lineEdit()->setText(code);
    }
    mCode = code;
    changeReadOnlyStatus();
}

#include "moc_selectheadertypecombobox.cpp"
