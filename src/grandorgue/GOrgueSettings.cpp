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

#include "GOrgueSettings.h"

#include "GOSoundDefs.h"
#include "GOrgueConfigFileWriter.h"
#include "GOrgueConfigFileReader.h"
#include "GOrgueConfigReader.h"
#include "GOrgueConfigReaderDB.h"
#include "GOrgueConfigWriter.h"
#include "GOrgueLimits.h"
#include "GOrgueMemoryPool.h"
#include "GOrguePath.h"
#include "GOrgueSettingNumber.cpp"
#include "GOrgueStdPath.h"
#include <wx/confbase.h>
#include <wx/filename.h>
#include <wx/log.h>
#include <wx/stdpaths.h>
#include <wx/thread.h>
#include <algorithm>

const GOMidiSetting GOrgueSettings:: m_MIDISettings[] = {
	{ MIDI_RECV_MANUAL, 1, wxTRANSLATE("Manuals"), wxTRANSLATE("Pedal") },
	{ MIDI_RECV_MANUAL, 2, wxTRANSLATE("Manuals"), wxTRANSLATE("Manual 1") },
	{ MIDI_RECV_MANUAL, 3, wxTRANSLATE("Manuals"), wxTRANSLATE("Manual 2") },
	{ MIDI_RECV_MANUAL, 4, wxTRANSLATE("Manuals"), wxTRANSLATE("Manual 3") },
	{ MIDI_RECV_MANUAL, 5, wxTRANSLATE("Manuals"), wxTRANSLATE("Manual 4") },
	{ MIDI_RECV_MANUAL, 6, wxTRANSLATE("Manuals"), wxTRANSLATE("Manual 5") },
	{ MIDI_RECV_ENCLOSURE, 1, wxTRANSLATE("Enclosures"), wxTRANSLATE("Enclosure 1") },
	{ MIDI_RECV_ENCLOSURE, 2, wxTRANSLATE("Enclosures"), wxTRANSLATE("Enclosure 2") },
	{ MIDI_RECV_ENCLOSURE, 3, wxTRANSLATE("Enclosures"), wxTRANSLATE("Enclosure 3") },
	{ MIDI_RECV_ENCLOSURE, 4, wxTRANSLATE("Enclosures"), wxTRANSLATE("Enclosure 4") },
	{ MIDI_RECV_ENCLOSURE, 5, wxTRANSLATE("Enclosures"), wxTRANSLATE("Enclosure 5") },
	{ MIDI_RECV_ENCLOSURE, 6, wxTRANSLATE("Enclosures"), wxTRANSLATE("Enclosure 6") },
	{ MIDI_RECV_SETTER, 0, wxTRANSLATE("Sequencer"), wxTRANSLATE("Previous Memory") },
	{ MIDI_RECV_SETTER, 1, wxTRANSLATE("Sequencer"), wxTRANSLATE("Next Memory") },
	{ MIDI_RECV_SETTER, 2, wxTRANSLATE("Sequencer"), wxTRANSLATE("Memory Set") },
	{ MIDI_RECV_SETTER, 3, wxTRANSLATE("Sequencer"), wxTRANSLATE("Current") },
	{ MIDI_RECV_SETTER, 4, wxTRANSLATE("Sequencer"), wxTRANSLATE("G.C.") },
	{ MIDI_RECV_SETTER, 5, wxTRANSLATE("Sequencer"), wxTRANSLATE("-10") },
	{ MIDI_RECV_SETTER, 6, wxTRANSLATE("Sequencer"), wxTRANSLATE("+10") },
	{ MIDI_RECV_SETTER, 7, wxTRANSLATE("Sequencer"), wxTRANSLATE("__0") },
	{ MIDI_RECV_SETTER, 8, wxTRANSLATE("Sequencer"), wxTRANSLATE("__1") },
	{ MIDI_RECV_SETTER, 9, wxTRANSLATE("Sequencer"), wxTRANSLATE("__2") },
	{ MIDI_RECV_SETTER, 10, wxTRANSLATE("Sequencer"), wxTRANSLATE("__3") },
	{ MIDI_RECV_SETTER, 11, wxTRANSLATE("Sequencer"), wxTRANSLATE("__4") },
	{ MIDI_RECV_SETTER, 12, wxTRANSLATE("Sequencer"), wxTRANSLATE("__5") },
	{ MIDI_RECV_SETTER, 13, wxTRANSLATE("Sequencer"), wxTRANSLATE("__6") },
	{ MIDI_RECV_SETTER, 14, wxTRANSLATE("Sequencer"), wxTRANSLATE("__7") },
	{ MIDI_RECV_SETTER, 15, wxTRANSLATE("Sequencer"), wxTRANSLATE("__8") },
	{ MIDI_RECV_SETTER, 16, wxTRANSLATE("Sequencer"), wxTRANSLATE("__9") },
	{ MIDI_RECV_SETTER, 17, wxTRANSLATE("Master Controls"), wxTRANSLATE("-1 Cent") },
	{ MIDI_RECV_SETTER, 18, wxTRANSLATE("Master Controls"), wxTRANSLATE("+1 Cent") },
	{ MIDI_RECV_SETTER, 19, wxTRANSLATE("Master Controls"), wxTRANSLATE("-100 Cent") },
	{ MIDI_RECV_SETTER, 20, wxTRANSLATE("Master Controls"), wxTRANSLATE("+100 Cent") },
	{ MIDI_RECV_SETTER, 21, wxTRANSLATE("Master Controls"), wxTRANSLATE("Prev temperament") },
	{ MIDI_RECV_SETTER, 22, wxTRANSLATE("Master Controls"), wxTRANSLATE("Next temperament") },
	{ MIDI_RECV_SETTER, 23, wxTRANSLATE("Master Controls"), wxTRANSLATE("Transpose -") },
	{ MIDI_RECV_SETTER, 24, wxTRANSLATE("Master Controls"), wxTRANSLATE("Transpose +") },
};

