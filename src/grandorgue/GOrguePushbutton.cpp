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

#include "GOrguePushbutton.h"
#include "IniFileConfig.h"
#include "GrandOrgueFile.h"
#include "GrandOrgue.h"
#include "GrandOrgueFrame.h"
#include "GOrgueSound.h"
#include "MIDIListenDialog.h"
#include "GOrgueDisplayMetrics.h"
#include "GOrgueMidi.h"
#include "OrganPanel.h"
#include "KeyConvert.h"

extern GrandOrgueFile* organfile;
extern GOrgueSound* g_sound;

GOrguePushbutton::GOrguePushbutton(GOrgueDisplayMetrics& display_metrics) :
	GOrgueControl(),
	m_DisplayMetrics(display_metrics),
	m_ManualNumber(0),
	DispButtonRow(0),
	DispButtonCol(0),
	DispImageNum(0),
	MIDIProgramChangeNumber(0)
{

}

void GOrguePushbutton::MIDI(void)
{

	int event = 0xC000;
	if (m_ManualNumber > -1)
		event = g_sound->GetMidi().GetMidiEventByChannel(organfile->GetManual(m_ManualNumber)->GetMIDIInputNumber() + 7) ^ 0x5000;

	MIDIListenDialog dlg
		(::wxGetApp().frame
		,wxString(wxT("Pushbutton Trigger"))
		,(m_ManualNumber > -1) ? MIDIListenDialog::LSTN_DRAWSTOP : MIDIListenDialog::LSTN_NON_DRAWSTOP_BUTTON
		,event | (MIDIProgramChangeNumber - 1)
		);

	if (dlg.ShowModal() == wxID_OK)
	{
		MIDIProgramChangeNumber = (dlg.GetEvent() & 0x7F) + 1;
		::wxGetApp().m_docManager->GetCurrentDocument()->Modify(true);
	}

}

void GOrguePushbutton::Load(IniFileConfig& cfg, wxString group)
{
	DispButtonRow = cfg.ReadInteger(group, wxT("DispButtonRow"), 0, 99 + m_DisplayMetrics.NumberOfExtraButtonRows());
	DispButtonCol = cfg.ReadInteger(group, wxT("DispButtonCol"), 1, m_DisplayMetrics.NumberOfButtonCols());
	DispImageNum = cfg.ReadInteger(group, wxT("DispImageNum"), 1, 2);
	MIDIProgramChangeNumber = cfg.ReadInteger(group, wxT("MIDIProgramChangeNumber"), 1, 128);
	DispImageNum--;
	GOrgueControl::Load(cfg, group);
}

void GOrguePushbutton::Save(IniFileConfig& cfg, bool prefix, wxString group)
{
	cfg.SaveHelper(prefix, group, wxT("MIDIProgramChangeNumber"), MIDIProgramChangeNumber);
}

unsigned GOrguePushbutton::GetLayer()
{

	return 1;

}

unsigned GOrguePushbutton::GetX()
{

	int x, y;
	m_DisplayMetrics.GetPushbuttonBlitPosition(DispButtonRow, DispButtonCol, &x, &y);
	return x;

}

unsigned GOrguePushbutton::GetY()
{

	int x, y;
	m_DisplayMetrics.GetPushbuttonBlitPosition(DispButtonRow, DispButtonCol, &x, &y);
	return y;

}

unsigned GOrguePushbutton::GetWidth()
{

	return 31;

}

unsigned GOrguePushbutton::GetHeight()
{

	return 30;

}

void GOrguePushbutton::Draw(wxDC& dc)
{

	int x, y;
	m_DisplayMetrics.GetPushbuttonBlitPosition(DispButtonRow, DispButtonCol, &x, &y);

	// Draw the button
	wxBitmap& button_bitmap = m_DisplayMetrics.GetHW1Images().GetStopBitmap(DispImageNum + 4);
	dc.DrawBitmap(button_bitmap, x, y, true);

	// Draw the text
	wxFont font = m_DisplayMetrics.GetControlLabelFont();
	font.SetPointSize(DispLabelFontSize);
	dc.SetFont(font);
	dc.SetTextForeground(DispLabelColour);
	wxRect rect(x + 1, y + 1, 30, 29);

	OrganPanel::WrapText(dc, Name, 28);

	dc.DrawLabel(Name, rect, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

}

void GOrguePushbutton::MouseButtonDown(const unsigned x, const unsigned y, const GO_MouseButton button)
{

	if (button == MOUSE_CLICK_RIGHT)
	{
		MIDI();
	}
	else
	{
		Push();
	}

}

void GOrguePushbutton::OnKeyEvent(const int wx_key, const unsigned flags)
{

	if ((flags & KEY_EVENT_UP) || (flags & KEY_EVENT_ALT))
		return;

	if (WXKtoVK(wx_key) == ShortcutKey)
		Push();

}
