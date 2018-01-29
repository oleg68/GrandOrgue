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

#include "MIDIEventDialog.h"

#include "MIDIEventKeyDialog.h"
#include "MIDIEventRecvDialog.h"
#include "MIDIEventSendDialog.h"
#include <wx/bookctrl.h>

BEGIN_EVENT_TABLE(MIDIEventDialog, wxPropertySheetDialog)
	EVT_BUTTON(wxID_APPLY, MIDIEventDialog::OnApply)
	EVT_BUTTON(wxID_OK, MIDIEventDialog::OnOK)
	EVT_BUTTON(wxID_CANCEL, MIDIEventDialog::OnCancel)
END_EVENT_TABLE()

MIDIEventDialog::MIDIEventDialog (GOrgueDocumentBase* doc, wxWindow* parent, wxString title, GOrgueSettings& settings, GOrgueMidiReceiver* event, GOrgueMidiSender* sender, GOrgueKeyReceiver* key, GOrgueMidiSender* division):
	wxPropertySheetDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize),
	GOrgueView(doc, this),
	m_recvPage(NULL),
	m_sendPage(NULL),
	m_sendDivisionPage(NULL),
	m_keyPage(NULL)
{
	CreateButtons(wxOK | wxCANCEL);

	wxBookCtrlBase* notebook = GetBookCtrl();

	if (event)
	{
		m_recvPage = new MIDIEventRecvDialog(notebook, event, settings);
		notebook->AddPage(m_recvPage,  _("Receive"));
	}
	if (sender)
	{
		m_sendPage = new MIDIEventSendDialog(notebook, sender, m_recvPage, settings);
		notebook->AddPage(m_sendPage,  _("Send"));
	}
	if (key)
	{
		m_keyPage = new MIDIEventKeyDialog(notebook, key);
		notebook->AddPage(m_keyPage,  _("Shortcut"));
	}
	if (division)
	{
		m_sendDivisionPage = new MIDIEventSendDialog(notebook, division, m_recvPage, settings);
		notebook->AddPage(m_sendDivisionPage,  _("Send Division Output"));
	}

	LayoutDialog();
}

MIDIEventDialog::~MIDIEventDialog()
{
}

void MIDIEventDialog::RegisterMIDIListener(GOrgueMidi* midi)
{
	if (m_recvPage)
		m_recvPage->RegisterMIDIListener(midi);
}

void MIDIEventDialog::OnApply(wxCommandEvent& event)
{
	DoApply();
}

void MIDIEventDialog::OnOK(wxCommandEvent& event)
{
	DoApply();
	Destroy();
}

void MIDIEventDialog::OnCancel(wxCommandEvent& event)
{
	Destroy();
}

void MIDIEventDialog::DoApply()
{
	if (m_recvPage)
		m_recvPage->DoApply();
	if (m_sendPage)
		m_sendPage->DoApply();
	if (m_sendDivisionPage)
		m_sendDivisionPage->DoApply();
	if (m_keyPage)
		m_keyPage->DoApply();
}
