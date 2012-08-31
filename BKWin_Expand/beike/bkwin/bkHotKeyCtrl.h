//////////////////////////////////////////////////////////////////////////
//   File Name: bkHotKeyCtrl.h
// Description: HotKey Control
//     Creator: Liuan
//     Version: 2011.07.28 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "bkwndedit.h"

#define BK_HOTKEYF_SHIFT           0x01 //1
#define BK_HOTKEYF_CONTROL         0x02 //2
#define BK_HOTKEYF_ALT             0x04 //3
#define BK_HOTKEYF_EXT               0x08 //4
#define MAPVK_VK_TO_VSC      0
#define MAPVK_VSC_TO_VK      1
#define MAPVK_VK_TO_CHAR     2
#define MAPVK_VSC_TO_VK_EX   3
#define MAPVK_VK_TO_VSC_EX   4

#define  MAX_UNSETABLE_HOTKEY 40
struct hotkey_data
{
	BOOL bControlKey;
	BOOL bShiftKey;
	BOOL bAltKey;
	UINT nKey;
};
class CBkHotKeyCtrl: public CBkEdit
{
public:
	  BKOBJ_DECLARE_CLASS_NAME(CBkHotKeyCtrl, "hotkeyctrl")
      CBkHotKeyCtrl();
	  virtual ~CBkHotKeyCtrl();
public:
	BEGIN_MSG_MAP_EX(CBkHotKeyCtrl)
		MESSAGE_HANDLER( WM_KEYDOWN, OnKeyDown )
	    MESSAGE_HANDLER( WM_CHAR, OnChar)
		MESSAGE_HANDLER(WM_SYSKEYDOWN, OnSysKeyDown)
		MESSAGE_HANDLER(WM_SYSCHAR, OnSysChar)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode )
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER( WM_MOUSEMOVE, OnMouseMove )
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
        CHAIN_MSG_MAP(CBkEdit)
		END_MSG_MAP()
public:
      LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	  LRESULT OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	  LRESULT OnSysKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	  LRESULT OnSysChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	  LRESULT OnGetDlgCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	  LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	  LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	  LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	  LRESULT  OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	  LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	  
public:
	virtual void CreateHandle( HWND hParent );
	void  SetHotKey(WORD wVirtualKeyCode,WORD wModifiers);
	DWORD GetHotKey() const;
	void  GetHotKey(WORD &wVirtualKeyCode,WORD &wModifiers)const;
    CString GetHotKeyName( );
	static CString GetKeyName(UINT vk,BOOL fExtended );
     BOOL IsUnsetableHotKey();
	 BOOL IsEqualKey();
protected:
	WORD m_wVirtualKeyCode;
	WORD m_wModifiers;
private:
	BOOL IsExtendKey(UINT vk); 
	void UpdateModifiers(BOOL bExtended);
	static hotkey_data m_unsetableHotKey[MAX_UNSETABLE_HOTKEY];
	
};
__declspec(selectany) hotkey_data CBkHotKeyCtrl::m_unsetableHotKey[] = 
{
	{1, 0, 0, 'O'},
	{1, 0, 0, VK_F4},
	{0, 1, 1, 'Q'},
	{0, 0,0,VK_PRIOR},
	{0, 0, 0, VK_NEXT},
	{0, 0, 0, VK_ESCAPE},
	{1,0, 0, 'C'},
	{1, 0, 0, 'V'},
	{1, 0, 0, 'Z'},
	{1, 0, 0, 'X'},
	{0, 0, 0, VK_HOME},
	{0, 0, 0, VK_END},
	{0, 0, 0, VK_DELETE},
	{0, 0, 0, VK_BACK},
	{0, 0, 0, VK_TAB},
	{0, 0, 0, VK_LWIN},
	{0, 0, 0, VK_RWIN},
	{0, 0, 0, VK_RETURN},
	{0, 0, 0, VK_CAPITAL},
	{0, 0, 0, VK_MENU},
	{0, 0, 0, VK_CONTROL},
	{0, 0, 0, VK_SHIFT},
	{0, 0, 0, VK_SPACE},
	{1, 0, 0, VK_TAB},
	{0, 1, 1, 'S'},
	{0, 0, 0, VK_F1}

};
#include "bkHotKeyCtrl.inl"