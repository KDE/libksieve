/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QUrl>

namespace KSieveUi
{
namespace SieveEditorUtil
{
enum HelpVariableName {
    UnknownHelp,
    AddressCondition,
    BodyCondition,
    ConvertCondition,
    CurrentdateCondition,
    DateCondition,
    EnvelopeCondition,
    EnvironmentCondition,
    ExistsCondition,
    FalseCondition,
    HasflagCondition,
    HeaderCondition,
    IhaveCondition,
    MailboxexistsCondition,
    MetadataexistsCondition,
    MetadataCondition,
    ServermetadataexistsCondition,
    ServermetadataCondition,
    SizeCondition,
    SpamtestCondition,
    TrueCondition,
    VirustestCondition,
    NotCondition,
    AnyOf,
    Allof,
    AbstracteditheaderAction,
    AddflagsAction,
    AddheaderAction,
    BreakAction,
    ConvertAction,
    DeleteheaderAction,
    DiscardAction,
    EncloseAction,
    ExtracttextAction,
    FileintoAction,
    KeepAction,
    NotifyAction,
    RedirectAction,
    RejectAction,
    RemoveflagsAction,
    ReplaceAction,
    ReturnAction,
    SetflagsAction,
    SetvariableAction,
    StopAction,
    VacationAction,
    GlobalVariable,
    Includes,
    ForEveryPart,
    CopyExtension,
    MBoxMetaDataExtension,
    SubAddressExtension,
    ERejectAction,
    ImapFlagsExtension,
    EditHeaderExtension
};
KSieveUi::SieveEditorUtil::HelpVariableName strToVariableName(const QString &str);

QUrl helpUrl(KSieveUi::SieveEditorUtil::HelpVariableName type);
}
}
