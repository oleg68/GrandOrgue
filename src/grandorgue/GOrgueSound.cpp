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

#include "GOrgueSound.h"

#include "GOSoundDefs.h"
#include "GOSoundThread.h"
#include "GOrgueEvent.h"
#include "GOrgueMidi.h"
#include "GOrgueSettings.h"
#include "GOrgueSoundPort.h"
#include "GrandOrgueFile.h"
#include "mutex_locker.h"
#include <wx/app.h>
#include <wx/intl.h>
#include <wx/log.h>
#include <wx/window.h>

GOrgueSound::GOrgueSound(GOrgueSettings& settings) :
	logSoundErrors(true),
	m_AudioOutputs(),
	m_WaitCount(),
	m_CalcCount(),
	m_SamplesPerBuffer(0),
	meter_counter(0),
	m_defaultAudioDevice(),
	m_organfile(0),
	m_Settings(settings),
	m_midi(settings)
{
}

GOrgueSound::~GOrgueSound()
{

	CloseSound();

	GOrgueSoundPort::terminate();
}

void GOrgueSound::StartThreads()
{
	StopThreads();

	unsigned n_cpus = m_Settings.Concurrency();

	GOMutexLocker thread_locker(m_thread_lock);
	for(unsigned i = 0; i < n_cpus; i++)
		m_Threads.push_back(new GOSoundThread(&GetEngine().GetScheduler()));

	for(unsigned i = 0; i < m_Threads.size(); i++)
		m_Threads[i]->Run();
}

void GOrgueSound::StopThreads()
{
	for(unsigned i = 0; i < m_Threads.size(); i++)
		m_Threads[i]->Delete();

	GOMutexLocker thread_locker(m_thread_lock);
	m_Threads.resize(0);
}

void GOrgueSound::OpenMidi()
{
	m_midi.Open();
}

bool GOrgueSound::OpenSound()
{
	m_LastErrorMessage = wxEmptyString;
	assert(m_AudioOutputs.size() == 0);
	bool opened_ok = false;

	unsigned audio_group_count = m_Settings.GetAudioGroups().size();
	std::vector<GOAudioDeviceConfig> audio_config = m_Settings.GetAudioDeviceConfig();
	std::vector<GOAudioOutputConfiguration> engine_config;

	m_AudioOutputs.resize(audio_config.size());
	for(unsigned i = 0; i < m_AudioOutputs.size(); i++)
		m_AudioOutputs[i].port = NULL;
	engine_config.resize(audio_config.size());
	for(unsigned i = 0; i < engine_config.size(); i++)
	{
		engine_config[i].channels = audio_config[i].channels;
		engine_config[i].scale_factors.resize(engine_config[i].channels);
		for(unsigned j = 0; j <  engine_config[i].channels; j++)
		{
			engine_config[i].scale_factors[j].resize(audio_group_count * 2);
			for(unsigned k = 0; k < audio_group_count * 2; k++)
				engine_config[i].scale_factors[j][k] = -121;

			if (j >= audio_config[i].scale_factors.size())
				continue;
			for(unsigned k = 0; k < audio_config[i].scale_factors[j].size(); k++)
			{
				int id = m_Settings.GetStrictAudioGroupId(audio_config[i].scale_factors[j][k].name);
				if (id == -1)
					continue;
				if (audio_config[i].scale_factors[j][k].left >= -120)
					engine_config[i].scale_factors[j][id * 2] = audio_config[i].scale_factors[j][k].left;
				if (audio_config[i].scale_factors[j][k].right >= -120)
					engine_config[i].scale_factors[j][id * 2 + 1] = audio_config[i].scale_factors[j][k].right;
			}
		}
	}
	m_SamplesPerBuffer = m_Settings.SamplesPerBuffer();
	m_SoundEngine.SetSamplesPerBuffer(m_SamplesPerBuffer);
	m_SoundEngine.SetPolyphonyLimiting(m_Settings.ManagePolyphony());
	m_SoundEngine.SetHardPolyphony(m_Settings.PolyphonyLimit());
	m_SoundEngine.SetScaledReleases(m_Settings.ScaleRelease());
	m_SoundEngine.SetRandomizeSpeaking(m_Settings.RandomizeSpeaking());
	m_SoundEngine.SetInterpolationType(m_Settings.InterpolationType());
	m_SoundEngine.SetAudioGroupCount(audio_group_count);
	unsigned sample_rate = m_Settings.SampleRate();
	m_AudioRecorder.SetBytesPerSample(m_Settings.WaveFormatBytesPerSample());
	GetEngine().SetSampleRate(sample_rate);
	m_AudioRecorder.SetSampleRate(sample_rate);
	m_SoundEngine.SetAudioOutput(engine_config);
	m_SoundEngine.SetupReverb(m_Settings);
	m_SoundEngine.SetAudioRecorder(&m_AudioRecorder, m_Settings.RecordDownmix());

	if (m_organfile)
		m_SoundEngine.Setup(m_organfile, m_Settings.ReleaseConcurrency());
	else
		m_SoundEngine.ClearSetup();

	try
	{
		for(unsigned i = 0; i < m_AudioOutputs.size(); i++)
		{
			wxString name = audio_config[i].name;
			
			const GOrgueSoundPortsConfig &portsConfig(m_Settings.GetPortsConfig());
			
			if (name == wxEmptyString)
				name = GetDefaultAudioDevice(portsConfig);

			wxLogMessage("GOrgueSound::OpenSound before GOrgueSoundPort::create: %s", name);

			m_AudioOutputs[i].port = GOrgueSoundPort::create(portsConfig, this, name);
			if (!m_AudioOutputs[i].port)
				throw wxString::Format(_("Output device %s not found - no sound output will occure"), name.c_str());

			wxLogMessage("GOrgueSound::OpenSound after create before init: %s", name);
			
			m_AudioOutputs[i].port->Init(audio_config[i].channels, GetEngine().GetSampleRate(), m_SamplesPerBuffer, audio_config[i].desired_latency, i);
			wxLogMessage("GOrgueSound::OpenSound after init: %s", name);
		}

		OpenMidi();
		wxLogMessage("GOrgueSound::OpenSound before StartStreams");
		StartStreams();
		wxLogMessage("GOrgueSound::OpenSound after StartStreams");
		StartThreads();
		opened_ok = true;

		if (m_organfile)
			m_organfile->PreparePlayback(&GetEngine(), &GetMidi(), &m_AudioRecorder);
	}
	catch (wxString &msg)
	{
		if (logSoundErrors)
			GOMessageBox(msg, _("Error"), wxOK | wxICON_ERROR, NULL);
		else
		  m_LastErrorMessage = msg;
	}

	if (!opened_ok)
		CloseSound();

	return opened_ok;

}

