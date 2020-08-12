/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionlist.h"
#include "sieveaction.h"
#include "sieveactiondiscard.h"
#include "sieveactionsetflags.h"
#include "sieveactionstop.h"
#include "sieveactionaddflags.h"
#include "sieveactionfileinto.h"
#include "sieveactionreject.h"
#include "sieveactionereject.h"
#include "sieveactionkeep.h"
#include "sieveactionredirect.h"
#include "sieveactionremoveflags.h"
#include "sieveactionnotify.h"
#include "sieveactiondeleteheader.h"
#include "sieveactionaddheader.h"
#include "sieveactionvacation.h"
#include "sieveactionenclose.h"
#include "sieveactionreplace.h"
#include "sieveactionextracttext.h"
#include "sieveactionbreak.h"
#include "sieveactionconvert.h"
#include "sieveactionsetvariable.h"
#include "sieveactionreturn.h"

QVector<KSieveUi::SieveAction *> KSieveUi::SieveActionList::actionList(SieveEditorGraphicalModeWidget *graphicalModeWidget)
{
    QVector<KSieveUi::SieveAction *> list;
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
