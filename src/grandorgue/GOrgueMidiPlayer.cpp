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

#include "GOrgueMidiPlayer.h"

#include "GOrgueEvent.h"
#include "GOrgueMidiEvent.h"
#include "GOrgueMidiMap.h"
#include "GOrgueMidiFileReader.h"
#include "GOrgueSetterButton.h"
#include "GOrgueSettings.h"
#include "GrandOrgueFile.h"
#include <wx/intl.h>

enum {
	ID_MIDI_PLAYER_PLAY = 0,
	ID_MIDI_PLAYER_STOP,
	ID_MIDI_PLAYER_PAUSE,
};

const struct ElementListEntry GOrgueMidiPlayer::m_element_types[] = {
	{ wxT("MidiPlayerPlay"), ID_MIDI_PLAYER_PLAY, false, true },
	{ wxT("MidiPlayerStop"), ID_MIDI_PLAYER_STOP, false, true },
	{ wxT("MidiPlayerPause"), ID_MIDI_PLAYER_PAUSE, false, true },
	{ wxT(""), -1, false, false },
};

const struct ElementListEntry* GOrgueMidiPlayer::GetButtonList()
{
	return m_element_types;
}

GOrgueMidiPlayer::GOrgueMidiPlayer(GrandOrgueFile* organfile) :
	m_organfile(organfile),
	m_content(),
	m_PlayingTime(organfile),
	m_Start(0),
	m_PlayingSeconds(0),
	m_Speed(1),
	m_IsPlaying(false),
	m_Pause(false)
{
	CreateButtons(m_organfile);
	Clear();
	m_DeviceID = m_organfile->GetSettings().GetMidiMap().GetDeviceByString(_("GrandOrgue MIDI Player"));
	UpdateDisplay();
}

GOrgueMidiPlayer::~GOrgueMidiPlayer()
{
	StopPlaying();
}

void GOrgueMidiPlayer::Load(GOrgueConfigReader& cfg)
{
	m_button[ID_MIDI_PLAYER_PLAY]->Init(cfg, wxT("MidiPlayerPlay"), _("PLAY"));
	m_button[ID_MIDI_PLAYER_STOP]->Init(cfg, wxT("MidiPlayerStop"), _("STOP"));
	m_button[ID_MIDI_PLAYER_PAUSE]->Init(cfg, wxT("MidiPlayerPause"), _("PAUSE"));
	m_PlayingTime.Init(cfg, wxT("MidiPlayerTime"), _("MIDI playing time"));
}

void GOrgueMidiPlayer::ButtonChanged(int id)
{
	switch(id)
	{
	case ID_MIDI_PLAYER_STOP:
		StopPlaying();
		break;

	case ID_MIDI_PLAYER_PLAY:
		Play();
		break;

	case ID_MIDI_PLAYER_PAUSE:
		Pause();
		break;
	}
}

void GOrgueMidiPlayer::Clear()
{
	StopPlaying();
	m_content.Clear();
}

void GOrgueMidiPlayer::LoadFile(const wxString& filename, unsigned manuals, bool pedal)
{
	Clear();
	GOrgueMidiFileReader reader(m_organfile->GetSettings().GetMidiMap());
	if (!reader.Open(filename))
	{
		GOMessageBox(wxString::Format(_("Failed to load %s"), filename.c_str()), _("MIDI Player"), wxOK | wxICON_ERROR, NULL);
		return;
	}
	if (!m_content.Load(reader, m_organfile->GetSettings().GetMidiMap(), manuals, pedal))
	{
		m_content.Clear();
		GOMessageBox(wxString::Format(_("Failed to load %s"), filename.c_str()), _("MIDI Player"), wxOK | wxICON_ERROR, NULL);
		return;
	}
	if (!reader.Close())
	{
		GOMessageBox(wxString::Format(_("Failed to decode %s"), filename.c_str()), _("MIDI Player"), wxOK | wxICON_ERROR, NULL);
		return;
	}
}

bool GOrgueMidiPlayer::IsLoaded()
{
	return m_content.IsLoaded();
}

