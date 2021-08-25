/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2019 GrandOrgue contributors (see AUTHORS)
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

#include "GOSoundThread.h"

#include "GOSoundScheduler.h"
#include "GOSoundWorkItem.h"
#include <iostream>
#include "mutex_locker.h"
#include <wx/log.h>

GOSoundThread::GOSoundThread(GOSoundScheduler* scheduler):
	GOrgueThread(),
	m_Scheduler(scheduler),
	m_Stop(false),
	m_Condition(m_Mutex)
{
	wxLogDebug(wxT("Create Thread"));
}

void GOSoundThread::Entry()
{
	while(!ShouldStop() && !m_Stop)
	{
		GOSoundWorkItem *next;
		do
		{
			next = m_Scheduler->GetNextGroup();
			if (next != NULL)
			{
			  m_CurrentGroup = next->GetGroup();
			  if (ShouldStop())
			    std::cout << "GOSoundThread::Entry: running group=" << m_CurrentGroup << std::endl;
			  next->Run();
			}
		}
		while (next != NULL);
		m_CurrentGroup = 0;

		GOMutexLocker lock(m_Mutex);
		if (ShouldStop() || m_Stop)
			break;
		m_Condition.Wait();
	}
	return;
}

void GOSoundThread::Run()
{
	Start();
}

void GOSoundThread::Wakeup()
{
	m_Condition.Signal();
}

void GOSoundThread::Delete()
{
	std::cout << "GOSoundThread::Delete" << std::endl;
	{
		std::cout << "  GOSoundThread::Delete before lock" << std::endl;
		GOMutexLocker lock(m_Mutex);
		std::cout << "  GOSoundThread::Delete after lock" << std::endl;
		m_Stop = true;
		m_Condition.Signal();
		std::cout << "  GOSoundThread::Delete after signal" << std::endl;
	}
	std::cout << "  GOSoundThread::Delete before Stop(); lastWorkingGroup=" << load_once(m_CurrentGroup) << std::endl;
	Stop();
	std::cout << "GOSoundThread::Delete finished" << std::endl;
}
