/*
�û�����ģ��
����1 �û���֤
����2 �û�ע��
����3 �һ�����
����4 �ϴ��ļ�
����5 �û�����
*/
#include <string>
#define NULLPOINT			-1
#define LOGIN_OK			1
#define TEST_FALSE			2

using namespace std;

struct userinfo
{
	//�����кܶ��û���Ϣ
};
class CUserManage
{
private:
	
public:
	//����ֵ�� LOGIN_OK ��½�ɹ� TEST_FALSE ��֤ʧ�� NULLPOINT ָ����Ч
	//�����½�ɹ������û���Ϣ
	int Authenticate(string sUser, char* pPassword, userinfo* pUserInfo, int nPassLen = 20)
{
	char strPass[20] = "0000000000000000000";
	
	if (false)//�˺��Ƿ���� �����ݿ�
	{
		return TEST_FALSE;
	}
	
	int i = 0;
	do
	{
		if (strPass[i] != pPassword[i])
		{
			return TEST_FALSE;
		}
		++i;
	} while (i < nPassLen);

	if ((int)pUserInfo == 0)
	{
		return NULLPOINT;
	}

	//����û���Ϣ
	//userinfo->xx == xx;
	return LOGIN_OK;
}

};

