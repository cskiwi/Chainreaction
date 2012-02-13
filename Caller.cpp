//-----------------------------------------------------------------
// Caller Object
// C++ Source - Caller.cpp - version 2008 v2_06_3
// Copyright Kevin Hoefman - kevin.hoefman@howest.be
// http://www.digitalartsandentertainment.be/

//---------------------------
// Includes
//---------------------------
#include "StdAfx.h"
#include "Caller.h"

//---------------------------
// Defines
//---------------------------
//

//-----------------------------------------------------------------
// Caller methods
//-----------------------------------------------------------------
bool Caller::AddActionListener(Callable* targetPtr)
{
	return AddListenerObject(targetPtr);
}	

bool Caller::RemoveActionListener(Callable* targetPtr) 
{
	return RemoveListenerObject(targetPtr);
}

bool Caller::CallListeners()   
{			
	for (vector<Callable*>::iterator it = m_TargetList.begin(); it != m_TargetList.end(); ++it)
	{
		(*it)->CallAction(this);	
	}

	return (m_TargetList.size() > 0);
}

bool Caller::AddListenerObject(Callable* targetPtr) 
{
	for (vector<Callable*>::iterator it = m_TargetList.begin(); it != m_TargetList.end(); ++it)
	{
		if ((*it) == targetPtr) return false;
	}

	m_TargetList.push_back(targetPtr);
	return true;
}
	
bool Caller::RemoveListenerObject(Callable* targetPtr) 
{
	vector<Callable*>::iterator pos = find(m_TargetList.begin(), m_TargetList.end(), targetPtr); // find algorithm from STL

	if (pos == m_TargetList.end()) return false;
	else
	{
		m_TargetList.erase(pos);
		return true;
	}
}
