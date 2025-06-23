/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vacationutils.h"
#include "vacationscriptextractor.h"
#include <KIdentityManagementCore/Identity>
#include <KIdentityManagementCore/IdentityManager>
#include <ksievecore/sieve-vacation.h>

#include <KLocalizedString>
#include <QDate>
#include <QLocale>
#include <QRegularExpression>

using KMime::Types::AddrSpecList;
using namespace KSieveCore;

static inline QString dotstuff(QString s) // krazy:exclude=passbyvalue
{
    if (s.startsWith(u'.')) {
        return u'.' + s.replace(QLatin1StringView("\n."), u"\n.."_s);
    } else {
        return s.replace(QLatin1StringView("\n."), u"\n.."_s);
    }
}

static inline QString stringReplace(QString s)
{
    static QRegularExpression reg(u"[\n\t]+"_s);
    s.replace(reg, u" "_s);
    return s.replace(u'\"', u"\\\""_s);
}

QString VacationUtils::defaultSubject()
{
    return i18n("Out of office till %1", QLocale().toString(QDate::currentDate().addDays(1)));
}

QString KSieveCore::VacationUtils::mailAction(KSieveCore::VacationUtils::MailAction action)
{
    switch (action) {
    case Keep:
        return i18n("Keep");
    case Discard:
        return i18n("Discard");
    case Sendto:
        return i18n("Redirect to");
    case CopyTo:
        return i18n("Copy to");
    }
    Q_UNREACHABLE();
    return {};
}

KSieveCore::VacationUtils::MailAction KSieveCore::VacationUtils::defaultMailAction()
{
    return KSieveCore::VacationUtils::Keep;
}

QString KSieveCore::VacationUtils::defaultMessageText()
{
    return i18n(
        "I am out of office till %1.\n"
        "\n"
        "In urgent cases, please contact Mrs. \"vacation replacement\"\n"
        "\n"
        "email: \"email address of vacation replacement\"\n"
        "phone: +49 711 1111 11\n"
        "fax.:  +49 711 1111 12\n"
        "\n"
        "Yours sincerely,\n"
        "-- \"enter your name and email address here\"\n",
        QLocale().toString(QDate::currentDate().addDays(1)));
}

int VacationUtils::defaultNotificationInterval()
{
    return 7; // days
}

KMime::Types::AddrSpecList VacationUtils::defaultMailAliases()
{
    KMime::Types::AddrSpecList sl;
    KIdentityManagementCore::IdentityManager *manager = KIdentityManagementCore::IdentityManager::self();
    KIdentityManagementCore::IdentityManager::ConstIterator end(manager->end());
    for (KIdentityManagementCore::IdentityManager::ConstIterator it = manager->begin(); it != end; ++it) {
        if (!(*it).primaryEmailAddress().isEmpty()) {
            KMime::Types::Mailbox a;
            a.fromUnicodeString((*it).primaryEmailAddress());
            sl.push_back(a.addrSpec());
        }
        const QStringList lstEmails = (*it).emailAliases();
        for (const QString &email : lstEmails) {
            KMime::Types::Mailbox a;
            a.fromUnicodeString(email);
            sl.push_back(a.addrSpec());
        }
    }

    return sl;
}

bool VacationUtils::defaultSendForSpam()
{
    return KSieveCore::VacationSettings::outOfOfficeReactToSpam();
}

QString VacationUtils::defaultDomainName()
{
    return KSieveCore::VacationSettings::outOfOfficeDomain();
}

QDate VacationUtils::defaultStartDate()
{
    return QDate::currentDate();
}

QDate VacationUtils::defaultEndDate()
{
    return defaultStartDate().addDays(7);
}

