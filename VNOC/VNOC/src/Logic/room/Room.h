#pragma once
// 定义房间
#include "../../Base/IRefCountImpl.h"
#include <atlstr.h>
#include <atltime.h>

typedef CString UserID;

class CRoomBase : public IRefCountImpl
{
public:
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
	// 锁定.锁定期间不允许设置任何属性值.返回的值用于解锁
	virtual int			GetLock();
	// 解锁
	virtual BOOL		ReleaseLock(int token);
	virtual void		SetCreateTime(CTime time);
	virtual CTime		GetCreateTime();

	// 用户管理
	// 设置房主，房主必须是房客之一
	virtual HRESULT		SetRoomOwner(UserID userID);
	virtual UserID		GetRoomOwner();
	virtual HRESULT		AddGuest(UserID userID);
	// 房主退出时自动将房主设置为最先登陆的用户
	virtual HRESULT		RemoveGuest(UserID userID);
	// 关闭房间强制所有用户退出
	virtual HRESULT		Free();

private:
	CRoomBase(const CRoomBase&){};
	CRoomBase& operator=(const CRoomBase&){return *this;};

protected:
	// 私有结构体、typedef等定义处

	// 私有函数定义处，例:
	// void _Test();
protected:
	// 成员变量
};

class CRoom_CPP : public CRoomBase
{
public:
};
