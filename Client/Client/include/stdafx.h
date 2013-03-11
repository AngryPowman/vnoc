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
#include <atlstr.h>

#define _WTL_NO_CSTRING

#include "../../../ThirdLibs/TinyXML/tinyxml.h"

#include "../GUI/BKWin/wtl/atlapp.h"
#include "../GUI/BKWin/wtl/atlframe.h"
#include "../GUI/BKWin/wtl/atlctrls.h"
#include "../GUI/BKWin/wtl/atlctrlx.h"
#include "../GUI/BKWin/wtl/atldlgs.h"
#include "../GUI/BKWin/wtl/atlcrack.h"
#include "../GUI/BKWin/wtl/atlmisc.h"

#include "../GUI/BKWin/bkwin/bkatldefine.h"
#include "../GUI/BKWin/bkwin/bklistview.h"
#include "../GUI/BKWin/bkwin/bkdlgview.h"
#include "../GUI/BKWin/bkwin/bkwnddlgfile.h"
#include "../GUI/BKWin/bkres/bkres.h"
#include "../GUI/BKWin/bkwin/ListViewCtrlEx.h"

#include "../FrameWork/IGlobal.h"
#include "../util/util.h"
#include "XMessage.hpp"
using namespace VNOC::Message::Define;