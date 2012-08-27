#pragma once

// Inner Notify (BkWindows to Container)
#define BKINM_FIRST             1000
#define BKINM_LAST              1999
// Invalidate Rect Notify
#define BKINM_INVALIDATERECT    (BKINM_FIRST + 1)
#define BKINM_UPDATE_DRAGITEM   (BKINM_FIRST + 2)
#define BKINM_GET_BITMAP        (BKINM_FIRST + 3)
#define BKINM_DELETE_THIS       (BKINM_FIRST + 4)

class CBkWnd;
typedef DWORD HBKWND;

struct BKINMINVALIDATERECT
{
	NMHDR       hdr;
	HBKWND      hbkWnd;
	RECT        rect;
};

struct BkInMesUpdateDragItem
{
	NMHDR       hdr;
	HBKWND      hbkWnd;
	BOOL        oper;    // false:get, true:set
	CBkWnd*     pBkWnd;
};

struct BkInMesGetBitmap
{
	NMHDR       hdr;
	HBKWND      hbkWnd;
	CBkWnd*     pBkWnd;
	HBITMAP     hBitmap;
};


struct BKDLG_POSITION_ITEM
{
    BOOL bCenter;
    BOOL bMinus;
    int  nPos;
};

struct BKDLG_POSITION
{
    int nType;      // 1:width+height; 2:left+height; 4:l,t,r,b
    union
    {
        struct
        {
            BKDLG_POSITION_ITEM Left;
            BKDLG_POSITION_ITEM Top;
            BKDLG_POSITION_ITEM Right;
            BKDLG_POSITION_ITEM Bottom;
        };
        struct
        {
            BKDLG_POSITION_ITEM width;
            BKDLG_POSITION_ITEM height;
        };
        BKDLG_POSITION_ITEM Item[4];
    };
};


interface IBkWnd;
interface IBkEffectObject;

enum DragOpt
{
	DRAGOPT_FROM_EXPLORER = 0,
	DRAGOPT_FORM_MYSELF = 1,
};
typedef std::vector<CString>     vStrings;
struct BkDragData
{	
	POINT					 ptSrc;
	POINT					 ptDest;
	DWORD					 dwEffect;
	DWORD					 dwDragOpt;
	DWORD					 dwSrcIndex;
	DWORD					 dwDestIndex;
	DWORD					 dwCmdID;
	IBkWnd*					 bkItem;
	vStrings*                pvStrings;

	BkDragData& operator =(BkDragData& item)
	{
		ptSrc = item.ptSrc;
		ptDest = item.ptDest;
		dwEffect = item.dwEffect;
		dwDragOpt = item.dwDragOpt;
		dwSrcIndex = item.dwSrcIndex;
		dwCmdID = item.dwCmdID;
		bkItem = item.bkItem;
		dwDestIndex = item.dwDestIndex;
		pvStrings = item.pvStrings;
		return *this;
	};
	BkDragData()
	{
		dwEffect = DROPEFFECT_NONE;
		ptSrc.x = 0;
		ptSrc.y = 0;
		ptDest.x = 0;
		ptDest.y = 0;
		dwDragOpt = DRAGOPT_FROM_EXPLORER;
		dwSrcIndex = -1;
		dwCmdID = 0;
		bkItem = 0;
		dwDestIndex = -1;
		pvStrings = NULL;
	}
};

struct NotifyInfo
{
    UINT    nAction;
    POINT   pt;
    IBkWnd* pFrom;
    DWORD   dwData;
    IBkEffectObject* pFromEffect;
};

typedef struct _BKM_EDITLIST_ARG
{
    BOOL    isCancel;
    int     itemIndex;
    CString editText;

    _BKM_EDITLIST_ARG()
    {
        isCancel = FALSE;
        itemIndex = -1;
        editText = L"";
    }

} BKM_EDITLIST_ARG;

