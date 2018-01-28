/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2018 GrandOrgue contributors (see AUTHORS)
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

#include "GOrgueLoadThread.h"

#include "GOrgueAlloc.h"
#include "GOrgueCacheObject.h"
#include "GOrgueEventDistributor.h"
#include "GOrgueMemoryPool.h"

GOrgueLoadThread::GOrgueLoadThread(GOrgueEventDistributor& objs, GOrgueMemoryPool& pool, atomic_uint& pos) :
	GOrgueThread(),
	m_Objects(objs),
	m_Pos(pos),
	m_pool(pool),
	m_Error(),
	m_OutOfMemory(false)
{
}

GOrgueLoadThread::~GOrgueLoadThread()
{
	Stop();
}

void GOrgueLoadThread::checkResult()
{
	Wait();
	if (m_Error != wxEmptyString)
		throw m_Error;
	if (m_OutOfMemory)
		throw GOrgueOutOfMemory();
}

void GOrgueLoadThread::Run()
{
	Start();
}

void GOrgueLoadThread::Entry()
{
	while (!ShouldStop())
	{
		if (m_pool.IsPoolFull())
			return;
		unsigned pos = m_Pos.fetch_add(1);
		try
		{
			GOrgueCacheObject* obj = m_Objects.GetCacheObject(pos);
			if (!obj)
				return;
			obj->LoadData();
		}
		catch (GOrgueOutOfMemory e)
		{
			m_OutOfMemory = true;
			return;
		}
		catch (wxString error)
		{
			m_Error = error;
			return;
		}
	}
	return;
}
