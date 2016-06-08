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

#include "GOrgueRank.h"

#include "GOrgueConfigReader.h"
#include "GOrgueDocument.h"
#include "GOrgueDummyPipe.h"
#include "GOrgueReferencePipe.h"
#include "GOrgueSettings.h"
#include "GOrgueSoundingPipe.h"
#include "GOrgueWindchest.h"
#include "GrandOrgueFile.h"
#include <wx/intl.h>

GOrgueRank::GOrgueRank(GrandOrgueFile* organfile) :
	m_organfile(organfile),
	m_Name(),
	m_Pipes(),
	m_StopCount(0),
	m_Velocity(),
	m_Velocities(),
	m_FirstMidiNoteNumber(0),
	m_Percussive(false),
	m_WindchestGroup(0),
	m_HarmonicNumber(8),
	m_PitchCorrection(0),
	m_MinVolume(100),
	m_MaxVolume(100),
	m_RetuneRank(true),
	m_sender(organfile, MIDI_SEND_MANUAL),
	m_PipeConfig(NULL, organfile, NULL)
{
	m_organfile->RegisterMidiConfigurator(this);
}

GOrgueRank::~GOrgueRank()
{
}

void GOrgueRank::Resize()
{
	m_Velocity.resize(m_Pipes.size());
	m_Velocities.resize(m_Pipes.size());
	for(unsigned i = 0; i < m_Velocities.size(); i++)
		m_Velocities[i].resize(m_StopCount);
}

void GOrgueRank::Init(GOrgueConfigReader& cfg, wxString group, wxString name, int first_midi_note_number, unsigned windchest_nr)
{
	m_organfile->RegisterSaveableObject(this);
	m_group = group;

	m_FirstMidiNoteNumber = first_midi_note_number;
	m_Name = name;

	m_PipeConfig.Init(cfg, group, wxEmptyString);
	m_WindchestGroup = windchest_nr;
	m_Percussive = false;
	m_HarmonicNumber = 8;
	m_PitchCorrection = 0;
	m_MinVolume = 100;
	m_MaxVolume = 100;
	m_RetuneRank = false;

	GOrgueWindchest* windchest = m_organfile->GetWindchest(m_WindchestGroup - 1);
	windchest->AddRank(this);
	m_PipeConfig.SetParent(&windchest->GetPipeConfig());

	m_Pipes.clear();
	m_sender.Load(cfg, group + wxT("Rank"), m_organfile->GetSettings().GetMidiMap());
	m_PipeConfig.SetName(GetName());
	Resize();
}

void GOrgueRank::Load(GOrgueConfigReader& cfg, wxString group, int first_midi_note_number)
{
	m_organfile->RegisterSaveableObject(this);
	m_group = group;

	m_FirstMidiNoteNumber = cfg.ReadInteger(ODFSetting, group, wxT("FirstMidiNoteNumber"), 0, 256, first_midi_note_number < 0, first_midi_note_number);
	m_Name = cfg.ReadString(ODFSetting, group, wxT("Name"), true);

	unsigned number_of_logical_pipes       = cfg.ReadInteger(ODFSetting, group, wxT("NumberOfLogicalPipes"), 1, 192);
	m_PipeConfig.Load(cfg, group, wxEmptyString);
	m_WindchestGroup                       = cfg.ReadInteger(ODFSetting, group, wxT("WindchestGroup"), 1, m_organfile->GetWindchestGroupCount());
	m_Percussive                           = cfg.ReadBoolean(ODFSetting, group, wxT("Percussive"));
	m_HarmonicNumber                       = cfg.ReadInteger(ODFSetting, group, wxT("HarmonicNumber"), 1, 1024, false, 8);
	m_PitchCorrection                      = cfg.ReadFloat(ODFSetting, group, wxT("PitchCorrection"), -1200, 1200, false, 0);
	m_MinVolume                            = cfg.ReadFloat(ODFSetting, group, wxT("MinVelocityVolume"), 0, 1000, false, 100);
	m_MaxVolume                            = cfg.ReadFloat(ODFSetting, group, wxT("MaxVelocityVolume"), 0, 1000, false, 100);
	m_RetuneRank = cfg.ReadBoolean(ODFSetting, group, wxT("AcceptsRetuning"), false, true);

	GOrgueWindchest* windchest = m_organfile->GetWindchest(m_WindchestGroup - 1);
	windchest->AddRank(this);
	m_PipeConfig.SetParent(&windchest->GetPipeConfig());

	m_Pipes.clear();
	for (unsigned i = 0; i < number_of_logical_pipes; i++)
	{
		wxString buffer;
		buffer.Printf(wxT("Pipe%03u"), i + 1);
		wxString name = cfg.ReadStringTrim(ODFSetting, group, buffer);
		if (name == wxT("DUMMY"))
		{
			m_Pipes.push_back(new GOrgueDummyPipe (m_organfile, this, m_FirstMidiNoteNumber + i));
		}
		else if (name.StartsWith(wxT("REF:")))
		{
			m_Pipes.push_back(new GOrgueReferencePipe (m_organfile, this, m_FirstMidiNoteNumber + i));
		}
		else
		{
			m_Pipes.push_back(new GOrgueSoundingPipe (m_organfile, this, m_Percussive, m_WindchestGroup, m_FirstMidiNoteNumber + i, m_HarmonicNumber, m_PitchCorrection, m_MinVolume, m_MaxVolume, m_RetuneRank));
		}
               m_Pipes[i]->Load(cfg, group, buffer);
	}
	m_sender.Load(cfg, group + wxT("Rank"), m_organfile->GetSettings().GetMidiMap());
	m_PipeConfig.SetName(GetName());
	Resize();
}

