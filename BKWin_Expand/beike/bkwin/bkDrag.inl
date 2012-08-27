#pragma once

#include "bkDrag.h"
#include "IBkWnd.h"

inline CIDataObject::CIDataObject(CIDropSource* pDropSource)
    : m_cRefCount(0)
    , m_pDropSource(pDropSource)
{
}

inline CIDataObject::~CIDataObject()
{
	for(int i = 0; i < m_StgMedium.GetSize(); ++i)
	{
		ReleaseStgMedium(m_StgMedium[i]);
		delete m_StgMedium[i];
	}
	for(int j = 0; j < m_ArrFormatEtc.GetSize(); ++j)
		delete m_ArrFormatEtc[j];
}

inline STDMETHODIMP CIDataObject::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	*ppvObject = NULL;
	if (IID_IUnknown==riid || IID_IDataObject==riid)
             *ppvObject=this;
    if (NULL!=*ppvObject)
    {
        ((LPUNKNOWN)*ppvObject)->AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

inline STDMETHODIMP_(ULONG) CIDataObject::AddRef(void)
{
	return ++m_cRefCount;
}

inline STDMETHODIMP_(ULONG) CIDataObject::Release(void)
{
   long nTemp;
   nTemp = --m_cRefCount;
   if(nTemp==0)
      delete this;

   return nTemp;
}

inline STDMETHODIMP CIDataObject::GetData(FORMATETC __RPC_FAR *pformatetcIn, STGMEDIUM __RPC_FAR *pmedium)
{ 
	if(pformatetcIn == NULL || pmedium == NULL)
		return E_INVALIDARG;
	pmedium->hGlobal = NULL;

	for(int i=0; i < m_ArrFormatEtc.GetSize(); ++i)
	{
		if(pformatetcIn->tymed & m_ArrFormatEtc[i]->tymed &&
		   pformatetcIn->dwAspect == m_ArrFormatEtc[i]->dwAspect &&
		   pformatetcIn->cfFormat == m_ArrFormatEtc[i]->cfFormat)
		{
			CopyMedium(pmedium, m_StgMedium[i], m_ArrFormatEtc[i]);
			return S_OK;
		}
	}
	return DV_E_FORMATETC;
}

inline STDMETHODIMP CIDataObject::GetDataHere(FORMATETC __RPC_FAR *pformatetc, STGMEDIUM __RPC_FAR *pmedium)
{  
   return E_NOTIMPL;
}

inline STDMETHODIMP CIDataObject::QueryGetData(FORMATETC __RPC_FAR *pformatetc)
{ 
	if(pformatetc == NULL)
		return E_INVALIDARG;

	if (!(DVASPECT_CONTENT & pformatetc->dwAspect))
		return (DV_E_DVASPECT);
	HRESULT  hr = DV_E_TYMED;
	for(int i = 0; i < m_ArrFormatEtc.GetSize(); ++i)
	{
	   if(pformatetc->tymed & m_ArrFormatEtc[i]->tymed)
	   {
		  if(pformatetc->cfFormat == m_ArrFormatEtc[i]->cfFormat)
			 return S_OK;
		  else
			 hr = DV_E_CLIPFORMAT;
	   }
	   else
		  hr = DV_E_TYMED;
	}
	return hr;
}

inline STDMETHODIMP CIDataObject::GetCanonicalFormatEtc(FORMATETC __RPC_FAR *pformatectIn, FORMATETC __RPC_FAR *pformatetcOut)
{ 
	if (pformatetcOut == NULL)
		return E_INVALIDARG;
	return DATA_S_SAMEFORMATETC;
}

inline STDMETHODIMP CIDataObject::SetData(FORMATETC __RPC_FAR *pformatetc, STGMEDIUM __RPC_FAR *pmedium,BOOL fRelease)
{ 
	if(pformatetc == NULL || pmedium == NULL)
      return E_INVALIDARG;

	ATLASSERT(pformatetc->tymed == pmedium->tymed);
	FORMATETC* fetc=new FORMATETC;
	STGMEDIUM* pStgMed = new STGMEDIUM;

	if(fetc == NULL || pStgMed == NULL)
		return E_OUTOFMEMORY;

	ZeroMemory(fetc,sizeof(FORMATETC));
	ZeroMemory(pStgMed,sizeof(STGMEDIUM));

	*fetc = *pformatetc;
	m_ArrFormatEtc.Add(fetc);

    if(fRelease)
      *pStgMed = *pmedium;
    else
    {
		CopyMedium(pStgMed, pmedium, pformatetc);
	}
	m_StgMedium.Add(pStgMed);

    return S_OK;
}

inline void CIDataObject::CopyMedium(STGMEDIUM* pMedDest, STGMEDIUM* pMedSrc, FORMATETC* pFmtSrc)
{
		switch(pMedSrc->tymed)
		{
		case TYMED_HGLOBAL:
				pMedDest->hGlobal = (HGLOBAL)OleDuplicateData(pMedSrc->hGlobal,pFmtSrc->cfFormat, NULL);
			break;
		case TYMED_GDI:
			pMedDest->hBitmap = (HBITMAP)OleDuplicateData(pMedSrc->hBitmap,pFmtSrc->cfFormat, NULL);
			break;
		case TYMED_MFPICT:
			pMedDest->hMetaFilePict = (HMETAFILEPICT)OleDuplicateData(pMedSrc->hMetaFilePict,pFmtSrc->cfFormat, NULL);
			break;
		case TYMED_ENHMF:
			pMedDest->hEnhMetaFile = (HENHMETAFILE)OleDuplicateData(pMedSrc->hEnhMetaFile,pFmtSrc->cfFormat, NULL);
			break;
		case TYMED_FILE:
			pMedSrc->lpszFileName = (LPOLESTR)OleDuplicateData(pMedSrc->lpszFileName,pFmtSrc->cfFormat, NULL);
			break;
		case TYMED_ISTREAM:
			pMedDest->pstm = pMedSrc->pstm;
			pMedSrc->pstm->AddRef();
			break;
		case TYMED_ISTORAGE:
			pMedDest->pstg = pMedSrc->pstg;
			pMedSrc->pstg->AddRef();
			break;
		case TYMED_NULL:
		default:
			break;
		}
		pMedDest->tymed = pMedSrc->tymed;
		pMedDest->pUnkForRelease = NULL;
		if(pMedSrc->pUnkForRelease != NULL)
		{
			pMedDest->pUnkForRelease = pMedSrc->pUnkForRelease;
			pMedSrc->pUnkForRelease->AddRef();
		}
}

inline STDMETHODIMP CIDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC __RPC_FAR *__RPC_FAR *ppenumFormatEtc)
{ 
	if(ppenumFormatEtc == NULL)
      return E_POINTER;

	*ppenumFormatEtc=NULL;
	switch (dwDirection)
    {
      case DATADIR_GET:
         *ppenumFormatEtc= new CEnumFormatEtc(m_ArrFormatEtc);
		 if(*ppenumFormatEtc == NULL)
			 return E_OUTOFMEMORY;
         (*ppenumFormatEtc)->AddRef(); 
         break;
      
	  case DATADIR_SET:
      default:
		 return E_NOTIMPL;
         break;
    }

   return S_OK;
}

