//-----------------------------------------------------------------
// Caller Object
// C++77 Source - Caller.h - version 2008 v2_06_3
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/

#pragma once

#include <windows.h>

#include <vector>
#include <stdlib.h>
#include <algorithm>
using namespace std;


//------------------------------------------------------------------------------------------------
// Callable Interface
//
// Interface implementation for classes that can be called by "caller" objects
//------------------------------------------------------------------------------------------------
class Caller;	// forward declaration

class Callable
{
public:
	virtual ~Callable() {}						// virtual destructor for polymorphism
	virtual void CallAction(Caller* callerPtr) = 0;
};

//------------------------------------------------------------------------------------------------
// Caller Base Class
//
// Base Clase implementation for up- and downcasting of "caller" objects: TextBox, Button, Timer, Audio and Video
//------------------------------------------------------------------------------------------------
class Caller
{
public:
	virtual ~Caller() {}				// do not delete the targets!

	static const int TextBox = 0;
	static const int Button = 1;
	static const int Timer = 2;
	static const int Audio = 3;
	static const int Video = 4;
    static const int Slider = 5;

	virtual int GetType() = 0;

	virtual bool AddActionListener(Callable* targetPtr);		// public interface method, call is passed on to private method
	virtual bool RemoveActionListener(Callable* targetPtr);	// public interface method, call is passed on to private method

protected:
	Caller() {}								// constructor only for derived classes
	vector<Callable*> m_TargetList;

	virtual bool CallListeners();			// placing the event code in a separate method instead of directly in the windows messaging
											// function allows inheriting classes to override the event code. 

private:
	bool AddListenerObject(Callable* targetPtr);
	bool RemoveListenerObject(Callable* targetPtr);

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, the declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Caller(const Caller& cRef);
	Caller& operator=(const Caller& cRef);
};