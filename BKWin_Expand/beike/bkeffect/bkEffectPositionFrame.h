#pragma once
#include "bkEffectObject.h"
#include "../math/Spline.h"
#include "../math/Vector2.h"

//////////////////////////////////////////////////////////////////////////
//   File Name: bkEffectRealMove.h
// Description: 动态特效 -移动
//				
//     Creator: YangDa
//     Version: 2011.09.08 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////


class CBkEffectPositionFrame : public CBkEffectObject
{
    BKOBJ_DECLARE_CLASS_NAME(CBkEffectPositionFrame, "effect-positionframe")

public:
    CBkEffectPositionFrame();
    virtual ~CBkEffectPositionFrame();
    virtual BOOL Load(TiXmlElement* pXmlElem);
    virtual void Init(EffectActionInfo info);
    virtual void Render(CDCHandle& dc);
    virtual BOOL UpdateInfo();


protected:
	virtual BOOL OnPreEnd();
	virtual BOOL OnPreStart();
	virtual BOOL _CreateEffectImage();
	inline BKMath::Vector2 ParseVector2(CStringA strValue)
	{
		BKMath::Vector2 temp;
		if (!strValue.IsEmpty())
		{
			strValue.Remove(' ');
			strValue.Remove('\t');
			int nPos = 0;
			nPos = strValue.Find(',');
			temp.x = atof(strValue.Mid(0,nPos));
			temp.y = atof(strValue.Mid(nPos+1));
		}
		return temp;
	}
    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
    BKWIN_DECLARE_ATTRIBUTES_END()

protected:
    CRect       m_rcDraw;
	CRect       m_rcOriginal;
	BKMath::Spline<BKMath::Vector2> m_spline;
};

inline BOOL CBkEffectPositionFrame::Load( TiXmlElement* pXmlElem )
{
	if(!CBkObject::Load(pXmlElem))
		return FALSE;
	TiXmlElement * pTiXmlChildElem = pXmlElem->FirstChildElement();
	for (TiXmlElement* pXmlChild = pTiXmlChildElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
	{
		float t;
		t = atof(pXmlChild->Attribute("time"));
		m_spline.append(t,ParseVector2(pXmlChild->Attribute("pos")));
	}
	return TRUE;
}

#include "bkEffectPositionFrame.inl"