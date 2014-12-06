/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2014 GrandOrgue contributors (see AUTHORS)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "GOrgueSwitch.h"

#include "GrandOrgueFile.h"
#include <wx/intl.h>

GOrgueSwitch::GOrgueSwitch(GrandOrgueFile* organfile) :
	GOrgueDrawstop(organfile)
{
}

GOrgueSwitch::~GOrgueSwitch()
{
}

void GOrgueSwitch::ChangeState(bool on)
{
}

void GOrgueSwitch::SetupCombinationState()
{
	m_StoreDivisional = m_organfile->CombinationsStoreNonDisplayedDrawstops() || IsDisplayed();
	m_StoreGeneral = m_organfile->CombinationsStoreNonDisplayedDrawstops() || IsDisplayed();
}

wxString GOrgueSwitch::GetMidiType()
{
	return _("Drawstop");
}