void GOrgueSound::StartStreams()
{
	for(unsigned i = 0; i < m_AudioOutputs.size(); i++)
		m_AudioOutputs[i].port->Open();

	if (m_SamplesPerBuffer > MAX_FRAME_SIZE)
		throw wxString::Format(_("Cannot use buffer size above %d samples; unacceptable quantization would occur."), MAX_FRAME_SIZE);

	m_WaitCount.exchange(0);
	m_CalcCount.exchange(0);
	for(unsigned i = 0; i < m_AudioOutputs.size(); i++)
	{
		GOMutexLocker dev_lock(m_AudioOutputs[i].mutex);
		m_AudioOutputs[i].wait = false;
		m_AudioOutputs[i].waiting = true;
	}

	for(unsigned i = 0; i < m_AudioOutputs.size(); i++)
		m_AudioOutputs[i].port->StartStream();
}

void GOrgueSound::CloseSound()
{
	wxLogMessage("GOrgueSound::CloseSound before StopThreads");
	StopThreads();
	wxLogMessage("GOrgueSound::CloseSound after StopThreads");

	for(unsigned i = 0; i < m_AudioOutputs.size(); i++)
	{
		m_AudioOutputs[i].waiting = false;
		m_AudioOutputs[i].wait = false;
		m_AudioOutputs[i].condition.Broadcast();
	}

	wxLogMessage("GOrgueSound::CloseSound after broadcast");

	for(unsigned i = 1; i < m_AudioOutputs.size(); i++)
	{
		GOMutexLocker dev_lock(m_AudioOutputs[i].mutex);
		m_AudioOutputs[i].condition.Broadcast();
	}

	wxLogMessage("GOrgueSound::CloseSound after dev_lock broadcast");

	for(int i = m_AudioOutputs.size() - 1; i >= 0; i--)
	{
		if (m_AudioOutputs[i].port)
		{
		  GOrgueSoundPort* const port = m_AudioOutputs[i].port;
		  
		  m_AudioOutputs[i].port = NULL;
		  
		  wxLogMessage("GOrgueSound::CloseSound before close port: %d", i);
		  port->Close();
		  wxLogMessage("GOrgueSound::CloseSound after close before delete port: %d", i);
		  delete port;
		  wxLogMessage("GOrgueSound::CloseSound after delete port: %d", i);
		}
	}

	if (m_organfile)
		m_organfile->Abort();

	wxLogMessage("GOrgueSound::CloseSound before ResetMeters");

	ResetMeters();
	m_AudioOutputs.clear();
	wxLogMessage("GOrgueSound::CloseSound after clear");
}

bool GOrgueSound::ResetSound(bool force)
{
	wxBusyCursor busy;
	if (!m_AudioOutputs.size() && !force)
		return false;
	wxLogMessage("GOrgueSound::ResetSound before CloseSound");
	CloseSound();
	wxLogMessage("GOrgueSound::ResetSound after CloseSound before OpenSound");
	if (!OpenSound())
	wxLogMessage("GOrgueSound::ResetSound after OpenSound");
		return false;

	return true;
}