inline STDMETHODIMP CIDataObject::DAdvise(FORMATETC __RPC_FAR *pformatetc, DWORD advf, IAdviseSink __RPC_FAR *pAdvSink, DWORD __RPC_FAR *pdwConnection)
{ 
	return OLE_E_ADVISENOTSUPPORTED;
}

inline STDMETHODIMP CIDataObject::DUnadvise(DWORD dwConnection)
{
	return E_NOTIMPL;
}

inline HRESULT STDMETHODCALLTYPE CIDataObject::EnumDAdvise(IEnumSTATDATA __RPC_FAR *__RPC_FAR *ppenumAdvise)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

//////////////////////////////////////////////////////////////////////
// CIDropSource Class
//////////////////////////////////////////////////////////////////////

inline STDMETHODIMP CIDropSource::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
   *ppvObject = NULL;
   if (IID_IUnknown==riid || IID_IDropSource==riid)
       *ppvObject=this;

    if (*ppvObject != NULL)
    {
       ((LPUNKNOWN)*ppvObject)->AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

inline STDMETHODIMP_(ULONG) CIDropSource::AddRef( void)
{
	ATLTRACE("CIDropSource::AddRef\n");
	return ++m_cRefCount;
}

inline STDMETHODIMP_(ULONG) CIDropSource::Release( void)
{
   long nTemp;
   nTemp = --m_cRefCount;
   ATLASSERT(nTemp >= 0);
   if(nTemp==0)
      delete this;

   return nTemp;
}

inline STDMETHODIMP CIDropSource::QueryContinueDrag( BOOL fEscapePressed, DWORD grfKeyState)
{
   if(fEscapePressed)
      return DRAGDROP_S_CANCEL;
   if(!(grfKeyState & (MK_LBUTTON|MK_RBUTTON)))
   {
	  m_bDropped = true;
      return DRAGDROP_S_DROP;
   }

   return S_OK;
}

inline STDMETHODIMP CIDropSource::GiveFeedback( /* [in] */ DWORD dwEffect)
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}

