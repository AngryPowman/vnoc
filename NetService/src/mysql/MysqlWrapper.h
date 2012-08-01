

#pragma once


#include <string>
#include "../../include/Mysql/mysql.h"



using std::string;


#define   MW_SQL_OK   0
#define   MW_SQL_ERR  -1

class MWType;

class MWConnection
{
public:
	MWConnection(){
		mysql_init(&m_Mysql);
	}
	~MWConnection(){
		Shutdown();
	};
	
	friend class MWCommand;

public:
	
	//host为NULL 或者 localhost 时 为默认连接本地数据库
	int  Connect(string host,string name,string password,string db,int port = 3306);
	
	void Shutdown();

private:

	MYSQL m_Mysql;

};


class MWCommand
{
public:
	MWCommand():m_SQL(NULL),m_Result(NULL),m_Field_Num(0),m_MWTpye(NULL),m_Row(NULL){}
	MWCommand(MWConnection* conn,string runsql):m_SQL(NULL),m_Result(NULL),m_Field_Num(0),m_MWTpye(NULL),m_Row(NULL){
		_Command(conn,runsql);
	}
	~MWCommand(){
		_Close();
	}
public:
	
	MYSQL_ROW  FetchNext();

	MYSQL_ROW  First();

	MWType* GetData(int index);
	
	//一个结果的结果集
	MWType* GetOneData();

	int GetField_Num(){
		return m_Field_Num;
	}

	int GetLine_Num(){
		return m_Line_Num;
	}

	int Execute();

private:
	
	void  _Close();

	int  _Command(MWConnection* conn,string runsql);

private:
	
	MYSQL_RES *m_Result;
	MYSQL_ROW m_Row;
	
	MWType*  m_MWTpye;

	int    m_Line_Num;
	int    m_Field_Num;
	MYSQL* m_SQL;
	string m_runsql;
};


class MWType
{
public:
	MWType(){}
	~MWType(){}
	
	friend class MWCommand;

	const char* asString() const;
	
	long   asLong() const;

	int    asInt()  const;

	double asDoubule() const;

	short  asShort() const;

private:
	string  m_Data;
};