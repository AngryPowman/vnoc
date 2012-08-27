#pragma once

//////////////////////////////////////////////////////////////////////////
// CEnumFormatEtc
class CEnumFormatEtc : public IEnumFORMATETC
{
private:
    ULONG           m_cRefCount;
    CSimpleArray<FORMATETC>  m_pFmtEtc;
    int           m_iCur;

public:
    CEnumFormatEtc(const CSimpleArray<FORMATETC>& ArrFE);
    CEnumFormatEtc(const CSimpleArray<FORMATETC*>& ArrFE);
    //IUnknown members
    STDMETHOD(QueryInterface)(REFIID, void FAR* FAR*);
    STDMETHOD_(ULONG, AddRef)(void);
    STDMETHOD_(ULONG, Release)(void);

    //IEnumFORMATETC members
    STDMETHOD(Next)(ULONG, LPFORMATETC, ULONG FAR *);
    STDMETHOD(Skip)(ULONG);
    STDMETHOD(Reset)(void);
    STDMETHOD(Clone)(IEnumFORMATETC FAR * FAR*);
};

//////////////////////////////////////////////////////////////////////////
// CIDropSource
class CIDropSource : public IDropSource
{
    long m_cRefCount;
public:
    bool m_bDropped;

    CIDropSource::CIDropSource():m_cRefCount(0),m_bDropped(false) {}
    //IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);        
    virtual ULONG STDMETHODCALLTYPE AddRef( void);
    virtual ULONG STDMETHODCALLTYPE Release( void);
    //IDropSource
    virtual HRESULT STDMETHODCALLTYPE QueryContinueDrag( 
        /* [in] */ BOOL fEscapePressed,
        /* [in] */ DWORD grfKeyState);

    virtual HRESULT STDMETHODCALLTYPE GiveFeedback( 
        /* [in] */ DWORD dwEffect);
};

///////////////////////////////////////////////////////////////////////////////////////////////
class CIDataObject : public IDataObject//,public IAsyncOperation
{
    CIDropSource* m_pDropSource;
    long m_cRefCount;
    CSimpleArray<FORMATETC*> m_ArrFormatEtc;
    CSimpleArray<STGMEDIUM*> m_StgMedium;
	UINT m_uClipbrdFormat;
public:
    CIDataObject(CIDropSource* pDropSource);
    ~CIDataObject();
    void CopyMedium(STGMEDIUM* pMedDest, STGMEDIUM* pMedSrc, FORMATETC* pFmtSrc);
    //IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);        
    virtual ULONG STDMETHODCALLTYPE AddRef( void);
    virtual ULONG STDMETHODCALLTYPE Release( void);

    //IDataObject
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetData( 
        /* [unique][in] */ FORMATETC __RPC_FAR *pformatetcIn,
        /* [out] */ STGMEDIUM __RPC_FAR *pmedium);

    virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetDataHere( 
        /* [unique][in] */ FORMATETC __RPC_FAR *pformatetc,
        /* [out][in] */ STGMEDIUM __RPC_FAR *pmedium);

    virtual HRESULT STDMETHODCALLTYPE QueryGetData( 
        /* [unique][in] */ FORMATETC __RPC_FAR *pformatetc);

    virtual HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc( 
        /* [unique][in] */ FORMATETC __RPC_FAR *pformatectIn,
        /* [out] */ FORMATETC __RPC_FAR *pformatetcOut);

    virtual /* [local] */ HRESULT STDMETHODCALLTYPE SetData( 
        /* [unique][in] */ FORMATETC __RPC_FAR *pformatetc,
        /* [unique][in] */ STGMEDIUM __RPC_FAR *pmedium,
        /* [in] */ BOOL fRelease);

    virtual HRESULT STDMETHODCALLTYPE EnumFormatEtc( 
        /* [in] */ DWORD dwDirection,
        /* [out] */ IEnumFORMATETC __RPC_FAR *__RPC_FAR *ppenumFormatEtc);

    virtual HRESULT STDMETHODCALLTYPE DAdvise( 
        /* [in] */ FORMATETC __RPC_FAR *pformatetc,
        /* [in] */ DWORD advf,
        /* [unique][in] */ IAdviseSink __RPC_FAR *pAdvSink,
        /* [out] */ DWORD __RPC_FAR *pdwConnection);

    virtual HRESULT STDMETHODCALLTYPE DUnadvise( 
        /* [in] */ DWORD dwConnection);

    virtual HRESULT STDMETHODCALLTYPE EnumDAdvise( 
        /* [out] */ IEnumSTATDATA __RPC_FAR *__RPC_FAR *ppenumAdvise);
};

///////////////////////////////////////////////////////////////////////////////////////////////

interface IBkDropContainer;
class CIDropTarget : public IBkDropTarget,public IDropTarget
{
protected:

	DWORD m_cRefCount;
	FORMATETC m_SupportedFrmt;
	IBkDropContainer* m_pContainer;
	BOOL  m_bDoDragDrop;
	BOOL  m_bDoDragDroping;
	CPoint m_ptDown;
	CPoint m_ptLast;
	HWND   m_hTarget;
	vStrings m_vString;
	BkDragData m_DragData;
public:	
	virtual~CIDropTarget();
    CIDropTarget(IBkDropContainer* pContainer = NULL);
    //return values: true - release the medium. false - don't release the medium 
    //virtual bool OnDrop(FORMATETC* pFmtEtc, STGMEDIUM& medium, DWORD *pdwEffect) { return true;}

    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef( void) ;
    virtual ULONG STDMETHODCALLTYPE Release( void);

