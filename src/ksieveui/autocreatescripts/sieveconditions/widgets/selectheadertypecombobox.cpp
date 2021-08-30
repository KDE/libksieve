/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "selectheadertypecombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include <KLocalizedString>
#include <Libkdepim/LineEditCatchReturnKey>
#include <QIcon>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>

#include <KConfigGroup>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

using namespace KSieveUi;

static const char selectMultipleHeaders[] = I18N_NOOP("Select multiple headers...");

SelectHeadersDialog::SelectHeadersDialog(QWidget *parent)
    : QDialog(parent)
    , mListWidget(new SelectHeadersWidget(this))
    , mNewHeader(new QLineEdit(this))
    , mAddNewHeader(new QPushButton(this))
{
    setWindowTitle(i18nc("@title:window", "Headers"));

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    auto mainLayout = new QVBoxLayout(this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SelectHeadersDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SelectHeadersDialog::reject);
    okButton->setFocus();

    auto lay = new QVBoxLayout;
    lay->setObjectName(QStringLiteral("widgetlayout"));
    lay->setContentsMargins({});
    mainLayout->addLayout(lay);
    mListWidget->setObjectName(QStringLiteral("listwidget"));
    lay->addWidget(mListWidget);

    auto lab = new QLabel(i18n("Add new header:"), this);
    lab->setObjectName(QStringLiteral("label"));
    lay->addWidget(lab);

    auto hbox = new QHBoxLayout;

    new KPIM::LineEditCatchReturnKey(mNewHeader, this);
    mNewHeader->setObjectName(QStringLiteral("newheader"));
    mNewHeader->setClearButtonEnabled(true);
    // mNewHeader->setTrapReturnKey(true);
    connect(mNewHeader, &QLineEdit::returnPressed, this, &SelectHeadersDialog::slotAddNewHeader);
    mNewHeader->setClearButtonEnabled(true);

    mAddNewHeader->setObjectName(QStringLiteral("addnewheader"));
    mAddNewHeader->setEnabled(false);
    mAddNewHeader->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
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
    KConfigGroup group(KSharedConfig::openStateConfig(), "SelectHeadersDialog");
    const QSize size = group.readEntry("Size", QSize(400, 300));
    if (size.isValid()) {
        resize(size);
    }
}

void SelectHeadersDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "SelectHeadersDialog");
    group.writeEntry("Size", size());
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

SelectHeadersWidget::~SelectHeadersWidget()
{
}

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
    QMapIterator<QString, QString> i(lst);
    while (i.hasNext()) {
        i.next();
        if (!i.value().isEmpty()) {
            auto item = new QListWidgetItem(i.value(), this);
            item->setData(HeaderId, i.key());
            if (selectedHeaders.contains(i.key())) {
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
                result += QLatin1String(", ");
            }
            selected = true;
            result += QLatin1String("\"") + it->data(HeaderId).toString() + QLatin1String("\"");
        }
    }
    if (!result.isEmpty()) {
        result = QLatin1String("[ ") + result + QLatin1String(" ]");
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
    connect(this, qOverload<int>(&SelectHeaderTypeComboBox::activated), this, &SelectHeaderTypeComboBox::valueChanged);
}

SelectHeaderTypeComboBox::~SelectHeaderTypeComboBox()
{
}

void SelectHeaderTypeComboBox::changeReadOnlyStatus()
{
    const bool readOnly = (currentIndex() > 0);
    lineEdit()->setReadOnly(readOnly);
    lineEdit()->setClearButtonEnabled(!readOnly);
}

void SelectHeaderTypeComboBox::slotSelectItem(const QString &str)
{
    changeReadOnlyStatus();
    if (str == i18n(selectMultipleHeaders)) {
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
    mHeaderMap.insert(QStringLiteral("from"), i18n("From"));
    mHeaderMap.insert(QStringLiteral("to"), i18n("To"));
    mHeaderMap.insert(QStringLiteral("Reply-To"), i18n("Reply To"));
    mHeaderMap.insert(QStringLiteral("cc"), i18n("Cc"));
    mHeaderMap.insert(QStringLiteral("bcc"), i18n("Bcc"));
    mHeaderMap.insert(QStringLiteral("Resent-From"), i18n("Resent From"));
    mHeaderMap.insert(QStringLiteral("Resent-To"), i18n("Resent To"));
    mHeaderMap.insert(QStringLiteral("sender"), i18n("Sender"));
    if (!onlyEnvelopType) {
        mHeaderMap.insert(QStringLiteral("subject"), i18n("Subject"));
        mHeaderMap.insert(QStringLiteral("Date"), i18n("Date"));
        mHeaderMap.insert(QStringLiteral("Message-ID"), i18n("Message Id"));
        mHeaderMap.insert(QStringLiteral("Content-Type"), i18n("Content type"));
    }
}

void SelectHeaderTypeComboBox::initialize(bool onlyEnvelopType)
{
    headerMap(onlyEnvelopType);
    QMapIterator<QString, QString> i(mHeaderMap);
    while (i.hasNext()) {
        i.next();
        addItem(i.value(), i.key());
    }
    addItem(i18n(selectMultipleHeaders));
}

QString SelectHeaderTypeComboBox::code() const
{
    QString str = (currentIndex() > -1) ? itemData(currentIndex()).toString() : QString();
    if (str.isEmpty()) {
        str = currentText();
        if (str == i18n(selectMultipleHeaders)) {
            str = QString(); // return QString();
        }
    }
    if (!str.isEmpty() && !str.startsWith(QLatin1Char('['))) {
        str = QLatin1String("\"") + str + QLatin1String("\"");
    }
    return str;
}

void SelectHeaderTypeComboBox::setCode(const QString &code)
{
    QMapIterator<QString, QString> i(mHeaderMap);
    bool foundHeaders = false;
    while (i.hasNext()) {
        i.next();
        if (i.key() == code) {
            const int index = findData(i.key());
            setCurrentIndex(index);
            lineEdit()->setText(i.value());
            foundHeaders = true;
            break;
        }
    }
    // If not found select last combobox item
    if (!foundHeaders) {
        if (code.startsWith(QLatin1Char('['))) {
            setCurrentIndex(count() - 1);
        } else {
            setCurrentIndex(0);
        }
        lineEdit()->setText(code);
    }
    mCode = code;
    changeReadOnlyStatus();
}
