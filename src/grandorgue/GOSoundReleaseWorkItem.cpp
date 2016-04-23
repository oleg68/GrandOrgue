/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2016 GrandOrgue contributors (see AUTHORS)
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

#include "GOSoundReleaseWorkItem.h"

#include "GOSoundEngine.h"

GOSoundReleaseWorkItem::GOSoundReleaseWorkItem(GOSoundEngine& sound_engine) :
	m_engine(sound_engine),
	m_Stop(false)
{
}

unsigned GOSoundReleaseWorkItem::GetGroup()
{
	return RELEASE;
}

unsigned GOSoundReleaseWorkItem::GetCost()
{
	return 0;
}

bool GOSoundReleaseWorkItem::GetRepeat()
{
	return true;
}

void GOSoundReleaseWorkItem::Clear()
{
	m_List.Clear();
}

void GOSoundReleaseWorkItem::Reset()
{
	m_Stop = false;
	m_Cnt = 0;
}

void GOSoundReleaseWorkItem::Add(GO_SAMPLER* sampler)
{
	m_List.Put(sampler);
}

void GOSoundReleaseWorkItem::Run()
{
	GO_SAMPLER* sampler;
	while((sampler = m_List.Get()))
	{
		m_Cnt.fetch_add(1);
		m_engine.ProcessRelease(sampler);
		if (m_Stop && m_Cnt > 10)
			break;
	}
}

void GOSoundReleaseWorkItem::Exec()
{
	m_Stop = true;
	Run();
	GO_SAMPLER* sampler;
	while((sampler = m_List.Get()))
		m_engine.PassSampler(sampler);
}
