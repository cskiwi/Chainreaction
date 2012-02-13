//-----------------------------------------------------------------
// Audio Object
// C++ Source - Audio.h - version 2008 v2_06_3
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/

//---------------------------
// Includes
//---------------------------
#include "StdAfx.h"
#include "resource.h"
#include "Audio.h"

#include "Painter.h"
#include "ResourceLoader.h"
#include "InputState.h"
#include "CollisionMesh.h"
#include "Image.h"
#include "TextFormat.h"

#include <Mmsystem.h>	// winmm.lib header, used for playing sound

//-----------------------------------------------------------------
// Pragma Library includes
//-----------------------------------------------------------------
#pragma comment(lib, "winmm.lib")		// used for sound

//---------------------------
// Defines
//---------------------------
//

//-----------------------------------------------------------------
// Audio methods
//-----------------------------------------------------------------

// set static datamember to zero
int Audio::m_nr = 0;

#pragma warning(disable:4311)
#pragma warning(disable:4312)
Audio::Audio(String const& nameRef) : m_Playing(false), m_Paused(false), m_MustRepeat(false), m_hWnd(0), m_Volume(100)
{	
	if (nameRef.EndsWith(".mp3") || nameRef.EndsWith(".wav") || nameRef.EndsWith(".mid"))
	{
		m_Alias = String("audio") + m_nr++;
		m_FileName = nameRef;

		Create(nameRef);
	}
}

Audio::Audio(int IDAudio, String const& typeRef) : m_Playing(false), m_Paused(false), m_MustRepeat(false), m_hWnd(0), m_Volume(100)
{
	if (typeRef == "MP3" || typeRef == "WAV" || typeRef == "MID")
	{
		m_Alias = String("audio") + m_nr++;
		m_FileName = String("temp\\") + m_Alias;

		if (typeRef == "MP3") m_FileName += ".mp3";
		else if (typeRef == "WAV") m_FileName += ".wav";
		else m_FileName += ".mid";
			
		Extract(IDAudio, typeRef, m_FileName);

		Create(m_FileName);
	}
}

void Audio::Create(const String& nameRef)
{
	TCHAR buffer[100];

	String sendString;

	if (nameRef.EndsWith(".mp3")) sendString = String("open \"") + m_FileName + "\" type mpegvideo alias " + m_Alias;
	else if (nameRef.EndsWith(".wav")) sendString = String("open \"") + m_FileName + "\" type waveaudio alias " + m_Alias;
	else if (nameRef.EndsWith(".mid")) sendString = String("open \"") + m_FileName + "\" type sequencer alias " + m_Alias;

	int result = mciSendString(sendString.ToTChar(), 0, 0, 0);	
	if (result != 0) return;
	
	sendString = String("set ") + m_Alias + " time format milliseconds";
	mciSendString(sendString.ToTChar(), 0, 0, 0);

	sendString = String("status ") + m_Alias + " length";
	mciSendString(sendString.ToTChar(), buffer, 100, 0);

	m_Duration = String(buffer).ToInteger();
	
	// Create a window to catch the MM_MCINOTIFY message with
	m_hWnd = CreateWindow(TEXT("STATIC"), TEXT(""), 0, 0, 0, 0, 0, 0, 0, GetModuleHandle(0), 0);
	SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG) AudioProcStatic);	// set the custom message loop (subclassing)
	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG) this);			// set this object as the parameter for the Proc
}

