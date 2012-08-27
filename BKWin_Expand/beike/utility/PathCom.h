// $_FILEHEADER_BEGIN ****************************
// 文件名称：PathCom.h
// 创建日期：2010.10.11
// 创建人：董明
// 文件说明：一些获取路径的封装
//
// namespace PATH_COM 提供下列方法:
// GetModulePath()          获取当前模块的全路径(文件夹路径+文件名)
// GetModuleName()          获取当前模块的文件名
// GetModuleFolder()        获取当前模块的文件夹路径
// GetPersonalFolder()      获取当前用户的个人文件夹
// 
// $_FILEHEADER_END ******************************


#pragma once

#ifndef _PATH_COM_
#define _PATH_COM_

#include <ShTypes.h>
#include <ShlObj.h>
#include <shellapi.h>
#include <atlstr.h>

namespace PATH_COM
{
    inline CString GetModulePath()
    {
        TCHAR buffer[MAX_PATH] = {0};
        DWORD dw = ::GetModuleFileName(NULL, buffer, MAX_PATH);
        return buffer;
    };

    inline CString GetModuleName()
    {
        CString strExePath = GetModulePath();
        int iPos = strExePath.ReverseFind('\\');
        return strExePath.Right(strExePath.GetLength() -  iPos - 1);
    };

    inline CString GetModuleFolder()
    {
        CString strExePath = GetModulePath();
        int iPos = strExePath.ReverseFind('\\');
        return strExePath.Left(iPos + 1);
    };

    inline CString GetParentFolder(LPCTSTR szPath)
    {
        CString strParent;
        CString strPath = szPath;
        PathRemoveBackslash(strPath.GetBuffer(MAX_PATH));
        strPath.ReleaseBuffer();
        int iPos = strPath.ReverseFind('\\');
        if (iPos >= 0)
        {
            strParent = strPath.Left(iPos + 1);
        }
        return strParent;
    }

    inline CString GetPersonalFolder()
    {
        LPMALLOC pShellMalloc;
        TCHAR szDir[MAX_PATH] = {0};
        if (SUCCEEDED(SHGetMalloc(&pShellMalloc)))
        {
            LPITEMIDLIST pidl;
            if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidl)))
            {
                SHGetPathFromIDList(pidl, szDir);
                pShellMalloc->Free(pidl);

                size_t nLen = _tcslen(szDir);
                szDir[nLen] = '\\';
            }
            pShellMalloc->Release();
        }
        return szDir;
    };

    inline BOOL DelFile(LPCTSTR szFile, BOOL bRecycle)
    {
        CString strFrom = szFile;
        strFrom.AppendChar(_T('\0'));

        SHFILEOPSTRUCT tagFO = {0};
        tagFO.pFrom = strFrom;
        tagFO.pTo = NULL;
        tagFO.wFunc = FO_DELETE;
        tagFO.hwnd = GetDesktopWindow();
        tagFO.fFlags = FOF_NOERRORUI | FOF_SILENT | FOF_NOCONFIRMATION;
        if (bRecycle)
        {
            tagFO.fFlags |= FOF_ALLOWUNDO;
        }
        int iRet = ::SHFileOperation(&tagFO);
        return (iRet == 0);
    }

    inline void RemoveEmptyDir(LPCTSTR szPath, BOOL bRecycle)
    {
        CString strDirPath = szPath;
        PathRemoveBackslash(strDirPath.GetBuffer(MAX_PATH));
        strDirPath.ReleaseBuffer();
        if (!PathIsDirectory(strDirPath))
        {
            return;
        }

        CString szFindPath = strDirPath + _T("\\*.*");
        WIN32_FIND_DATA FileData; 
        HANDLE hSearch = NULL; 
        BOOL bWorking = TRUE; 
        hSearch = FindFirstFile(szFindPath, &FileData); 
        if (hSearch == INVALID_HANDLE_VALUE)
            return;

        while (bWorking) 
        {
            if (_tcscmp(FileData.cFileName, _T(".")) != 0 
                && _tcscmp(FileData.cFileName, _T("..")) != 0) 
            {
                CString szNewPath = strDirPath + _T("\\") + FileData.cFileName;
                if ((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {	
                    SetFileAttributes(szNewPath, FILE_ATTRIBUTE_NORMAL);
                    RemoveEmptyDir(szNewPath.GetBuffer(0),bRecycle);		
                }
            }
            bWorking = FindNextFile(hSearch, &FileData);
        }
        FindClose(hSearch);

        if (PathIsDirectoryEmpty(szPath))
        {
            if (bRecycle)
            {
                DelFile(szPath, bRecycle);
            }
            else
            {
                if (!RemoveDirectory(szPath))
                {
                    MoveFileEx(szPath, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
                }
            }
        }
    }
}


#endif   // _PATH_COM_

