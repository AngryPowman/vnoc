#include "stdafx.h"
#include "room.h"

CRoomBase::CRoomBase( CString ID )
{

}

CRoomBase::~CRoomBase()
{

}

void CRoomBase::SetID( CString id )
{

}

CString CRoomBase::GetID()
{
	return _T("");
}

void CRoomBase::SetName( CString name )
{

}

CString CRoomBase::GetName()
{
	return _T("");
}

void CRoomBase::SetGuestLimitCount( int count )
{

}

int CRoomBase::GetGuestLimitCount()
{
	return 0;
}

void CRoomBase::SetEnable( BOOL isEnable/*=TRUE*/ )
{

}

BOOL CRoomBase::IsEnable()
{
	return 0;
}

int CRoomBase::GetLock()
{
	return 0;
}

BOOL CRoomBase::ReleaseLock( int token )
{
	return 0;
}

void CRoomBase::SetCreateTime( CTime time )
{

}

CTime CRoomBase::GetCreateTime()
{
	return CTime();
}

HRESULT CRoomBase::SetRoomOwner( UserID userID )
{
	return E_NOTIMPL;
}

UserID CRoomBase::GetRoomOwner()
{
	return _T("");
}

HRESULT CRoomBase::AddGuest( UserID userID )
{
	return E_NOTIMPL;
}

HRESULT CRoomBase::RemoveGuest( UserID userID )
{
	return E_NOTIMPL;
}

HRESULT CRoomBase::Free()
{
	return E_NOTIMPL;
}


//////////////////////////////////////////////////////////////////////////

