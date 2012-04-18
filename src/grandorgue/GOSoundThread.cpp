/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2012 GrandOrgue contributors (see AUTHORS)
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
 * MA 02111-1307, USA.
 */

#include "GOSoundThread.h"
#include "GOSoundEngine.h"

GOSoundThread::GOSoundThread(GOSoundEngine* engine, int min_sampler_id, int max_sampler_id, unsigned n_frames):
	wxThread(wxTHREAD_JOINABLE),
	m_MinSamplerID(min_sampler_id),
	m_MaxSamplerID(max_sampler_id),
	m_Engine(engine),
	m_FrameCount(n_frames),
	m_Stop(false),
	m_Condition(m_Mutex)
{
	wxLogDebug(wxT("Create Thread %d %d %d"), m_MinSamplerID, m_MaxSamplerID, m_FrameCount);
	Create();
	SetPriority(WXTHREAD_MAX_PRIORITY);
}

void* GOSoundThread::Entry()
{
	while(!TestDestroy() && !m_Stop)
	{
		for(int i = m_MaxSamplerID; i >= m_MinSamplerID; i--)
		{
			m_Engine->Process(i, m_FrameCount);
		}

		GOMutexLocker lock(m_Mutex);
		if (TestDestroy() || m_Stop)
			break;
		m_Condition.Wait();
	}
	return 0;
}


void GOSoundThread::Wakeup()
{
	m_Condition.Signal();
}

void GOSoundThread::Delete()
{
	{
		GOMutexLocker lock(m_Mutex);
		m_Stop = true;
		m_Condition.Signal();
	}
	wxThread::Delete();
}
