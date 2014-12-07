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

#ifndef GORGUEENCLOSURE_H_
#define GORGUEENCLOSURE_H_

#include "GOrgueEventHandler.h"
#include "GOrgueMidiConfigurator.h"
#include "GOrgueMidiReceiver.h"
#include "GOrgueMidiSender.h"
#include "GOrgueSaveableObject.h"
#include <wx/string.h>

class GOrgueConfigReader;
class GOrgueConfigWriter;
class GOrgueMidiEvent;
class GrandOrgueFile;

class GOrgueEnclosure : private GOrgueEventHandler, private GOrgueSaveableObject, 
	public GOrgueMidiConfigurator
{
private:
	GOrgueMidiReceiver m_midi;
	GOrgueMidiSender m_sender;
	GrandOrgueFile* m_organfile;
	int m_AmpMinimumLevel;
	int m_MIDIInputNumber;
	int m_MIDIValue;
	wxString m_Name;
	bool m_Displayed1;
	bool m_Displayed2;

	void ProcessMidi(const GOrgueMidiEvent& event);
	void HandleKey(int key);

	void Save(GOrgueConfigWriter& cfg);

public:

	GOrgueEnclosure(GrandOrgueFile* organfile);
	virtual ~GOrgueEnclosure();
	void Init(GOrgueConfigReader& cfg, wxString group, wxString Name, unsigned def_value);
	void Load(GOrgueConfigReader& cfg, wxString group, int enclosure_nb);
	void Set(int n);
	GOrgueMidiReceiver& GetMidiReceiver();
	GOrgueMidiSender& GetMidiSender();
	const wxString& GetName();
	int GetValue();
	int GetMIDIInputNumber();
	float GetAttenuation();

	void Scroll(bool scroll_up);
	bool IsDisplayed(bool new_format);
	virtual void Abort();
	virtual void PreparePlayback();
	virtual void PrepareRecording();
	void SetElementID(int id);

	wxString GetMidiType();
	wxString GetMidiName();
	void ShowConfigDialog();
};

#endif /* GORGUEENCLOSURE_H_ */