GOrgueSettings::GOrgueSettings(wxString instance) :
	m_Config(*wxConfigBase::Get()),
	m_InstanceName(instance),
	m_MemoryLimit(0),
	m_Channels(2),
	m_Concurrency(0),
	m_ReleaseConcurrency(1),
	m_LoadConcurrency(0),
	m_LosslessCompression(true),
	m_ManagePolyphony(true),
	m_ManageCache(true),
	m_CompressCache(false),
	m_ScaleRelease(true),
	m_RandomizeSpeaking(true),
	m_LoadLastFile(true),
	m_ODFCheck(false),
	m_SamplesPerBuffer(1024),
	m_SampleRate(44100),
	m_BitsPerSample(24),
	m_InterpolationType(0),
	m_WaveFormat(4),
	m_RecordDownmix(false),
	m_LoopLoad(0),
	m_AttackLoad(0),
	m_ReleaseLoad(0),
	m_Volume(-15),
	m_PolyphonyLimit(2048),
	m_Preset(0),
	m_OrganList(),
	m_WAVPath(),
	m_OrganPath(),
	m_SettingPath(),
	m_UserSettingPath(),
	m_UserCachePath(),
	m_LastFile(),
	m_ResourceDir(),
	m_AudioGroups(),
	m_AudioDeviceConfig(),
	m_Transpose(0),
	m_ReleaseLength(0),
	m_ReverbEnabled(false),
	m_ReverbDirect(true),
	m_ReverbChannel(1),
	m_ReverbStartOffset(0),
	m_ReverbLen(0),
	m_ReverbDelay(0),
	m_ReverbGain(1),
	m_ReverbFile(),
	m_MIDIEvents()
{
	GetConfig().SetRecordDefaults();
	m_ConfigFileName = wxStandardPaths::Get().GetUserConfigDir() + wxFileName::GetPathSeparator() + wxT("GrandOrgueConfig") + m_InstanceName;
	for(unsigned i = 0; i < GetEventCount(); i++)
		m_MIDIEvents.push_back(new GOrgueMidiReceiver(NULL, m_MIDISettings[i].type));
	m_ResourceDir = GOrgueStdPath::GetResourceDir();
}

GOrgueSettings::~GOrgueSettings()
{
	Flush();
}

wxConfigBase& GOrgueSettings::GetConfig()
{
	return m_Config;
}

