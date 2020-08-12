/*  -*- c++ -*-
    error.cpp

    This file is part of KSieve,
    the KDE internet mail/usenet news message filtering library.
    SPDX-FileCopyrightText: 2002-2003 Marc Mutz <mutz@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#include <ksieve/error.h>

#include <KLocalizedString> // i18n

#include <climits> // UINT_MAX

namespace KSieve {
const char *Error::typeToString(Type t)
{
    switch (t) {
#define CASE(x) case x: \
    return #x
        CASE(None);
        CASE(Custom);

        CASE(CRWithoutLF);
        CASE(SlashWithoutAsterisk);
        CASE(IllegalCharacter);
        CASE(UnexpectedCharacter);
        CASE(NoLeadingDigits);
        CASE(NonCWSAfterTextColon);

        CASE(NumberOutOfRange);
        CASE(InvalidUTF8);

        CASE(UnfinishedBracketComment);
        CASE(PrematureEndOfMultiLine);
        CASE(PrematureEndOfQuotedString);
        CASE(PrematureEndOfStringList);
        CASE(PrematureEndOfTestList);
        CASE(PrematureEndOfBlock);
        CASE(MissingWhitespace);
        CASE(MissingSemicolonOrBlock);

        CASE(ExpectedBlockOrSemicolon);
        CASE(ExpectedCommand);
        CASE(ConsecutiveCommasInStringList);
        CASE(ConsecutiveCommasInTestList);
        CASE(MissingCommaInTestList);
        CASE(MissingCommaInStringList);
        CASE(NonStringInStringList);
        CASE(NonCommandInCommandList);
        CASE(NonTestInTestList);

        CASE(RequireNotFirst);
        CASE(RequireMissingForCommand);
        CASE(RequireMissingForTest);
        CASE(RequireMissingForComparator);
        CASE(UnsupportedCommand);
        CASE(UnsupportedTest);
        CASE(UnsupportedComparator);
        CASE(TestNestingTooDeep);
        CASE(BlockNestingTooDeep);
        CASE(InvalidArgument);
        CASE(ConflictingArguments);
        CASE(ArgumentsRepeated);
        CASE(CommandOrderingConstraintViolation);

        CASE(IncompatibleActionsRequested);
        CASE(MailLoopDetected);
        CASE(TooManyActions);
#undef CASE
    default:
        return "<unknown>";
    }
}

QString Error::asString() const
{
    QString err;
    switch (type()) {
    case None:
        return QString();
    case Custom:
        return mStringOne;

    // Parse errors:
    case CRWithoutLF:
        err = i18n("Parse error: Carriage Return (CR) without Line Feed (LF)");
        break;
    case SlashWithoutAsterisk:
        err = i18n("Parse error: Unquoted Slash ('/') without Asterisk ('*'). "
                   "Broken Comment?");
        break;
    case IllegalCharacter:
        err = i18n("Parse error: Illegal Character");
        break;
    case UnexpectedCharacter:
        err = i18n("Parse error: Unexpected Character, probably a missing space?");
        break;
    case NoLeadingDigits:
        err = i18n("Parse error: Tag Name has leading Digits");
        break;
    case NonCWSAfterTextColon:
        err = i18n("Parse error: Only whitespace and #comments may "
                   "follow \"text:\" on the same line");
        break;
    case NumberOutOfRange:
        err = i18n("Parse error: Number out of Range (must be smaller than %1)", UINT_MAX);
        break;
    case InvalidUTF8:
        err = i18n("Parse error: Invalid UTF-8 sequence");
        break;
    case PrematureEndOfMultiLine:
        err = i18n("Parse error: Premature end of Multiline String (did you forget the '.'?)");
        break;
    case PrematureEndOfQuotedString:
        err = i18n("Parse error: Premature end of Quoted String (missing closing '\"')");
        break;
    case PrematureEndOfStringList:
        err = i18n("Parse error: Premature end of String List (missing closing ']')");
        break;
    case PrematureEndOfTestList:
        err = i18n("Parse error: Premature end of Test List (missing closing ')')");
        break;
    case PrematureEndOfBlock:
        err = i18n("Parse error: Premature end of Block (missing closing '}')");
        break;
    case MissingWhitespace:
        err = i18n("Parse error: Missing Whitespace");
        break;
    case MissingSemicolonOrBlock:
        err = i18n("Parse error: Missing ';' or Block");
        break;
    case ExpectedBlockOrSemicolon:
        err = i18n("Parse error: Expected ';' or '{', got something else");
        break;
    case ExpectedCommand:
        err = i18n("Parse error: Expected Command, got something else");
        break;
    case ConsecutiveCommasInStringList:
        err = i18n("Parse error: Trailing, Leading or Duplicate Commas in String List");
        break;
    case ConsecutiveCommasInTestList:
        err = i18n("Parse error: Trailing, Leading or Duplicate Commas in Test List");
        break;
    case MissingCommaInStringList:
        err = i18n("Parse error: Missing ',' between Strings in String List");
        break;
    case MissingCommaInTestList:
        err = i18n("Parse error: Missing ',' between Tests in Test List");
        break;
    case NonCommandInCommandList:
        err = i18n("Parse error: Expected Command, got something else");
        break;
    case NonStringInStringList:
        err = i18n("Parse error: Only Strings allowed in String Lists");
        break;
    case NonTestInTestList:
        err = i18n("Parse error: Only Tests allowed in Test Lists");
        break;

    // validity errors:
    case RequireNotFirst:
        err = i18n("\"require\" must be first command");
        break;
    case RequireMissingForCommand:
        err = i18n("\"require\" missing for command \"%1\"", mStringOne);
        break;
    case RequireMissingForTest:
        err = i18n("\"require\" missing for test \"%1\"", mStringOne);
        break;
    case RequireMissingForComparator:
        err = i18n("\"require\" missing for comparator \"%1\"", mStringOne);
        break;
    case UnsupportedCommand:
        err = i18n("Command \"%1\" not supported", mStringOne);
        break;
    case UnsupportedTest:
        err = i18n("Test \"%1\" not supported", mStringOne);
        break;
    case UnsupportedComparator:
        err = i18n("Comparator \"%1\" not supported", mStringOne);
        break;
    case TestNestingTooDeep:
        err = i18n("Site Policy Limit Violation: Test nesting too deep (max. %1)", mStringOne.toUInt());
        break;
    case BlockNestingTooDeep:
        err = i18n("Site Policy Limit Violation: Block nesting too deep (max. %1)", mStringOne.toUInt());
        break;
    case InvalidArgument:
        err = i18n("Invalid Argument \"%1\" to \"%2\"", mStringOne, mStringTwo);
        break;
    case ConflictingArguments:
        err = i18n("Conflicting Arguments: \"%1\" and \"%2\"", mStringOne, mStringTwo);
        break;
    case ArgumentsRepeated:
        err = i18n("Argument \"%1\" Repeated", mStringOne);
        break;
    case CommandOrderingConstraintViolation:
        err = i18n("Command \"%1\" violates command ordering constraints", mStringOne);
        break;

    // runtime errors:
    case IncompatibleActionsRequested:
        err = i18n("Incompatible Actions \"%1\" and \"%2\" requested", mStringOne, mStringTwo);
        break;
    case MailLoopDetected:
        err = i18n("Mail Loop detected");
        break;
    case TooManyActions:
        err = i18n("Site Policy Limit Violation: Too many Actions requested (max. %1)", mStringOne.toUInt());
        break;
    default:
        err = i18n("Unknown error");
        break;
    }

    return err;
}
} // namespace KSieve
