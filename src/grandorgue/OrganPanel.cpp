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

#include "OrganPanel.h"

#include <algorithm>
#include <wx/event.h>
#include <wx/mstream.h>
#include "GOrgueCoupler.h"
#include "GOrgueDrawStop.h"
#include "GOrgueDisplayMetrics.h"
#include "GOrgueDivisional.h"
#include "GOrgueDivisionalCoupler.h"
#include "GOrgueDrawStop.h"
#include "GOrgueEnclosure.h"
#include "GOrgueGeneral.h"
#include "GOrgueLabel.h"
#include "GOrgueMeter.h"
#include "GOrgueMidi.h"
#include "GOrguePiston.h"
#include "GOrguePushbutton.h"
#include "GOrgueSound.h"
#include "GOrgueStop.h"
#include "GOrgueTremulant.h"
#include "GrandOrgue.h"
#include "GrandOrgueFile.h"
#include "GrandOrgueFrame.h"
#include "GrandOrgueID.h"
#include "KeyConvert.h"
#include "OrganDocument.h"

BEGIN_EVENT_TABLE(OrganPanel, wxPanel)
EVT_ERASE_BACKGROUND(OrganPanel::OnErase)
EVT_LEFT_DOWN(OrganPanel::OnMouseLeftDown)
EVT_LEFT_DCLICK(OrganPanel::OnMouseLeftDown)
EVT_RIGHT_DOWN(OrganPanel::OnMouseRightDown)
EVT_RIGHT_DCLICK(OrganPanel::OnMouseRightDown)
EVT_MOUSEWHEEL(OrganPanel::OnMouseScroll)
EVT_KEY_DOWN(OrganPanel::OnKeyCommand)
EVT_PAINT(OrganPanel::OnPaint)

END_EVENT_TABLE()

extern GrandOrgueFile* organfile;
extern GOrgueSound* g_sound;
extern const unsigned char* ImageLoader_Wood[];
extern int c_ImageLoader_Wood[];

OrganPanel::OrganPanel(wxWindow* parent) :
	wxPanel(parent, wxID_ANY,wxDefaultPosition, wxDefaultSize,  wxTAB_TRAVERSAL, wxT("panel"))
{
}

void OrganPanel::TileWood(wxDC& dc, int which, int sx, int sy, int cx, int cy)
{
	int x, y;

	wxImage wood_image = organfile->GetDisplayMetrics()->GetHW1Images().GetWoodBitmap((which - 1) >> 1).ConvertToImage();
	if ((which - 1) & 1)
		wood_image.Rotate90();
	wxBitmap wood(wood_image);

	dc.SetClippingRegion(sx, sy, cx, cy);
	for (y = sy & 0xFFFFFF00; y < sy + cy; y += 256)
		for (x = sx & 0xFFFFFF00; x < sx + cx; x += 256)
			dc.DrawBitmap(wood, x, y, false);
	dc.DestroyClippingRegion();
}

void OrganPanel::OnUpdate(wxView *WXUNUSED(sender), wxObject *hint)
{

	m_ClientOrigin = GetClientAreaOrigin();

	if (!organfile)
		return;

	m_DisplayMetrics = organfile->GetDisplayMetrics();
	m_ClientBitmap = wxBitmap(m_DisplayMetrics->GetScreenWidth(), m_DisplayMetrics->GetScreenHeight());
	wxMemoryDC dc;
	dc.SelectObject(m_ClientBitmap);
	organfile->GetOrganScreen().Draw(dc, NULL);

	GetParent()->SetClientSize
		(m_DisplayMetrics->GetScreenWidth()
		,m_DisplayMetrics->GetScreenHeight()
		);

	SetSize
		(m_DisplayMetrics->GetScreenWidth()
		,m_DisplayMetrics->GetScreenHeight()
		);

	GetParent()->Center(wxBOTH);
	GetParent()->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	GetParent()->Refresh();

}

void OrganPanel::OnErase(wxEraseEvent& event)
{
	if (!m_ClientBitmap.Ok() || !organfile || !m_DisplayMetrics->GetJambLeftRightWidth())
	{
		event.Skip();
		return;
	}
	wxDC* dc = event.GetDC();
	OnDraw(dc);
}
void OrganPanel::OnPaint(wxPaintEvent& event)
{
	if (!m_ClientBitmap.Ok() || !organfile || !m_DisplayMetrics->GetJambLeftRightWidth())
	{
		event.Skip();
		return;
	}
	wxPaintDC dc(this);
	OnDraw((wxDC*)&dc);
}