void Audio::Extract(WORD id , String sType, String sFilename)
{
	CreateDirectory(TEXT("temp\\"), NULL);

    HRSRC hrsrc = FindResource(NULL, MAKEINTRESOURCE(id), sType.ToTChar());
    HGLOBAL hLoaded = LoadResource( NULL, hrsrc);
    LPVOID lpLock =  LockResource(hLoaded);
    DWORD dwSize = SizeofResource(NULL, hrsrc);
    HANDLE hFile = CreateFile(sFilename.ToTChar(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD dwByteWritten;
    WriteFile(hFile, lpLock , dwSize , &dwByteWritten , NULL);
    CloseHandle(hFile);
    FreeResource(hLoaded);
} 

#pragma warning(default:4311)
#pragma warning(default:4312)

Audio::~Audio()
{
	Stop();

	String sendString = String("close ") + m_Alias;
	mciSendString(sendString.ToTChar(), 0, 0, 0);

	// release the window resources if necessary
	if (m_hWnd)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = 0;
	}
}

void Audio::Play(int msecStart, int msecStop)
{
	if (!m_Playing)
	{
		m_Playing = true;
		m_Paused = false;

		if (msecStop == -1) QueuePlayCommand(msecStart);
		else QueuePlayCommand(msecStart, msecStop);
	}	
	else if (m_Paused)
	{
		m_Paused = false;

		QueueResumeCommand();
	}
}

void Audio::Pause()
{
	if (m_Playing && !m_Paused) 
	{
		m_Paused = true;

		QueuePauseCommand();
	}
}

void Audio::Stop()
{
	if (m_Playing)
	{
		m_Playing = false;
		m_Paused = false;

		QueueStopCommand();
	}
}

void Audio::QueuePlayCommand(int msecStart)
{
	QueueCommand(String("play ") + m_Alias + " from " + msecStart +  " notify");
}

void Audio::QueuePlayCommand(int msecStart, int msecStop)
{
	QueueCommand(String("play ") + m_Alias + " from " + msecStart + " to " + msecStop + " notify");
}

void Audio::QueuePauseCommand()
{
	QueueCommand(String("pause ") + m_Alias);
}

void Audio::QueueResumeCommand()
{
	QueueCommand(String("resume ") + m_Alias);
}

void Audio::QueueStopCommand()
{
	QueueCommand(String("stop ") + m_Alias);
}

void Audio::QueueVolumeCommand(int volume)
{
	QueueCommand(String("setaudio ") + m_Alias + " volume to " + volume * 10);
}

void Audio::QueueCommand(String const& commandRef)
{
	//OutputDebugString(String("Queueing command: ") + command + "\n");
	m_CommandQueue.push(commandRef);
}

void Audio::Tick()
{
	if (!m_CommandQueue.empty())
	{
		SendMCICommand(m_CommandQueue.front());
		//OutputDebugString(String("Executing queued command: ") + m_CommandQueue.front() + "\n");
		m_CommandQueue.pop();
	}
}

void Audio::SendMCICommand(String const& commandRef)
{
	int result = mciSendString(commandRef.ToTChar(), 0, 0, m_hWnd);
}

String& Audio::GetName()
{
	return m_FileName;
}
	
String& Audio::GetAlias()
{
	return m_Alias;
}

bool Audio::IsPlaying()
{
	return m_Playing;
}

bool Audio::IsPaused()
{
	return m_Paused;
}

void Audio::SwitchPlayingOff()
{
	m_Playing = false;
	m_Paused = false;
}

void Audio::SetRepeat(bool repeat)
{
	m_MustRepeat = repeat;
}

bool Audio::GetRepeat()
{
	return m_MustRepeat;
}

int Audio::GetDuration()
{
	return m_Duration;
}

void Audio::SetVolume(int volume)
{
	m_Volume = min(100, max(0, volume));	// values below 0 and above 100 are trimmed to 0 and 100, respectively

	QueueVolumeCommand(volume);
}

int Audio::GetVolume()
{
	return m_Volume;
}

bool Audio::Exists()
{
	return m_hWnd?true:false;
}

int Audio::GetType()
{
	return Caller::Audio;
}

LRESULT Audio::AudioProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	#pragma warning(disable: 4312)
	Audio* audio = reinterpret_cast<Audio*>(GetWindowLong(hWnd, GWL_USERDATA));
	#pragma warning(default: 4312)

	switch (msg)
	{		
	case MM_MCINOTIFY: // message received when an audio file has finished playing - used for repeat function

		if (wParam == MCI_NOTIFY_SUCCESSFUL && audio->IsPlaying()) 
		{
			audio->SwitchPlayingOff();

			if (audio->GetRepeat()) audio->Play();	// repeat the audio
			else audio->CallListeners();			// notify listeners that the audio file has come to an end
		}
	}
	return 0;	
}