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

#ifndef GORGUEMIDIRTOUTPORT_H
#define GORGUEMIDIRTOUTPORT_H

#include "GOrgueMidiOutPort.h"
#include "RtMidi.h"

class GOrgueMidiRtOutPort : public GOrgueMidiOutPort
{
protected:
  RtMidi::Api m_api;
  RtMidiOut* m_port;

  void Close(bool isToFreePort);

  void SendData(std::vector<unsigned char>& msg);

public:
  GOrgueMidiRtOutPort(GOrgueMidi* midi, wxString prefix, wxString name, RtMidi::Api api);
  ~GOrgueMidiRtOutPort();

  bool Open();
  void Close() { Close(true); }
};

#endif
