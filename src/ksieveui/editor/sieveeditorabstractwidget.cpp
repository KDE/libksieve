/* SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "sieveeditorabstractwidget.h"
#include <PimCommon/PimUtil>

#include <KLocalizedString>
#include <KMessageBox>
#include <QFileDialog>

#include <QPointer>
#include <QTextStream>

#include <cerrno>

using namespace KSieveUi;
SieveEditorAbstractWidget::SieveEditorAbstractWidget(QWidget *parent)
    : QWidget(parent)
{
}

SieveEditorAbstractWidget::~SieveEditorAbstractWidget() = default;

void SieveEditorAbstractWidget::saveAs(const QString &defaultName)
{
    const QString filter = i18n("Sieve Files (*.siv);;All Files (*)");
    PimCommon::Util::saveTextAs(currentscript(), filter, this, QUrl::fromLocalFile(defaultName), i18nc("@title:window", "Save Script"));
}

KSieveCore::SieveImapAccountSettings SieveEditorAbstractWidget::sieveImapAccountSettings() const
{
    return mSieveImapAccountSettings;
}

void SieveEditorAbstractWidget::setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &sieveImapAccountSettings)
{
    mSieveImapAccountSettings = sieveImapAccountSettings;
}

QString SieveEditorAbstractWidget::currentscript()
{
    return {};
}

void SieveEditorAbstractWidget::setImportScript(const QString &)
{
}

void SieveEditorAbstractWidget::slotImport()
{
    if (!currentscript().isEmpty()) {
        const int answer = KMessageBox::warningTwoActions(this,
                                                          i18n("You will overwrite script. Do you want to continue?"),
                                                          i18nc("@title:window", "Import Script"),
                                                          KStandardGuiItem::cont(),
                                                          KStandardGuiItem::cancel());
        if (answer == KMessageBox::ButtonCode::SecondaryAction) {
            return;
        }
    }
    const QString filter = i18n("*.siv;;sieve files (*.siv);;all files (*)");
    QPointer<QFileDialog> fdlg(new QFileDialog(this, i18nc("@title:window", "Import Script Sieve"), QString(), filter));
    fdlg->setFileMode(QFileDialog::ExistingFile);
    if (fdlg->exec() == QDialog::Accepted) {
        const QStringList fileNames = fdlg->selectedFiles();
        if (!loadFromFile(fileNames.at(0))) {
            KMessageBox::error(this,
                               i18n("Could not load the file %1:\n"
                                    "\"%2\" is the detailed error description.",
                                    fileNames.at(0),
                                    QString::fromLocal8Bit(strerror(errno))),
                               i18n("Sieve Editor Error"));
        }
    }
    delete fdlg;
}

bool SieveEditorAbstractWidget::loadFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    QString scriptText;
    while (!line.isNull()) {
        if (scriptText.isEmpty()) {
            scriptText = line;
        } else {
            scriptText += QLatin1Char('\n') + line;
        }
        line = in.readLine();
    }
    setImportScript(scriptText);
    return true;
}

#include "moc_sieveeditorabstractwidget.cpp"
