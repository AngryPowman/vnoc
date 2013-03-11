#pragma once
// ���巿��
#include "../../FrameWork/FrameBase.hpp"
#include <atlstr.h>
#include <atltime.h>

typedef CString UserID;

class CRoomBase : public CFrameBase
{
public:
	CRoomBase();
	CRoomBase(CString ID);
	~CRoomBase();
public:
	virtual void		SetID(CString id);
	virtual CString		GetID();
	virtual void		SetName(CString name);
	virtual CString		GetName();
	virtual void		SetGuestLimitCount(int count);
	virtual int			GetGuestLimitCount();
	virtual void		SetEnable(BOOL isEnable=TRUE);
	virtual BOOL		IsEnable();
	// ����.�����ڼ䲻���������κ�����ֵ.���ص�ֵ���ڽ���
	virtual int			GetLock();
	// ����
	virtual BOOL		ReleaseLock(int token);
	virtual void		SetCreateTime(CTime time);
	virtual CTime		GetCreateTime();

	// �û�����
	// ���÷��������������Ƿ���֮һ
	virtual HRESULT		SetRoomOwner(UserID userID);
	virtual UserID		GetRoomOwner();
	virtual HRESULT		AddGuest(UserID userID);
	// �����˳�ʱ�Զ�����������Ϊ���ȵ�½���û�
	virtual HRESULT		RemoveGuest(UserID userID);
	// �رշ���ǿ�������û��˳�
	virtual HRESULT		Free();

	Begin_XMessage(CRoomBase)
	End_XMessage()

private:
	CRoomBase(const CRoomBase&) : CFrameBase(module_ClassroomWinData) {};
	CRoomBase& operator=(const CRoomBase&){return *this;};

protected:
	// ˽�нṹ�塢typedef�ȶ��崦

	// ˽�к������崦����:
	// void _Test();
protected:
	// ��Ա����
};

class CRoom_CPP : public CRoomBase
{
public:
};
