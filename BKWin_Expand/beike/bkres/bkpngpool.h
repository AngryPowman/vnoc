//////////////////////////////////////////////////////////////////////////
//  Class Name: BKPngPool
// Description: PNG Pool
//     Creator: ZhangXiaoxuan
//     Version: 2010.2.24 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")

#include <atlcoll.h>

#include "bkresutil.h"

class BkPngPool
{
public:
    BkPngPool()
    {
		m_gdiplusToken = 0;
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
    }
    virtual ~BkPngPool()
    {
		Release();
    }

	

	static void Release()
	{
		if (_Instance()->m_gdiplusToken)
		{
			Gdiplus::Image *pImg = NULL;

			POSITION pos = _Instance()->m_mapPng.GetStartPosition();

			while (pos != NULL) 
			{
				pImg = _Instance()->m_mapPng.GetNextValue(pos);
				if (pImg)
					delete pImg;
			}

			_Instance()->m_mapPng.RemoveAll();

			Gdiplus::GdiplusShutdown(_Instance()->m_gdiplusToken);
			_Instance()->m_gdiplusToken = NULL;
		}
	}

    typedef CAtlMap<CString, Gdiplus::Image *> _TypePngPool;

    static Gdiplus::Image* Get(LPCTSTR szRes)
    {
        _TypePngPool::CPair* pPairRet = _Instance()->m_mapPng.Lookup(szRes);
        Gdiplus::Image* pImg = NULL;

        if (NULL == pPairRet)
        {
            _LoadPNGImageFromResourceID(szRes, pImg);
            if (pImg)
                _Instance()->m_mapPng[szRes] = pImg;
        }
        else
            pImg = pPairRet->m_value;

        return pImg;
    }

    static Gdiplus::Image* GetGif(LPCTSTR szRes)
    {
        _TypePngPool::CPair* pPairRet = _Instance()->m_mapPng.Lookup(szRes);
        Gdiplus::Image* pImg = NULL;

        if (NULL == pPairRet)
        {
            _LoadGIFImageFromResourceID(szRes, pImg);
            if (pImg)
                _Instance()->m_mapPng[szRes] = pImg;
        }
        else
            pImg = pPairRet->m_value;

        return pImg;
    }

    static size_t GetCount()
    {
        return _Instance()->m_mapPng.GetCount();
    }

protected:

    ULONG_PTR m_gdiplusToken;

    _TypePngPool m_mapPng;

    static BkPngPool* _Instance()
    {
		static BkPngPool pool;
        return &pool;
    }

    static BOOL _LoadPNGImageFromResourceID(LPCTSTR szRes, Gdiplus::Image* &pImg)
    {
        CStringA strResource;

        BOOL bRet = BkResManager::LoadResource(szRes, strResource);
        int len = strResource.GetLength();

        HGLOBAL hMem = ::GlobalAlloc(GMEM_FIXED, len);
        BYTE* pMem = (BYTE*)::GlobalLock(hMem);
		if (pMem)
		{
			memcpy(pMem, (LPCSTR)strResource, len);

			IStream* pStm = NULL;
			::CreateStreamOnHGlobal(hMem, FALSE, &pStm);

			pImg = Gdiplus::Image::FromStream(pStm);

			::GlobalUnlock(hMem);
			pStm->Release();
		}
		GlobalFree(hMem);

        return TRUE;
    }

    static BOOL _LoadGIFImageFromResourceID(LPCTSTR szRes, Gdiplus::Image* &pImg)
    {
        return BkResManager::LoadResourceImage(szRes, pImg);
    }
};