
#ifndef VNOC_MSG_CMESSAGE
#define VNOC_MSG_CMESSAGE

#include "MessageDef.h"
/*
class MakeParam
{
public:	

	int MateParam( std::string in_str )
	{
		
		return -1;
	}

	EnumFunList& returnParamMakeObject(){ return m_ParamComList;}

private:

	EnumFunList m_ParamComList;
};
*/

uint byteToInt(const byte* in_byte,size_t len);

void IntTobyte(int in_int,byte* out_byte);


class CMessage
{
public:

	friend class CMessageParser;
	friend class PackMessage;

	CMessage(){
		_Initialization();
	}

	virtual ~CMessage(){
		_Close();
	}
	//��ȡ��Ϣ����
	int  GetMessageType() const;


public:

	bool GetBeginTab() const ;

	bool GetEndTab() const;

	byte GetCommand() const;

	byte GetComListLen(uint in_index) const;

	uint GetSerial() const;

	const byte* GetGUID() const;

	const byte* GetComCommandList(uint in_index) const;


	uint GetVerify() const;

	uint GetObligate() const;

	uint GetVersion() const;

	uint GetCmlCount() const;

	uint GetDataLen() const;


	// Set

	bool SetCommand(byte in_byte);

	bool SetSerial(ushort in_short);

	bool SetGUID(byte* in_byte_ptr);

	bool SetComCommandList(int ComCount);

	bool SetComListLen(byte* in_byte_ptr,int CmlCount);

	bool SetVerify(uint in_Int);

	bool SetObligate(uint in_Int);

	bool SetVersion(uint in_Int);

	bool SetCmlCount(uint in_Int);

	bool SetDataLen(uint in_Int);

protected:

	//ģ�庯�����⴦��

	template<typename Type>
	void   SetParam( uint Pos,const std::vector<Type> PeopleList)
	{
		if (Pos == -1)
		{
			return;
		}
		if (PeopleList.empty())
		{
			return;
		}
		byte tmpByte[4] = {0};
        uint PeopleLen = PeopleList.size();
		byte* tmpParam = new byte[PeopleLen * sizeof(Type)];
		memset(tmpParam,0,PeopleLen * sizeof(Type));
		int  tmpInt = 0;
		for (int index = 0,ParamPos = 0; index <  (int)PeopleLen;index++)
		{
			IntTobyte(PeopleList[index],tmpByte);
			for (int i = 0; i < sizeof(Type); (i++,ParamPos++))
			{
				tmpParam[ParamPos] = tmpByte[i];
			}
			memset(tmpByte,0,4);
		}
		SetParam(Pos,tmpParam,PeopleLen * sizeof(Type));
		if (tmpParam != NULL)
		{
			delete[] tmpParam;
			tmpParam = NULL;
		}
	}


	template<typename Type>
	Type  GetParam(uint int_index,std::vector<Type>& PeopleList) const
	{
		if (int_index != -1)
		{
			std::vector<std::vector<Type> > ComTmpList;
			if (m_ComCommandList[int_index].second.size() == 0)
			{
				return 1;
			}
			if ((m_ComCommandList[int_index].second.size() % sizeof(Type) ) != 0)
			{
				return 1;
			}
			ComTmpList.resize(((int)m_ComCommandList[int_index].second.size() / sizeof(Type)));
			for (int i = 0,index = 0;index < (int)(m_ComCommandList[int_index].second.size() / sizeof(Type)) ;index++)
			{
				for (int j = 0; j < sizeof(Type); (i++,j++))
				{
					ComTmpList[index].push_back(m_ComCommandList[int_index].second[i]);
				}
			}

			for (int i = 0; i < (int)ComTmpList.size(); i++)
			{
				byte* tempBytes = new byte[sizeof(Type)];
				memset(tempBytes,0,sizeof(Type));
				for (int j=0; j<sizeof(Type); ++j)
				{
					tempBytes[sizeof(Type)-j-1] = ComTmpList[i][j];
				}
				PeopleList.push_back(byteToInt(tempBytes,sizeof(Type)));
				delete[] tempBytes;
			}
		}
		return 0;
	}

protected:

	void   SetParam( int Pos, byte in_Param_byte);

	void   SetParam( int Pos,const byte* in_Param_byte_Ptr,uint len);

	void   SetParam( int Pos, uint  in_Param_int);

	//��������ʼ�� 
	virtual void   InitiaPack(int in_ComType, int in_ComNum);

	virtual uint   GetParamLen(uint int_index) const;

	virtual const  byte*  GetParam( int int_index ) const;

	virtual void   SetParamLen(uint int_index, int int_Value);

	//Ԥ��
	//virtual int PopParam();

protected:

	int _MessageType(byte in_Command) const;

	int MateParam( std::string in_str ) const;

	EnumFunList m_ParamMate;

	bool  m_Begin;  //�����Ϣ�Ŀ�ʼ  ͳһ�ַ�'V"��ASC���� 0x56
	bool  m_End;    //�����Ϣ�Ľ���  �̶��ַ�'C����ASC���� 0x43
	//�����б�    4�ֽڣ���Ӧ����N�ĳ���
	std::vector<byte>			 m_ComListLen;  
	//�����б�    �����Ĳ������������͸��ݾ���ָ�����
	std::vector<std::pair<int,std::vector<byte> > >  m_ComCommandList;
	byte   m_Command;			  //ָ��			�����ָ�������ע�����ݰ��Ĺ���
	byte   m_Serial[2];			  //���       ָ��ı��
	byte   m_GUID[16];            //GUID       �����ṩ�û���֤
	//DWORD dwType;				  //��Ϣ����		


	uint  m_Verify;               //Ч���� Ԥ��

	uint  m_Obligate;              //Ԥ���ռ�
	uint  m_Ver;				   //�汾��     ��־��VNOCЭ��İ汾
	uint  m_CmlCount;			   //��������    �����Ĳ������������͸��ݾ���ָ�����
	uint  m_Len;                   //���峤��	   4���ֽڣ�����ָ������ĳ��ȣ����Ը������ݽ�����

	//������
	int m_Error;

private:

	void   _Initialization();
	void   _Close();
};


#endif