void GOrgueSettings::Load()
{
	GOrgueConfigFileReader cfg_file;
	if (wxFileExists(m_ConfigFileName))
	{
		if (!cfg_file.Read(m_ConfigFileName))
			wxLogError(_("Unable to read '%s'"), m_ConfigFileName.c_str());
	}

	try
	{
		GOrgueConfigReaderDB cfg_db;
		cfg_db.ReadData(cfg_file, CMBSetting, false);
		GOrgueConfigReader cfg(cfg_db);

		m_OrganList.clear();
		unsigned organ_count = cfg.ReadInteger(CMBSetting, wxT("General"), wxT("OrganCount"), 0, 99999, false, 0);
		for(unsigned i = 0; i < organ_count; i++)
			m_OrganList.push_back(new GOrgueOrgan(cfg, wxString::Format(wxT("Organ%03d"), i + 1), m_MidiMap));

		m_Temperaments.InitTemperaments();
		m_Temperaments.Load(cfg);

		m_AudioGroups.clear();
		unsigned count = cfg.ReadInteger(CMBSetting, wxT("AudioGroups"), wxT("Count"), 0, 200, false, 0);
		for(unsigned i = 0; i < count; i++)
			m_AudioGroups.push_back(cfg.ReadString(CMBSetting, wxT("AudioGroups"), wxString::Format(wxT("Name%03d"), i + 1), false, wxString::Format(_("Audio group %d"), i + 1)));
		
		m_AudioDeviceConfig.clear();
		count = cfg.ReadInteger(CMBSetting, wxT("AudioDevices"), wxT("Count"), 0, 200, false, 0);
		for(unsigned i = 0; i < count; i++)
		{
			GOAudioDeviceConfig conf;
			conf.name = cfg.ReadString(CMBSetting, wxT("AudioDevices"), wxString::Format(wxT("Device%03dName"), i + 1));
			conf.channels = cfg.ReadInteger(CMBSetting, wxT("AudioDevices"), wxString::Format(wxT("Device%03dChannelCount"), i + 1), 0, 200);
			conf.scale_factors.resize(conf.channels);
			for(unsigned j = 0; j < conf.channels; j++)
			{
				wxString prefix = wxString::Format(wxT("Device%03dChannel%03d"), i + 1, j + 1);
				unsigned group_count = cfg.ReadInteger(CMBSetting, wxT("AudioDevices"), prefix + wxT("GroupCount"), 0, 200);
				for(unsigned k = 0; k < group_count; k++)
				{
					GOAudioGroupOutputConfig group;
					wxString p = prefix + wxString::Format(wxT("Group%03d"), k + 1);

					group.name = cfg.ReadString(CMBSetting, wxT("AudioDevices"), p + wxT("Name"));
					group.left = cfg.ReadFloat(CMBSetting, wxT("AudioDevices"), p + wxT("Left"), -121.0, 40);
					group.right = cfg.ReadFloat(CMBSetting, wxT("AudioDevices"), p + wxT("Right"), -121.0, 40);

					conf.scale_factors[j].push_back(group);
				}
			}
			m_AudioDeviceConfig.push_back(conf);
		}

		for(unsigned i = 0; i < GetEventCount(); i++)
			m_MIDIEvents[i]->Load(cfg, GetEventSection(i), m_MidiMap);

		long cpus = wxThread::GetCPUCount();
		if (cpus == -1)
			cpus = 4;
		if (cpus > MAX_CPU)
			cpus = MAX_CPU;
		if (cpus == 0)
			cpus = 1;
		GOrgueSettingStore::Load(cfg);
		SetConcurrency(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("Concurrency"), 0, MAX_CPU, false, cpus));
		SetReleaseConcurrency(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("ReleaseConcurrency"), 1, MAX_CPU, false, cpus));
		SetLoadConcurrency(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("LoadConcurrency"), 0, MAX_CPU, false, cpus));

		SetInterpolationType(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("InterpolationType"), 0, 1, false, 1));
		SetWaveFormatBytesPerSample(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("WaveFormat"), 1, 4, false, 4));
		SetRecordDownmix(cfg.ReadBoolean(CMBSetting, wxT("General"), wxT("RecordDownmix"), false, false));
		SetAttackLoad(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("AttackLoad"), 0, 1, false, 1));
		SetLoopLoad(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("LoopLoad"), 0, 2, false, 2));
		SetReleaseLoad(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("ReleaseLoad"), 0, 1, false, 1));

		SetManageCache(cfg.ReadBoolean(CMBSetting, wxT("General"), wxT("ManageCache"), false, true));
		SetCompressCache(cfg.ReadBoolean(CMBSetting, wxT("General"), wxT("CompressCache"), false, false));
		SetLoadLastFile(cfg.ReadBoolean(CMBSetting, wxT("General"), wxT("LoadLastFile"), false, true));
		SetLastFile(cfg.ReadString(CMBSetting, wxT("General"), wxT("LastFile"), false, wxEmptyString));
		SetODFCheck(cfg.ReadBoolean(CMBSetting, wxT("General"), wxT("StrictODFCheck"), false, false));

		SetLoadChannels(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("Channels"), 0, 2, false, 2));
		SetLosslessCompression(cfg.ReadBoolean(CMBSetting, wxT("General"), wxT("LosslessCompression"), false, false));
		SetManagePolyphony(cfg.ReadBoolean(CMBSetting, wxT("General"), wxT("ManagePolyphony"), false, true));
		SetScaleRelease(cfg.ReadBoolean(CMBSetting, wxT("General"), wxT("ScaleRelease"), false, true));
		SetRandomizeSpeaking(cfg.ReadBoolean(CMBSetting, wxT("General"), wxT("RandomizeSpeaking"), false, true));
		SetMemoryLimit(cfg.ReadFloat(CMBSetting, wxT("General"), wxT("MemoryLimit"), 0, 1024 * 1024, false, GOrgueMemoryPool::GetSystemMemoryLimit()) * (1024.0 * 1024.0));

		SetReverbEnabled(cfg.ReadBoolean(CMBSetting, wxT("Reverb"), wxT("ReverbEnabled"), false, false));
		SetReverbDirect(cfg.ReadBoolean(CMBSetting, wxT("Reverb"), wxT("ReverbDirect"), false, true));
		SetReverbChannel(cfg.ReadInteger(CMBSetting, wxT("Reverb"), wxT("ReverbChannel"), 1, 4, false, 1));
		SetReverbStartOffset(cfg.ReadInteger(CMBSetting, wxT("Reverb"), wxT("ReverbStartOffset"), 0, MAX_SAMPLE_LENGTH, false, 0));
		SetReverbLen(cfg.ReadInteger(CMBSetting, wxT("Reverb"), wxT("ReverbLen"), 0, MAX_SAMPLE_LENGTH, false, 0));
		SetReverbDelay(cfg.ReadInteger(CMBSetting, wxT("Reverb"), wxT("ReverbDelay"), 0, 10000, false, 0));
		SetReverbGain(cfg.ReadFloat(CMBSetting, wxT("Reverb"), wxT("ReverbGain"), 0, 50, false, 1));
		SetReverbFile(cfg.ReadString(CMBSetting, wxT("Reverb"), wxT("ReverbFile"), false, wxEmptyString));

		SetSamplesPerBuffer(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("SamplesPerBuffer"), 1, MAX_FRAME_SIZE, false, 1024));
		SetSampleRate(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("SampleRate"), 1000, 100000, false, 44100));
		SetBitsPerSample(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("BitsPerSample"), 8, 24, false, 24));
		SetVolume(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("Volume"), -120, 20, false, -15));
		SetPolyphonyLimit(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("PolyphonyLimit"), 0, MAX_POLYPHONY, false, cpus * 725));
		SetUserSettingPath (cfg.ReadString(CMBSetting, wxT("General"), wxT("SettingPath"), false, m_Config.Read(wxT("SettingPath"), wxEmptyString)));
		SetUserCachePath (cfg.ReadString(CMBSetting, wxT("General"), wxT("CachePath"), false, m_Config.Read(wxT("CachePath"), wxEmptyString)));
		SetPreset(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("Preset"), 0, MAX_PRESET, false, 0));

		SetReleaseLength(cfg.ReadInteger(CMBSetting, wxT("General"), wxT("ReleaseLength"), 0, 3000, false, 0));
		SetMidiRecorderOutputDevice(cfg.ReadString(CMBSetting, wxT("MIDIOut"), wxT("MIDIRecorderDevice"), false, wxEmptyString));
		
		count = cfg.ReadInteger(CMBSetting, wxT("MIDIIn"), wxT("Count"), 0, MAX_MIDI_DEVICES, false, 0);
		for(unsigned i = 0; i < count; i++)
		{
			wxString name = cfg.ReadString(CMBSetting, wxT("MIDIIn"), wxString::Format(wxT("Device%03d"), i + 1));
			SetMidiInState(name, cfg.ReadBoolean(CMBSetting, wxT("MIDIIn"), wxString::Format(wxT("Device%03dEnabled"), i + 1)));
			SetMidiInDeviceChannelShift(name, cfg.ReadInteger(CMBSetting, wxT("MIDIIn"), wxString::Format(wxT("Device%03dShift"), i + 1), 0, 15));;
		}

		count = cfg.ReadInteger(CMBSetting, wxT("MIDIOut"), wxT("Count"), 0, MAX_MIDI_DEVICES, false, 0);
		for(unsigned i = 0; i < count; i++)
		{
			wxString name = cfg.ReadString(CMBSetting, wxT("MIDIOut"), wxString::Format(wxT("Device%03d"), i + 1));
			SetMidiOutState(name, cfg.ReadBoolean(CMBSetting, wxT("MIDIOut"), wxString::Format(wxT("Device%03dEnabled"), i + 1)));
		}

		if (wxFileExists(m_ConfigFileName))
			wxCopyFile(m_ConfigFileName, m_ConfigFileName + wxT(".last"));
	}
	catch (wxString error)
	{
		wxLogError(wxT("%s\n"),error.c_str());
	}

	m_WAVPath = m_Config.Read(wxT("wavPath"), GetStandardDocumentDirectory());
	m_OrganPath = m_Config.Read(wxT("organPath"), GetStandardOrganDirectory());
	m_SettingPath = m_Config.Read(wxT("cmbPath"), GetStandardOrganDirectory());

	if (m_AudioGroups.size() == 0)
	{
		unsigned count = m_Config.Read(wxT("AudioGroup/Count"), 0L);
		for(unsigned i = 0; i < count; i++)
			m_AudioGroups.push_back(m_Config.Read(wxString::Format(wxT("AudioGroup/Name%d"), i + 1), wxString::Format(_("Audio group %d"), i + 1)));
		if (!m_AudioGroups.size())
			m_AudioGroups.push_back(_("Default audio group"));
	}

	if (!m_AudioDeviceConfig.size())
	{
		unsigned count = m_Config.Read(wxT("AudioDevices/Count"), 0L);
		for(unsigned i = 0; i < count; i++)
		{
			GOAudioDeviceConfig conf;
			conf.name = m_Config.Read(wxString::Format(wxT("AudioDevice/Device%d/Name"), i + 1), wxEmptyString);
			conf.channels = m_Config.Read(wxString::Format(wxT("AudioDevice/Device%d/Channels"), i + 1), 2L);
			conf.scale_factors.resize(conf.channels);
			for(unsigned j = 0; j < conf.channels; j++)
			{
				wxString prefix = wxString::Format(wxT("AudioDevice/Device%d/Channel%d/"), i + 1, j + 1);
				unsigned group_count = m_Config.Read(prefix + wxT("GroupCount"), 0L);
				for(unsigned k = 0; k < group_count; k++)
				{
					GOAudioGroupOutputConfig group;
					wxString p = prefix + wxString::Format(wxT("Group%d/"), k + 1);

					group.name = m_Config.Read(p + wxT("Name"), wxEmptyString);
					double tmp;
					m_Config.Read(p + wxT("Left"), &tmp, -121.0f);
					if (tmp > 40.0 || tmp < -121.0)
						tmp = 0;
					group.left = tmp;
					m_Config.Read(p + wxT("Right"), &tmp, -121.0f);
					if (tmp > 40.0 || tmp < -121.0)
						tmp = 0;
					group.right = tmp;

					conf.scale_factors[j].push_back(group);
				}
			}
			m_AudioDeviceConfig.push_back(conf);
		}
	}
	if (!m_AudioDeviceConfig.size())
	{
		GOAudioDeviceConfig conf;
		conf.name = wxEmptyString;
		conf.channels = 2;
		conf.scale_factors.resize(conf.channels);
		for(unsigned k = 0; k < m_AudioGroups.size(); k++)
		{
			GOAudioGroupOutputConfig group;
			group.name = m_AudioGroups[k];

			group.left = 0.0f;
			group.right = -121.0f;
			conf.scale_factors[0].push_back(group);

			group.left = -121.0f;
			group.right = 0.0f;
			conf.scale_factors[1].push_back(group);
		}
		m_AudioDeviceConfig.push_back(conf);
	}
}

