
#include "NetClient.h"
#include <iostream>

using namespace std;

NetClient Test;

int main()
{
	// �����׽���
	SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sListen == INVALID_SOCKET)
	{
		printf("Failed socket() \n");
		return 0;
	}

	// ���sockaddr_in�ṹ
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(5555);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	// ������׽��ֵ�һ�����ص�ַ
	if(::bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind() \n");
		return 0;
	}

	// �������ģʽ
	if(::listen(sListen, 2) == SOCKET_ERROR)
	{
		printf("Failed listen() \n");
		return 0;
	}

	// ѭ�����ܿͻ�����������
	sockaddr_in remoteAddr; 
	int nAddrLen = sizeof(remoteAddr);
	SOCKET sClient;
	char szText[] = "<Login Name = \"xx\" Password = \"mdmxmdmdmwqweqe\" />";
	char szRecv[256] = {0};
	while(TRUE)
	{
		// ����һ��������
		sClient = ::accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
		if(sClient == INVALID_SOCKET)
		{
			printf("Failed accept()");
			continue;
		}

		printf(" ���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		// ��ͻ��˷�������
		::send(sClient, szText, strlen(szText), 0);

		::recv(sClient,szRecv,256,0);
		printf(" ���ܵ�һ����Ϣ��%s \n",szRecv);
		// �ر�ͬ�ͻ��˵�����
		::closesocket(sClient);
	}

	// �رռ����׽���
	::closesocket(sListen);

	return 0;
}