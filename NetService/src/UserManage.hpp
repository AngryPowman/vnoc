/*
用户管理模块
操作1 用户验证
操作2 用户注册
操作3 找回密码
操作4 上传文件
操作5 用户下线
*/
#include <string>
#define NULLPOINT			-1
#define LOGIN_OK			1
#define TEST_FALSE			2

using namespace std;

struct userinfo
{
	//其中有很多用户信息
};
class CUserManage
{
private:
	
public:
	//返回值： LOGIN_OK 登陆成功 TEST_FALSE 验证失败 NULLPOINT 指针无效
	//如果登陆成功则获得用户信息
	int Authenticate(string sUser, char* pPassword, userinfo* pUserInfo, int nPassLen = 20)
{
	char strPass[20] = "0000000000000000000";
	
	if (false)//账号是否存在 查数据库
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

	//获得用户信息
	//userinfo->xx == xx;
	return LOGIN_OK;
}

};

