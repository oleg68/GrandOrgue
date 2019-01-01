/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2019 GrandOrgue contributors (see AUTHORS)
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

#include "GrandOrgueTool.h"

#include "GOrgueArchiveCreator.h"
#include "GOrgueSettingDirectory.h"
#include "GOrgueStdPath.h"
#include "GrandOrgueDef.h"
#include <wx/filename.h>
#include <wx/log.h>

IMPLEMENT_APP(GOrgueTool)

const wxCmdLineEntryDesc GOrgueTool::m_cmdLineDesc [] = {
	{ wxCMD_LINE_SWITCH, wxTRANSLATE("h"), wxTRANSLATE("help"), wxTRANSLATE("displays help on the command line parameters"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
	{ wxCMD_LINE_SWITCH, wxTRANSLATE("c"), wxTRANSLATE("create"), wxTRANSLATE("create an organ package"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("o"), wxTRANSLATE("organ-package"), wxTRANSLATE("specifiy generated organ package filename"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("i"), wxTRANSLATE("input-directory"), wxTRANSLATE("specifiy input directory"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("t"), wxTRANSLATE("title"), wxTRANSLATE("organ package title"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("p1"), NULL, wxTRANSLATE("depend on organ package"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("p2"), NULL, wxTRANSLATE("depend on organ package"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("p3"), NULL, wxTRANSLATE("depend on organ package"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("p4"), NULL, wxTRANSLATE("depend on organ package"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("p5"), NULL, wxTRANSLATE("depend on organ package"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("o1"), NULL, wxTRANSLATE("provide odf-file-name"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("o2"), NULL, wxTRANSLATE("provide odf-file-name"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("o3"), NULL, wxTRANSLATE("provide odf-file-name"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("o4"), NULL, wxTRANSLATE("provide odf-file-name"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("o5"), NULL, wxTRANSLATE("provide odf-file-name"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("o6"), NULL, wxTRANSLATE("provide odf-file-name"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("o7"), NULL, wxTRANSLATE("provide odf-file-name"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("o8"), NULL, wxTRANSLATE("provide odf-file-name"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("o9"), NULL, wxTRANSLATE("provide odf-file-name"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_OPTION, wxTRANSLATE("o10"), NULL, wxTRANSLATE("provide odf-file-name"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
	{ wxCMD_LINE_NONE }
};

GOrgueTool::GOrgueTool()
{
}

bool GOrgueTool::OnInit()
{
	wxMessageOutput::Set(new wxMessageOutputLog());
	
	GOrgueStdPath::InitLocaleDir();
	m_locale.Init(wxLANGUAGE_DEFAULT);
	m_locale.AddCatalog(wxT("GrandOrgue"));

	return wxAppConsole::OnInit();
}

void GOrgueTool::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.SetLogo(wxString::Format(_("GrandOrgueTool %s"), wxT(APP_VERSION)));
	parser.SetDesc (m_cmdLineDesc);
}

bool GOrgueTool::OnCmdLineParsed(wxCmdLineParser& parser)
{
	if (parser.Found(wxT("c")))
		return CmdLineCreate(parser);

	wxLogError(_("No function selected"));
	return false;
}

bool GOrgueTool::CmdLineCreate(wxCmdLineParser& parser)
{
	wxString organPackage, inputDirectory, title;
	std::vector<wxString> packages, odfs;

	if (!parser.Found(wxT("o"), &organPackage))
	{
		wxLogError(_("No output-file specified"));
		return false;
	}
	if (!parser.Found(wxT("i"), &inputDirectory))
	{
		wxLogError(_("No input directory specified"));
		return false;
	}
	if (!parser.Found(wxT("t"), &title))
	{
		wxLogError(_("No title specified"));
		return false;
	}
	for(unsigned i = 0; i < 5; i++)
	{
		wxString tmp;
		if (parser.Found(wxString::Format(wxT("p%d"), i + 1), &tmp))
			packages.push_back(tmp);
	}
	for(unsigned i = 0; i < 10; i++)
	{
		wxString tmp;
		if (parser.Found(wxString::Format(wxT("o%d"), i + 1), &tmp))
			odfs.push_back(tmp);
	}
	if (!CreateOrganPackage(organPackage, title, inputDirectory, odfs, packages))
	{
		wxLogError(_("organ package creation failed"));
		return false;
	}
	return true;
}


int GOrgueTool::OnRun()
{
	return 0;
}

bool GOrgueTool::CreateOrganPackage(wxString organPackage, wxString title, wxString inputDirectory, std::vector<wxString> odfs, std::vector<wxString> packages)
{
	GOrgueSettingDirectory cacheDir(NULL, wxEmptyString, wxEmptyString, wxEmptyString);
	cacheDir(GOrgueStdPath::GetCacheDir() + wxFileName::GetPathSeparator() + wxT("GrandOrgueToolCache"));

	GOrgueArchiveCreator archiveCreator(cacheDir);
	for(unsigned i = 0; i < packages.size(); i++)
		if (!archiveCreator.AddPackage(packages[i]))
		{
			wxLogError(_("Failed to open archive %s"), packages[i].c_str());
			return false;
		}
	for(unsigned i = 0; i < odfs.size(); i++)
		archiveCreator.AddOrgan(odfs[i]);
	if (!archiveCreator.CreatePackage(organPackage, title))
		return false;
	if (!archiveCreator.AddDirectory(inputDirectory))
		return false;
	if (!archiveCreator.FinishPackage())
		return false;
	wxLogInfo(_("organ package %s created"), organPackage.c_str());
	return true;
}

