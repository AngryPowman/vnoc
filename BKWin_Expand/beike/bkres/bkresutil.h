//////////////////////////////////////////////////////////////////////////
//   File Name: bkresutil.h
// Description: Beike Resource Helper
//     Creator: Zhang Xiaoxuan
//     Version: 2009.5.13 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <GdiPlus.h>

#define BKRES_TYPE _T("xml")
#define BKRES_PNG_TYPE _T("png")

#ifdef _DEBUG
#   define BKRES_ASSERT(expr, format, ...) \
    (void) ((!!(expr)) || \
    (1 != _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, format, __VA_ARGS__)) || \
    (_CrtDbgBreak(), 0))
#else
#   define BKRES_ASSERT
#endif

class BkResManager
{
public:
    BkResManager()
        : m_hInstanceRes(NULL)
    {

    }

    static void SetResourcePath(LPCTSTR lpszPath)
    {
        _Instance()->m_strResourcePath = lpszPath;
    }

    static void SetResourceDLL(LPCTSTR lpszPath)
    {
        if (_Instance()->m_hInstanceRes)
            ::FreeLibrary(_Instance()->m_hInstanceRes);

        _Instance()->m_hInstanceRes = ::LoadLibrary(lpszPath);
    }

    static BOOL LoadResource(LPCTSTR szRes, CStringA &strBuffRet)
    {
        BOOL bRet = FALSE;
        if (!_Instance()->m_strResourcePath.IsEmpty()) 
		{
            CString strFileName;
            strFileName.Format(L"%s\\%s", _Instance()->m_strResourcePath, szRes);
            HANDLE hFile = ::CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (INVALID_HANDLE_VALUE != hFile) 
			{
                DWORD dwSize = ::GetFileSize(hFile, NULL);
                if (0 != dwSize) 
				{
                    DWORD dwRead = 0;
                    bRet = ::ReadFile(hFile, strBuffRet.GetBuffer(dwSize + 10), dwSize, &dwRead, NULL);
                    if (bRet && dwRead == dwSize)
                        strBuffRet.ReleaseBuffer(dwSize);
					else
						strBuffRet.ReleaseBuffer(0);
                }
                ::CloseHandle(hFile);
            }
        }
        return bRet;
    }

    static BOOL LoadResource(UINT uResID, CStringA &strBuffRet, LPCTSTR lpszResType = BKRES_TYPE)
    {
        BOOL bRet = FALSE;

        if (!_Instance()->m_strResourcePath.IsEmpty())
        {
            CString strFileName;

            strFileName.Format(L"%s\\%d.%s", _Instance()->m_strResourcePath, uResID, lpszResType);
            HANDLE hFile = ::CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (INVALID_HANDLE_VALUE != hFile)
            {
                DWORD dwSize = ::GetFileSize(hFile, NULL);
                if (0 != dwSize)
                {
                    DWORD dwRead = 0;
                    bRet = ::ReadFile(hFile, strBuffRet.GetBuffer(dwSize + 10), dwSize, &dwRead, NULL);
                    if (bRet && dwRead == dwSize)
                        strBuffRet.ReleaseBuffer(dwSize);
					else
						strBuffRet.ReleaseBuffer(0);
                }
                ::CloseHandle(hFile);
            }
        }

        bRet = _LoadEmbedResource(uResID, strBuffRet, lpszResType);

        BKRES_ASSERT(bRet, L"Failed loading %s %u", lpszResType, uResID);

        return bRet;
    }

    static BOOL LoadResourceBmp(LPCTSTR szRes, HBITMAP &hBitmap)
    {
        if (!_Instance()->m_strResourcePath.IsEmpty())
        {
            CString strFileName;
            strFileName.Format(L"%s\\%s", _Instance()->m_strResourcePath, szRes);
            hBitmap = (HBITMAP)::LoadImage(NULL, strFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            if (NULL != hBitmap)
                return TRUE;
        }
        BKRES_ASSERT(NULL != hBitmap, L"Failed loading bitmap %s", szRes);

        return NULL != hBitmap;
    }

    static BOOL LoadResourceIcon(LPCTSTR szRes, HICON& hIcon)
    {
        if (!_Instance()->m_strResourcePath.IsEmpty())
        {
            CString strFileName;
            strFileName.Format(L"%s\\%s", _Instance()->m_strResourcePath, szRes);
            hIcon = ExtractIcon(NULL, strFileName, 0);
            if (NULL != hIcon)
                return TRUE;
        }
        BKRES_ASSERT(NULL != hIcon, L"Failed loading icon %s", szRes);

        return NULL != hIcon;
    }

    static BOOL LoadResourceImage(LPCTSTR szRes, Gdiplus::Image* &image)
    {
        Gdiplus::Image* pImage = NULL;
        if (!_Instance()->m_strResourcePath.IsEmpty())
        {
            CString strFileName;
            strFileName.Format(L"%s\\%s", _Instance()->m_strResourcePath, szRes);
            pImage = Gdiplus::Image::FromFile( strFileName );
            if (NULL != pImage)
            {
                image = pImage;
                return TRUE;
            }
        }
        BKRES_ASSERT(NULL != pImage, L"Failed loading bitmap %s", szRes);

        return NULL != pImage;
    }

protected:

    static BkResManager* _Instance()
    {
		static BkResManager instance;
        return &instance;
    }

    static BOOL _LoadEmbedResource(UINT uResID, CStringA &strRet, LPCTSTR lpszResType = BKRES_TYPE)
    {
        HRSRC hRsrc = ::FindResource((HINSTANCE)&__ImageBase, MAKEINTRESOURCE(uResID), lpszResType);

        if (NULL == hRsrc)
            return FALSE;

        DWORD dwSize = ::SizeofResource((HINSTANCE)&__ImageBase, hRsrc); 
        if (0 == dwSize)
            return FALSE;

        HGLOBAL hGlobal = ::LoadResource((HINSTANCE)&__ImageBase, hRsrc); 
        if (NULL == hGlobal)
            return FALSE;

        LPVOID pBuffer = ::LockResource(hGlobal); 
        if (NULL == pBuffer)
            return FALSE;

        memcpy(strRet.GetBuffer(dwSize + 1), pBuffer, dwSize);
        strRet.ReleaseBuffer(dwSize);

        ::FreeResource(hGlobal);

        return TRUE;
    }

    CString m_strResourcePath;
    HINSTANCE m_hInstanceRes;
};
