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

#include "GOSoundReverb.h"

#include "GOSoundResample.h"
#include "GOrgueStandardFile.h"
#include "GOrgueSettings.h"
#include "GOrgueWave.h"
#include "contrib/zita-convolver.h"
#include <wx/intl.h>
#include <wx/log.h>
#include <algorithm>

GOSoundReverb::GOSoundReverb(unsigned channels) :
	m_channels(channels),
	m_engine()
{
}

GOSoundReverb::~GOSoundReverb()
{
	Cleanup();
}

void GOSoundReverb::Cleanup()
{
	for(unsigned i = 0; i < m_engine.size(); i++)
	{
		m_engine[i]->stop_process();
		m_engine[i]->cleanup();
	}
}

void GOSoundReverb::Setup(GOrgueSettings& settings)
{
	Cleanup();

	if (!settings.ReverbEnabled())
		return;

	m_engine.clear();
	for(unsigned i = 0; i < m_channels; i++)
		m_engine.push_back(new Convproc());
	unsigned val = settings.SamplesPerBuffer();
	if (val < Convproc::MINPART)
		val = Convproc::MINPART;
	if (val > Convproc::MAXPART)
		val = Convproc::MAXPART;
	float* data = NULL;
	unsigned len = 0;
	try
	{
		for(unsigned i = 0; i < m_engine.size(); i++)
			if (m_engine[i]->configure(1, 1, 1000000, settings.SamplesPerBuffer(), val, Convproc::MAXPART))
				throw (wxString)_("Invalid reverb configuration (samples per buffer)");

		GOrgueWave wav;
		unsigned block = 0x4000;
		unsigned offset = settings.ReverbStartOffset();
		float gain = settings.ReverbGain();

		GOrgueStandardFile reverb_file(settings.ReverbFile());
		wav.Open(&reverb_file);
		if (offset > wav.GetLength())
			throw (wxString)_("Invalid reverb start offset");
		len = wav.GetLength();
		data = (float*)malloc(sizeof(float)*len);
		if (!data)
			throw (wxString)_("Out of memory");
		wav.ReadSamples(data, GOrgueWave::SF_IEEE_FLOAT, wav.GetSampleRate(), -settings.ReverbChannel());
		for(unsigned i = 0; i < len; i++)
			data[i] *= gain;
		if (len >= offset + settings.ReverbLen() && settings.ReverbLen())
			len = offset + settings.ReverbLen();
		if (wav.GetSampleRate() != settings.SampleRate())
		{
			float* new_data = resample_block(data, len, wav.GetSampleRate(), settings.SampleRate());
			if (!new_data)
				throw (wxString)_("Resampling failed");
			free(data);
			data = new_data;
			offset = (offset * settings.SampleRate()) / (float)wav.GetSampleRate();
		}
		unsigned delay = (settings.SampleRate() * settings.ReverbDelay()) / 1000;
		for(unsigned i = 0; i < m_channels; i++)
		{
			float* d = data + offset;
			unsigned l = len - offset;
			float g = 1;
			if  (settings.ReverbDirect())
				m_engine[i]->impdata_create(0, 0, 0, &g, 0, 1);
			for(unsigned j = 0; j < l; j+= block)
			{
				m_engine[i]->impdata_create(0, 0, 1, d + j, delay + j, delay + j + std::min(l - j, block));
			}
		}
		wav.Close();
		for(unsigned i = 0; i < m_engine.size(); i++)
			m_engine[i]->start_process(0, 0);
	}
	catch(wxString error)
	{
		wxLogError(_("Reverb load error: %s"), error.c_str());
		m_engine.clear();
	}
	if (data)
		free(data);
}

void GOSoundReverb::Reset()
{
	for(unsigned i = 0; i < m_engine.size(); i++)
		m_engine[i]->reset();
}

void GOSoundReverb::Process(float *output_buffer, unsigned n_frames)
{
	if (!m_engine.size())
		return;

	for(unsigned i = 0; i < m_channels; i++)
	{
		float* ptr = m_engine[i]->inpdata(0);
		for(unsigned j = 0; j < n_frames; j++)
			ptr[j] = output_buffer[i + m_channels * j];
	}

	for(unsigned i = 0; i < m_engine.size(); i++)
		m_engine[i]->process();

	for(unsigned i = 0; i < m_channels; i++)
	{
		float* ptr = m_engine[i]->outdata(0);
		for(unsigned j = 0; j < n_frames; j++)
			output_buffer[i + m_channels * j] = ptr[j];
	}
}
