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

#ifndef GOGUIPANELWIDGET_H
#define GOGUIPANELWIDGET_H

#include "GOrgueBitmap.h"
#include <wx/bitmap.h>
#include <wx/panel.h>

class GOGUIPanel;

DECLARE_LOCAL_EVENT_TYPE(wxEVT_GOCONTROL, -1)

class GOGUIPanelWidget : public wxPanel
{
private:
	GOGUIPanel* m_panel;
	wxImage m_BGImage;
	bool m_BGInit;
	GOrgueBitmap m_Background;
	wxBitmap m_ClientBitmap;
	double m_Scale;

	void OnCreate(wxWindowCreateEvent& event);
	void OnDraw(wxDC* dc);
	void OnErase(wxEraseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnGOControl(wxCommandEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseRightDown(wxMouseEvent& event);
	void OnMouseScroll(wxMouseEvent& event);
	bool ForwardMouseEvent(wxMouseEvent& event);
	void OnKeyCommand(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);

public:
	GOGUIPanelWidget(GOGUIPanel* panel, wxWindow* parent, wxWindowID id = wxID_ANY);
	~GOGUIPanelWidget();

	void Focus();
	void OnUpdate();
	wxSize UpdateSize(wxSize size);

	DECLARE_EVENT_TABLE();
};

#endif
