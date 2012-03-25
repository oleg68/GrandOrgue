/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2012 GrandOrgue contributors (see AUTHORS)
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
 * MA 02111-1307, USA.
 */

#include "GOGUIPanel.h"
#include "OrganDocument.h"
#include "GOrgueEvent.h"
#include "GOrgueSound.h"
#include "GrandOrgueID.h"
#include "GrandOrgueFile.h"

IMPLEMENT_DYNAMIC_CLASS(OrganDocument, wxDocument)

extern GOrgueSound* g_sound;

OrganDocument::OrganDocument() :
	m_OrganFileReady(false),
	m_organfile(NULL)
{
}

OrganDocument::~OrganDocument()
{
	CloseOrgan();
}

bool OrganDocument::OnCloseDocument()
{
    return wxDocument::OnCloseDocument();
}

bool OrganDocument::DoOpenDocument(const wxString& file)
{
	return DoOpenDocument(file, wxEmptyString);
}

bool OrganDocument::DoOpenDocument(const wxString& file, const wxString& file2)
{
	wxBusyCursor busy;

	CloseOrgan();
	bool open_sound = g_sound->OpenSound();

	/* abort if we failed to open the sound device */
	if (!open_sound)
		return false;

	m_organfile = new GrandOrgueFile(this, g_sound->GetSettings());
	wxString error = m_organfile->Load(file, file2);
	if (!error.IsEmpty())
	{
		if (error != wxT("!"))
		{
			wxLogError(wxT("%s\n"),error.c_str());
			wxMessageBox(error, _("Load error"), wxOK | wxICON_ERROR, NULL);
		}
		CloseOrgan();
		if (!file2.IsEmpty())
			DeleteAllViews();
		return false;
	}
	g_sound->PreparePlayback(m_organfile);
	if (m_organfile->GetVolume() != -121)
	{
		wxCommandEvent event(wxEVT_SETVALUE, ID_METER_AUDIO_SPIN);
		event.SetInt(m_organfile->GetVolume());
		wxTheApp->GetTopWindow()->GetEventHandler()->AddPendingEvent(event);

		g_sound->GetEngine().SetVolume(m_organfile->GetVolume());
	}
	m_OrganFileReady = true;

	SetTitle(m_organfile->GetChurchName());

	UpdateAllViews();

	for (unsigned i = 1; i < m_organfile->GetPanelCount(); i++)
		if (m_organfile->GetPanel(i)->InitialOpenWindow())
		{
			wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, ID_PANEL_FIRST + i - 1);
			wxTheApp->GetTopWindow()->GetEventHandler()->AddPendingEvent(event);
		}

	return true;
}

bool OrganDocument::DoSaveDocument(const wxString& file)
{
	m_organfile->SetVolume(g_sound->GetEngine().GetVolume());
	m_organfile->Save(file);
	return true;
}

void OrganDocument::CloseOrgan()
{
	g_sound->CloseSound();

	m_OrganFileReady = false;
	GOMutexLocker locker(m_lock);
	if (m_organfile)
	{
		delete m_organfile;
		m_organfile = 0;
	}
}

GrandOrgueFile* OrganDocument::GetOrganFile()
{
	return m_organfile;
}

void OrganDocument::OnMidiEvent(GOrgueMidiEvent& event)
{
	GOMutexLocker locker(m_lock);

	if (!m_OrganFileReady)
		return;

	if (m_organfile)
		m_organfile->ProcessMidi(event);
}