VacationUtils::Vacation VacationUtils::parseScript(const QString &script)
{
    KSieveCore::VacationUtils::Vacation vacation;
    if (script.trimmed().isEmpty()) {
        vacation.valid = false;
        vacation.active = false;
        vacation.mailAction = VacationUtils::defaultMailAction();
        vacation.messageText = VacationUtils::defaultMessageText();
        vacation.subject = VacationUtils::defaultSubject();
        vacation.notificationInterval = VacationUtils::defaultNotificationInterval();
        vacation.aliases = VacationUtils::defaultMailAliases();
        vacation.sendForSpam = VacationUtils::defaultSendForSpam();
        vacation.reactOndomainName = VacationUtils::defaultDomainName();
        return vacation;
    }

    // The trimmed() call below prevents parsing errors. The
    // slave somehow omits the last \n, which results in a lone \r at
    // the end, leading to a parse error.
    const QByteArray scriptUTF8 = script.trimmed().toUtf8();
    qCDebug(LIBKSIEVECORE_LOG) << "scriptUtf8 = \"" + scriptUTF8 + "\"";
    KSieve::Parser parser(scriptUTF8.begin(), scriptUTF8.begin() + scriptUTF8.length());
    VacationDataExtractor vdx;
    SpamDataExtractor sdx;
    DomainRestrictionDataExtractor drdx;
    DateExtractor dx;
    KSieveExt::MultiScriptBuilder tsb(&vdx, &sdx, &drdx, &dx);
    parser.setScriptBuilder(&tsb);
    if (!parser.parse() || !vdx.commandFound()) {
        vacation.active = false;
        vacation.valid = false;
        return vacation;
    }
    vacation.valid = true;
    vacation.active = vdx.active();
    vacation.mailAction = vdx.mailAction();
    vacation.mailActionRecipient = vdx.mailActionRecipient();
    vacation.messageText = vdx.messageText().trimmed();
    if (!vdx.subject().isEmpty()) {
        vacation.subject = vdx.subject().trimmed();
    }
    vacation.notificationInterval = vdx.notificationInterval();
    vacation.aliases = KMime::Types::AddrSpecList();
    const QStringList lstAliases = vdx.aliases();
    for (const QString &alias : lstAliases) {
        KMime::Types::Mailbox a;
        a.fromUnicodeString(alias);
        vacation.aliases.append(a.addrSpec());
    }

    if (!vacation.active && !vdx.ifComment().isEmpty()) {
        const QByteArray newScript = QByteArrayLiteral("if ") + vdx.ifComment().toUtf8() + QByteArrayLiteral("{vacation;}");
        tsb = KSieveExt::MultiScriptBuilder(&sdx, &drdx, &dx);
        KSieve::Parser activeScriptParser(newScript.begin(), newScript.begin() + newScript.length());
        activeScriptParser.setScriptBuilder(&tsb);
        if (!activeScriptParser.parse()) {
            vacation.valid = false;
            return vacation;
        }
    }

    vacation.sendForSpam = !sdx.found();
    vacation.reactOndomainName = drdx.domainName();
    vacation.startDate = dx.startDate();
    vacation.startTime = dx.startTime();
    vacation.endDate = dx.endDate();
    vacation.endTime = dx.endTime();

    return vacation;
}

QString KSieveCore::VacationUtils::composeScript(const Vacation &vacation)
{
    QStringList condition;
    QStringList require;

    require << u"vacation"_s;

    if (vacation.startDate.isValid() || vacation.endDate.isValid()) {
        require << u"date"_s;
        require << u"relational"_s;
    }

    if (vacation.startDate.isValid()) {
        if (vacation.startTime.isValid()) {
            const QDateTime start(vacation.startDate, vacation.startTime);
            condition.append(u"currentdate :value \"ge\" \"iso8601\" \"%1\""_s.arg(start.toString(Qt::ISODate)));
        } else {
            condition.append(u"currentdate :value \"ge\" \"date\" \"%1\""_s.arg(vacation.startDate.toString(Qt::ISODate)));
        }
    }

    if (vacation.endDate.isValid()) {
        if (vacation.endTime.isValid()) {
            const QDateTime end(vacation.endDate, vacation.endTime);
            condition.append(u"currentdate :value \"le\" \"iso8601\" \"%1\""_s.arg(end.toString(Qt::ISODate)));
        } else {
            condition.append(u"currentdate :value \"le\" \"date\" \"%1\""_s.arg(vacation.endDate.toString(Qt::ISODate)));
        }
    }

    if (!vacation.sendForSpam) {
        condition.append(u"not header :contains \"X-Spam-Flag\" \"YES\""_s);
    }

    if (!vacation.reactOndomainName.isEmpty()) {
        condition.append(u"address :domain :contains \"from\" \"%1\""_s.arg(vacation.reactOndomainName));
    }

    QString addressesArgument;
    QStringList aliases;
    if (!vacation.aliases.empty()) {
        addressesArgument += u":addresses [ "_s;
        QStringList sl;
        sl.reserve(vacation.aliases.count());
        AddrSpecList::const_iterator end = vacation.aliases.constEnd();
        for (AddrSpecList::const_iterator it = vacation.aliases.begin(); it != end; ++it) {
            sl.push_back(u'"' + (*it).asString().replace(u'\\', u"\\\\"_s).replace(u'"', u"\\\""_s) + u'"');
            aliases.push_back((*it).asString());
        }
        addressesArgument += sl.join(QLatin1StringView(", ")) + u" ] "_s;
    }

    QString sVacation(u"vacation "_s);
    sVacation += addressesArgument;
    if (vacation.notificationInterval > 0) {
        sVacation += u":days %1 "_s.arg(vacation.notificationInterval);
    }

    if (!vacation.subject.trimmed().isEmpty()) {
        sVacation += u":subject \"%1\" "_s.arg(stringReplace(vacation.subject).trimmed());
    }

    sVacation += u"text:\n"_s;
    sVacation += dotstuff(vacation.messageText.isEmpty() ? VacationUtils::defaultMessageText() : vacation.messageText);
    sVacation += u"\n.\n;"_s;

    switch (vacation.mailAction) {
    case VacationUtils::Keep:
        break;
    case VacationUtils::Discard:
        sVacation += u"\ndiscard;"_s;
        break;
    case VacationUtils::Sendto:
        sVacation += QLatin1StringView("\nredirect \"") + vacation.mailActionRecipient + QLatin1StringView("\";");
        break;
    case VacationUtils::CopyTo:
        require << u"copy"_s;
        sVacation += QLatin1StringView("\nredirect :copy \"") + vacation.mailActionRecipient + QLatin1StringView("\";");
        break;
    }

    QString script = u"require [\"%1\"];\n\n"_s.arg(require.join(u"\", \""_s));

    if (condition.isEmpty()) {
        if (vacation.active) {
            script += sVacation;
        } else {
            script += u"if false\n{\n\t"_s;
            script += sVacation;
            script += u"\n}"_s;
        }
    } else {
        if (vacation.active) {
            script += u"if allof(%1)\n{\n\t"_s.arg(condition.join(u", "_s));
        } else {
            script += u"if false # allof(%1)\n{\n\t"_s.arg(condition.join(u", "_s));
        }
        script += sVacation;
        script += u"\n}"_s;
    }

    script += u"\n"_s;

    return script;
}

