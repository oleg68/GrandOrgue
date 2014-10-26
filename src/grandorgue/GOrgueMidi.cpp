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

#include "GOrgueMidi.h"

#include "GOrgueEvent.h"
#include "GOrgueMidiListener.h"
#include "GOrgueMidiWXEvent.h"
#include "GOrgueRtHelpers.h"
#include "GOrgueSettings.h"
#include "RtMidi.h"
#include <wx/intl.h>
#include <wx/log.h>
#include <wx/stopwatch.h>
#include <vector>

#define DELETE_AND_NULL(x) do { if (x) { delete x; x = NULL; } } while (0)

BEGIN_EVENT_TABLE(GOrgueMidi, wxEvtHandler)
	EVT_MIDI(GOrgueMidi::OnMidiEvent)
END_EVENT_TABLE()

GOrgueMidi::GOrgueMidi(GOrgueSettings& settings) :
	m_Settings(settings),
	m_midi_in_device_map(),
	m_midi_in_devices(),
	m_midi_out_device_map(),
	m_midi_out_devices(),
	m_Listeners(),
	m_MidiRecorder(*this),
	m_MidiPlayer(*this)
{
	UpdateDevices();
}

void GOrgueMidi::UpdateDevices()
{
	std::vector<RtMidi::Api> apis;
	RtMidi::getCompiledApi(apis);

	try
	{

		for(unsigned j = 0; j < apis.size(); j++)
		{
			RtMidiIn midi_dev(apis[j], "GrandOrgue");
			for (unsigned i = 0; i < midi_dev.getPortCount(); i++)
			{
				wxString name = GOrgueRtHelpers::GetMidiApiPrefix(apis[j]) + wxString::FromAscii(midi_dev.getPortName(i).c_str());
				if (!m_midi_in_device_map.count(name))
				{
					MIDI_IN_DEVICE *t = new MIDI_IN_DEVICE;
					t->midi_in = new RtMidiIn(apis[j], "GrandOrgue");
					t->midi_in->ignoreTypes(false);
					t->channel_shift = 0;
					t->midi = this;
					t->rtmidi_port_no = i;
					t->active = false;
					t->name = name;
					t->id = GetMidiMap().GetDeviceByString(t->name);
					t->midi_in->setCallback(&MIDICallback, t);
					m_midi_in_devices.push_back(t);
			
					name.Replace(wxT("\\"), wxT("|"));
					m_midi_in_device_map[name] = m_midi_in_devices.size() - 1;
				}
				else
					m_midi_in_devices[m_midi_in_device_map[name]]->rtmidi_port_no = i;
			}
		}
	}
	catch (RtError &e)
	{
		wxString error = wxString::FromAscii(e.getMessage().c_str());
		wxLogError(_("RtMidi error: %s"), error.c_str());
	}

	try
	{
		for(unsigned j = 0; j < apis.size(); j++)
		{
			RtMidiOut midi_dev(apis[j], "GrandOrgue");
			for (unsigned i = 0; i <  midi_dev.getPortCount(); i++)
			{
				wxString name = GOrgueRtHelpers::GetMidiApiPrefix(apis[j]) + wxString::FromAscii(midi_dev.getPortName(i).c_str());
				if (!m_midi_out_device_map.count(name))
				{
					MIDI_OUT_DEVICE *t = new MIDI_OUT_DEVICE;
					t->midi_out = new RtMidiOut(apis[j], "GrandOrgue");
					t->rtmidi_port_no = i;
					t->active = false;
					t->name = name;
					t->id = GetMidiMap().GetDeviceByString(t->name);
					m_midi_out_devices.push_back(t);
			
					name.Replace(wxT("\\"), wxT("|"));
					m_midi_out_device_map[name] = m_midi_out_devices.size() - 1;
				}
				else
					m_midi_out_devices[m_midi_out_device_map[name]]->rtmidi_port_no = i;
			}
		}
	}
	catch (RtError &e)
	{
		wxString error = wxString::FromAscii(e.getMessage().c_str());
		wxLogError(_("RtMidi error: %s"), error.c_str());
	}
}


GOrgueMidi::~GOrgueMidi()
{
	try
	{
		/* dispose of all midi devices */
		for (unsigned i = 0; i < m_midi_in_devices.size(); i++)
		{
			m_midi_in_devices[i]->active = false;

			if (m_midi_in_devices[i]->midi_in)
				m_midi_in_devices[i]->midi_in->closePort();
			DELETE_AND_NULL(m_midi_in_devices[i]->midi_in);
		}

		for (unsigned i = 0; i < m_midi_out_devices.size(); i++)
		{
			m_midi_out_devices[i]->active = false;

			if (m_midi_out_devices[i]->midi_out)
				m_midi_out_devices[i]->midi_out->closePort();
			DELETE_AND_NULL(m_midi_out_devices[i]->midi_out);
		}
	}
	catch (RtError &e)
	{
		wxString error = wxString::FromAscii(e.getMessage().c_str());
		wxLogError(_("RtMidi error: %s"), error.c_str());
	}

}

