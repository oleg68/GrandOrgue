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

#include "GOrguePath.h"

#include <wx/file.h>
#include <wx/filename.h>
#include <wx/log.h>

void GOCreateDirectory(const wxString& path)
{
	if (wxFileName::DirExists(path))
		return;
	if(!wxFileName::Mkdir(path, 0777, wxPATH_MKDIR_FULL))
	{
		wxLogError(_("Failed to create directory '%s'"), path.c_str());
	}
}

wxString GONormalizePath(const wxString& path)
{
	wxFileName name(path);
	name.MakeAbsolute();
	return name.GetLongPath();
}

wxString GOGetPath(const wxString& path)
{
	wxFileName name(GONormalizePath(path));
	return name.GetPath();
}

void GOSyncDirectory(const wxString& path)
{
	int fd = wxOpen(path.c_str(), O_RDONLY, 0);
	if (fd == -1)
		return;
	wxFsync(fd);
	wxClose(fd);
}

bool GORenameFile(const wxString& from, const wxString& to)
{
	wxFileName name(to);
	if (!wxRenameFile(from, to))
	{
		if (::wxFileExists(to) && !::wxRemoveFile(to))
		{
			wxLogError(_("Could not remove '%s'"), to.c_str());
			return false;
		}
		if (!wxRenameFile(from, to))
		{
			wxLogError(_("Could not write to '%s'"), to.c_str());
			return false;
		}
	}
	GOSyncDirectory(name.GetPath());
	return true;
}