QString KSieveCore::VacationUtils::mergeRequireLine(const QString &script, const QString &scriptUpdate)
{
    const QByteArray scriptUTF8 = script.trimmed().toUtf8();
    if (scriptUTF8.isEmpty()) {
        return scriptUpdate;
    }

    const QByteArray scriptUpdateUTF8 = scriptUpdate.trimmed().toUtf8();
    if (scriptUpdateUTF8.isEmpty()) {
        return script;
    }

    KSieve::Parser parser(scriptUTF8.begin(), scriptUTF8.begin() + scriptUTF8.length());
    KSieve::Parser parserUpdate(scriptUpdateUTF8.begin(), scriptUpdateUTF8.begin() + scriptUpdateUTF8.length());
    RequireExtractor rx;
    RequireExtractor rxUpdate;
    parser.setScriptBuilder(&rx);
    parserUpdate.setScriptBuilder(&rxUpdate);

    int insert(0);
    QStringList lines = script.split(u'\n');
    QSet<QString> requirements;

    if (parser.parse() && rx.commandFound()) {
        insert = rx.lineStart();
        const int endOld(rx.lineEnd());
        for (int i = insert; i <= endOld; ++i) {
            lines.removeAt(insert);
        }
        const auto requirementsSet = rx.requirements();
        requirements = QSet<QString>(requirementsSet.begin(), requirementsSet.end());
    }

    if (parserUpdate.parse() && rxUpdate.commandFound()) {
        const auto requirementsSet = rxUpdate.requirements();
        requirements += QSet<QString>(requirementsSet.begin(), requirementsSet.end());
    }

    const int requirementscount = requirements.count();
    if (requirementscount > 1) {
        QStringList req = requirements.values();
        req.sort();
        lines.insert(insert, u"require [\"%1\"];"_s.arg(req.join(u"\", \""_s)));
    } else if (requirementscount == 1) {
        lines.insert(insert, u"require \"%1\";"_s.arg(requirements.values().constFirst()));
    }

    return lines.join(u'\n');
}

QString KSieveCore::VacationUtils::updateVacationBlock(const QString &oldScript, const QString &newScript)
{
    const QByteArray oldScriptUTF8 = oldScript.trimmed().toUtf8();
    if (oldScriptUTF8.isEmpty()) {
        return newScript;
    }

    const QByteArray newScriptUTF8 = newScript.trimmed().toUtf8();
    if (newScriptUTF8.isEmpty()) {
        return oldScript;
    }

    KSieve::Parser parserOld(oldScriptUTF8.begin(), oldScriptUTF8.begin() + oldScriptUTF8.length());
    KSieve::Parser parserNew(newScriptUTF8.begin(), newScriptUTF8.begin() + newScriptUTF8.length());
    VacationDataExtractor vdxOld;
    VacationDataExtractor vdxNew;
    RequireExtractor rx;
    KSieveExt::MultiScriptBuilder tsb(&vdxOld, &rx);
    parserOld.setScriptBuilder(&tsb);
    parserNew.setScriptBuilder(&vdxNew);

    int startOld(0);

    QStringList lines = oldScript.split(u'\n');

    if (parserOld.parse() && vdxOld.commandFound()) {
        startOld = vdxOld.lineStart();
        const int endOld(vdxOld.lineEnd());
        for (int i = startOld; i <= endOld; ++i) {
            lines.removeAt(startOld);
        }
    } else {
        if (rx.commandFound()) { // after require
            startOld = rx.lineEnd() + 1;
        } else {
            startOld = 0;
        }
    }

    if (parserNew.parse() && vdxNew.commandFound()) {
        const int startNew(vdxNew.lineStart());
        const int endNew(vdxNew.lineEnd());
        QStringList linesNew = newScript.split(u'\n');
        for (int i = endNew; i >= startNew; --i) {
            lines.insert(startOld, linesNew.at(i));
        }
    }

    return lines.join(u'\n');
}
