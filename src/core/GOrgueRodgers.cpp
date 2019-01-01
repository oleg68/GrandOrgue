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

#include "GOrgueRodgers.h"

uint8_t
GORodgersChecksum(const std::vector<uint8_t>& msg, unsigned start, unsigned len)
{
	uint8_t sum = 0;
	for (unsigned i = 0; i < len; i++)
		sum += msg[start + i];
	return (-sum) & 0x7f;
}

MIDI_BIT_STATE
GORodgersGetBit(unsigned stop, unsigned offset, const std::vector<uint8_t> data)
{
	unsigned start = offset * 7;
	if (stop < start)
		return MIDI_BIT_STATE::MIDI_BIT_NOT_PRESENT;
	stop -= start;
	unsigned pos = stop / 7;
	unsigned bit = stop - pos * 7;
	if (pos >= data.size())
		return MIDI_BIT_STATE::MIDI_BIT_NOT_PRESENT;
	if (data[pos] & (1 << bit))
		return MIDI_BIT_STATE::MIDI_BIT_SET;
	else
		return MIDI_BIT_STATE::MIDI_BIT_CLEAR;
}
unsigned GORodgersSetBit(unsigned stop, bool state, std::vector<uint8_t>& data)
{
	unsigned offset = stop / 7;
	unsigned bit = stop - offset * 7;
	if (data.size() <= offset)
		data.resize(offset + 1);
	uint8_t &byte = data[offset];
	if (state)
		byte |= 1 << bit;
	else
		byte &= ~(1 << bit);

	return offset;
}