unsigned GOrgueSettings::GetEventCount()
{
	return sizeof(m_MIDISettings) / sizeof(m_MIDISettings[0]);
}

wxString GOrgueSettings::GetEventSection(unsigned index)
{
	assert(index < GetEventCount());
	switch(m_MIDISettings[index].type)
	{
	case MIDI_RECV_ENCLOSURE:
		return wxString::Format(wxT("Enclosure%03d"), m_MIDISettings[index].index);

	case MIDI_RECV_MANUAL:
		return wxString::Format(wxT("Manual%03d"), m_MIDISettings[index].index);

	case MIDI_RECV_SETTER:
		return wxString::Format(wxT("Setter%03d"), m_MIDISettings[index].index);
		
	default:
	assert(false);
	return wxEmptyString;
	}
}

wxString GOrgueSettings::GetEventGroup(unsigned index)
{
	assert(index < GetEventCount());
	return wxGetTranslation(m_MIDISettings[index].group);
}

wxString GOrgueSettings::GetEventTitle(unsigned index)
{
	assert(index < GetEventCount());
	return wxGetTranslation(m_MIDISettings[index].name);
}

GOrgueMidiReceiver* GOrgueSettings::GetMidiEvent(unsigned index)
{
	assert(index < GetEventCount());
	return m_MIDIEvents[index];
}

GOrgueMidiReceiver* GOrgueSettings::FindMidiEvent(MIDI_RECEIVER_TYPE type, unsigned index)
{
	for(unsigned i = 0; i < GetEventCount(); i++)
		if (m_MIDISettings[i].type == type && m_MIDISettings[i].index == index)
			return m_MIDIEvents[i];
	return NULL;
}

size_t GOrgueSettings::GetMemoryLimit()
{
	return m_MemoryLimit;
}

void GOrgueSettings::SetMemoryLimit(size_t limit)
{
	m_MemoryLimit = limit;
}

wxString GOrgueSettings::GetStandardDocumentDirectory()
{
	return wxStandardPaths::Get().GetDocumentsDir();
}

wxString GOrgueSettings::GetStandardOrganDirectory()
{
	return GetStandardDocumentDirectory() + wxFileName::GetPathSeparator() + _("My Organs");
}

wxString GOrgueSettings::GetStandardDataDirectory()
{
	return wxStandardPaths::Get().GetUserConfigDir() + wxFileName::GetPathSeparator() + wxT("GrandOrgueData") + m_InstanceName;
}

wxString GOrgueSettings::GetStandardCacheDirectory()
{
	return wxStandardPaths::Get().GetUserConfigDir() + wxFileName::GetPathSeparator() + wxT("GrandOrgueCache") + m_InstanceName;
}

const wxString GOrgueSettings::GetResourceDirectory()
{
	return m_ResourceDir.c_str();
}

wxString GOrgueSettings::GetOrganPath()
{
	return m_OrganPath;
}

void GOrgueSettings::SetOrganPath(wxString path)
{
	m_OrganPath = path;
	m_Config.Write(wxT("organPath"), m_OrganPath);
}

wxString GOrgueSettings::GetSettingPath()
{
	return m_SettingPath;
}

void GOrgueSettings::SetSettingPath(wxString path)
{
	m_SettingPath = path;
	m_Config.Write(wxT("cmbPath"), m_SettingPath);
}

wxString GOrgueSettings::GetWAVPath()
{
	return m_WAVPath;
}

