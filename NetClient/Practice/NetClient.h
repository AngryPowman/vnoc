/*
////////////////////////////////////////////////////////////////////////
�ļ�����NetClient.h
�������ڣ�2011��10��07��
�����£�2011��10��08��
��д�ߣ�Sum
����������
    �ͻ���ͨѶ
 
��Ȩ���У�VNOC�Ŷ�
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
	//WinSock�汾��
	//mainVer  ���汾��
	//minorVer ���汾��
	NetClient(BYTE mainVer = 2,BYTE minorVer = 2);
	virtual ~NetClient();
	
	//����socket �����ȵ��� CreateSocket������ʼ��
	// Type: 
	//	VNOC_TCP	 SOCK_STREAM      TCPЭ��
	//	VNOC_UDP     SOCK_DGRAM       UDPЭ��

	//  _family , Protocol ����Ĭ�ϼ���

	bool CreateSocket(int Type,short _family = AF_INET,int Protocol = 0);

	//�ر�socket
	bool CloseSocket();
	
	//�����������������
	// Addr: ��������ַ
	// Port: ���Ӷ˿�
	bool Build(LPCTSTR Addr,u_short Port);
	
	// ��������
	// lpszBuffer: ��������ַ
	// flag:       ����0����
	bool Send(LPTSTR lpszBuffer,int flag = 0);
	
	// ��������
	// lpszBuffer: ��������ַ
	// nlen:       ����������
	// flag:       ����0����
	bool Result(LPTSTR lpszBuffer,int nlen,int flag = 0);


/*	int  Message();*/


protected:

	sockaddr_in m_sockAddr;

private:
	
	WSADATA m_wsaData;
	SOCKET m_hSocket;
};

#endif