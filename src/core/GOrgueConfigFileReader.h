/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2017 GrandOrgue contributors (see AUTHORS)
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

#ifndef GORGUECONFIGFILEREADER_H
#define GORGUECONFIGFILEREADER_H

#include <wx/string.h>
#include <map>

class GOrgueFile;

class GOrgueConfigFileReader
{
private:
	std::map<wxString, std::map<wxString, wxString> > m_Entries;
	wxString m_Hash;

	wxString GetNextLine(const wxString& buffer, unsigned &pos);
public:
	GOrgueConfigFileReader();
	~GOrgueConfigFileReader();
	
	bool Read(GOrgueFile* file);
	bool Read(wxString filename);
	wxString GetHash();

	const std::map<wxString, std::map<wxString, wxString> >& GetContent();
	wxString getEntry(wxString group, wxString name);
};

#endif
