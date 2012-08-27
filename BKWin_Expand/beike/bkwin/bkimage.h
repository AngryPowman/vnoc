//////////////////////////////////////////////////////////////////////////
//  Class Name: CBkImage
// Description: Image Helper for KAV32 (Ver.2008)
//     Creator: Zhang Xiaoxuan
//     Version: 2007.10.19 - 1.0 - Create
//              2007.10.20 - 1.1 - Finish the draw method of 32bit bitmap
//                                 with alpha channel
//              2007.10.23 - 1.2 - support no MFC and no WTL
//              2007.11.01 - 1.3 - add some method for use:
//                                 CreateBitmap
//                                 Draw(HBITMAP, int, int, int)
//
//      Notice: 1. 32bit bitmap should be loaded by LoadDIBSection to
//                 get faster
//////////////////////////////////////////////////////////////////////////

#pragma once

class CBkImage : public CBitmapHandle
{
public:
    BOOL CreateBitmap(int nWidth, int nHeight, COLORREF crBackground = CLR_INVALID)
    {
        HDC hDCDesktop = ::GetDC(NULL);
        HBITMAP hBmpCreate = ::CreateCompatibleBitmap(hDCDesktop, nWidth, nHeight);

        Attach(hBmpCreate);

        if (crBackground != CLR_INVALID)
        {
            HDC hDCDraw = ::CreateCompatibleDC(hDCDesktop);
            HGDIOBJ hbmpOld = ::SelectObject(hDCDraw, hBmpCreate);
            RECT rcDraw = {0, 0, nWidth, nHeight};
            COLORREF clrOld = ::SetBkColor(hDCDraw, crBackground);

            ATLASSERT(clrOld != CLR_INVALID);

            if(clrOld != CLR_INVALID)
            {
                ::ExtTextOut(hDCDraw, 0, 0, ETO_OPAQUE, &rcDraw, NULL, 0, NULL);
                ::SetBkColor(hDCDraw, clrOld);
            }

            ::SelectObject(hDCDraw, hbmpOld);
            ::DeleteDC(hDCDraw);
        }

        ::ReleaseDC(NULL, hDCDesktop);

        return hBmpCreate != NULL;
    }
};
