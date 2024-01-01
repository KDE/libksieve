/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionlist.h"
#include "sieveaction.h"
#include "sieveactionaddflags.h"
#include "sieveactionaddheader.h"
#include "sieveactionbreak.h"
#include "sieveactionconvert.h"
#include "sieveactiondeleteheader.h"
#include "sieveactiondiscard.h"
#include "sieveactionenclose.h"
#include "sieveactionereject.h"
#include "sieveactionextracttext.h"
#include "sieveactionfileinto.h"
#include "sieveactionkeep.h"
#include "sieveactionnotify.h"
#include "sieveactionredirect.h"
#include "sieveactionreject.h"
#include "sieveactionremoveflags.h"
#include "sieveactionreplace.h"
#include "sieveactionreturn.h"
#include "sieveactionsetflags.h"
#include "sieveactionsetvariable.h"
#include "sieveactionstop.h"
#include "sieveactionvacation.h"

QList<KSieveUi::SieveAction *> KSieveUi::SieveActionList::actionList(SieveEditorGraphicalModeWidget *graphicalModeWidget)
{
    QList<KSieveUi::SieveAction *> list;
    list.append(new KSieveUi::SieveActionDiscard(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionStop(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionSetFlags(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionAddFlags(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionRemoveFlags(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionFileInto(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionReject(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionEReject(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionKeep(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionRedirect(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionNotify(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionDeleteHeader(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionAddHeader(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionVacation(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionEnclose(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionReplace(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionExtractText(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionBreak(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionConvert(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionSetVariable(graphicalModeWidget));
    list.append(new KSieveUi::SieveActionReturn(graphicalModeWidget));
    return list;
}
