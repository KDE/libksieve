/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

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
    if (str == QLatin1String("address")) {
        return AddressCondition;
    } else if (str == QLatin1String("body")) {
        return BodyCondition;
    } else if (str == QLatin1String("currentdate")) {
        return CurrentdateCondition;
    } else if (str == QLatin1String("date")) {
        return DateCondition;
    } else if (str == QLatin1String("envelope")) {
        return EnvelopeCondition;
    } else if (str == QLatin1String("environment")) {
        return EnvironmentCondition;
    } else if (str == QLatin1String("exists")) {
        return ExistsCondition;
    } else if (str == QLatin1String("false")) {
        return FalseCondition;
    } else if (str == QLatin1String("hasflag")) {
        return HasflagCondition;
    } else if (str == QLatin1String("header")) {
        return HeaderCondition;
    } else if (str == QLatin1String("ihave")) {
        return IhaveCondition;
    } else if (str == QLatin1String("mailboxexists")) {
        return MailboxexistsCondition;
    } else if (str == QLatin1String("metadata")) {
        return MetadataCondition;
    } else if (str == QLatin1String("metadataexists")) {
        return MetadataexistsCondition;
    } else if (str == QLatin1String("servermetadata")) {
        return ServermetadataCondition;
    } else if (str == QLatin1String("servermetadataexists")) {
        return ServermetadataexistsCondition;
    } else if (str == QLatin1String("size")) {
        return SizeCondition;
    } else if (str == QLatin1String("spamtest")) {
        return SpamtestCondition;
    } else if (str == QLatin1String("true")) {
        return TrueCondition;
    } else if (str == QLatin1String("not")) {
        return NotCondition;
    } else if (str == QLatin1String("anyof")) {
        return AnyOf;
    } else if (str == QLatin1String("allof")) {
        return Allof;
    } else if (str == QLatin1String("virustest")) {
        return VirustestCondition;
    } else if (str == QLatin1String("break")) {
        return BreakAction;
    } else if (str == QLatin1String("convert")) {
        return ConvertAction; // TODO
    } else if (str == QLatin1String("discard")) {
        return DiscardAction;
    } else if (str == QLatin1String("enclose")) {
        return EncloseAction;
    } else if (str == QLatin1String("extracttext")) {
        return ExtracttextAction;
    } else if (str == QLatin1String("fileinto")) {
        return FileintoAction;
    } else if (str == QLatin1String("keep")) {
        return KeepAction;
    } else if (str == QLatin1String("notify")) {
        return NotifyAction;
    } else if (str == QLatin1String("redirect")) {
        return RedirectAction;
    } else if (str == QLatin1String("reject")) {
        return RejectAction;
    } else if (str == QLatin1String("ereject")) {
        return ERejectAction;
    } else if (str == QLatin1String("replace")) {
        return ReplaceAction;
    } else if (str == QLatin1String("return")) {
        return ReturnAction;
    } else if (str == QLatin1String("set")) {
        return SetvariableAction;
    } else if (str == QLatin1String("stop")) {
        return StopAction;
    } else if (str == QLatin1String("vacation")) {
        return VacationAction;
    } else if (str == QLatin1String("include")) {
        return Includes;
    } else if (str == QLatin1String("globalvariable")) {
        return GlobalVariable;
    } else if (str == QLatin1String("foreverypart")) {
        return ForEveryPart;
    } else if (str == QLatin1String("copy")) {
        return CopyExtension;
    } else if (str == QLatin1String("mboxmetadata")) {
        return MBoxMetaDataExtension;
    } else if (str == QLatin1String("subaddress")) {
        return SubAddressExtension;
    } else if (str == QLatin1String("imapflags")) {
        return ImapFlagsExtension;
    } else if (str == QLatin1String("editheader")) {
        return EditHeaderExtension;
    }

    // TODO
    return UnknownHelp;
}