enum BK_MES
{
    // CBkTree
    BKM_TREE_BASE    = 0x10000000,
    BKM_TREE_SELCHANGING,                   // ���㷢���仯��
    BKM_TREE_SELCHANGED,                    // ���㷢���仯��
    BKM_TREE_ACCEPTDROP,                    // ����drop�¼�
    BKM_TREE_NODE_EXPAND,                   // �㵽�ڵ��չ������ť����
    BKM_TREE_NODE_SELECT,                   // �㵽�ӽڵ�����
    BKM_TREE_COLLAPSING,                    // �ӽڵ�������
    BKM_TREE_COLLAPSED,                     // �ӽڵ������
    BKM_TREE_EXPANDING,                     // �ӽڵ�չ����
    BKM_TREE_EXPANDED,                      // �ӽڵ�չ����

    // CBkList
    BKM_LIST_BASE   = 0x10000100,
    BKM_LIST_HIT_HEAD,                      // ����б�ͷ
    BKM_LIST_SELCHANGED,                    // ���㷢���仯
    BKM_LIST_LDBCLICK,                      // ���˫��
    BKM_LIST_MOUSEWHEEL,                    // ������
    BKM_LIST_SCROLL_V,                      // �������
    BKM_LIST_SCROLL_H,                      // �������
    BKM_LIST_UPDATE_COL_WIDTH,              // �����п�
    BKM_LIST_ACCEPTDROP,                    // ����drop�¼�
    BKM_LIST_KEYDOWN,                       // 
    BKM_LIST_HOVER,                         // ���Hover��Row Col Index��
    BKM_LIST_LEAVE,                         // ���Leave Row Col (!Index������)
    BKM_LIST_QUERY_DRAG_ICON,               // ��ѯ�϶�ͼ��
    BKM_LIST_FILLROW,                         //

    // CBkEditList
    BKM_EDITLIST_BASE   = 0x10000200,
    BKM_EDITLIST_PREEDIT,                   // �༭ǰ
    BKM_EDITLIST_AFTEREDIT,                 // �༭��

    // CBkEdit
    BKM_EDIT_BASE   = 0x10000300,
    BKM_EDIT_PRESSENTER,                    // �س�ȷ��
	BKM_EDIT_PRESSESC,						// ESCȡ��
    BKM_EDIT_TEXTCHANGE,                    // ���ָ���

    // CBkTabControl
    BKM_TABCTRL_BASE   = 0x10000400,
    BKM_TABCTRL_PRESELCHANGE,               // ��ǩ����ǰ
    BKM_TABCTRL_SELCHANGED,                 // ��ǩ����

    // CBkIconView
    BKM_ICONVIEW_BASE   = 0x10000500,
    BKM_ICONVIEW_SELCHANGED,
    BKM_ICONVIEW_HOVER,
    BKM_ICONVIEW_LEAVE,
	BKM_ICONVIEW_CUSTOMDRAW,
    BKM_ICONVIEW_LCLICK,                    // �������
    BKM_ICONVIEW_RCLICK,                    // �Ҽ�����
    BKM_ICONVIEW_LDBCLICK,                  // ���˫��
	BKM_ICONVIEW_KEYDOWN,

    // CBkHotkey
    BKM_HOTKEY_BASE   = 0x10000600,
    BKM_HOTKEY_KILLFOCUS,                  // ���ָ���

    // CBkCombobox
    BKM_COMBOBOX_BASE   = 0x10000700,
    BKM_COMBOBOX_SELCHANGED,                // ѡ�����
    BKM_COMBOBOX_EDITCHANGED,
    BKM_COMBOBOX_EDITENTER,                 // �༭��س�ȷ��

    // Button
    BKM_BTN_BASE = 0x10000800,
    BKM_BTN_CLICK,                          // �����ť
    BKM_BTN_DROPDOWN,                       // ��������˵�����ť
    BKM_BTN_HOVER,                          // �������
    BKM_BTN_LEAVE,                          // ����Ƴ�

    //Checkbox
    BKM_CHECK_BASE = 0x10000A00,
    BKM_CHECK_CLICK,                        //���checkbox

