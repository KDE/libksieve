/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include <KSyntaxHighlighting/Repository>

namespace KSieveUi
{
class SyntaxHighlightingRepoManager
{
public:
    SyntaxHighlightingRepoManager();
    ~SyntaxHighlightingRepoManager();

    static SyntaxHighlightingRepoManager *self();

    [[nodiscard]] KSyntaxHighlighting::Repository &syntaxRepo();

private:
    KSyntaxHighlighting::Repository mSyntaxRepo;
};
}