    virtual HRESULT STDMETHODCALLTYPE DragEnter(
        /* [unique][in] */ IDataObject __RPC_FAR *pDataObj,
        /* [in] */ DWORD grfKeyState,
        /* [in] */ POINTL pt,
        /* [out][in] */ DWORD __RPC_FAR *pdwEffect);
    virtual HRESULT STDMETHODCALLTYPE DragOver( 
        /* [in] */ DWORD grfKeyState,
        /* [in] */ POINTL pt,
        /* [out][in] */ DWORD __RPC_FAR *pdwEffect);
    virtual HRESULT STDMETHODCALLTYPE DragLeave( void);    
    virtual HRESULT STDMETHODCALLTYPE Drop(
        /* [unique][in] */ IDataObject __RPC_FAR *pDataObj,
        /* [in] */ DWORD grfKeyState,
        /* [in] */ POINTL pt,
        /* [out][in] */ DWORD __RPC_FAR *pdwEffect);

public:
	virtual HRESULT	 Register(HWND hWnd);
	virtual HRESULT  DoDragDrop(BkDragData* bkDragDropData);
	virtual void     PreDragDrop(CPoint ptDown);
	virtual void     UnPreDragDrop();
	virtual FORMATETC*  GetFormatetc();
	virtual vStrings&   GetDragQueryFile();
	virtual BkDragData* GetDragInfo();
	virtual CPoint	 GetLeavePoint();
	virtual CPoint	 GetLBDownPoint();
};

class CDropTargetHelper
{
public:
	static HRESULT ReadHDropInfo(IDataObject *pDataObj, BkDragData* bkDragDropData,vStrings&vString,FORMATETC* lpfmtetc = NULL)
	{
		HGLOBAL hg = GetGlobalData (pDataObj, lpfmtetc->cfFormat ,lpfmtetc);
		if (NULL == hg)
		{
			FORMATETC fmtetc = {CF_HDROP,0,DVASPECT_CONTENT,-1,TYMED_HGLOBAL};
			hg = GetGlobalData (pDataObj,fmtetc.cfFormat ,&fmtetc);
			if (NULL == hg)
				return E_FAIL;

			HDROP hdrop = (HDROP)GlobalLock (hg);
			if ( NULL == hdrop )
			{
				GlobalUnlock ( hg );
				return E_FAIL;
			}
			bkDragDropData->dwDragOpt = DRAGOPT_FROM_EXPLORER;
			ReadHDropInfoFromHDROP(hdrop, vString);
			GlobalUnlock ( hg );
		}
		else
		{
			HDROP hdrop = (HDROP)GlobalLock (hg);
			if ( NULL == hdrop )
			{
				GlobalUnlock ( hg );
				return E_FAIL;
			}
			if (lpfmtetc->tymed == TYMED_MFPICT)
			{
				BkDragData* bkDragDrop = (BkDragData*)GlobalLock(hg); 
				if (bkDragDrop)
					*bkDragDropData = *bkDragDrop;
					bkDragDropData->dwDragOpt = DRAGOPT_FORM_MYSELF;
			}
			else if (lpfmtetc->tymed == TYMED_HGLOBAL)
			{
				bkDragDropData->dwDragOpt = DRAGOPT_FROM_EXPLORER;
				ReadHDropInfoFromHDROP(hdrop, vString);
			}

			GlobalUnlock ( hg );
		}
		return S_OK;
	}
private:
	static BOOL	ReadHDropInfoFromHDROP(HDROP hDrop, vStrings&vString)
	{
		ATLASSERT(NULL != hDrop);

		WCHAR szFile[MAX_PATH] = {0};  
		UINT cFiles = DragQueryFile(hDrop, (UINT) -1, NULL, 0);  
		for( UINT count = 0; count < cFiles; count++)
		{   
			DragQueryFile( hDrop, count, szFile, sizeof( szFile));   
			vString.push_back(szFile);
		}

		return (cFiles > 0);
	};
	static LPFORMATETC	FillFormatEtc(LPFORMATETC lpFormatEtc,CLIPFORMAT cfFormat,LPFORMATETC lpFormatEtcFill)
	{
		ATLASSERT(lpFormatEtcFill != NULL);
		if (lpFormatEtc == NULL && cfFormat!=0)
		{
			lpFormatEtc = lpFormatEtcFill;
			lpFormatEtc->cfFormat = cfFormat;
			lpFormatEtc->ptd = NULL;
			lpFormatEtc->dwAspect = DVASPECT_CONTENT;
			lpFormatEtc->lindex = -1;
			lpFormatEtc->tymed = TYMED_HGLOBAL;
		}
		return lpFormatEtc;
	};
	static HGLOBAL GetGlobalData(IDataObject *pDataObj,CLIPFORMAT cfFormat,LPFORMATETC lpFormatEtc = NULL)
	{
		if (pDataObj == NULL)
			return NULL;

		// fill in FORMATETC struct
		FORMATETC formatEtc;
		BOOL bFillFormatEtc = (lpFormatEtc == NULL);
		lpFormatEtc = FillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);
		if (bFillFormatEtc)
		{
			lpFormatEtc->tymed = TYMED_HGLOBAL|TYMED_MFPICT;
		}

		// attempt to get the data
		STGMEDIUM stgMedium;
		SCODE sc = pDataObj->GetData(lpFormatEtc, &stgMedium);
		if (FAILED(sc))
			return NULL;

		// handle just hGlobal types
		switch (stgMedium.tymed)
		{
		case TYMED_MFPICT:
		case TYMED_HGLOBAL:
			if (stgMedium.pUnkForRelease == NULL)
				return stgMedium.hGlobal;
		}

		::ReleaseStgMedium(&stgMedium);
		return NULL;
	};
};

#include "bkDrag.inl"