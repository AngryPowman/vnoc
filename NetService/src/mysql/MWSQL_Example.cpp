
#include <iostream>
#include "MysqlWrapper.h"


#pragma  comment(lib,"sql/lib/libmysql.lib")


using namespace std;


int main()
{

	MWConnection conn;
	MWCommand    Table(&conn,"select * from writers");
	MWCommand    Table1(&conn,"INSERT INTO writers VALUES('Test')");
	MWCommand    Password(&conn,"select password from writers");
	MWCommand    TestDouble(&conn,"select password from writers");
	//�������ݿ�
	if(conn.Connect("��ַ","�˺�","����","���ݿ���") != MW_SQL_ERR)
	{
		cout<<"OK!"<<endl;
		//ִ��
		Table.Execute();
		//ȡ��һ��
		while(Table.FetchNext())
		{
			for (int index = 0; index < Table.GetField_Num();index++)
			{
				//ȡ������
				cout<< Table.GetData(index)->asC_String()<<endl;
			}
		}
		cout<< "--------------------------------"<<endl;
		Password.Execute();
		while(Password.FetchNext())
		{
			for (int index = 0; index < Password.GetField_Num();index++)
			{
				cout<< Password.GetData(index)->asInt()<<endl;
			}
		}

		cout<< "--------------------------------"<<endl;

		TestDouble.Execute();
		while(TestDouble.FetchNext())
		{
			for (int index = 0; index < TestDouble.GetField_Num();index++)
			{
				cout<< TestDouble.GetData(index)->asDoubule()<<endl;
			}
		}
	}
	return 0;
}	



