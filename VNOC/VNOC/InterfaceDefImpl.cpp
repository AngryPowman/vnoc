// 在此处提供接口的默认实现。接口尽量做成纯虚不要有默认实现，而是由子类实现，即使什么都不干。
// 一些辅助用的类函数也可以将实现写在这里
#include "stdafx.h"

void ConfigNode::xPath(LPCTSTR cfgPath)
{
	CString tempStr = cfgPath;
	LPTSTR thePath = tempStr.GetBuffer(tempStr.GetLength());
	ASSERT(thePath);
	if (thePath)
	{
		LPCTSTR currPos = thePath;
		LPCTSTR startPos = thePath;
		do 
		{
			if (*thePath == ConfigPathSplit)
			{
				*thePath = _T('\0');
				path.push_back(startPos);
				++thePath;
				while(*thePath == ConfigPathSplit)
				{	//防止连着写了多个'/'符号
					++thePath;
				}
				startPos = thePath;
			}
			else if(*thePath == _T('\0'))
			{
				path.push_back(startPos);
				startPos = thePath;
			}
			else
			{
				++thePath;
			}
		} while (*startPos);
	}
	tempStr.ReleaseBuffer(0);;
}