void GOrgueMidiPlayer::Play()
{
	StopPlaying();
	m_content.Reset();
	m_Start = wxGetLocalTimeMillis();
	m_PlayingSeconds = 0;
	m_IsPlaying = IsLoaded();
	m_Pause = false;
	if (m_IsPlaying)
	{
		m_button[ID_MIDI_PLAYER_PLAY]->Display(true);
		UpdateDisplay();
		HandleTimer();
	}
	else
		StopPlaying();
}

void GOrgueMidiPlayer::Pause()
{
	if (!m_IsPlaying)
		return;
	if (m_Pause)
	{
		m_Pause = false;
		m_button[ID_MIDI_PLAYER_PAUSE]->Display(m_Pause);
		m_Start = wxGetLocalTimeMillis() - m_Start;
		HandleTimer();
	}
	else
	{
		m_Pause = true;
		m_button[ID_MIDI_PLAYER_PAUSE]->Display(m_Pause);
		m_Start = wxGetLocalTimeMillis() - m_Start;
		m_organfile->DeleteTimer(this);
	}
}

void GOrgueMidiPlayer::StopPlaying()
{
	if (m_IsPlaying)
	{
		for(unsigned i = 1; i < 16; i++)
		{
			GOrgueMidiEvent e;
			e.SetMidiType(MIDI_CTRL_CHANGE);
			e.SetChannel(i);
			e.SetKey(MIDI_CTRL_NOTES_OFF);
			e.SetValue(0);
			e.SetDevice(m_DeviceID);
			e.SetTime(wxGetLocalTimeMillis());
			m_organfile->ProcessMidi(e);
		}
	}

	m_IsPlaying = false;
	m_button[ID_MIDI_PLAYER_PLAY]->Display(false);
	m_button[ID_MIDI_PLAYER_PAUSE]->Display(false);
	UpdateDisplay();
	m_organfile->DeleteTimer(this);
}

bool GOrgueMidiPlayer::IsPlaying()
{
	return m_IsPlaying;
}

void GOrgueMidiPlayer::UpdateDisplay()
{
	if (!IsLoaded())
		m_PlayingTime.SetContent(_("<no file loaded>"));
	else if (!IsPlaying())
		m_PlayingTime.SetContent(_("-:--:--"));
	else
		m_PlayingTime.SetContent(wxString::Format(_("%d:%02d:%02d"), m_PlayingSeconds / 3600, (m_PlayingSeconds / 60) % 60, m_PlayingSeconds % 60));
}

void GOrgueMidiPlayer::HandleTimer()
{
	if (!m_IsPlaying)
		return;
	GOTime now = wxGetLocalTimeMillis();
	if (m_Start + m_Speed * (m_PlayingSeconds + 1) * 1000 <= now)
	{
		m_PlayingSeconds++;
		UpdateDisplay();
	}
	do
	{
		GOrgueMidiEvent e = m_content.GetCurrentEvent();
		if (e.GetTime() * m_Speed + m_Start <= now)
		{
			if (!m_content.Next())
			{
				StopPlaying();
				return;
			}
			e.SetDevice(m_DeviceID);
			e.SetTime(wxGetLocalTimeMillis());
			m_organfile->ProcessMidi(e);
		}
		else
		{
			GOTime next = e.GetTime() * m_Speed + m_Start;
			if (next > m_Start + m_Speed * (m_PlayingSeconds + 1) * 1000)
				next = m_Start + m_Speed * (m_PlayingSeconds + 1) * 1000;
			m_organfile->SetTimer(next, this);
			return;
		}
	}
	while(true);
}

GOrgueEnclosure* GOrgueMidiPlayer::GetEnclosure(const wxString& name, bool is_panel)
{
	return NULL;
}

GOrgueLabel* GOrgueMidiPlayer::GetLabel(const wxString& name, bool is_panel)
{
	if (is_panel)
		return NULL;

	if (name == wxT("MidiPlayerLabel"))
		return &m_PlayingTime;
	return NULL;
}
