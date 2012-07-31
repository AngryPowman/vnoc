#include "SQLUserStorage.h"

sUserStorage ::sUserStorage ()
{
	if (conn.Connect("xxy1991.dlinkddns.com", "vnoc", "ldldld", "vnoc", 3306) == MW_SQL_ERR)
	{
		bIsConn = false;
		return;
	}
	bIsConn = true;
}

sUserStorage ::~sUserStorage ()
{
	conn.Shutdown();
}

bool sUserStorage::IfUserExist(const char* pUser)
{
	if (!bIsConn)
	{
		return false;
	}

	char SQL[100] = "select * from vnoc.user_info where user='";
	strcat(SQL, pUser);
	strcat(SQL, "'");
	MWCommand com(&conn, SQL);
	
	com.Execute();
	com.FetchNext();
	if (com.GetLine_Num() == 0)
	{
		return false;
	}
	return true;
}
bool sUserStorage::GetPassword(const char* pUser, char* pPassWordBuff, long cbBuff)
{

	if (!bIsConn)
	{
		return false;
	}

	char SQL[100] = "select passwd from vnoc.user_info where user='";
	strcat(SQL, pUser);
	strcat(SQL, "'");
	MWCommand com(&conn, SQL);
	com.Execute();
	com.FetchNext();
	if (com.GetField_Num() == 0)
	{
		return false;
	}

	const char *p = (com.GetOneData()->asString());
	if (strlen(p) > cbBuff)
	{
		return false;
	}
	strcpy(pPassWordBuff, p);
	return true;
}
bool sUserStorage::GetUserInfo(const char* pUser, userinfo* pUserInfo)
{

	if (!bIsConn)
	{
		return false;
	}
	char SQL[100] = "select passwd from vnoc.user_info where user='";
	strcat(SQL, pUser);
	strcat(SQL, "'");
	MWCommand com(&conn, SQL);
	com.Execute();
	com.FetchNext();
	if (com.GetField_Num() == 0)
	{
		return false;
	}
	
	strcpy(pUserInfo->strUser, pUser);
	strcpy(pUserInfo->passwordData, com.GetOneData()->asString());

	char SQL1[100] = "select nickname from vnoc.user_info where user='";
	strcat(SQL1, pUser);
	strcat(SQL1, "'");
	MWCommand com1(&conn, SQL1);
	com1.Execute();
	com1.FetchNext();
	strcpy(pUserInfo->strNickname, com1.GetOneData()->asString());

	char SQL2[100] = "select email from vnoc.user_info where user='";
	strcat(SQL2, pUser);
	strcat(SQL2, "'");
	MWCommand com2(&conn, SQL2);
	com2.Execute();
	com2.FetchNext();
	strcpy(pUserInfo->strEmailAddr, com2.GetOneData()->asString());

	char SQL3[100] = "select gender from vnoc.user_info where user='";
	strcat(SQL3, pUser);
	strcat(SQL3, "'");
	MWCommand com3(&conn, SQL3);
	com3.Execute();
	com3.FetchNext();
	pUserInfo->gender = com3.GetOneData()->asInt();

	char SQL4[100] = "select old from vnoc.user_info where user='";
	strcat(SQL4, pUser);
	strcat(SQL4, "'");
	MWCommand com4(&conn, SQL4);
	com4.Execute();
	com4.FetchNext();
	pUserInfo->uOld = com4.GetOneData()->asInt();
	return true;
}