#pragma once

struct MSG_GENERIC
{
	DWORD dwType;  //消息类型
	int   nLen;    //消息长度，需分包时用
};