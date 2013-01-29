#pragma once

#define _SECURE_ATL 1
#define NOLAYERS

// Windows Header Files:
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ShellAPI.h>
#include <CommCtrl.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here

#include <atlbase.h>
#include <atlwin.h>
#include <atltrace.h>

#include "../FrameWork/IGlobal.h"
#include "../util/util.h"
#include "XMessage.hpp"
