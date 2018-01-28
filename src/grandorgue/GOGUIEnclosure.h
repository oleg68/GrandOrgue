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

#ifndef GOGUIENCLOSURE_H
#define GOGUIENCLOSURE_H

#include "GOGUIControl.h"
#include "GOrgueBitmap.h"
#include "GOrgueFont.h"
#include <wx/colour.h>
#include <vector>

class GOrgueEnclosure;

class GOGUIEnclosure : public GOGUIControl
{
private:
	GOrgueEnclosure* m_enclosure;
	unsigned m_FontSize;
	wxString m_FontName;
	GOrgueFont m_Font;
	wxColor m_TextColor;
	wxString m_Text;
	wxRect m_TextRect;
	unsigned m_TextWidth;
	wxRect m_MouseRect;
	int m_MouseAxisStart;
	int m_MouseAxisEnd;
	unsigned m_TileOffsetX;
	unsigned m_TileOffsetY;
	std::vector<GOrgueBitmap> m_Bitmaps;
	
public:
	GOGUIEnclosure(GOGUIPanel* panel, GOrgueEnclosure* control);

	void Init(GOrgueConfigReader& cfg, wxString group);
	void Load(GOrgueConfigReader& cfg, wxString group);
	void Layout();

	void PrepareDraw(double scale, GOrgueBitmap* background);
	void Draw(GOrgueDC& dc);
	bool HandleMousePress(int x, int y, bool right, GOGUIMouseState& state);
	bool HandleMouseScroll(int x, int y, int amount);
};

#endif
