// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

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
#include "../../TinyXML/tinyxml.h"

#include <atlbase.h>
#include <atlwin.h>
#include <atlstr.h>
#include <atltrace.h>

#define _WTL_NO_CSTRING


#include "src/BKWin/wtl/atlapp.h"
#include "src/BKWin/wtl/atlframe.h"
#include "src/BKWin/wtl/atlctrls.h"
#include "src/BKWin/wtl/atlctrlx.h"
#include "src/BKWin/wtl/atldlgs.h"
#include "src/BKWin/wtl/atlcrack.h"
#include "src/BKWin/wtl/atlmisc.h"

#include "src/BKWin/bkwin/bkatldefine.h"
#include "src/BKWin/bkwin/bklistview.h"
#include "src/BKWin/bkwin/bkdlgview.h"
#include "src/BKWin/bkwin/bkwnddlgfile.h"
#include "src/BKWin/bkres/bkres.h"
#include "src/BKWin/bkwin/ListViewCtrlEx.h"

#include "src/util/util.h"
#include "src/Global/IGlobal.h"

