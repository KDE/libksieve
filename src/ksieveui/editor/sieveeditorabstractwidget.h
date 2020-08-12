/* SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef SIEVEEDITORABSTRACTWIDGET_H
#define SIEVEEDITORABSTRACTWIDGET_H

#include <QWidget>
#include "../util/sieveimapaccountsettings.h"
#include "ksieveui_private_export.h"
namespace KSieveUi {
class SieveImapAccountSettings;
class KSIEVEUI_TESTS_EXPORT SieveEditorAbstractWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveEditorAbstractWidget(QWidget *parent = nullptr);
    ~SieveEditorAbstractWidget();

    Q_REQUIRED_RESULT virtual QString currentscript();
    virtual void setImportScript(const QString &);
    void saveAs(const QString &defaultName);

    Q_REQUIRED_RESULT KSieveUi::SieveImapAccountSettings sieveImapAccountSettings() const;

    void setSieveImapAccountSettings(const KSieveUi::SieveImapAccountSettings &sieveImapAccountSettings);

public Q_SLOTS:
    void slotImport();

protected:
    KSieveUi::SieveImapAccountSettings mSieveImapAccountSettings;

private:
    bool loadFromFile(const QString &filename);
};
}

#endif // SIEVEEDITORABSTRACTWIDGET_H
