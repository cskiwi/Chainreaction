//-----------------------------------------------------------------
// Audio Object
// C++ Source - Audio.h - version 2008 v2_06_3
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/

#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Caller.h"
#include "String.h"
//-----------------------------------------------------
// Class Forward Declarations
//-----------------------------------------------------
class ResourceLoader;
class Painter;
class InputState;
class Image;
class CollisionMesh;
class TextFormat;

#include <queue>
//-----------------------------------------------------------------
// Audio Class
//-----------------------------------------------------------------
class Audio : public Caller
{
public:
	Audio(String const& nameRef);
	Audio(int IDAudio, String const& typeRef);

	virtual ~Audio();

	String& GetName();
	String& GetAlias();
	int GetDuration();
	bool IsPlaying();
	bool IsPaused();
	void SwitchPlayingOff();				// internal use only, don't use this unless you know what you're doing
	void SetRepeat(bool repeat);
	bool GetRepeat();
	bool Exists();
	int GetVolume();
	int GetType();

	// these methods are called to instruct the object. The methods that perform the actual sendstrings are private.
	void Play(int msecStart = 0, int msecStop = -1);
	void Pause();
	void Stop();
	void SetVolume(int volume);

	// commands are queued and sent through a Tick() which should be called by the main thread (mcisendstring isn't thread safe) 
	// has the additional benefit of creating a delay between mcisendstring commands
	void Tick();

private:	
	// -------------------------
	// Datamembers
	// -------------------------
	static int m_nr;
	String m_FileName;
	String m_Alias;
	bool m_Playing, m_Paused;
	bool m_MustRepeat;
	HWND m_hWnd;
	int m_Duration;
	int m_Volume;

	// -------------------------
	// Member functions
	// -------------------------	
	
	void Create(String const& nameRef);
	void Extract(WORD id, String sType, String sFilename);

	// private mcisendstring command methods and command queue datamember
	queue<String> m_CommandQueue;
	void QueuePlayCommand(int msecStart);
	void QueuePlayCommand(int msecStart, int msecStop);
	void QueuePauseCommand();
	void QueueResumeCommand();
	void QueueStopCommand();
	void QueueVolumeCommand(int volume);		// add a m_Volume datamember? What volume does the video start on by default?
	void QueuePositionCommand(int x, int y);
	void QueueCommand(String const& commandRef);
	void SendMCICommand(String const& commandRef);
			
	// -------------------------
	// Handler functions
	// -------------------------	
	static LRESULT CALLBACK AudioProcStatic(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
	
	// -------------------------
	// Disabling default constructor, default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Audio();
	Audio(const Audio& aRef);
	Audio& operator=(const Audio& aRef);
};
 