void OrganPanel::OnDrawstop(wxCommandEvent& event)
{

	if (!organfile)
		return;

	wxMemoryDC mdc;
	mdc.SelectObject(m_ClientBitmap);
	wxClientDC dc(this);
	dc.SetDeviceOrigin(m_ClientOrigin.x, m_ClientOrigin.y);

	if (!m_ClientBitmap.Ok() || !m_DisplayMetrics->GetJambLeftRightWidth())
		return;

	GO_IControl* control = static_cast<GO_IControl*>(event.GetClientData());
	organfile->GetOrganScreen().Draw(mdc, control);
	dc.Blit(control->GetX(), control->GetY(), control->GetWidth(), control->GetHeight(), &mdc, control->GetX(), control->GetY());

}

void OrganPanel::OnNoteOnOff(wxCommandEvent& event)
{

	if (!m_ClientBitmap.Ok() || !organfile || !m_DisplayMetrics->GetJambLeftRightWidth())
		return;

	wxMemoryDC mdc;
	mdc.SelectObject(m_ClientBitmap);
	wxClientDC dc(this);
	dc.SetDeviceOrigin(m_ClientOrigin.x, m_ClientOrigin.y);

	GOrgueManual* man = organfile->GetManual(event.GetInt());
	if (man->IsDisplayed())
	{
		man->DrawKey(mdc, event.GetExtraLong());
		man->DrawKey( dc, event.GetExtraLong());
	}

}

void OrganPanel::OnMouseLeftDown(wxMouseEvent& event)
{

	if (!organfile)
	{
		event.Skip();
		return;
	}

	organfile->GetOrganScreen().MouseButtonDown(event.GetX(), event.GetY(), MOUSE_CLICK_LEFT);
	event.Skip();

}

void OrganPanel::OnMouseRightDown(wxMouseEvent& event)
{

	if (!organfile)
	{
		event.Skip();
		return;
	}

	organfile->GetOrganScreen().MouseButtonDown(event.GetX(), event.GetY(), MOUSE_CLICK_RIGHT);
	event.Skip();

}

void OrganPanel::OnMouseScroll(wxMouseEvent& event)
{

	if (!organfile)
	{
		event.Skip();
		return;
	}

	organfile->GetOrganScreen().Scroll(event.GetX(), event.GetY(), event.GetWheelRotation());
	event.Skip();

}

void OrganPanel::WrapText(wxDC& dc, wxString& string, int width)
{

	wxString str = string;

	wxChar *ptr = (wxChar*)str.c_str();
	wxChar *p = ptr, *lastspace = 0;
	wxCoord cx, cy;

	while (p)
	{
		p = wxStrchr(p, ' ');
		if (p)
		{
			*p = 0;
			dc.GetTextExtent(ptr, &cx, &cy);
			*p = ' ';
		}
		else
			dc.GetTextExtent(ptr, &cx, &cy);
		if (cx > width)
		{
			if (lastspace)
			{
				*lastspace = '\n';
				if (p)
					ptr = p = lastspace + 1;
			}
			else
			{
				if (p)
					*p++ = '\n';
				ptr = p;
			}
			lastspace = 0;
		}
		else if (p)
			lastspace = p++;
	}

	lastspace = 0;
	p = ptr = (wxChar*)str.c_str();
	while (*p)
	{
		if (*p == ' ')
		{
			if (!lastspace)
				lastspace = ptr;
		}
		else
		{
			if (*p == '\n' && lastspace)
				ptr = lastspace;
			lastspace = 0;
		}
		*ptr++ = *p++;
	}
	*ptr = 0;

	string = str.c_str();

}


void OrganPanel::OnKeyCommand(wxKeyEvent& event)
{

	if (g_sound && g_sound->GetMidi().SetterActive() ^ event.ShiftDown())
	{
		::wxGetApp().frame->ProcessCommand(ID_AUDIO_MEMSET);
		UpdateWindowUI();
	}

	int k = event.GetKeyCode();
	if ( !event.AltDown())
	{

		GOrgueMeter* meter = ::wxGetApp().frame->m_meters[2];
		OrganDocument* doc = (OrganDocument*)::wxGetApp().m_docManager->GetCurrentDocument();
		switch(k)
		{

		case WXK_ESCAPE:
			::wxGetApp().frame->ProcessCommand(ID_AUDIO_PANIC);
			break;
		case WXK_LEFT:
			meter->SetValue(meter->GetValue() - 1);
			break;
		case WXK_DOWN:
			if (organfile)
				organfile->GetFrameGeneral(meter->GetValue() - 1)->Push();
			break;
		case WXK_RIGHT:
			meter->SetValue(meter->GetValue() + 1);
			break;
		default:
			if (organfile)
			{
				int flags = 0;
				if (event.AltDown())
					flags |= KEY_EVENT_ALT;
				organfile->GetOrganScreen().KeyEvent(event.GetKeyCode(), flags);
			}
		}
	}
	event.Skip();
}

void OrganPanel::OnDraw(wxDC* dc)
{
	if (!m_ClientBitmap.Ok() || !organfile || !m_DisplayMetrics->GetJambLeftRightWidth())
		return;
	dc->DrawBitmap(m_ClientBitmap, m_ClientOrigin.x, m_ClientOrigin.y, false);
}
