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

#include "GOrgueStdPath.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>

wxString GOrgueStdPath::GetBaseDir()
{
	wxFileName fn(wxStandardPaths::Get().GetExecutablePath());
	fn = fn.GetPath();
	return fn.GetPath();
}

void GOrgueStdPath::InitLocaleDir()
{
#ifndef __WXMAC__
	wxLocale::AddCatalogLookupPathPrefix(GetBaseDir() + wxFILE_SEP_PATH +  wxT("share") + wxFILE_SEP_PATH + wxT("locale"));
#endif
}

wxString GOrgueStdPath::GetResourceDir()
{
#ifdef __WXMAC__
	return wxStandardPaths::Get().GetResourcesDir();
#else
	return GetBaseDir() + wxFILE_SEP_PATH +  wxT("share") + wxFILE_SEP_PATH + wxT("GrandOrgue");
#endif
}

wxString GOrgueStdPath::GetConfigDir()
{
	return wxStandardPaths::Get().GetUserConfigDir();
}

wxString GOrgueStdPath::GetDocumentDir()
{
	return wxStandardPaths::Get().GetDocumentsDir();
}

wxString GOrgueStdPath::GetCacheDir()
{
	return wxStandardPaths::Get().GetUserConfigDir();
}