void GOrgueMidi::Open()
{
	UpdateDevices();

	for (unsigned i = 0; i < m_midi_in_devices.size(); i++)
	{
		const wxString name = _("GrandOrgue Input");
		MIDI_IN_DEVICE& this_dev = *m_midi_in_devices[i];
		this_dev.merger.Clear();
		int channel_shift = m_Settings.GetMidiInDeviceChannelShift(this_dev.name);
		if (m_Settings.GetMidiInState(this_dev.name))
		{
			try
			{
				this_dev.active = false;

				assert(this_dev.midi_in);
				this_dev.channel_shift = channel_shift;
				this_dev.midi_in->closePort();
				this_dev.midi_in->openPort(this_dev.rtmidi_port_no, (const char*)name.fn_str());
				this_dev.active = true;
			}
			catch (RtError &e)
			{
				wxString error = wxString::FromAscii(e.getMessage().c_str());
				wxLogError(_("RtMidi error: %s"), error.c_str());
			}

		}
		else if (this_dev.active)
		{
			this_dev.active = false;
			this_dev.midi_in->closePort();
		}

	}

	for (unsigned i = 0; i < m_midi_out_devices.size(); i++)
	{
		const wxString name = _("GrandOrgue Output");
		MIDI_OUT_DEVICE& this_dev = *m_midi_out_devices[i];
		if (m_Settings.GetMidiOutState(this_dev.name))
		{
			try
			{
				this_dev.active = false;

				assert(this_dev.midi_out);
				this_dev.midi_out->closePort();
				this_dev.midi_out->openPort(this_dev.rtmidi_port_no, (const char*)name.fn_str());
				this_dev.active = true;
			}
			catch (RtError &e)
			{
				wxString error = wxString::FromAscii(e.getMessage().c_str());
				wxLogError(_("RtMidi error: %s"), error.c_str());
			}
		}
		else if (this_dev.active)
		{
			this_dev.active = false;
			this_dev.midi_out->closePort();
		}
	}

	m_MidiRecorder.SetOutputDevice(GetMidiMap().GetDeviceByString(m_Settings.GetMidiRecorderOutputDevice()));
}

std::vector<wxString> GOrgueMidi::GetInDevices()
{
	std::vector<wxString> list;
	for (std::map<wxString, int>::iterator it2 = m_midi_in_device_map.begin(); it2 != m_midi_in_device_map.end(); it2++)
		list.push_back(it2->first);
	return list;
}

std::vector<wxString> GOrgueMidi::GetOutDevices()
{
	std::vector<wxString> list;
	for (std::map<wxString, int>::iterator it2 = m_midi_out_device_map.begin(); it2 != m_midi_out_device_map.end(); it2++)
		list.push_back(it2->first);
	return list;
}

bool GOrgueMidi::HasActiveDevice()
{
	for (unsigned i = 0; i < m_midi_in_devices.size(); i++)
		if (m_midi_in_devices[i]->active)
			return true;

	return false;
}

void GOrgueMidi::ProcessMessage(std::vector<unsigned char>& msg, MIDI_IN_DEVICE* device)
{
	if (!device->active)
		return;

	GOrgueMidiEvent e;
	e.FromMidi(msg, GetMidiMap());
	if (e.GetMidiType() == MIDI_NONE)
		return;
	e.SetDevice(device->id);
	e.SetTime(wxGetLocalTimeMillis());

	if (!device->merger.Process(e))
		return;

	/* Compat stuff */
	if (e.GetChannel() != -1)
		e.SetChannel(((e.GetChannel() - 1 + device->channel_shift) & 0x0F) + 1);

	wxMidiEvent event(e);
	AddPendingEvent(event);
}

void GOrgueMidi::OnMidiEvent(wxMidiEvent& event)
{
	GOrgueMidiEvent e = event.GetMidiEvent();
	for(unsigned i = 0; i < m_Listeners.size(); i++)
		if (m_Listeners[i])
			m_Listeners[i]->Send(e);
}

void GOrgueMidi::Send(GOrgueMidiEvent& e)
{
	std::vector<std::vector<unsigned char>> msg;
	e.ToMidi(msg, GetMidiMap());
	for(unsigned i = 0; i < msg.size(); i++)
	{
		for(unsigned j = 0; j < m_midi_out_devices.size(); j++)
		{
			MIDI_OUT_DEVICE& this_dev = *m_midi_out_devices[j];
			if (!this_dev.active)
				continue;
			if (this_dev.id == e.GetDevice() || e.GetDevice() == 0)
				this_dev.midi_out->sendMessage(&msg[i]);
		}
	}
}

void GOrgueMidi::Register(GOrgueMidiListener* listener)
{
	if (!listener)
		return;
	for(unsigned i = 0; i < m_Listeners.size(); i++)
		if (m_Listeners[i] == listener)
			return;
	for(unsigned i = 0; i < m_Listeners.size(); i++)
		if (!m_Listeners[i])
		{
			m_Listeners[i] = listener;
			return;
		}
	m_Listeners.push_back(listener);
}

void GOrgueMidi::Unregister(GOrgueMidiListener* listener)
{
	for(unsigned i = 0; i < m_Listeners.size(); i++)
		if (m_Listeners[i] == listener)
		{
			m_Listeners[i] = NULL;
		}

}

void GOrgueMidi::MIDICallback (double timeStamp, std::vector<unsigned char>* msg, void* userData)
{
	MIDI_IN_DEVICE* m_dev = (MIDI_IN_DEVICE*)userData;
	m_dev->midi->ProcessMessage(*msg, m_dev);
}

GOrgueMidiRecorder& GOrgueMidi::GetMidiRecorder()
{
	return m_MidiRecorder;
}

GOrgueMidiPlayer& GOrgueMidi::GetMidiPlayer()
{
	return m_MidiPlayer;
}

GOrgueMidiMap& GOrgueMidi::GetMidiMap()
{
	return m_Settings.GetMidiMap();
}
