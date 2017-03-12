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

#ifndef GORGUEDOCUMENT_H
#define GORGUEDOCUMENT_H

#include "GOrgueMidiListener.h"
#include "mutex.h"
#include <wx/string.h>
#include <vector>

class GOrgueKeyReceiver;
class GOrgueMidiEvent;
class GOrgueMidiReceiver;
class GOrgueMidiSender;
class GOrgueOrgan;
class GOrgueProgressDialog;
class GOrgueSound;
class GOrgueView;
class GrandOrgueFile;

class GOrgueDocument : protected GOrgueMidiCallback
{
public:
	typedef enum { ORGAN_DIALOG, MIDI_EVENT, MIDI_LIST, PANEL } WindowType;
private:
	typedef struct {
		WindowType type;
		void* data;
		GOrgueView* window;
	} WindowInfo;

	GOMutex m_lock;
	bool m_OrganFileReady;
	GrandOrgueFile* m_organfile;
	GOrgueSound& m_sound;
	GOrgueMidiListener m_listener;
	std::vector<WindowInfo> m_Windows;
	bool m_modified;

	void OnMidiEvent(const GOrgueMidiEvent& event);

	void SyncState();
	void CloseOrgan();

public:
	GOrgueDocument(GOrgueSound* sound);
	~GOrgueDocument();

	bool IsModified();
	void Modify(bool modified);

	void ShowPanel(unsigned id);
	void ShowOrganDialog();
	void ShowMIDIEventDialog(void* element, wxString title, GOrgueMidiReceiver* event, GOrgueMidiSender* sender, GOrgueKeyReceiver* key, GOrgueMidiSender* division = NULL);
	void ShowMidiList();

	bool Load(GOrgueProgressDialog* dlg, const GOrgueOrgan& organ);
	bool Save();
	bool Export(const wxString& cmb);
	bool Import(GOrgueProgressDialog* dlg, const GOrgueOrgan& organ, const wxString& cmb);
	bool ImportCombination(const wxString& cmb);
	bool Revert(GOrgueProgressDialog* dlg);
	bool UpdateCache(GOrgueProgressDialog* dlg, bool compress);

	bool WindowExists(WindowType type, void* data);
	bool showWindow(WindowType type, void* data);
	void registerWindow(WindowType type, void* data, GOrgueView *window);
	void unregisterWindow(GOrgueView* window);

	GrandOrgueFile* GetOrganFile();
};

#endif