    // scroll
    BKM_SCROLL_BASE = 0x10000B00,
    BKM_SCROLL_LINE_UP,                     // �����ͷ��(��)
    BKM_SCROLL_LINE_DOWN,                   // �����ͷ��(��)
    BKM_SCROLL_PAGE_UP,                     // ����հ���(��)
    BKM_SCROLL_PAGE_DOWN,                   // ����հ���(��)
    BKM_SCROLL_THUMB_POSITION,              // �϶�

    // Menu
    BKM_MENU_BASE = 0x10000C00,
    BKM_MENU_CLICK,                         // ����˵�


    // Effect
    BKM_EFFECT_BASE = 0x20000000,
    BKM_EFFECT_START,                       // �������ſ�ʼ
    BKM_EFFECT_END,                         // �������Ž���
    BKM_EFFECT_PART1_START,                 // �����¼�����һ�ζ������ſ�ʼ
    BKM_EFFECT_PART1_END,                   // �����¼�����һ�ζ������Ž���
    BKM_EFFECT_PART2_START,                 // �����¼����ڶ��ζ������ſ�ʼ
    BKM_EFFECT_PART2_END,                   // �����¼����ڶ��ζ������ſ�ʼ

	//dragdrop
	BKM_DRAGDROP_ACCEPTDROP_FROM_EXPLORER = 0x20000100,    // drop explorer
	BKM_DRAGDROP_ACCEPTDROP_FROM_WINDOW_INNER = 0x20000101,// drop window
	BKM_DRAGDROP_DRAGOVER_FROM_EXPLORER = 0x20000102,      // dragover explorer
	BKM_DRAGDROP_DRAGOVER_FROM_WINDOW_INNER = 0x20000103,  // dragover window
};


#define BEGIN_ACTION_MAP(theClass)                      \
    BOOL theClass::Action(NotifyInfo* pInfo)            \
    {                                                   \
        if (pInfo != NULL && pInfo->pFrom != NULL)      \
        {                                               \
            int nBkWndID = pInfo->pFrom->GetCmdID();    \
            switch (nBkWndID)                           \
            {                                           \
            case 0:                                     \
                break;

#define BEGIN_ACTION_MAP_EX(theClass)                   \
    BOOL theClass::Action(NotifyInfo* pInfo)            \
    {                                                   \
        if (pInfo != NULL && pInfo->pFrom != NULL)      \
        {                                               \
            int nBkWndID = pInfo->pFrom->GetCmdID();    \
            switch (nBkWndID)                           \
            {                                           


#define ON_CTRL_ACTION_VOID(id, act, fun)               \
            case id:                                    \
            {                                           \
                if (act == pInfo->nAction)              \
                    fun();                              \
                break;                                  \
            }

#define ON_CTRL_ACTION_RETURN(id, act, fun)             \
            case id:                                    \
            {                                           \
                if (act == pInfo->nAction)              \
                {                                       \
                    return fun(pInfo);                  \
                }                                       \
            }                                           

#define ON_CLICK(id, fun)       ON_CTRL_ACTION_VOID(id, WM_LBUTTONUP, fun)

#define ON_CTRL(id, fun)                                \
            case id:                                    \
            {                                           \
                return fun(pInfo);                      \
            }

#define ON_CTRL_ACTION(id)                              \
            case id:                                    \
            {                                           

#define ON_ACTION(act,fun)                              \
                if (act == pInfo->nAction)              \
                    return fun(pInfo);                  \

#define ON_CTRL_ACTION_END                              \
            }                                           \

#define ON_OBJECT_EVENT                                 \
            default:                                    \
            {

#define ON_OBJECT_EVENT_END                             \
            }

#define ON_OBJECT(obj, fun)                             \
            if (obj == pInfo->pFrom)                    \
            {                                           \
                return fun(pInfo);                      \
            }                                       

#define ON_OBJECT_ACTION(obj)                           \
            if (obj == pInfo->pFrom)                    \
            {

#define ON_OBJECT_ACTION_END                            \
            }               

#define END_ACTION_MAP                                  \
            default:                                    \
                break;                                  \
            }                                           \
        }                                               \
    return FALSE;                                       \
    }

#define END_ACTION_MAP_EX                               \
            }                                           \
        }                                               \
    return FALSE;                                       \
    }