void GOrgueRank::AddPipe(GOrguePipe* pipe)
{
	m_Pipes.push_back(pipe);
	Resize();
}

void GOrgueRank::Save(GOrgueConfigWriter& cfg)
{
	m_sender.Save(cfg, m_group + wxT("Rank"), m_organfile->GetSettings().GetMidiMap());
}

unsigned GOrgueRank::RegisterStop(GOrgueStop* stop)
{
	unsigned id = m_StopCount++;
	Resize();
	return id;
}

void GOrgueRank::SetKey(int note, unsigned velocity, unsigned stopID)
{
	if (note < 0 || note >= (int)m_Pipes.size())
		return;

	if (m_Velocities[note][stopID] <= velocity && velocity <= m_Velocity[note])
	{
		m_Velocities[note][stopID] = velocity;
		return;
	}
	if (velocity >= m_Velocity[note])
	{
		m_Velocities[note][stopID] = velocity;
		m_Velocity[note] = velocity;
	}
	else
	{
		m_Velocities[note][stopID] = velocity;
		m_Velocity[note] = m_Velocities[note][0];
		for(unsigned i = 1; i < m_Velocities[note].size(); i++)
			if (m_Velocity[note] < m_Velocities[note][i])
				m_Velocity[note] = m_Velocities[note][i];
	}
	m_Pipes[note]->Set(m_Velocity[note]);
}

GOrguePipe* GOrgueRank::GetPipe(unsigned index)
{
	return m_Pipes[index];
}

unsigned GOrgueRank::GetPipeCount()
{
	return m_Pipes.size();
}

const wxString& GOrgueRank::GetName()
{
	return m_Name;
}

GOrguePipeConfigNode& GOrgueRank::GetPipeConfig()
{
	return m_PipeConfig;
}

void GOrgueRank::SetTemperament(const GOrgueTemperament& temperament)
{
	for(unsigned j = 0; j < m_Pipes.size(); j++)
		m_Pipes[j]->SetTemperament(temperament);
}

void GOrgueRank::AbortPlayback()
{
}

void GOrgueRank::PreparePlayback()
{
	m_sender.ResetKey();
	for(unsigned i = 0; i < m_Velocity.size(); i++)
		m_Velocity[i] = 0;
	for(unsigned i = 0; i < m_Velocities.size(); i++)
		for(unsigned j = 0; j < m_Velocities[i].size(); j++)
			m_Velocities[i][j] = 0;
}

void GOrgueRank::StartPlayback()
{
}

void GOrgueRank::PrepareRecording()
{
}

void GOrgueRank::SendKey(unsigned note, unsigned velocity)
{
	m_sender.SetKey(note, velocity);
}

wxString GOrgueRank::GetMidiType()
{
	return _("Rank");
}

wxString GOrgueRank::GetMidiName()
{
	return m_Name;
}

void GOrgueRank::ShowConfigDialog()
{
	wxString title = wxString::Format(_("Midi-Settings for %s - %s"), GetMidiType().c_str(), GetMidiName().c_str());

	m_organfile->GetDocument()->ShowMIDIEventDialog(this, title, NULL, &m_sender, NULL, NULL);
}