void GOrgueSound::AssignOrganFile(GrandOrgueFile* organfile)
{
	if (organfile == m_organfile)
		return;

	GOMutexLocker locker(m_lock);
	GOMultiMutexLocker multi;
	for(unsigned i = 0; i < m_AudioOutputs.size(); i++)
		multi.Add(m_AudioOutputs[i].mutex);
	
	if (m_organfile)
	{
		m_organfile->Abort();
		m_SoundEngine.ClearSetup();
	}

	m_organfile = organfile;

	if (m_organfile && m_AudioOutputs.size())
	{
		m_SoundEngine.Setup(organfile, m_Settings.ReleaseConcurrency());
		m_organfile->PreparePlayback(&GetEngine(), &GetMidi(), &m_AudioRecorder);
	}
}

GOrgueSettings& GOrgueSound::GetSettings()
{
	return m_Settings;
}

GrandOrgueFile* GOrgueSound::GetOrganFile()
{
	return m_organfile;
}

void GOrgueSound::SetLogSoundErrorMessages(bool settingsDialogVisible)
{
	logSoundErrors = settingsDialogVisible;
}

std::vector<GOrgueSoundDevInfo> GOrgueSound::GetAudioDevices(
  const GOrgueSoundPortsConfig &portsConfig
) {
  m_defaultAudioDevice = wxEmptyString;
  std::vector<GOrgueSoundDevInfo> list = GOrgueSoundPort::getDeviceList(portsConfig);
  for(unsigned i = 0; i < list.size(); i++)
	  if (list[i].isDefault)
	  {
		  m_defaultAudioDevice = list[i].name;
		  break;
	  }
  return list;
}

const wxString GOrgueSound::GetDefaultAudioDevice(
  const GOrgueSoundPortsConfig &portsConfig
) {
  if (m_defaultAudioDevice.IsEmpty())
    GetAudioDevices(portsConfig);
  return m_defaultAudioDevice;
}

GOrgueMidi& GOrgueSound::GetMidi()
{
	return m_midi;
}

void GOrgueSound::ResetMeters()
{
	wxCommandEvent event(wxEVT_METERS, 0);
	event.SetInt(0x1);
	if (wxTheApp->GetTopWindow())
		wxTheApp->GetTopWindow()->GetEventHandler()->AddPendingEvent(event);
}

void GOrgueSound::UpdateMeter()
{
	/* Update meters */
	meter_counter += m_SamplesPerBuffer;
	if (meter_counter >= 6144)	// update 44100 / (N / 2) = ~14 times per second
	{
		wxCommandEvent event(wxEVT_METERS, 0);
		event.SetInt(0x0);
		if (wxTheApp->GetTopWindow())
			wxTheApp->GetTopWindow()->GetEventHandler()->AddPendingEvent(event);
		meter_counter = 0;
	}
}

bool GOrgueSound::AudioCallback(unsigned dev_index, float* output_buffer, unsigned int n_frames)
{
	if (n_frames != m_SamplesPerBuffer)
	{
		wxLogError(_("No sound output will happen. Samples per buffer has been changed by the sound driver to %d"), n_frames);
		return 1;
	}
	GO_SOUND_OUTPUT* device = &m_AudioOutputs[dev_index];
	GOMutexLocker locker(device->mutex);

	if (device->wait && device->waiting)
		device->condition.Wait();

	unsigned cnt = m_CalcCount.fetch_add(1);
	m_SoundEngine.GetAudioOutput(output_buffer, n_frames, dev_index, cnt + 1>= m_AudioOutputs.size());
	device->wait = true;
	unsigned count = m_WaitCount.fetch_add(1);

	if (count + 1 == m_AudioOutputs.size())
	{
		m_SoundEngine.NextPeriod();
		UpdateMeter();

		{
			GOMutexLocker thread_locker(m_thread_lock);
			for(unsigned i = 0; i < m_Threads.size(); i++)
				m_Threads[i]->Wakeup();
		}
		m_CalcCount.exchange(0);
		m_WaitCount.exchange(0);

		for(unsigned i = 0; i < m_AudioOutputs.size(); i++)
		{
			GOMutexLocker(m_AudioOutputs[i].mutex, i == dev_index);
			m_AudioOutputs[i].wait = false;
			m_AudioOutputs[i].condition.Signal();
		}
	}

	return true;
}

GOSoundEngine& GOrgueSound::GetEngine()
{
	return m_SoundEngine;
}

wxString GOrgueSound::getState()
{
	if (!m_AudioOutputs.size())
		return _("No sound output occurring");
	wxString result = wxString::Format(_("%d samples per buffer, %d Hz\n"), m_SamplesPerBuffer, m_SoundEngine.GetSampleRate());
	for(unsigned i = 0; i < m_AudioOutputs.size(); i++)
		result = result + _("\n") + m_AudioOutputs[i].port->getPortState();
	return result;
}
