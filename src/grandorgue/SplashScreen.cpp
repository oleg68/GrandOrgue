/*
 * GrandOrgue - a free pipe organ simulator
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

#include "SplashScreen.h"

#include "GrandOrgueDef.h"
#include "Images.h"
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/frame.h>
#include <wx/image.h>

BEGIN_EVENT_TABLE(GOrgueSplashBitmap, wxControl)
	EVT_PAINT(GOrgueSplashBitmap::OnPaint)
	EVT_LEFT_DOWN(GOrgueSplashBitmap::OnClick)
	EVT_KEY_DOWN(GOrgueSplashBitmap::OnKey)
END_EVENT_TABLE()


GOrgueSplashBitmap::GOrgueSplashBitmap(wxWindow *parent, wxWindowID id, wxBitmap& bitmap) :
wxControl(parent, id, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE),
	m_Bitmap(bitmap)
{
}

void GOrgueSplashBitmap::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	dc.DrawBitmap(m_Bitmap, 0, 0);
}

void GOrgueSplashBitmap::OnClick(wxMouseEvent& event)
{
	GetParent()->Close(true);
}

void GOrgueSplashBitmap::OnKey(wxKeyEvent& event)
{
	GetParent()->Close(true);
}

#define GORGUE_SPLASH_TIMER_ID       (9999)
#define GORGUE_SPLASH_TIMEOUT_LENGTH (3000)

BEGIN_EVENT_TABLE(GOrgueSplash, wxDialog)
	EVT_TIMER(GORGUE_SPLASH_TIMER_ID, GOrgueSplash::OnNotify)
	EVT_CLOSE(GOrgueSplash::OnCloseWindow)
END_EVENT_TABLE()

GOrgueSplash::GOrgueSplash(int has_timeout, wxWindow *parent, wxWindowID id) :
	wxDialog(parent, id, wxEmptyString, wxPoint(0, 0), wxSize(100, 100), wxBORDER_NONE | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP),
	m_Timer(this, GORGUE_SPLASH_TIMER_ID)
{
	wxBitmap bitmap = GetImage_Splash();
	DrawText(bitmap);
	m_Image = new GOrgueSplashBitmap(this, wxID_ANY, bitmap);
	SetClientSize(bitmap.GetWidth(), bitmap.GetHeight());
	CentreOnScreen();
	if (has_timeout)
	{
		m_Timer.Start(GORGUE_SPLASH_TIMEOUT_LENGTH, true);
		Show(true);
	}
	m_Image->SetFocus();
	Update();
	if (!has_timeout)
	{
		ShowModal();
	}
}

void GOrgueSplash::DrawText(wxBitmap& bitmap)
{
	wxMemoryDC dc(bitmap);
	wxFont font;

	font = *wxNORMAL_FONT;
	font.SetPointSize(14);
	font.SetWeight(wxFONTWEIGHT_BOLD);
	dc.SetFont(font);
	dc.DrawLabel(wxString::Format(_("GrandOrgue %s"), wxT(APP_VERSION)), wxRect(60, 0, 370, 22), wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	font = *wxNORMAL_FONT;
	font.SetWeight(wxFONTWEIGHT_BOLD);
	font.SetPointSize(10);
	dc.SetFont(font);
	dc.DrawLabel(_("virtual pipe organ simulator"), wxRect(60, 22, 370, 20), wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	font = *wxNORMAL_FONT;
	font.SetPointSize(10);
	font.SetWeight(wxFONTWEIGHT_NORMAL);
	dc.SetFont(font);
	dc.DrawLabel(_("licensed under the GNU GPLv2 (or later)"), wxRect(60, 42, 370, 20), wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);

	font = *wxNORMAL_FONT;
	font.SetWeight(wxFONTWEIGHT_NORMAL);
	font.SetPointSize(7);
	dc.SetFont(font);
	wxString msg = _("Copyright 2006 Milan Digital Audio LLC\nCopyright 2009-2015 GrandOrgue contributors\n\nThis software comes with no warranty.\n\nASIO Interface Technology by Steinberg Media Technologies GmbH,\nby use of the Steinberg ASIO SDK, Version 2.2.\nASIO is a trademark and software of Steinberg Media Technologies GmbH.");
	dc.DrawLabel(msg, wxRect(60, 62, 370, 100), wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);
}

GOrgueSplash::~GOrgueSplash()
{
	m_Timer.Stop();
}

void GOrgueSplash::OnNotify(wxTimerEvent& WXUNUSED(event))
{
	Close(true);
}

void GOrgueSplash::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
	m_Timer.Stop();
	this->Destroy();
}

