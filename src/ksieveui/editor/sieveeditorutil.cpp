/*
  SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "sieveeditorutil.h"
using namespace Qt::Literals::StringLiterals;


QUrl KSieveUi::SieveEditorUtil::helpUrl(KSieveUi::SieveEditorUtil::HelpVariableName type)
{
    switch (type) {
    case AddressCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc5228#page-16"_s);
    case BodyCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc5173"_s);
    case ConvertCondition:
        return {}; // TODO
    case CurrentdateCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc5260#page-6"_s);
    case DateCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc5260#page-4"_s);
    case EnvelopeCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc3028#page-24"_s);
    case EnvironmentCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc5183"_s);
    case ExistsCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc3028#page-25"_s);
    case FalseCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc3028#page-25"_s);
    case HasflagCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc5232#page-5"_s);
    case HeaderCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc5228#page-9"_s);
    case IhaveCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc5463"_s);
    case MailboxexistsCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc5490#page-2"_s);
    case MetadataexistsCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc5490#page-6"_s);
    case MetadataCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc5490#page-5"_s);
    case ServermetadataexistsCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc5490#page-4"_s);
    case ServermetadataCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc5490#page-5"_s);
    case SizeCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc3028#page-26"_s);
    case SpamtestCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc3685#page-3"_s);
    case TrueCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc3028#page-26"_s);
    case NotCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc3028#page-26"_s);
    case AnyOf:
        return QUrl(u"https://tools.ietf.org/html/rfc3028#page-24"_s);
    case Allof:
        return QUrl(u"https://tools.ietf.org/html/rfc3028#page-23"_s);
    case VirustestCondition:
        return QUrl(u"https://tools.ietf.org/html/rfc3685#page-4"_s);
    case AbstracteditheaderAction:
        return QUrl(u"https://tools.ietf.org/html/rfc5228#page-9"_s);
    case AddflagsAction:
        return QUrl(u"https://tools.ietf.org/html/rfc5232#page-5"_s);
    case AddheaderAction:
    case DeleteheaderAction:
    case EditHeaderExtension:
        return QUrl(u"https://tools.ietf.org/html/rfc5293"_s);
    case BreakAction:
        return QUrl(u"https://tools.ietf.org/html/rfc5703#page-3"_s);
    case ConvertAction:
        return {}; // TODO
    case DiscardAction:
        return QUrl(u"https://tools.ietf.org/html/rfc3028#page-22"_s);
    case EncloseAction:
        return QUrl(u"https://tools.ietf.org/html/rfc5703#page-10"_s);
    case ExtracttextAction:
        return QUrl(u"https://tools.ietf.org/html/rfc5703#page-11"_s);
    case FileintoAction:
        return QUrl(u"https://tools.ietf.org/html/rfc3028#page-20"_s);
    case KeepAction:
        return QUrl(u"https://tools.ietf.org/html/rfc3028#page-21"_s);
    case NotifyAction:
        return QUrl(u"https://tools.ietf.org/html/rfc5435#page-3"_s);
    case RedirectAction:
        return QUrl(u"https://tools.ietf.org/html/rfc3028#page-20"_s);
    case RejectAction:
        return QUrl(u"https://tools.ietf.org/html/rfc3028#page-20"_s);
    case ERejectAction:
        return QUrl(u"https://tools.ietf.org/html/rfc5429#section-2.1"_s);
    case RemoveflagsAction:
        return QUrl(u"https://tools.ietf.org/html/rfc5232#page-5"_s);
    case ReplaceAction:
        return QUrl(u"https://tools.ietf.org/html/rfc5703#page-8"_s);
    case ReturnAction:
        return QUrl(u"https://tools.ietf.org/html/rfc6609#page-7"_s);
    case SetflagsAction:
        return QUrl(u"https://tools.ietf.org/html/rfc5232#page-4"_s);
    case SetvariableAction:
        return QUrl(u"https://tools.ietf.org/html/rfc5229"_s);
    case StopAction:
        return QUrl(u"https://tools.ietf.org/html/rfc3028#page-19"_s);
    case VacationAction:
        return QUrl(u"https://tools.ietf.org/html/rfc5230#page-3"_s);
    case GlobalVariable:
        return QUrl(u"https://tools.ietf.org/html/rfc5229"_s);
    case Includes:
        return QUrl(u"https://tools.ietf.org/html/rfc6609#page-4"_s);
    case ForEveryPart:
        return QUrl(u"https://tools.ietf.org/html/rfc5703#page-3"_s);
    case CopyExtension:
        return QUrl(u"https://tools.ietf.org/html/rfc3894"_s);
    case MBoxMetaDataExtension:
        return QUrl(u"https://tools.ietf.org/html/rfc5490#page-2"_s);
    case SubAddressExtension:
        return QUrl(u"https://tools.ietf.org/html/rfc5233"_s);
    case ImapFlagsExtension:
        return QUrl(u"https://tools.ietf.org/html/rfc5232"_s);
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
