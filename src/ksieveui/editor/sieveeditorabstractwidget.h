/* SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "ksieveui_private_export.h"
#include <KSieveCore/SieveImapAccountSettings>
#include <QWidget>
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT SieveEditorAbstractWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveEditorAbstractWidget(QWidget *parent = nullptr);
    ~SieveEditorAbstractWidget() override;

    Q_REQUIRED_RESULT virtual QString currentscript();
    virtual void setImportScript(const QString &);
    void saveAs(const QString &defaultName);

    Q_REQUIRED_RESULT KSieveCore::SieveImapAccountSettings sieveImapAccountSettings() const;

    void setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &sieveImapAccountSettings);

public Q_SLOTS:
    void slotImport();

protected:
    KSieveCore::SieveImapAccountSettings mSieveImapAccountSettings;

private:
    bool loadFromFile(const QString &filename);
};
}
