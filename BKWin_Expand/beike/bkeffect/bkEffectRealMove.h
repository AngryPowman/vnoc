#pragma once
#include "bkEffectObject.h"

//////////////////////////////////////////////////////////////////////////
//   File Name: bkEffectRealMove.h
// Description: 动态特效 -移动
//				
//     Creator: YangDa
//     Version: 2011.09.08 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////



class CBkEffectRealMove : public CBkEffectObject
{
    BKOBJ_DECLARE_CLASS_NAME(CBkEffectRealMove, "effect-realmove")

public:
    CBkEffectRealMove();
    virtual ~CBkEffectRealMove();

    virtual void Init(EffectActionInfo info);
    virtual void Render(CDCHandle& dc);
    virtual BOOL UpdateInfo();


protected:
	virtual BOOL OnPreEnd();
	virtual BOOL OnPreStart();
	inline HRESULT SetDirection(CStringA& strValue, BOOL bLoading)
	{
		int x,y;
		if (!strValue.IsEmpty())
		{
			strValue.Remove(' ');
			strValue.Remove('\t');
			int nPos = 0;
			nPos = strValue.Find(',');
			x = ::StrToIntA(strValue.Mid(0,nPos));
			y = ::StrToIntA(strValue.Mid(nPos+1));
			m_pDirection = CPoint(x,y);
			return S_OK;
		}

		return S_FALSE;
	}
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_CUSTOM_ATTRIBUTE("dir", SetDirection)  
    BKWIN_DECLARE_ATTRIBUTES_END()

protected:
	CPoint      m_pDirection;
    CRect       m_rcDraw;
	CRect       m_rcOriginal;
};


#include "bkEffectRealMove.inl"