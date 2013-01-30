

#include "MysqlWrapper.h"



int MWConnection::Connect( string host,string name,string password,string db,int port /*= 3306*/ )
{
	if (host == "" || db == "")
	{
		return MW_SQL_ERR;
	}
	
	if(mysql_real_connect(&m_Mysql,host.c_str(),name.c_str(),password.c_str(),db.c_str(),port,0,0))
	{
		return MW_SQL_OK;
	}
	return MW_SQL_ERR;
}

void MWConnection::Shutdown()
{
	mysql_close(&m_Mysql);
}

//MWCommand

int MWCommand::_Command( MWConnection* conn,string runsql )
{
	if (conn == NULL)
	{
		return MW_SQL_ERR;
	}
	
	if (runsql == "")
	{
		return MW_SQL_ERR;
	}

	m_SQL = &conn->m_Mysql;
	m_runsql = runsql; 
	return MW_SQL_OK;
}

int MWCommand::Execute()
{
	return mysql_query(m_SQL,m_runsql.c_str());
}

MYSQL_ROW MWCommand::FetchNext()
{
	if (m_SQL == NULL)
	{
		return 0;
	}

	if (m_Result == NULL)
	{
	  m_Result = mysql_store_result(m_SQL);
	}

	m_Field_Num = mysql_num_fields(m_Result);
	
	m_Line_Num  = (int)mysql_num_rows(m_Result);

	return (m_Row = mysql_fetch_row(m_Result));
}


MYSQL_ROW  MWCommand::First()
{
	if (m_SQL == NULL)
	{
		return 0;
	}

	if (m_Result == NULL)
	{
	  m_Result = mysql_store_result(m_SQL);
	}

	m_Field_Num = mysql_num_fields(m_Result);
	
	m_Line_Num  = (int)mysql_num_rows(m_Result);

	return (m_Row = mysql_fetch_row(m_Result));
}

MWType* MWCommand::GetData( int index )
{
	if (m_Row == NULL)
	{
		return NULL;
	}	
	
	if (m_MWTpye != NULL)
	{
		delete m_MWTpye;
		m_MWTpye = NULL;
	}

	m_MWTpye = new MWType;

	m_MWTpye->m_Data = m_Row[index];

	return m_MWTpye;
}

MWType* MWCommand::GetOneData()
{

	if (m_MWTpye != NULL)
	{
		delete m_MWTpye;
		m_MWTpye = NULL;
	}
    if (GetField_Num() == 0)
    {
		m_MWTpye = new MWType;

		m_MWTpye->m_Data = m_Row[GetField_Num()];
    }
	else
	{
		m_MWTpye = new MWType;

		m_MWTpye->m_Data = m_Row[GetField_Num() - 1];
	}
	return m_MWTpye;
}

void MWCommand::_Close()
{
	if (m_Row != NULL)
	{
		mysql_free_result(m_Result);
		m_Row = NULL;
	}

	if (m_MWTpye != NULL)
	{
		delete m_MWTpye;
		m_MWTpye = NULL;
	}
}


//MWType

const char* MWType::asString() const
{
	return m_Data.c_str();
}

int MWType::asInt() const
{
	return atoi(m_Data.data());
}

short MWType::asShort() const
{
	return atoi(m_Data.data());
}

long MWType::asLong() const
{
	return atol(m_Data.data());
}


double MWType::asDoubule() const
{
	double Ret = 0;
	sscanf(m_Data.c_str(),"%lf",&Ret);
	return Ret;
}