void GOrgueSettings::SetWAVPath(wxString path)
{
	m_WAVPath = path;
	m_Config.Write(wxT("wavPath"), m_WAVPath);
}

wxString GOrgueSettings::GetUserSettingPath()
{
	return m_UserSettingPath;
}

void GOrgueSettings::SetUserSettingPath(wxString path)
{
	if (path == wxEmptyString)
		path = GetStandardDataDirectory();
	if (!wxFileName::DirExists(path))
		path = GetStandardDataDirectory();
	wxFileName file(path);
	file.MakeAbsolute();
	path = file.GetFullPath();
	GOCreateDirectory(path);
	m_UserSettingPath = path;
}

wxString GOrgueSettings::GetUserCachePath()
{
	return m_UserCachePath;
}

void GOrgueSettings::SetUserCachePath(wxString path)
{
	if (path == wxEmptyString)
		path = GetStandardCacheDirectory();
	if (!wxFileName::DirExists(path))
		path = GetStandardCacheDirectory();
	wxFileName file(path);
	file.MakeAbsolute();
	path = file.GetFullPath();
	GOCreateDirectory(path);
	m_UserCachePath = path;
}

wxString GOrgueSettings::GetLastFile()
{
	return m_LastFile;
}

void GOrgueSettings::SetLastFile(wxString path)
{
	if (path == wxEmptyString)
		return;
	wxFileName file(path);
	file.MakeAbsolute();
	path = file.GetFullPath();
	m_LastFile = path;
}

unsigned  GOrgueSettings::GetPreset()
{
	return m_Preset;
}

void  GOrgueSettings::SetPreset(unsigned value)
{
	if (value > MAX_PRESET)
		value = MAX_PRESET;
	m_Preset = value;
}

unsigned GOrgueSettings::GetLoadChannels()
{
	return m_Channels;
}

void GOrgueSettings::SetLoadChannels(unsigned channels)
{
	if (channels > 2)
		channels = 2;
	m_Channels = channels;
}

unsigned GOrgueSettings::GetSamplesPerBuffer()
{
	return m_SamplesPerBuffer;
}

void GOrgueSettings::SetSamplesPerBuffer(unsigned sampler_per_buffer)
{
	m_SamplesPerBuffer = sampler_per_buffer;
	if (m_SamplesPerBuffer > MAX_FRAME_SIZE)
		m_SamplesPerBuffer = MAX_FRAME_SIZE;
	if (m_SamplesPerBuffer < 1)
		m_SamplesPerBuffer = 1;
}

unsigned GOrgueSettings::GetConcurrency()
{
	return m_Concurrency;
}

void GOrgueSettings::SetConcurrency(unsigned concurrency)
{
	if (concurrency > MAX_CPU)
		concurrency = MAX_CPU;
	m_Concurrency = concurrency;
}

unsigned GOrgueSettings::GetReleaseConcurrency()
{
	return m_ReleaseConcurrency;
}

void GOrgueSettings::SetReleaseConcurrency(unsigned concurrency)
{
	if (concurrency > MAX_CPU)
		concurrency = MAX_CPU;
	if (concurrency < 1)
		concurrency = 1;
	m_ReleaseConcurrency = concurrency;
}

unsigned GOrgueSettings::GetLoadConcurrency()
{
	return m_LoadConcurrency;
}

void GOrgueSettings::SetLoadConcurrency(unsigned concurrency)
{
	if (concurrency > MAX_CPU)
		concurrency = MAX_CPU;
	m_LoadConcurrency = concurrency;
}

bool GOrgueSettings::GetLosslessCompression()
{
	return m_LosslessCompression;
}

void GOrgueSettings::SetLosslessCompression(bool lossless_compression)
{
	m_LosslessCompression = lossless_compression;
}

bool GOrgueSettings::GetManagePolyphony()
{
	return m_ManagePolyphony;
}

void GOrgueSettings::SetManagePolyphony(bool manage_polyphony)
{
	m_ManagePolyphony = manage_polyphony;
}

bool GOrgueSettings::GetManageCache()
{
	return m_ManageCache;
}

void GOrgueSettings::SetManageCache(bool manage)
{
	m_ManageCache = manage;
}

bool GOrgueSettings::GetCompressCache()
{
	return m_CompressCache;
}

void GOrgueSettings::SetCompressCache(bool compress)
{
	m_CompressCache = compress;
}

bool GOrgueSettings::GetODFCheck()
{
	return m_ODFCheck;
}

void GOrgueSettings::SetODFCheck(bool strict)
{
	m_ODFCheck = strict;
}

bool GOrgueSettings::GetScaleRelease()
{
	return m_ScaleRelease;
}

void GOrgueSettings::SetScaleRelease(bool scale_release)
{
	m_ScaleRelease = scale_release;
}

bool GOrgueSettings::GetLoadLastFile()
{
	return m_LoadLastFile;
}

void GOrgueSettings::SetLoadLastFile(bool last_file)
{
	m_LoadLastFile = last_file;
}

unsigned GOrgueSettings::GetInterpolationType()
{
	return m_InterpolationType;
}

void GOrgueSettings::SetInterpolationType(unsigned type)
{
	if (type > 1)
		type = 0;
	m_InterpolationType = type;
}

bool GOrgueSettings::GetRandomizeSpeaking()
{
	return m_RandomizeSpeaking;
}

void GOrgueSettings::SetRandomizeSpeaking(bool randomize)
{
	m_RandomizeSpeaking = randomize;
}

unsigned GOrgueSettings::GetSampleRate()
{
	return m_SampleRate;
}

void GOrgueSettings::SetSampleRate(unsigned sample_rate)
{
	if (sample_rate < 1000)
		sample_rate = 44100;
	if (sample_rate > 100000)
		sample_rate = 100000;
	m_SampleRate = sample_rate;
}

unsigned GOrgueSettings::GetBitsPerSample()
{
	return m_BitsPerSample;
}

void GOrgueSettings::SetBitsPerSample(unsigned bits_per_sample)
{
	if (bits_per_sample != 8 && bits_per_sample != 12 && bits_per_sample != 16 && bits_per_sample != 20 && bits_per_sample != 24)
		bits_per_sample = 24;
	m_BitsPerSample = bits_per_sample;
}


