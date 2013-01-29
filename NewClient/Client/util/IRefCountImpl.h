#pragma once
#include "unknwn.h"

class IRefCountImpl : public IUnknown
{
public:
	IRefCountImpl() : m_refCount(0){}
	virtual ~IRefCountImpl(){}
	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		return InterlockedIncrement(&m_refCount);
	}
	virtual ULONG STDMETHODCALLTYPE Release()
	{
		unsigned long ul = 0;
		if ((ul = InterlockedDecrement(&m_refCount)) == 0)
		{
			delete this;
		}
		return ul;
	}
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID &,void **)
	{
		return E_NOTIMPL;
	}
public:
	LONG m_refCount;
};