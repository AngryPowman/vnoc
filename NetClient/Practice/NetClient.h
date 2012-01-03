/*
////////////////////////////////////////////////////////////////////////
文件名：NetClient.h
创建日期：2011年10月07日
最后更新：2011年10月08日
编写者：Sum
功能描述：
    客户端通讯
 
版权所有：VNOC团队
///////////////////////////////////////////////////////////////////////
*/
#ifndef MET_CLIENT
#define MET_CLIENT

#include <WinSock2.h>

#pragma comment(lib,"WS2_32")


#define VNOC_SOC_S   0;
#define VNOC_SOC_F  -1;

#define VNOC_TCP	 SOCK_STREAM
#define VNOC_UDP     SOCK_DGRAM

class NetClient
{
public:
	//WinSock版本号
	//mainVer  主版本号
	//minorVer 副版本号
	NetClient(BYTE mainVer = 2,BYTE minorVer = 2);
	virtual ~NetClient();
	
	//创建socket 必须先调用 CreateSocket函数初始化
	// Type: 
	//	VNOC_TCP	 SOCK_STREAM      TCP协议
	//	VNOC_UDP     SOCK_DGRAM       UDP协议

	//  _family , Protocol 保持默认即可

	bool CreateSocket(int Type,short _family = AF_INET,int Protocol = 0);

	//关闭socket
	bool CloseSocket();
	
	//建立与服务器的连接
	// Addr: 服务器地址
	// Port: 连接端口
	bool Build(LPCTSTR Addr,u_short Port);
	
	// 发送内容
	// lpszBuffer: 缓冲区地址
	// flag:       保持0即可
	bool Send(LPTSTR lpszBuffer,int flag = 0);
	
	// 接受内容
	// lpszBuffer: 缓冲区地址
	// nlen:       缓冲区长度
	// flag:       保持0即可
	bool Result(LPTSTR lpszBuffer,int nlen,int flag = 0);


/*	int  Message();*/


protected:

	sockaddr_in m_sockAddr;

private:
	
	WSADATA m_wsaData;
	SOCKET m_hSocket;
};

#endif