unsigned GOrgueSettings::GetWaveFormatBytesPerSample()
{
	return m_WaveFormat;
}

void GOrgueSettings::SetWaveFormatBytesPerSample(unsigned bytes_per_sample)
{
	if (bytes_per_sample < 1)
		bytes_per_sample = 1;
	if (bytes_per_sample > 4)
		bytes_per_sample = 4;
	m_WaveFormat = bytes_per_sample;
}

bool GOrgueSettings::GetRecordDownmix()
{
	return m_RecordDownmix;
}

void GOrgueSettings::SetRecordDownmix(bool downmix)
{
	m_RecordDownmix = downmix;
}

unsigned GOrgueSettings::GetLoopLoad()
{
	return m_LoopLoad;
}

void GOrgueSettings::SetLoopLoad(unsigned loop_load)
{
	if (loop_load > 2)
		loop_load = 2;
	m_LoopLoad = loop_load;
}

unsigned GOrgueSettings::GetAttackLoad()
{
	return m_AttackLoad;
}

void GOrgueSettings::SetAttackLoad(unsigned attack_load)
{
	if (attack_load > 1)
		attack_load = 1;
	m_AttackLoad = attack_load;
}

unsigned GOrgueSettings::GetReleaseLoad()
{
	return m_ReleaseLoad;
}

void GOrgueSettings::SetReleaseLoad(unsigned release_load)
{
	if (release_load > 1)
		release_load = 1;
	m_ReleaseLoad = release_load;
}

int GOrgueSettings::GetVolume()
{
	return m_Volume;
}

void GOrgueSettings::SetVolume(int volume)
{
	if (volume < -120)
		volume = -120;
	if (volume > 20)
		volume = -15;
	m_Volume = volume;
}

unsigned GOrgueSettings::GetPolyphonyLimit()
{
	return m_PolyphonyLimit;
}

void GOrgueSettings::SetPolyphonyLimit(unsigned polyphony_limit)
{
	if (polyphony_limit > MAX_POLYPHONY)
		polyphony_limit = MAX_POLYPHONY;
	m_PolyphonyLimit = polyphony_limit;
}

unsigned GOrgueSettings::GetAudioDeviceLatency(wxString device)
{
	return m_Config.Read(wxT("Devices/Sound/") + device, 50L);
}

void GOrgueSettings::SetAudioDeviceLatency(wxString device, unsigned latency)
{
	m_Config.Write(wxT("Devices/Sound/") + device, (long) latency);
}

int GOrgueSettings::GetAudioDeviceActualLatency(wxString device)
{
	return m_Config.Read(wxT("Devices/Sound/ActualLatency/") + device, -1L);
}

void GOrgueSettings::SetAudioDeviceActualLatency(wxString device, unsigned latency)
{
	m_Config.Write(wxT("Devices/Sound/ActualLatency/") + device, (long) latency);
}

ptr_vector<GOrgueOrgan>& GOrgueSettings::GetOrganList()
{
	return m_OrganList;
}

static bool LRUCompare(GOrgueOrgan* a, GOrgueOrgan* b)
{
	return a->GetLastUse() > b->GetLastUse();
}

std::vector<GOrgueOrgan*> GOrgueSettings::GetLRUOrganList()
{
	std::vector<GOrgueOrgan*> lru;
	for(unsigned i = 0; i < m_OrganList.size(); i++)
		lru.push_back(m_OrganList[i]);
	std::sort(lru.begin(), lru.end(), LRUCompare);
	return lru;
}

void GOrgueSettings::AddOrgan(GOrgueOrgan* organ)
{
	for(unsigned i = 0; i < m_OrganList.size(); i++)
		if (organ->GetODFPath() == m_OrganList[i]->GetODFPath())
		{
			m_OrganList[i]->Update(organ);
			delete organ;
			return;
		}
	m_OrganList.push_back(organ);
}

bool GOrgueSettings::GetMidiInState(wxString device)
{
	std::map<wxString, bool>::iterator it = m_MidiIn.find(device);
	if (it == m_MidiIn.end())
	{
		m_MidiIn[device] = device.Find(wxT("GrandOrgue")) == wxNOT_FOUND;
		return m_MidiIn[device];
	}
	else
		return it->second;
}

void GOrgueSettings::SetMidiInState(wxString device, bool enabled)
{
	m_MidiIn[device] = enabled;
}

unsigned GOrgueSettings::GetMidiInDeviceChannelShift(wxString device)
{
	std::map<wxString, unsigned>::iterator it = m_MidiInShift.find(device);
	if (it == m_MidiInShift.end())
		return 0;
	else
		return it->second;
}

void GOrgueSettings::SetMidiInDeviceChannelShift(wxString device, unsigned shift)
{
	shift = shift % 16;
	m_MidiInShift[device] = shift;
}

std::vector<wxString> GOrgueSettings::GetMidiInDeviceList()
{
	std::vector<wxString> list;
	for (std::map<wxString, bool>::iterator it = m_MidiIn.begin(); it != m_MidiIn.end(); it++)
		list.push_back(it->first);
	return list;
}

bool GOrgueSettings::GetMidiOutState(wxString device)
{
	std::map<wxString, bool>::iterator it = m_MidiOut.find(device);
	if (it == m_MidiOut.end())
		return false;
	else
		return it->second;
}

void GOrgueSettings::SetMidiOutState(wxString device, bool enabled)
{
	m_MidiOut[device] = enabled;
}

std::vector<wxString> GOrgueSettings::GetMidiOutDeviceList()
{
	std::vector<wxString> list;
	for (std::map<wxString, bool>::iterator it = m_MidiOut.begin(); it != m_MidiOut.end(); it++)
		list.push_back(it->first);
	return list;
}

wxString GOrgueSettings::GetMidiRecorderOutputDevice()
{
	return m_MidiRecorderOutputDevice;
}

void GOrgueSettings::SetMidiRecorderOutputDevice(wxString device)
{
	m_MidiRecorderOutputDevice = device;
}

const std::vector<wxString>& GOrgueSettings::GetAudioGroups()
{
	return m_AudioGroups;
}

