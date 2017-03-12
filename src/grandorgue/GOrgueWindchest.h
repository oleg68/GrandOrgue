/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2017 GrandOrgue contributors (see AUTHORS)
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

#ifndef GORGUEWINDCHEST_H
#define GORGUEWINDCHEST_H

#include "GOrguePipeConfigTreeNode.h"
#include "GOrguePlaybackStateHandler.h"
#include <wx/string.h>
#include <vector>

class GOrgueConfigReader;
class GOrgueEnclosure;
class GOrguePipeWindchestCallback;
class GOrgueRank;
class GOrgueTremulant;
class GrandOrgueFile;

class GOrgueWindchest : private GOrguePlaybackStateHandler
{
private:
	GrandOrgueFile* m_organfile;
	wxString m_Name;
	float m_Volume;
	std::vector<GOrgueEnclosure*> m_enclosure;
	std::vector<unsigned> m_tremulant;
	std::vector<GOrgueRank*> m_ranks;
	std::vector<GOrguePipeWindchestCallback*> m_pipes;
	GOrguePipeConfigTreeNode m_PipeConfig;

	void AbortPlayback();
	void StartPlayback();
	void PreparePlayback();
	void PrepareRecording();

public:
	GOrgueWindchest(GrandOrgueFile* organfile);

	void Init(GOrgueConfigReader& cfg, wxString group, wxString name);
	void Load(GOrgueConfigReader& cfg, wxString group, unsigned index);
	void UpdateTremulant(GOrgueTremulant* tremulant);
	void UpdateVolume();
	float GetVolume();
	unsigned GetTremulantCount();
	unsigned GetTremulantId(unsigned index);
	unsigned GetRankCount();
	GOrgueRank* GetRank(unsigned index);
	void AddRank(GOrgueRank* rank);
	void AddPipe(GOrguePipeWindchestCallback* pipe);
	void AddEnclosure(GOrgueEnclosure* enclosure);
	const wxString& GetName();
	GOrguePipeConfigNode& GetPipeConfig();
};

#endif /* GORGUEWINDCHEST_H_ */
