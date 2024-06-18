/*
 * SPDX-FileCopyrightText: 2015 Sandro Knauß <knauss@kolabsys.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <QObject>
#include <QStringList>

#include "ksievecore_export.h"
#include <QUrl>
#include <memory>

namespace KManageSieve
{
class SieveJob;
}

namespace KSieveCore
{
class CheckKolabKep14SupportJobPrivate;

/**
\brief Checks for support of Non-conflicting edits of Sieve scripts by multiple editors (KEP:14)

\par Introduction

This Kolab Enhancement Proposal defines the conventions and application behaviour to enable
non-conflicting edits of RFC 5228 Sieve scripts on the server by multiple editors.

The result is the ability to have different layers of Sieve management per user, with different
levels of required authorization, and the ability to provide users with multiple active scripts
which can be activated and deactivated as required, thus enabling splitting of the user-facing
Sieve functionality into more conveniently managed files.

Every sieve server can support KEP:14, because it is only a standardisation of filenames and a
way to support and enable multiple scripts.

For more information on Kolab KEP:14 see:
 * http://wiki.kolab.org/KEP:14
 * http://git.kolabsys.com/keps/tree/KEP-0014.txt

*/

class KSIEVECORE_EXPORT CheckKolabKep14SupportJob : public QObject
{
    Q_OBJECT
public:
    explicit CheckKolabKep14SupportJob(QObject *parent = nullptr);
    ~CheckKolabKep14SupportJob() override;

    void start();

    void setServerUrl(const QUrl &url);
    void setServerName(const QString &name);
    [[nodiscard]] QString serverName() const;

    [[nodiscard]] QStringList availableScripts() const;
    [[nodiscard]] bool hasKep14Support() const;
    [[nodiscard]] QUrl serverUrl() const;

Q_SIGNALS:
    void result(KSieveCore::CheckKolabKep14SupportJob *, bool);

private:
    std::unique_ptr<CheckKolabKep14SupportJobPrivate> const d;
    KSIEVECORE_NO_EXPORT void
    slotCheckKep14Support(KManageSieve::SieveJob *job, bool success, const QStringList &availableScripts, const QString &activeScript);
};
}
