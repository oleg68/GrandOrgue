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

#ifndef GORGUEDIALOGVIEW_H
#define GORGUEDIALOGVIEW_H

#include <wx/window.h>

class GOrgueDocument;

class GOrgueView
{
private:
	GOrgueDocument* m_doc;
	wxWindow* m_wnd;

public:
	GOrgueView(GOrgueDocument* doc, wxWindow* wnd);
	virtual ~GOrgueView();

	virtual void RemoveView();
	void ShowView();
};

#endif
