//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndedit.h
// Description: Edit Control
//     Creator: skylly
//     Version: 2012.3.25 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkwndnotify.h"

class CBkEdit : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkEdit, "edit")

	CBkEdit() :
	  m_pImg(NULL)
		  , m_dwFont(0)
		  , hPenNormal(NULL)
		  , hPenFocus(NULL)
		  , m_hWndEditor(NULL)
		  , m_hBkBrush(NULL)
		  , m_bFocused(false)
		  , m_bMouseOver(false)
		  , m_bEnabled(true)  //之前忘了赋初值
		  , m_uMaxChar(0)
		  , m_bReadOnly(FALSE)
		  , m_bNumberMode(false)
		  , m_bMultiLine(false)
		  , m_bForever(true)   //是否永存
		  , m_bPasswordMode(FALSE)
		  , m_cPasswordChar(_T('*'))
		  ,	m_strGroup(_T(""))
	  {
	  }
	  ~CBkEdit()
	  {
		//  BkWnds::UnregisterEditGroup(this, m_strGroup);
	  }
	  bool IsEnabled()
	  {
		  return m_bEnabled;
	  }

	  BOOL IsPasswordMode()
	  {
		  return m_bPasswordMode;
	  }

	  TCHAR GetPasswordChar() const
	  {
		  return m_cPasswordChar;
	  }

	  // Create children
	  virtual BOOL Load(TiXmlElement* pTiXmlElem)
	  {  
		  if (!CBkWindow::Load(pTiXmlElem))
			  return FALSE;
		  return TRUE; //BkWnds::RegisterEditGroup(this, m_strGroup);
	  }

	  void OnDestroy()
	  {

	  }

	  HRESULT SetInnerText(LPCTSTR pText)
	  {
		  __super::SetInnerText(pText);
		  if (m_hWndEditor)
		  {
			  ::SetWindowText(m_hWndEditor, pText);
		  }
		  return 0;
	  }

	  void DrawBkColor(CDCHandle& dc)
	  {
		  COLORREF colorbk = 0;
		  if (m_bEnabled)
		  {
			 colorbk  = RGB(255, 255, 255);
		  }
		  else
		  {
			  colorbk = RGB(244, 244, 244);
		  }

		  dc.SetBkColor(colorbk);
		  dc.ExtTextOut(0, 0, ETO_OPAQUE, &m_rcWindow, NULL, 0, NULL);
	  }

	  void DrawBkImage(CDCHandle& dc)
	  {
	  }

	  virtual BOOL NeedRedrawWhenStateChange()
	  {
		  return TRUE;
	  }

	  void DrawStatusImage(CDCHandle& dc)
	  {
		  if (m_pImg == NULL)
		  {
			//  BkPngPool::_LoadPNGImageFromResourceID(3005, m_pImg);
		  }

		  Gdiplus::Graphics graphics(dc);
		  CRect rc = m_rcWindow;
		  //根据区域大小显示图片
		  graphics.DrawImage(m_pImg, Gdiplus::Rect(rc.left, rc.top, rc.Width(), rc.Height()), 0, 0, rc.Width(), rc.Height(), Gdiplus::UnitPixel);
	  }

	  LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	  {
		  LRESULT lRes = ::DefWindowProc(m_hWndEditor, uMsg, wParam, lParam);
		  if (!m_bForever)
			 ::PostMessage(m_hWndEditor, WM_CLOSE, 0, 0);
		  return lRes;
	  }

	  LRESULT OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	  {
		  // Copy text back
		  int cchLen = ::GetWindowTextLength(m_hWndEditor) + 1;
		  LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
		  ATLASSERT(pstr);
		  if( pstr == NULL ) return 0;
		  ::GetWindowText(m_hWndEditor, pstr, cchLen);
		  m_strInnerText = pstr;
		  //刷新bkwin控件
		  NotifyInvalidate();
		  //SendNotify(m_pOwner, _T("textchanged"));
		  return 0;
	  }

	  COLORREF GetTextColor()
	  {
		  return RGB(0, 0, 0);
	  }

	  COLORREF GetNativeEditBkColor()
	  {
		  return RGB(255, 255, 255);
	  }

	  BOOL StartTrackMouseLeave()
	  {
		  TRACKMOUSEEVENT tme = { 0 };
		  tme.cbSize = sizeof(tme);
		  tme.dwFlags = TME_LEAVE;
		  tme.hwndTrack = m_hWndEditor;
		  return _TrackMouseEvent(&tme);
	  }

	  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	  {
		  LRESULT lRes = 0;
		  bHandled = TRUE;

		  if (uMsg == WM_KILLFOCUS ) 
			  lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
		  else if (uMsg == OCM_COMMAND) 
		  {
			  if (HIWORD(wParam) == EN_CHANGE)
				  lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
			  else if( HIWORD(wParam) == EN_UPDATE ) 
			  {
				  RECT rcClient;
				  ::GetClientRect(m_hWndEditor, &rcClient);
				  ::InvalidateRect(m_hWndEditor, &rcClient, FALSE);
			  }
		  }
		  else if (uMsg == WM_MOUSELEAVE)
		  {
			  m_bMouseOver = false;
			  //刷新bkwin控件
			  NotifyInvalidate();
		  }
		  else if (uMsg == WM_MOUSEMOVE)
		  {
			  m_bMouseOver = true;
			  //刷新bkwin控件
			  NotifyInvalidate();
			 StartTrackMouseLeave();
		  }
		  else if( uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN ) 
		  {
			  //SendNotify(m_pOwner, _T("return"));
		  }
		  else if( uMsg == OCM__BASE + WM_CTLCOLOREDIT  || uMsg == OCM__BASE + WM_CTLCOLORSTATIC ) 
		  {
			  if( GetNativeEditBkColor() == 0xFFFFFFFF ) return NULL;
			  ::SetBkMode((HDC)wParam, TRANSPARENT);
			  DWORD dwTextColor = GetTextColor();
			  ::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor),GetGValue(dwTextColor),GetRValue(dwTextColor)));
			  if( m_hBkBrush == NULL ) {
				  DWORD clrColor = GetNativeEditBkColor();
				  m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
			  }
			  return (LRESULT)m_hBkBrush;
		  }
		  else 
		  {
			  bHandled = FALSE;
		  }
		  return lRes;
	  }

	  void OnFinalMessage(HWND hWnd)
	  {
		  // Clear reference and die
		  if( m_hBkBrush != NULL ) 
			  ::DeleteObject(m_hBkBrush);
	  }

	  static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	  {
		  CBkEdit* pThis = NULL;
		  if (uMsg == WM_NCCREATE) 
		  {
			  LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			  pThis = static_cast<CBkEdit*>(lpcs->lpCreateParams);
			  ::SetProp(hWnd, _T("WndX"), (HANDLE) pThis);
			  pThis->m_hWndEditor = hWnd;
		  } 
		  else
		  {
			  pThis = reinterpret_cast<CBkEdit*>(::GetProp(hWnd, _T("WndX")));
			  if (uMsg == WM_NCDESTROY && pThis != NULL) 
			  {
				  LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
				  // if( pThis->m_bSubclassed ) pThis->Unsubclass();
				  ::SetProp(hWnd, _T("WndX"), NULL);
				  pThis->m_hWndEditor = NULL;
				  pThis->OnFinalMessage(hWnd);
				  return lRes;
			  }
		  }
		  if (pThis != NULL) 
		  { 
			  BOOL bHandled;
			  pThis->HandleMessage(uMsg, wParam, lParam, bHandled);
		  } 
		  return ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
	  }

	  LPCTSTR GetSuperClassName() const
	  {
		  return WC_EDIT;
	  }

	  LPCTSTR GetWindowClassName() const
	  {
		  return _T("BkEdit");
	  }

	  //注册窗口类
	  bool RegisterSuperclass()
	  {
		  // Get the class information from an existing
		  // window so we can subclass it later on...
		  WNDCLASSEX wc = { 0 };
		  wc.cbSize = sizeof(WNDCLASSEX);
		  if( !::GetClassInfoEx(NULL, GetSuperClassName(), &wc) ) {
			  if( !::GetClassInfoEx((HINSTANCE)&__ImageBase, GetSuperClassName(), &wc) ) {
				  ATLASSERT(!"Unable to locate window class");
				  return NULL;
			  }
		  }
		  m_OldWndProc = wc.lpfnWndProc;
		  wc.lpfnWndProc = &CBkEdit::__ControlProc;
		  wc.hInstance = (HINSTANCE)&__ImageBase;
		  wc.lpszClassName = GetWindowClassName();
		  ATOM ret = ::RegisterClassEx(&wc);
		  ATLASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
		  return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
	  }
