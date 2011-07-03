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

#ifndef GORGUEPUSHBUTTON_H
#define GORGUEPUSHBUTTON_H

#include <wx/wx.h>
#include "GOrgueControl.h"
#include "GOrgueDrawable.h"

class GOrgueDisplayMetrics;

class GOrguePushbutton : public GOrgueControl
{

private:

	GOrgueDisplayMetrics& m_DisplayMetrics;
	void MIDI(void);

public:

	GOrguePushbutton(GOrgueDisplayMetrics& display_metrics);
	virtual ~GOrguePushbutton() { };

	void Load(IniFileConfig& cfg, wxString group);
    void Save(IniFileConfig& cfg, bool prefix, wxString group);

	// Members of GO_IRenderable
	virtual unsigned GetLayer();
	virtual unsigned GetX();
	virtual unsigned GetY();
	virtual unsigned GetWidth();
	virtual unsigned GetHeight();
	virtual void Draw(wxDC& dc);

	// Members of GO_IControl
	virtual void MouseButtonDown(const unsigned x, const unsigned y, const GO_MouseButton button);
	virtual void OnKeyEvent(const int wx_key, const unsigned flags);

	// Must be overriden by derived class - gets called when the button is
	// pushed by the user.
	virtual void Push(int depth = 0) = 0;

	wxInt16 m_ManualNumber;
	wxInt16 DispButtonRow;
	wxInt16 DispButtonCol;
	wxInt16 DispImageNum;
	wxInt16 MIDIProgramChangeNumber;

};

#endif
