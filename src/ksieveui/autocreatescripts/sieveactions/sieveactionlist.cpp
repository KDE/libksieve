/*
   Copyright (C) 2013-2016 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
