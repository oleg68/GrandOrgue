/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2015 GrandOrgue contributors (see AUTHORS)
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

#ifndef SETTINGSAUDIOOUTPUT_H
#define SETTINGSAUDIOOUTPUT_H

#include "GOrgueSoundDevInfo.h"
#include "SettingsAudioGroup.h"
#include <wx/panel.h>
#include <vector>

class AudioItemData;
class GOrgueSound;
class wxButton;
class wxTreeCtrl;
class wxTreeEvent;
class wxTreeItemId;

class SettingsAudioOutput : public wxPanel
{
	enum {
		ID_OUTPUT_LIST = 200,
		ID_OUTPUT_ADD,
		ID_OUTPUT_DEL,
		ID_OUTPUT_CHANGE,
		ID_OUTPUT_PROPERTIES,
		ID_OUTPUT_DEFAULT,
	};

private:
	GOrgueSound& m_Sound;
	GOAudioGroupCallback& m_GroupCallback;
	wxTreeCtrl* m_AudioOutput;
	wxButton* m_Add;
	wxButton* m_Del;
	wxButton* m_Change;
	wxButton* m_Properties;
	wxButton* m_Default;
	std::vector<GOrgueSoundDevInfo> m_DeviceList;

	AudioItemData* GetObject(const wxTreeItemId& id);
	wxTreeItemId GetDeviceNode(const wxString& name);
	wxTreeItemId GetChannelNode(const wxTreeItemId& audio, unsigned channel);
	wxTreeItemId GetGroupNode(const wxTreeItemId& channel, const wxString& name, bool left);

	wxTreeItemId AddDeviceNode(wxString name);
	wxTreeItemId AddDeviceNode(wxString name, unsigned latency);
	wxTreeItemId AddChannelNode(const wxTreeItemId& audio, unsigned channel);
	wxTreeItemId AddGroupNode(const wxTreeItemId& channel, const wxString& name, bool left);
	void UpdateDevice(const wxTreeItemId& dev);
	void UpdateVolume(const wxTreeItemId& group, float volume);
	void UpdateButtons();

	std::vector<wxString> GetRemainingAudioDevices();
	std::vector<std::pair<wxString, bool> > GetRemainingAudioGroups(const wxTreeItemId& channel);

	void OnOutputChanged(wxTreeEvent& event);
	void OnOutputAdd(wxCommandEvent& event);
	void OnOutputDel(wxCommandEvent& event);
	void OnOutputChange(wxCommandEvent& event);
	void OnOutputProperties(wxCommandEvent& event);
	void OnOutputDefault(wxCommandEvent& event);

public:
	SettingsAudioOutput(GOrgueSound& sound, GOAudioGroupCallback& callback, wxWindow* parent);

	void Save();

	DECLARE_EVENT_TABLE()
};

#endif
