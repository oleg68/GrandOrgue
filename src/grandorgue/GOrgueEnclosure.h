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

#ifndef GORGUEENCLOSURE_H_
#define GORGUEENCLOSURE_H_

#include "GOrgueDrawable.h"
#include <wx/wx.h>
#include "GO_IControl.h"

class GOrgueDisplayMetrics;
class IniFileConfig;

class GOrgueEnclosure : public GO_IControl
{

private:

	unsigned m_enclosure_nb;
	int AmpMinimumLevel;
	int MIDIInputNumber;
	int MIDIValue;
	wxString Name;
	GOrgueDisplayMetrics* DisplayMetrics;

	void DrawLabel(wxDC& dc);

public:

	GOrgueEnclosure();

	void Load(IniFileConfig& cfg, const unsigned enclosure_nb, GOrgueDisplayMetrics* displayMetrics);
	void Set(int n);
	void MIDI(void);
	int GetMIDIInputNumber();
	float GetAttenuation();


	bool IsEnclosure(const unsigned nb) const;

	// Methods from GO_IRenderable
	virtual unsigned GetLayer();
	virtual unsigned GetX();
	virtual unsigned GetY();
	virtual unsigned GetWidth();
	virtual unsigned GetHeight();
	virtual void Draw(wxDC& dc);
	virtual bool IsDisplayed();

	// Methods from GO_IControl
	virtual void MouseButtonDown(const unsigned x, const unsigned y, const GO_MouseButton button);
	virtual void Scroll(const unsigned x, const unsigned y, const int amount);


};

#endif /* GORGUEENCLOSURE_H_ */