void GOrgueSettings::SetAudioGroups(const std::vector<wxString>& audio_groups)
{
	if (!audio_groups.size())
		return;
	m_AudioGroups = audio_groups;
}

unsigned GOrgueSettings::GetAudioGroupId(const wxString& str)
{
	for(unsigned i = 0; i < m_AudioGroups.size(); i++)
		if (m_AudioGroups[i] == str)
			return i;
	return 0;
}

int GOrgueSettings::GetStrictAudioGroupId(const wxString& str)
{
	for(unsigned i = 0; i < m_AudioGroups.size(); i++)
		if (m_AudioGroups[i] == str)
			return i;
	return -1;
}

const std::vector<GOAudioDeviceConfig>& GOrgueSettings::GetAudioDeviceConfig()
{
	return m_AudioDeviceConfig;
}

void GOrgueSettings::SetAudioDeviceConfig(const std::vector<GOAudioDeviceConfig>& config)
{
	if (!config.size())
		return;
	m_AudioDeviceConfig = config;
}

int GOrgueSettings::GetTranspose()
{
	return m_Transpose;
}

void GOrgueSettings::SetTranspose(int transpose)
{
	m_Transpose = transpose;
}

unsigned GOrgueSettings::GetReleaseLength()
{
	return m_ReleaseLength;
}

void GOrgueSettings::SetReleaseLength(unsigned reverb)
{
	if (reverb > 3000)
		reverb = 3000;
	m_ReleaseLength = reverb;
}

bool GOrgueSettings::GetReverbEnabled()
{
	return m_ReverbEnabled;
}

void GOrgueSettings::SetReverbEnabled(bool on)
{
	m_ReverbEnabled = on;
}

bool GOrgueSettings::GetReverbDirect()
{
	return m_ReverbDirect;
}

void GOrgueSettings::SetReverbDirect(bool on)
{
	m_ReverbDirect = on;
}

wxString GOrgueSettings::GetReverbFile()
{
	return m_ReverbFile;
}

void GOrgueSettings::SetReverbFile(wxString file)
{
	m_ReverbFile = file;
}

unsigned GOrgueSettings::GetReverbStartOffset()
{
	return m_ReverbStartOffset;
}

void GOrgueSettings::SetReverbStartOffset(unsigned offset)
{
	if (offset > MAX_SAMPLE_LENGTH)
		offset = MAX_SAMPLE_LENGTH;
	m_ReverbStartOffset = offset;
}

unsigned GOrgueSettings::GetReverbLen()
{
	return m_ReverbLen;
}

void GOrgueSettings::SetReverbLen(unsigned length)
{
	if (length > MAX_SAMPLE_LENGTH)
		length = MAX_SAMPLE_LENGTH;
	m_ReverbLen = length;
}

float GOrgueSettings::GetReverbGain()
{
	return m_ReverbGain;
}

void GOrgueSettings::SetReverbGain(float gain)
{
	if (gain <= 0)
		gain = 1;
	if (gain > 50)
		gain = 50;
	m_ReverbGain = gain;
}

int GOrgueSettings::GetReverbChannel()
{
	return m_ReverbChannel;
}

void GOrgueSettings::SetReverbChannel(int channel)
{
	if (channel < 1 || channel > 4)
		channel = 1;
	m_ReverbChannel = channel;
}

unsigned GOrgueSettings::GetReverbDelay()
{
	return m_ReverbDelay;
}

void GOrgueSettings::SetReverbDelay(unsigned delay)
{
	if (delay > 10000)
		delay = 10000;
	m_ReverbDelay = delay;
}

GOrgueMidiMap& GOrgueSettings::GetMidiMap()
{
	return m_MidiMap;
}

GOrgueTemperamentList& GOrgueSettings::GetTemperaments()
{
	return m_Temperaments;
}

