//-----------------------------------------------------------------
// GameConfigurator Object
// C++ Header - GameConfigurator.h - version 2011 v1_33
// Author Bart Uyttenhove 
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------

//-----------------------------------------------------
// Class Forward Declarations
//-----------------------------------------------------



//-----------------------------------------------------
// GameConfigurator Class									
//-----------------------------------------------------
class GameConfigurator
{
public:
	GameConfigurator();				// Constructor
	virtual ~GameConfigurator();		// Destructor

	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------
	//Sets the width of the  window client area
	//e.g. SetWidth(640);
	void SetWidth(int widthInPixels);
	//Sets the height of the  window client area
	//e.g. SetHeight(480);
	void SetHeight(int heightInPixels);
	// Sets the number of times GamePaint has to be called per second
	// 0 (the default) means synchronize with the display refresh rate.
	// (avoids tearing and jerking)
	void SetNumPaintsPerSec(int numPaintsPerSec);

	// Sets the number of times GameTick() has to be called per second
	// 0 (the default) means tick at the same rate as paint. 
	void SetNumTicksPerSec(int numTicksPerSec);

	// Sets the maximum delta-time passed to the game's Tick method.
	// Very large delta-times can cause numerical instability.
	// The default value is 0.1 (seconds)
	void SetMaxDeltaTime(double maxDeltaTime);

	void SetWindowTitle(String windowTitle);

	int GetRenderTargetWidth()const {return m_RendertargetWidth;}
	int GetRenderTargetHeight()const {return m_RendertargetHeight;}

	int GetNumPaintsPerSecs()const {return m_NumPaintsPerSec;}
	int GetNumTicksPerSecs()const {return m_NumTicksPerSec;}

	double GetMaxDeltaTime() const {return m_MaxDeltaTime;}
	
	bool GetWaitForDisplayRefresh() const { return m_NumPaintsPerSec==0; }

	//GetWindowTitle: returns the window title
	String GetWindowTitle()const {return m_sWindowTitle;}

	//EnableWindowScale: When window is resized:
	//Param b: if true then the window content is rescaled by the operating system
	//Paran b: if false, then the rendertarget is resized.
	void EnableWindowScale(bool b);

	//IsWindowScaleEnabled: not intended to be used by students
	bool IsWindowScaleEnabled()const;

	//SetWindowScale: set the scaling factors of the window
	void SetWindowScale(double scale);

	//GetWindowScale: not intended to be used by students
	double GetWindowScale()const;

	//EnableWindowResize: if argument is true, the window can not be resized
	void EnableWindowResize(bool bFixed){m_bEnableWindowResize=bFixed;}

	//IsWindowResizable: return true if window can be resized
	bool IsWindowResizable()const{return m_bEnableWindowResize;}

private: 
	//-------------------------------------------------
	// Eigen methoden								
	//-------------------------------------------------


	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	int m_RendertargetWidth;
	int m_RendertargetHeight;
	int m_NumPaintsPerSec;
	int m_NumTicksPerSec;
	double m_MaxDeltaTime;

	String m_sWindowTitle;

	bool m_bEnableWindowScale;
	double m_WindowScale;
	bool m_bEnableWindowResize;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameConfigurator(const GameConfigurator& yRef);									
	GameConfigurator& operator=(const GameConfigurator& yRef);	
};

 
