// �ڴ˴��ṩ�ӿڵ�Ĭ��ʵ�֡��ӿھ������ɴ��鲻Ҫ��Ĭ��ʵ�֣�����������ʵ�֣���ʹʲô�����ɡ�
// һЩ�����õ��ຯ��Ҳ���Խ�ʵ��д������
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
				{	//��ֹ����д�˶��'/'����
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