void GOrgueSettings::Flush()
{
	wxString tmp_name = m_ConfigFileName + wxT(".new");
	GOrgueConfigFileWriter cfg_file;
	GOrgueConfigWriter cfg(cfg_file, false);

	GOrgueSettingStore::Save(cfg);
	cfg.WriteInteger(wxT("General"), wxT("InterpolationType"), m_InterpolationType);
	cfg.WriteInteger(wxT("General"), wxT("WaveFormat"), m_WaveFormat);
	cfg.WriteBoolean(wxT("General"), wxT("RecordDownmix"), m_RecordDownmix);
	cfg.WriteInteger(wxT("General"), wxT("AttackLoad"), m_AttackLoad);
	cfg.WriteInteger(wxT("General"), wxT("LoopLoad"), m_LoopLoad);
	cfg.WriteInteger(wxT("General"), wxT("ReleaseLoad"), m_ReleaseLoad);

	cfg.WriteInteger(wxT("General"), wxT("Concurrency"), m_Concurrency);
	cfg.WriteInteger(wxT("General"), wxT("ReleaseConcurrency"), m_ReleaseConcurrency);
	cfg.WriteInteger(wxT("General"), wxT("LoadConcurrency"), m_LoadConcurrency);

	cfg.WriteBoolean(wxT("General"), wxT("ManageCache"), m_ManageCache);
	cfg.WriteBoolean(wxT("General"), wxT("CompressCache"), m_CompressCache);
	cfg.WriteBoolean(wxT("General"), wxT("LoadLastFile"), m_LoadLastFile);
	cfg.WriteString(wxT("General"), wxT("LastFile"), m_LastFile);
	cfg.WriteBoolean(wxT("General"), wxT("StrictODFCheck"), m_ODFCheck);

	cfg.WriteInteger(wxT("General"), wxT("Channels"), m_Channels);
	cfg.WriteBoolean(wxT("General"), wxT("LosslessCompression"), m_LosslessCompression);
	cfg.WriteBoolean(wxT("General"), wxT("ManagePolyphony"), m_ManagePolyphony);
	cfg.WriteBoolean(wxT("General"), wxT("ScaleRelease"), m_ScaleRelease);
	cfg.WriteBoolean(wxT("General"), wxT("RandomizeSpeaking"), m_RandomizeSpeaking);
	cfg.WriteFloat(wxT("General"), wxT("MemoryLimit"), (double)m_MemoryLimit / (1024.0 * 1024.0));

	cfg.WriteInteger(wxT("General"), wxT("SamplesPerBuffer"), m_SamplesPerBuffer);
	cfg.WriteInteger(wxT("General"), wxT("SampleRate"), m_SampleRate);
	cfg.WriteInteger(wxT("General"), wxT("BitsPerSample"), m_BitsPerSample);
	cfg.WriteInteger(wxT("General"), wxT("Volume"), m_Volume);
	cfg.WriteInteger(wxT("General"), wxT("PolyphonyLimit"), m_PolyphonyLimit);
	cfg.WriteString(wxT("General"), wxT("SettingPath"), m_UserSettingPath);
	cfg.WriteString(wxT("General"), wxT("CachePath"), m_UserCachePath);
	cfg.WriteInteger(wxT("General"), wxT("Preset"), m_Preset);

	cfg.WriteInteger(wxT("General"), wxT("ReleaseLength"), m_ReleaseLength);

	cfg.WriteInteger(wxT("General"), wxT("OrganCount"), m_OrganList.size());
	for(unsigned i = 0; i < m_OrganList.size(); i++)
		m_OrganList[i]->Save(cfg, wxString::Format(wxT("Organ%03d"), i + 1), m_MidiMap);

	m_Temperaments.Save(cfg);

	for(unsigned i = 0; i < GetEventCount(); i++)
		m_MIDIEvents[i]->Save(cfg, GetEventSection(i), m_MidiMap);

	for(unsigned i = 0; i < m_AudioGroups.size(); i++)
		cfg.WriteString(wxT("AudioGroups"), wxString::Format(wxT("Name%03d"), i + 1), m_AudioGroups[i]);
	cfg.WriteInteger(wxT("AudioGroups"), wxT("Count"), m_AudioGroups.size());

	for(unsigned i = 0; i < m_AudioDeviceConfig.size(); i++)
	{
		cfg.WriteString(wxT("AudioDevices"), wxString::Format(wxT("Device%03dName"), i + 1), m_AudioDeviceConfig[i].name);
		cfg.WriteInteger(wxT("AudioDevices"), wxString::Format(wxT("Device%03dChannelCount"), i + 1), m_AudioDeviceConfig[i].channels);
		for(unsigned j = 0; j < m_AudioDeviceConfig[i].channels; j++)
		{
			wxString prefix = wxString::Format(wxT("Device%03dChannel%03d"), i + 1, j + 1);
			cfg.WriteInteger(wxT("AudioDevices"), prefix + wxT("GroupCount"), m_AudioDeviceConfig[i].scale_factors[j].size());
			for(unsigned k = 0; k < m_AudioDeviceConfig[i].scale_factors[j].size(); k++)
			{
				wxString p = prefix + wxString::Format(wxT("Group%03d"), k + 1);
				cfg.WriteString(wxT("AudioDevices"), p + wxT("Name"), m_AudioDeviceConfig[i].scale_factors[j][k].name);
				cfg.WriteFloat(wxT("AudioDevices"), p + wxT("Left"), m_AudioDeviceConfig[i].scale_factors[j][k].left);
				cfg.WriteFloat(wxT("AudioDevices"), p + wxT("Right"), m_AudioDeviceConfig[i].scale_factors[j][k].right);
			}
		}
	}
	cfg.WriteInteger(wxT("AudioDevices"), wxT("Count"), m_AudioDeviceConfig.size());

	cfg.WriteBoolean(wxT("Reverb"), wxT("ReverbEnabled"), m_ReverbEnabled);
	cfg.WriteBoolean(wxT("Reverb"), wxT("ReverbDirect"), m_ReverbDirect);
	cfg.WriteInteger(wxT("Reverb"), wxT("ReverbChannel"), m_ReverbChannel);
	cfg.WriteInteger(wxT("Reverb"), wxT("ReverbStartOffset"), m_ReverbStartOffset);
	cfg.WriteInteger(wxT("Reverb"), wxT("ReverbLen"), m_ReverbLen);
	cfg.WriteInteger(wxT("Reverb"), wxT("ReverbDelay"), m_ReverbDelay);
	cfg.WriteFloat(wxT("Reverb"), wxT("ReverbGain"), m_ReverbGain);
	cfg.WriteString(wxT("Reverb"), wxT("ReverbFile"), m_ReverbFile);

	unsigned count = 0;
	for (std::map<wxString, bool>::iterator it = m_MidiIn.begin(); it != m_MidiIn.end(); it++)
	{
		count++;
		cfg.WriteString(wxT("MIDIIn"), wxString::Format(wxT("Device%03d"), count), it->first);
		cfg.WriteBoolean(wxT("MIDIIn"), wxString::Format(wxT("Device%03dEnabled"), count), it->second);
		cfg.WriteInteger(wxT("MIDIIn"), wxString::Format(wxT("Device%03dShift"), count), GetMidiInDeviceChannelShift(it->first));
	}
	if (count > MAX_MIDI_DEVICES)
		count = MAX_MIDI_DEVICES;
	cfg.WriteInteger(wxT("MIDIIn"), wxT("Count"), count);

	count = 0;
	for (std::map<wxString, bool>::iterator it = m_MidiOut.begin(); it != m_MidiOut.end(); it++)
	{
		count++;
		cfg.WriteString(wxT("MIDIOut"), wxString::Format(wxT("Device%03d"), count), it->first);
		cfg.WriteBoolean(wxT("MIDIOut"), wxString::Format(wxT("Device%03dEnabled"), count), it->second);
	}
	if (count > MAX_MIDI_DEVICES)
		count = MAX_MIDI_DEVICES;
	cfg.WriteInteger(wxT("MIDIOut"), wxT("Count"), count);
	cfg.WriteString(wxT("MIDIOut"), wxT("MIDIRecorderDevice"), m_MidiRecorderOutputDevice);

	if (::wxFileExists(tmp_name) && !::wxRemoveFile(tmp_name))
	{
		wxLogError(_("Could not write to '%s'"), tmp_name.c_str());
		return;
	}
	if (!cfg_file.Save(tmp_name))
	{
		wxLogError(_("Could not write to '%s'"), tmp_name.c_str());
		return;
	}
	if (!GORenameFile(tmp_name, m_ConfigFileName))
		return;

	m_Config.Flush();
}