//////////////////////////////////////////////////////////////////////
// CEnumFormatEtc Class
//////////////////////////////////////////////////////////////////////

inline CEnumFormatEtc::CEnumFormatEtc(const CSimpleArray<FORMATETC>& ArrFE)
    : m_cRefCount(0)
    , m_iCur(0)
{
   for(int i = 0; i < ArrFE.GetSize(); ++i)
		m_pFmtEtc.Add(ArrFE[i]);
}

inline CEnumFormatEtc::CEnumFormatEtc(const CSimpleArray<FORMATETC*>& ArrFE):
m_cRefCount(0),m_iCur(0)
{
   for(int i = 0; i < ArrFE.GetSize(); ++i)
		m_pFmtEtc.Add(*ArrFE[i]);
}

inline STDMETHODIMP  CEnumFormatEtc::QueryInterface(REFIID refiid, void FAR* FAR* ppv)
{
   *ppv = NULL;
   if (IID_IUnknown==refiid || IID_IEnumFORMATETC==refiid)
             *ppv=this;

    if (*ppv != NULL)
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

inline STDMETHODIMP_(ULONG) CEnumFormatEtc::AddRef(void)
{
   return ++m_cRefCount;
}

inline STDMETHODIMP_(ULONG) CEnumFormatEtc::Release(void)
{
   long nTemp = --m_cRefCount;
   ATLASSERT(nTemp >= 0);
   if(nTemp == 0)
     delete this;

   return nTemp; 
}

inline STDMETHODIMP CEnumFormatEtc::Next( ULONG celt,LPFORMATETC lpFormatEtc, ULONG FAR *pceltFetched)
{
   if(pceltFetched != NULL)
   	   *pceltFetched=0;
	
   ULONG cReturn = celt;

   if(celt <= 0 || lpFormatEtc == NULL || m_iCur >= m_pFmtEtc.GetSize())
      return S_FALSE;

   if(pceltFetched == NULL && celt != 1) 
      return S_FALSE;

	while (m_iCur < m_pFmtEtc.GetSize() && cReturn > 0)
	{
		*lpFormatEtc++ = m_pFmtEtc[m_iCur++];
		--cReturn;
	}

	if (pceltFetched != NULL)
		*pceltFetched = celt - cReturn;

    return (cReturn == 0) ? S_OK : S_FALSE;
}
   
inline STDMETHODIMP CEnumFormatEtc::Skip(ULONG celt)
{
	if((m_iCur + int(celt)) >= m_pFmtEtc.GetSize())
		return S_FALSE;

	m_iCur += celt;
	return S_OK;
}

inline STDMETHODIMP CEnumFormatEtc::Reset(void)
{
   m_iCur = 0;
   return S_OK;
}
               
inline STDMETHODIMP CEnumFormatEtc::Clone(IEnumFORMATETC FAR * FAR*ppCloneEnumFormatEtc)
{
  if(ppCloneEnumFormatEtc == NULL)
      return E_POINTER;
      
  CEnumFormatEtc *newEnum = new CEnumFormatEtc(m_pFmtEtc);
  if(newEnum ==NULL)
		return E_OUTOFMEMORY;  	

  newEnum->AddRef();
  newEnum->m_iCur = m_iCur;
  *ppCloneEnumFormatEtc = newEnum;

  return S_OK;
}

//////////////////////////////////////////////////////////////////////
// CIDropTarget Class
//////////////////////////////////////////////////////////////////////
inline CIDropTarget::CIDropTarget(IBkDropContainer* pContainer)
    : m_pContainer(pContainer)
    , m_cRefCount(0)
{
	OleInitialize(NULL);

	m_SupportedFrmt.dwAspect = DVASPECT_CONTENT;
	m_SupportedFrmt.lindex = -1;
	m_SupportedFrmt.cfFormat = CF_HDROP;
	m_SupportedFrmt.tymed = TYMED_HGLOBAL;
	m_SupportedFrmt.ptd = NULL;

	m_bDoDragDrop = FALSE;
	m_bDoDragDroping = FALSE;
	m_hTarget = NULL;
	m_ptDown = 0;
	m_ptLast = 0;
	m_DragData.pvStrings = &m_vString;

	AddRef();
}

inline CIDropTarget::~CIDropTarget()
{
	Release();
	OleUninitialize();
}

inline HRESULT __stdcall CIDropTarget::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	*ppvObject = NULL;

	if (__uuidof(IUnknown) ==riid)
		*ppvObject = (IUnknown*)this;
	else if (__uuidof(IDropTarget) ==riid)
		*ppvObject = (IDropTarget*)this;

	if (*ppvObject)
	{
		((LPUNKNOWN)*ppvObject)->AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

inline ULONG __stdcall CIDropTarget::AddRef( void)
{
	return ++m_cRefCount;
}

inline ULONG __stdcall CIDropTarget::Release( void)
{
   long nTemp = 0;
   nTemp = --m_cRefCount;
   ATLASSERT(nTemp >= 0);
   if(nTemp==0)
	  delete this;

   return nTemp;
}

inline HRESULT __stdcall CIDropTarget::DragEnter(IDataObject __RPC_FAR *pDataObj, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect)
{
    if(NULL == pDataObj)
        return E_INVALIDARG;

    return m_pContainer->OnDragEnter(pDataObj, CPoint(pt.x, pt.y), pdwEffect);
}

inline HRESULT __stdcall CIDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect)
{
    *pdwEffect = DROPEFFECT_NONE;
	m_ptLast = CPoint(pt.x, pt.y);
	return m_pContainer->OnDragOver(CPoint(pt.x, pt.y), pdwEffect);
}

inline HRESULT __stdcall CIDropTarget::DragLeave( void)
{
	m_bDoDragDroping = FALSE;
	m_bDoDragDrop = FALSE;

	if (m_pContainer)
		return m_pContainer->OnDragLeave();
	m_DragData.dwDragOpt = DRAGOPT_FROM_EXPLORER;

	return S_OK;
}

inline HRESULT __stdcall CIDropTarget::Drop( IDataObject __RPC_FAR *pDataObj, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect)
{
	HRESULT hr = E_FAIL;

	m_bDoDragDroping = FALSE;
	m_bDoDragDrop = FALSE;
	m_ptDown = 0;

	if(NULL == pDataObj)
		return E_INVALIDARG;

	*pdwEffect = DROPEFFECT_NONE;

	CDropTargetHelper::ReadHDropInfo(pDataObj,&m_DragData,m_vString,&m_SupportedFrmt);
	POINT ptDrop;
	ptDrop.x = pt.x;
	ptDrop.y = pt.y;
	::ScreenToClient(m_hTarget, &ptDrop);
	m_DragData.ptDest = ptDrop;
	m_DragData.pvStrings = &m_vString;


	hr =  m_pContainer->OnDrop(pDataObj, CPoint(pt.x, pt.y), pdwEffect);


	m_SupportedFrmt.cfFormat = CF_HDROP;
	m_SupportedFrmt.tymed = TYMED_HGLOBAL;
	m_DragData.dwDragOpt = DRAGOPT_FROM_EXPLORER;
	m_vString.clear();

	return hr;
}

inline void CIDropTarget::PreDragDrop(CPoint ptDown)
{
	m_bDoDragDrop = TRUE;
	m_ptDown = ptDown;
}

inline BkDragData* CIDropTarget::GetDragInfo()
{
	return &m_DragData;
}

inline void CIDropTarget::UnPreDragDrop()
{
	m_bDoDragDrop = FALSE;
}

inline CPoint CIDropTarget::GetLBDownPoint()
{
	return m_ptDown;
}

inline CPoint CIDropTarget::GetLeavePoint()
{
	POINT pt;
	pt.x = m_ptLast.x;
	pt.y = m_ptLast.y;
	::ScreenToClient(m_hTarget, &pt);

	return CPoint(pt);
}

inline vStrings& CIDropTarget::GetDragQueryFile()
{
	return m_vString;
}

inline FORMATETC* CIDropTarget::GetFormatetc()
{
	return &m_SupportedFrmt;
}

inline HRESULT CIDropTarget::Register(HWND hWnd)
{
	m_hTarget = hWnd;
	return RegisterDragDrop(hWnd, this);
}

inline HRESULT CIDropTarget::DoDragDrop(BkDragData* bkDragDropData)
{ 
	HRESULT hr = E_FAIL;
	CSize size = m_ptDown - CPoint(bkDragDropData->ptSrc);
	if (abs(size.cx) < 5 && abs(size.cy) < 5)
		return hr;

	if (m_bDoDragDrop&&!m_bDoDragDroping)
	{
		OutputDebugString(L"DoDragDrop\r\n");
		UINT uGblLen = sizeof(BkDragData); 
	 	HGLOBAL hGblFiles = GlobalAlloc(GHND | GMEM_SHARE, uGblLen); 
		char* szData = (char*)GlobalLock(hGblFiles);   
		memcpy(szData, (char*)(bkDragDropData), sizeof(BkDragData));   
		GlobalUnlock(hGblFiles);   

		CIDropSource* pdsrc = new CIDropSource;
		if(pdsrc == NULL)
			return hr;

		pdsrc->AddRef();

		CIDataObject* pdobj = new CIDataObject(pdsrc);
		if(pdobj == NULL)
			return hr;

		pdobj->AddRef();

		m_SupportedFrmt.cfFormat = CF_HDROP;
		m_SupportedFrmt.ptd = NULL;
		m_SupportedFrmt.dwAspect  = DVASPECT_CONTENT;
		m_SupportedFrmt.lindex = -1;
		m_SupportedFrmt.tymed = TYMED_MFPICT;

		STGMEDIUM medium = {0};
		medium.tymed = TYMED_MFPICT;
		medium.hGlobal = hGblFiles ;
		pdobj->SetData(&m_SupportedFrmt, &medium, TRUE);

		DWORD dwEffect = DROPEFFECT_NONE;
		m_bDoDragDroping = TRUE;
		m_DragData.dwDragOpt = DRAGOPT_FORM_MYSELF;
		HRESULT hr = ::DoDragDrop(pdobj, pdsrc, DROPEFFECT_COPY, &dwEffect);
		pdsrc->Release();
		pdobj->Release();

		hr = S_OK;
	}

	return hr;
}