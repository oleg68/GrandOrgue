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

#ifndef GORGUESOUNDRTPORT_H
#define GORGUESOUNDRTPORT_H

#include "GOrgueSoundPort.h"
#include "RtAudio.h"

class GOrgueSoundRtPort : public GOrgueSoundPort
{
private:
	RtAudio* m_rtApi;
	unsigned m_nBuffers;

	static int Callback(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData);

	static wxString getName(RtAudio* rtApi, unsigned index);

public:
	static const wxString PORT_NAME;
  
	// rtApi to be deleted in the destructor
	GOrgueSoundRtPort(GOrgueSound* sound, RtAudio* rtApi, wxString name);
	~GOrgueSoundRtPort();

	void Open();
	void StartStream();
	void Close();

	static const std::vector<wxString> & getApis();
	static GOrgueSoundPort* create(const GOrgueSoundPortsConfig &portsConfig, GOrgueSound* sound, wxString name);
	static void addDevices(const GOrgueSoundPortsConfig &portsConfig, std::vector<GOrgueSoundDevInfo>& list);
};

#endif
