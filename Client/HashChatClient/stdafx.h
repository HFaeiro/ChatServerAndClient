// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define BUFFER_SIZE 512


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define DEFAULT_PORT 6420
// Windows Header Files:
#include <WinSock2.h>
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <commctrl.h>
/////////////////////////////////////////////////////////


#include <WS2tcpip.h>
#include <stdio.h>
#include <conio.h>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include "time.h"

//
//#include <gdiplus.h>
//#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "Ws2_32.lib")
//#pragma comment(lib, "comctl32.lib")