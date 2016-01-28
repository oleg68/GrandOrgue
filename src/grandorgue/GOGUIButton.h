/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2016 GrandOrgue contributors (see AUTHORS)
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

#ifndef GOGUIBUTTON_H
#define GOGUIBUTTON_H

#include "GOGUIControl.h"
#include "GOrgueBitmap.h"
#include "GOrgueFont.h"
#include <wx/colour.h>

class GOrgueButton;

class GOGUIButton : public GOGUIControl
{
protected:
	bool m_IsPiston;
	bool m_DispKeyLabelOnLeft;
	GOrgueButton* m_Button;
	wxRect m_MouseRect;
	unsigned m_Radius;
	GOrgueBitmap m_OnBitmap;
	GOrgueBitmap m_OffBitmap;
	unsigned m_FontSize;
	wxString m_FontName;
	GOrgueFont m_Font;
	wxColor m_TextColor;
	wxString m_Text;
	wxRect m_TextRect;
	unsigned m_TextWidth;
	unsigned m_DispCol;
	unsigned m_DispRow;
	unsigned m_TileOffsetX;
	unsigned m_TileOffsetY;

public:
	GOGUIButton(GOGUIPanel* panel, GOrgueButton* control, bool is_piston = false);

	void Init(GOrgueConfigReader& cfg, wxString group, unsigned x_pos, unsigned y_pos, unsigned image = 0);
	void Load(GOrgueConfigReader& cfg, wxString group);
	void Layout();

	bool HandleMousePress(int x, int y, bool right, GOGUIMouseState& state);
	void PrepareDraw(double scale, GOrgueBitmap* background);
	void Draw(GOrgueDC& dc);
};

#endif
