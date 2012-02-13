//=======================================================================================
// GameTimer.h by Frank Luna (C) 2008 All Rights Reserved.
// Adapted for DAE GP1 by Bart Uyttenhove
//=======================================================================================

#ifndef GAMETIMER_H
#define GAMETIMER_H


class GameTimer
{
public:
	GameTimer();

	double GetGameTime()const;  // in seconds
	double GetDeltaTime()const; // in seconds

	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop();  // Call when paused.
	void Tick();  // Call every frame.
	bool IsStopped() const { return m_bStopped; }

private:
	double m_SecondsPerCount;
	double m_DeltaTime;

	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_StopTime;
	__int64 m_PrevTime;
	__int64 m_CurrTime;

	bool m_bStopped;
};

#endif // GAMETIMER_H