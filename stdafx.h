// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
// We'll use std::min and std::max
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define TIXML_USE_STL
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <ctype.h>
#include <stddef.h>
#include <algorithm>
#include <iterator> 
using namespace std;

//Direct2D headers
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <tchar.h>
#include <string>
#include <sstream>
#include <iostream>//  cout
#include <fstream>//file io

using namespace std;
using namespace D2D1;
// libs used for Direct2D
#pragma comment(lib, "d2d1.lib")			
#pragma comment(lib, "WindowsCodecs.lib")	//WIC
#pragma comment(lib, "dwrite.lib")			//WIC

#include "String.h"

template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = NULL;
    }
}
template<class Interface>
inline void SafeDelete(Interface **ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL)
    {
        delete (*ppInterfaceToRelease);
        (*ppInterfaceToRelease) = NULL;
    }
}

struct ColorRGBA
{
	int r,g,b,a;
	ColorRGBA():r(0),g(0),b(0),a(1){}
	ColorRGBA(int cr, int cg, int cb, int ca):r(cr), g(cg), b(cb), a(ca){}
};

#ifdef _UNICODE
#define tstring wstring
#define tstringstream wstringstream
#else
#define tstring string
#define tstringstream stringstream
#endif

#undef LoadImage