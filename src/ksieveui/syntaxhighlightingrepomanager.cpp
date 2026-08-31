/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "syntaxhighlightingrepomanager.h"

using namespace KSieveUi;

SyntaxHighlightingRepoManager::SyntaxHighlightingRepoManager() = default;

SyntaxHighlightingRepoManager::~SyntaxHighlightingRepoManager() = default;

SyntaxHighlightingRepoManager *SyntaxHighlightingRepoManager::self()
{
    static SyntaxHighlightingRepoManager s_self;
    return &s_self;
}

KSyntaxHighlighting::Repository &SyntaxHighlightingRepoManager::syntaxRepo()
{
    return mSyntaxRepo;
}
