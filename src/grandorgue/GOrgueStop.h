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

#ifndef GORGUESTOP_H
#define GORGUESTOP_H

#include "GOrgueDrawStop.h"
#include <wx/string.h>
#include <vector>

class GOrgueRank;

class GOrgueStop : public GOrgueDrawstop
{
private:
	typedef struct
	{
		GOrgueRank* Rank;
		unsigned StopID;
		unsigned FirstAccessibleKeyNumber;
		unsigned FirstPipeNumber;
		unsigned PipeCount;
	} RankInfo;
	std::vector<RankInfo> m_RankInfo;
	std::vector<unsigned> m_KeyVelocity;
	unsigned m_FirstMidiNoteNumber;
	unsigned m_FirstAccessiblePipeLogicalKeyNumber;
	unsigned m_NumberOfAccessiblePipes;

	void SetRankKey(unsigned key, unsigned velocity);
	void ChangeState(bool on);
	void SetupCombinationState();

	void AbortPlayback();
	void PreparePlayback();
	void StartPlayback();

public:
	GOrgueStop(GrandOrgueFile* organfile, unsigned first_midi_note_number);
	GOrgueRank* GetRank(unsigned index);
	void Load(GOrgueConfigReader& cfg, wxString group);
	void SetKey(unsigned note, unsigned velocity);
	~GOrgueStop(void);

	unsigned IsAuto() const;

	wxString GetMidiType();
};

#endif /* GORGUESTOP_H_ */
