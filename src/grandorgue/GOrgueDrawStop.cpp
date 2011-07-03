/*
 * GrandOrgue - free pipe organ simulator based on MyOrgan
 *
 * MyOrgan 1.0.6 Codebase - Copyright 2006 Milan Digital Audio LLC
 * MyOrgan is a Trademark of Milan Digital Audio LLC
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#include "GOrgueDrawStop.h"

#include <wx/docview.h>
#include "GOrgueDisplayMetrics.h"
#include "GOrguePiston.h"
#include "GOrgueSound.h"
#include "GrandOrgue.h"
#include "GrandOrgueFile.h"
#include "GrandOrgueFrame.h"
#include "MIDIListenDialog.h"
#include "GOrgueMidi.h"
#include "OrganPanel.h"
#include "KeyConvert.h"

extern GrandOrgueFile* organfile;
extern GOrgueSound* g_sound;

GOrgueDrawstop::GOrgueDrawstop() :
	GOrgueControl(),
	DefaultToEngaged(false),
	DisplayInInvertedState(false),
	DispDrawstopRow(0),
	DispDrawstopCol(0),
	DispImageNum(0),
	StopControlMIDIKeyNumber(0)
{

}

GOrgueDrawstop::~GOrgueDrawstop()
{

}

void GOrgueDrawstop::Load(IniFileConfig& cfg, wxString group, GOrgueDisplayMetrics* displayMetrics)
{
	DisplayMetrics = displayMetrics;
	DispDrawstopRow = cfg.ReadInteger(group, wxT("DispDrawstopRow"), 1, 99 + DisplayMetrics->NumberOfExtraDrawstopRowsToDisplay());
	DispDrawstopCol = cfg.ReadInteger(group, wxT("DispDrawstopCol"), 1, DispDrawstopRow > 99 ? DisplayMetrics->NumberOfExtraDrawstopColsToDisplay() : DisplayMetrics->NumberOfDrawstopColsToDisplay());
	DefaultToEngaged = cfg.ReadBoolean(group, wxT("DefaultToEngaged"));
	DisplayInInvertedState = cfg.ReadBoolean(group, wxT("DisplayInInvertedState"));
	DispImageNum = cfg.ReadInteger(group, wxT("DispImageNum"), 1, 2);
	StopControlMIDIKeyNumber = cfg.ReadInteger(group, wxT("StopControlMIDIKeyNumber"), 0, 127, false);
	GOrgueControl::Load(cfg, group);
}

void GOrgueDrawstop::Save(IniFileConfig& cfg, bool prefix, wxString group)
{
	cfg.SaveHelper(prefix, group, wxT("DefaultToEngaged"), DefaultToEngaged ? wxT("Y") : wxT("N"));
	cfg.SaveHelper(prefix, group, wxT("StopControlMIDIKeyNumber"), StopControlMIDIKeyNumber);
}

unsigned GOrgueDrawstop::GetLayer()
{

	return 1;

}

unsigned GOrgueDrawstop::GetX()
{

	int x, y;
	DisplayMetrics->GetDrawstopBlitPosition(DispDrawstopRow, DispDrawstopCol, &x, &y);
	return x;

}

unsigned GOrgueDrawstop::GetY()
{

	int x, y;
	DisplayMetrics->GetDrawstopBlitPosition(DispDrawstopRow, DispDrawstopCol, &x, &y);
	return y;

}

unsigned GOrgueDrawstop::GetWidth()
{

	return 65;

}

unsigned GOrgueDrawstop::GetHeight()
{

	return 65;

}

void GOrgueDrawstop::Draw(wxDC& dc)
{

	int x, y;
	DisplayMetrics->GetDrawstopBlitPosition(DispDrawstopRow, DispDrawstopCol, &x, &y);

	// Draw the knob
	wxBitmap& knob_bitmap = DisplayMetrics->GetHW1Images().GetStopBitmap
		(((DispImageNum - 1) << 1) + (DisplayInInvertedState ^ DefaultToEngaged ? 1 : 0));
	dc.DrawBitmap(knob_bitmap, x, y, true);

	// Draw the label
	wxFont font = DisplayMetrics->GetControlLabelFont();
	font.SetPointSize(DispLabelFontSize);
	dc.SetFont(font);
	dc.SetTextForeground(DispLabelColour);
	wxRect rect(x, y + 1, 65, 64);

	OrganPanel::WrapText(dc, Name, 51);
	dc.DrawLabel(Name, rect, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

}

void GOrgueDrawstop::Push()
{

	Set(DefaultToEngaged ^ true);

};

void GOrgueDrawstop::MIDI(void)
{

	MIDIListenDialog dlg
		(::wxGetApp().frame
		,wxString(wxT("Drawstop Trigger"))
		,MIDIListenDialog::LSTN_DRAWSTOP
		,g_sound->GetMidi().GetStopMidiEvent() | StopControlMIDIKeyNumber
		);

	if (dlg.ShowModal() == wxID_OK)
	{

		StopControlMIDIKeyNumber = dlg.GetEvent() & 0x7F;
		::wxGetApp().m_docManager->GetCurrentDocument()->Modify(true);

	}

}

bool GOrgueDrawstop::Set(bool on)
{
	if (DefaultToEngaged == on)
		return on;
	DefaultToEngaged = on;
	wxCommandEvent event(wxEVT_DRAWSTOP, 0);
	event.SetClientData(this);
	::wxGetApp().frame->AddPendingEvent(event);
	for (int i = 0; i < organfile->GetNumberOfReversiblePistons(); i++)
	{
		if (organfile->GetPiston(i)->drawstop == this)
		{
			organfile->GetPiston(i)->DispImageNum = (organfile->GetPiston(i)->DispImageNum & 1) | (on ^ DisplayInInvertedState ? 2 : 0);
			wxCommandEvent event(wxEVT_PUSHBUTTON, 0);
			/*event.SetClientData(organfile->m_piston + i);*/ /* TODO: the equivilent of this may be wrong */
			event.SetClientData(organfile->GetPiston(i));
			::wxGetApp().frame->AddPendingEvent(event);
	    }
	}
#ifdef __VFD__
	GOrgueLCD_WriteLineTwo(Name, 2000);
#endif
	return !on;
}

void GOrgueDrawstop::MouseButtonDown(const unsigned x, const unsigned y, const GO_MouseButton button)
{

	int avg_radius = (int)(GetWidth() + GetHeight()) / 4;
	int dx = (int)x - (GetWidth() / 2);
	int dy = (int)y - (GetHeight() / 2);

	if (dx * dx + dy * dy > avg_radius * avg_radius)
		return;

	if (button == GO_IControl::GO_MB_RIGHT)
	{
		MIDI();
	}
	else
	{
		Push();
	}

}

void GOrgueDrawstop::OnKeyEvent(const int wx_key, const unsigned flags)
{

	if ((flags & KEY_EVENT_UP) || (flags & KEY_EVENT_ALT))
		return;

	if (WXKtoVK(wx_key) == ShortcutKey)
		Push();

}