#define UI_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)
	  UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };

	  //无用
	  static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	  {
		  CBkEdit* pThis = NULL;
		  if( uMsg == WM_NCCREATE ) {
			  LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			  pThis = static_cast<CBkEdit*>(lpcs->lpCreateParams);
			  pThis->m_hWndEditor = hWnd;
			  ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
		  } 
		  else
		  {
			  pThis = reinterpret_cast<CBkEdit*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
			  if( uMsg == WM_NCDESTROY && pThis != NULL ) {
				  LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
				  ::SetWindowLongPtr(pThis->m_hWndEditor, GWLP_USERDATA, 0L);
				  //if( pThis->m_bSubclassed ) pThis->Unsubclass();
				  pThis->m_hWndEditor = NULL;
				  pThis->OnFinalMessage(hWnd);
				  return lRes;
			  }
		  }
		  if (pThis != NULL) 
		  { 
			  BOOL bHandled;
			  pThis->HandleMessage(uMsg, wParam, lParam, bHandled);
		  } 
		  return ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
	  }

	  //无用
	  bool RegisterWindowClass()
	  {
		  WNDCLASS wc = { 0 };
		  wc.style = GetClassStyle();
		  wc.cbClsExtra = 0;
		  wc.cbWndExtra = 0;
		  wc.hIcon = NULL;
		  wc.lpfnWndProc = &CBkEdit::__WndProc;
		  wc.hInstance = (HINSTANCE)&__ImageBase;
		  wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		  wc.hbrBackground = NULL;
		  wc.lpszMenuName  = NULL;
		  wc.lpszClassName = GetWindowClassName();
		  ATOM ret = ::RegisterClass(&wc);
		  ATLASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
		  return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
	  }
 
	  void InitEditor()
	  { 
		  if (!m_hWndContainer)
			  return;
		  DWORD dwStyle = WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_TABSTOP;
		  DWORD dwExStyle = 0;
		  HMENU hMenu = (HMENU)m_uCmdID;
		  CRect rc = m_rcWindow;
		  rc.DeflateRect(2, 2, 2, 2);  //留出边框
		  if (IsPasswordMode()) 
			  dwStyle |= ES_PASSWORD;
		
		  if (GetSuperClassName() != NULL && !RegisterSuperclass()) return;
		  if (GetSuperClassName() == NULL && !RegisterWindowClass()) return;
		  m_hWndEditor = ::CreateWindowEx(dwExStyle, GetWindowClassName(), NULL, dwStyle, rc.left, rc.top, rc.Width(),rc.Height(), m_hWndContainer,(HMENU)hMenu,(HINSTANCE)&__ImageBase, this);

		  if (!m_hWndEditor)
		  {
			  DWORD dd = GetLastError();
			  ATLVERIFY(FALSE);
			  return;
		  }

		  ::SendMessage(m_hWndEditor, WM_SETFONT, (WPARAM)((HFONT)BkFontPool::GetFont(LOWORD(m_dwFont))), MAKELPARAM(TRUE, 0));  

		  int cchMax = 100;
		  ::SendMessage(m_hWndEditor, EM_LIMITTEXT, (WPARAM)(cchMax), 0L);
		  if (IsPasswordMode()) 
			  ::SendMessage(m_hWndEditor, EM_SETPASSWORDCHAR, (WPARAM)(UINT)(GetPasswordChar()), 0L);
		  ::SetWindowText(m_hWndEditor, m_strInnerText);
		  ::SendMessage(m_hWndEditor, EM_SETMODIFY, (WPARAM)(UINT)(FALSE), 0L);
		  ::SendMessage(m_hWndEditor, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
		  bool bEnabled = IsEnabled();
		  ::EnableWindow(m_hWndEditor, bEnabled);
		  BOOL bReadOnly = m_bReadOnly;
		  ::SendMessage(m_hWndEditor, EM_SETREADONLY, (WPARAM)(BOOL)(bReadOnly), 0L);
		  ::ShowWindow(m_hWndEditor, SW_SHOWNOACTIVATE);//
		  //::SetFocus(m_hWndEditor);
	  }

	  void DrawText(CDCHandle& dc)
	  {	
		  CString  sText = m_strInnerText;
		  if (m_bPasswordMode ) 
		  {//密码框显示*
			  sText.Empty();
			  LPCTSTR p = m_strInnerText.GetBuffer(0);
			  while( *p != _T('\0') ) 
			  {
				  sText += m_cPasswordChar;
				  p = ::CharNext(p);
			  }
		  }

		  HFONT hftOld = dc.SelectFont(BkFontPool::GetFont(LOWORD(m_dwFont)));
		  CRect rcText = m_rcWindow;
		  rcText.DeflateRect(2, 2, 2, 2);   //留出边框 (2像素边框)
		  dc.DrawText(sText, sText.GetLength(), rcText, DT_SINGLELINE | DT_VCENTER);
		  dc.SelectFont(hftOld);
	  }

	  void DrawBtn(CDCHandle& dc)
	  {	
	  }

	  void DrawBorder(CDCHandle& dc)
	  {
		  COLORREF colorbd = 0;
		  CRect rc = m_rcWindow;
		  HPEN hPenOld = NULL;
		  if (m_bMouseOver)
		  {		
			  colorbd = RGB(133, 228, 255);
			  if (!hPenFocus)
			  {
				  hPenFocus = CreatePen(PS_INSIDEFRAME, 2, colorbd);
			  }
			  hPenOld = dc.SelectPen(hPenFocus);
		  }
		  else
		  {		
			  colorbd = RGB(78, 160, 209);
			  if (!hPenNormal)
			  {
				  hPenNormal = CreatePen(PS_INSIDEFRAME, 1, colorbd);  //colorbd
			  }
			  hPenOld = dc.SelectPen(hPenNormal);
		  }
		  HBRUSH hBrushOld = dc.SelectBrush((HBRUSH)::GetStockObject(HOLLOW_BRUSH));
		  dc.Rectangle(rc);
		  dc.SelectBrush(hBrushOld);
		  //CBrush brush1(::CreateSolidBrush(colorbd));
		  //dc.FrameRect( &rc, brush1);
		  dc.SelectPen(hPenOld);
	  }

	  // 绘制循序：背景颜色->背景图->状态图->文本->边框
	  void OnPaint(CDCHandle dc)
	  {
		  //dc.SetBkMode(TRANSPARENT);
		  DrawBkColor(dc);
		  DrawBkImage(dc);
		  DrawStatusImage(dc);
		  DrawText(dc);	
		  DrawBtn( dc );
		  DrawBorder(dc);	
	  } 

	  void SetFocus()
	  {
		  m_bFocused = true;
		  ::SetFocus(m_hWndEditor);
	  }

	  void OnLButtonDown(UINT nFlags, CPoint point)
	  { 
		  if( IsEnabled() )
		  {
			  if (m_hWndEditor == NULL)
			  {
				  InitEditor(); 
			  }
			  m_bFocused = true;
			  if (m_rcWindow.PtInRect(point) )
			  {
				  int nSize = GetWindowTextLength(m_hWndEditor);
				  if( nSize == 0 )
					  nSize = 1;

				  ::SendMessage(m_hWndEditor,  EM_SETSEL, (0), (nSize));
			  }
		  }//if( IsEnabled() )
	  }

	  void OnMouseMove(UINT nFlags, CPoint point)
	  {
		  if (m_bEnabled)
		  {
			  m_bMouseOver = true;
		  }
	  }

	  HWND _GetRealWindow()
	  {		
		  if (m_bForever && m_hWndEditor == NULL)
		  {
			  InitEditor(); 
		  }
		  return m_hWndEditor;
	  }

	  void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
	  {
		  CRect rcOldWnd = m_rcWindow;

		  __super::OnWindowPosChanged(lpWndPos);

		  if (rcOldWnd != m_rcWindow)
		  {
			  //移动窗口位置
			  HWND hWndReal = _GetRealWindow();
			  //by skylly 不让后面的子窗口排到前面的子窗口前面

			  CRect rc = m_rcWindow;
			    rc.DeflateRect(2, 2, 2, 2);  //留出边框
			  //::SetWindowPos(hWndReal, HWND_TOP, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(), SWP_NOREDRAW);
			  ::SetWindowPos(hWndReal, HWND_TOP, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER | SWP_NOREDRAW);

			  BKNMREALWNDRESIZED nms;
			  nms.hdr.code = BKNM_REALWND_RESIZED;
			  nms.hdr.hwndFrom = m_hWndContainer;
			  nms.hdr.idFrom = ::GetDlgCtrlID(m_hWndContainer);
			  nms.uItemID = GetCmdID();
			  nms.rcWnd = rc;

			  ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);

			  //显示窗口
			  if (::IsWindowVisible(hWndReal))
			  {
				  CRect rcUpdate = rc;
				  rcUpdate.MoveToXY(0, 0);
				  //::RedrawWindow(_GetRealWindow(), rcUpdate, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				  //::RedrawWindow(hWndReal, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
				  ::InvalidateRect(hWndReal, NULL, TRUE);
				  ::SetWindowPos(hWndReal, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
			  }
		  }
	  }

	  void ShowRealWindow(BOOL bShow)
	  {
		  CRect rc;
		  GetClientRect(_GetRealWindow(), rc);
		  if (rc != m_rcWindow)
		  {              
			  rc = m_rcWindow;
			  rc.DeflateRect(2, 2, 2, 2);  //留出边框
			  ::SetWindowPos(_GetRealWindow(), HWND_TOP, rc.left, rc.top, rc.Width(), rc.Height(),  SWP_NOZORDER | SWP_NOREDRAW);
		  }
		  ::ShowWindow(_GetRealWindow(), bShow ? SW_SHOW : SW_HIDE);
	  }

	  void OnShowWindow(BOOL bShow, UINT nStatus)
	  {
		  __super::OnShowWindow(bShow, nStatus);

		  if (bShow)
		  {
			  // 如果父窗口或者父窗口的父窗口隐藏（主要是由于Tab切换），则不显示真实窗口
			  CBkWindow* pWnd = this;

			  while (TRUE)
			  {
				  pWnd = BkWnds::GetWindow(pWnd->GetParent());

				  if (NULL == pWnd)
					  break;

				  if (!pWnd->IsVisible())
					  return;
			  }
		  }

		  ShowRealWindow(bShow);
	  }

	  void OnMouseLeave()
	  {
		  m_bMouseOver = false;
	  }

	  virtual void SetCursor()
	  {
		  ::SetCursor(::LoadCursor(NULL, (m_bMouseOver) ? IDC_IBEAM : IDC_ARROW));
	  }
protected:
	HBRUSH m_hBkBrush;  //画刷
	HPEN hPenNormal;  //pen
	HPEN hPenFocus;   //pen
	Gdiplus::Image* m_pImg;  //背景图
	WNDPROC m_OldWndProc; //原始窗口处理函数
	DWORD m_dwFont;   //字体
	HWND m_hWndEditor;  //控件句柄
	BOOL m_bPasswordMode; //密码模式
	TCHAR m_cPasswordChar; //密码掩码

	bool m_bEnabled;  //是否激活
	bool m_bFocused; //是否焦点
	bool m_bMouseOver;  //是否鼠标悬停

	//所属分组
	CStringA m_strGroup;

	bool m_bForever;  //是否永存

	//下面几个暂时未用到
	BOOL m_bReadOnly; //是否只读
	bool m_bNumberMode; //是否数字
	bool m_bMultiLine; //是否多行
	UINT m_uMaxChar;  //最大字符数

protected:
	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)  
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_SHOWWINDOW(OnShowWindow)
	BKWIN_END_MSG_MAP()

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_HEX_ATTRIBUTE("defaultfont", m_dwFont, FALSE)
		BKWIN_INT_ATTRIBUTE("password", m_bPasswordMode, FALSE)
		BKWIN_INT_ATTRIBUTE("readonly", m_bReadOnly, FALSE)

	BKWIN_DECLARE_ATTRIBUTES_END()
};
