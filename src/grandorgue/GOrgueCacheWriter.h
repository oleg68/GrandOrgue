/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2015 GrandOrgue contributors (see AUTHORS)
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

#ifndef GORGUECACHEWRITER_H_
#define GORGUECACHEWRITER_H_

class wxOutputStream;

class GOrgueCacheWriter {
	wxOutputStream* m_zstream;
	wxOutputStream* m_stream;

public:
	GOrgueCacheWriter(wxOutputStream& stream, bool compressed);
	virtual ~GOrgueCacheWriter();

	bool WriteHeader();
	bool Write(const void* data, unsigned length);
	/* Write an bigger malloced block */
	bool WriteBlock(const void* data, unsigned length);

	void Close();
};

#endif
