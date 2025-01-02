/*
  SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "sieveeditorutil.h"

QUrl KSieveUi::SieveEditorUtil::helpUrl(KSieveUi::SieveEditorUtil::HelpVariableName type)
{
    switch (type) {
    case AddressCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5228#page-16"));
    case BodyCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5173"));
    case ConvertCondition:
        return {}; // TODO
    case CurrentdateCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5260#page-6"));
    case DateCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5260#page-4"));
    case EnvelopeCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3028#page-24"));
    case EnvironmentCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5183"));
    case ExistsCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3028#page-25"));
    case FalseCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3028#page-25"));
    case HasflagCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5232#page-5"));
    case HeaderCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5228#page-9"));
    case IhaveCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5463"));
    case MailboxexistsCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5490#page-2"));
    case MetadataexistsCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5490#page-6"));
    case MetadataCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5490#page-5"));
    case ServermetadataexistsCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5490#page-4"));
    case ServermetadataCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5490#page-5"));
    case SizeCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3028#page-26"));
    case SpamtestCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3685#page-3"));
    case TrueCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3028#page-26"));
    case NotCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3028#page-26"));
    case AnyOf:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3028#page-24"));
    case Allof:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3028#page-23"));
    case VirustestCondition:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3685#page-4"));
    case AbstracteditheaderAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5228#page-9"));
    case AddflagsAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5232#page-5"));
    case AddheaderAction:
    case DeleteheaderAction:
    case EditHeaderExtension:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5293"));
    case BreakAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5703#page-3"));
    case ConvertAction:
        return {}; // TODO
    case DiscardAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3028#page-22"));
    case EncloseAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5703#page-10"));
    case ExtracttextAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5703#page-11"));
    case FileintoAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3028#page-20"));
    case KeepAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3028#page-21"));
    case NotifyAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5435#page-3"));
    case RedirectAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3028#page-20"));
    case RejectAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3028#page-20"));
    case ERejectAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5429#section-2.1"));
    case RemoveflagsAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5232#page-5"));
    case ReplaceAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5703#page-8"));
    case ReturnAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc6609#page-7"));
    case SetflagsAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5232#page-4"));
    case SetvariableAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5229"));
    case StopAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3028#page-19"));
    case VacationAction:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5230#page-3"));
    case GlobalVariable:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5229"));
    case Includes:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc6609#page-4"));
    case ForEveryPart:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5703#page-3"));
    case CopyExtension:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc3894"));
    case MBoxMetaDataExtension:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5490#page-2"));
    case SubAddressExtension:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5233"));
    case ImapFlagsExtension:
        return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5232"));
    case UnknownHelp:
        break;
    }
    return {};
}

KSieveUi::SieveEditorUtil::HelpVariableName KSieveUi::SieveEditorUtil::strToVariableName(const QString &str)
{
    if (str == QLatin1StringView("address")) {
        return AddressCondition;
    } else if (str == QLatin1StringView("body")) {
        return BodyCondition;
    } else if (str == QLatin1StringView("currentdate")) {
        return CurrentdateCondition;
    } else if (str == QLatin1StringView("date")) {
        return DateCondition;
    } else if (str == QLatin1StringView("envelope")) {
        return EnvelopeCondition;
    } else if (str == QLatin1StringView("environment")) {
        return EnvironmentCondition;
    } else if (str == QLatin1StringView("exists")) {
        return ExistsCondition;
    } else if (str == QLatin1StringView("false")) {
        return FalseCondition;
    } else if (str == QLatin1StringView("hasflag")) {
        return HasflagCondition;
    } else if (str == QLatin1StringView("header")) {
        return HeaderCondition;
    } else if (str == QLatin1StringView("ihave")) {
        return IhaveCondition;
    } else if (str == QLatin1StringView("mailboxexists")) {
        return MailboxexistsCondition;
    } else if (str == QLatin1StringView("metadata")) {
        return MetadataCondition;
    } else if (str == QLatin1StringView("metadataexists")) {
        return MetadataexistsCondition;
    } else if (str == QLatin1StringView("servermetadata")) {
        return ServermetadataCondition;
    } else if (str == QLatin1StringView("servermetadataexists")) {
        return ServermetadataexistsCondition;
    } else if (str == QLatin1StringView("size")) {
        return SizeCondition;
    } else if (str == QLatin1StringView("spamtest")) {
        return SpamtestCondition;
    } else if (str == QLatin1StringView("true")) {
        return TrueCondition;
    } else if (str == QLatin1StringView("not")) {
        return NotCondition;
    } else if (str == QLatin1StringView("anyof")) {
        return AnyOf;
    } else if (str == QLatin1StringView("allof")) {
        return Allof;
    } else if (str == QLatin1StringView("virustest")) {
        return VirustestCondition;
    } else if (str == QLatin1StringView("break")) {
        return BreakAction;
    } else if (str == QLatin1StringView("convert")) {
        return ConvertAction; // TODO
    } else if (str == QLatin1StringView("discard")) {
        return DiscardAction;
    } else if (str == QLatin1StringView("enclose")) {
        return EncloseAction;
    } else if (str == QLatin1StringView("extracttext")) {
        return ExtracttextAction;
    } else if (str == QLatin1StringView("fileinto")) {
        return FileintoAction;
    } else if (str == QLatin1StringView("keep")) {
        return KeepAction;
    } else if (str == QLatin1StringView("notify")) {
        return NotifyAction;
    } else if (str == QLatin1StringView("redirect")) {
        return RedirectAction;
    } else if (str == QLatin1StringView("reject")) {
        return RejectAction;
    } else if (str == QLatin1StringView("ereject")) {
        return ERejectAction;
    } else if (str == QLatin1StringView("replace")) {
        return ReplaceAction;
    } else if (str == QLatin1StringView("return")) {
        return ReturnAction;
    } else if (str == QLatin1StringView("set")) {
        return SetvariableAction;
    } else if (str == QLatin1StringView("stop")) {
        return StopAction;
    } else if (str == QLatin1StringView("vacation")) {
        return VacationAction;
    } else if (str == QLatin1StringView("include")) {
        return Includes;
    } else if (str == QLatin1StringView("globalvariable")) {
        return GlobalVariable;
    } else if (str == QLatin1StringView("foreverypart")) {
        return ForEveryPart;
    } else if (str == QLatin1StringView("copy")) {
        return CopyExtension;
    } else if (str == QLatin1StringView("mboxmetadata")) {
        return MBoxMetaDataExtension;
    } else if (str == QLatin1StringView("subaddress")) {
        return SubAddressExtension;
    } else if (str == QLatin1StringView("imapflags")) {
        return ImapFlagsExtension;
    } else if (str == QLatin1StringView("editheader")) {
        return EditHeaderExtension;
    }

    // TODO
    return UnknownHelp;
}
