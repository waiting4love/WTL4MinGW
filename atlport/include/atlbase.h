/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
//
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
//
// Module: atlbase.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-15 20:31:18
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __ATLBASE_H__
#define __ATLBASE_H__

#ifndef __ATLDEF_H__
#include "atldef.h"
#endif

#include <windows.h>
#include <winnls.h>
#include <ole2.h>

#ifndef __ATLCONV_H__
#include "atlconv.h"
#endif

#ifndef __ocidl_h__
#include <ocidl.h>
#endif

#ifndef _OLECTL_H_
#include <olectl.h>
#endif

#ifndef __comcat_h__
#include <comcat.h>
#endif

#ifndef _INC_TCHAR
#include <tchar.h>
#endif

#ifndef __atliface_h__
#include "atliface.h"
#endif

#include <shlwapi.h>

#undef NULL
#define NULL 0

#ifndef _ATL_DLL_IMPL
#define _ATLAPI_NS	ATL::
#else
#define _ATLAPI_NS
#endif

/////////////////////////////////////////////////////////////////////////////
// GUID_ATLVer30 - {B62F5910-6528-11d1-9611-0000F81E0D0D}

WINX_DEFINE_CLSID(
	_CoClassATLVer30, "{B62F5910-6528-11d1-9611-0000F81E0D0D}",
	0xb62f5910, 0x6528, 0x11d1, 0x96, 0x11, 0x0, 0x0, 0xf8, 0x1e, 0xd, 0xd);

#define GUID_ATLVer30 __uuidof(_CoClassATLVer30)

namespace ATL {

/////////////////////////////////////////////////////////////////////////////
// ATLTRACE

enum atlTraceFlags
{
	// Application defined categories
	atlTraceUser        = 0x00000001,
	atlTraceUser2       = 0x00000002,
	atlTraceUser3       = 0x00000004,
	atlTraceUser4       = 0x00000008,
	// ATL defined categories
	atlTraceGeneral     = 0x00000020,
	atlTraceCOM         = 0x00000040,
	atlTraceQI      = 0x00000080,
	atlTraceRegistrar   = 0x00000100,
	atlTraceRefcount    = 0x00000200,
	atlTraceWindowing   = 0x00000400,
	atlTraceControls    = 0x00000800,
	atlTraceHosting     = 0x00001000,
	atlTraceDBClient    = 0x00002000,
	atlTraceDBProvider  = 0x00004000,
	atlTraceSnapin      = 0x00008000,
	atlTraceNotImpl     = 0x00010000,
};

#ifndef ATL_TRACE_CATEGORY
#define ATL_TRACE_CATEGORY 0xFFFFFFFF
#endif

#ifdef _DEBUG

#ifndef ATL_TRACE_LEVEL
#define ATL_TRACE_LEVEL 0
#endif

inline void _cdecl AtlTrace(LPCSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	char szBuffer[512];

	size_t nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
	ATLASSERT(nBuf < sizeof(szBuffer)); //Output truncated as it was > sizeof(szBuffer)

	OutputDebugStringA(szBuffer);
	va_end(args);
}
inline void _cdecl AtlTrace2(DWORD category, UINT level, LPCSTR lpszFormat, ...)
{
	if (category & ATL_TRACE_CATEGORY && level <= ATL_TRACE_LEVEL)
	{
		va_list args;
		va_start(args, lpszFormat);

		char szBuffer[512];

		size_t nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		ATLASSERT(nBuf < sizeof(szBuffer)); //Output truncated as it was > sizeof(szBuffer)

		OutputDebugStringA("ATL: ");
		OutputDebugStringA(szBuffer);
		va_end(args);
	}
}
#ifndef OLE2ANSI
inline void _cdecl AtlTrace(LPCWSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	WCHAR szBuffer[512];

	size_t nBuf = _vsnwprintf(szBuffer, sizeof(szBuffer) / sizeof(WCHAR), lpszFormat, args);
	ATLASSERT(nBuf < sizeof(szBuffer));//Output truncated as it was > sizeof(szBuffer)

	OutputDebugStringW(szBuffer);
	va_end(args);
}
inline void _cdecl AtlTrace2(DWORD category, UINT level, LPCWSTR lpszFormat, ...)
{
	if (category & ATL_TRACE_CATEGORY && level <= ATL_TRACE_LEVEL)
	{
		va_list args;
		va_start(args, lpszFormat);

		WCHAR szBuffer[512];

		size_t nBuf = _vsnwprintf(szBuffer, sizeof(szBuffer) / sizeof(WCHAR), lpszFormat, args);
		ATLASSERT(nBuf < sizeof(szBuffer));//Output truncated as it was > sizeof(szBuffer)

		OutputDebugStringW(L"ATL: ");
		OutputDebugStringW(szBuffer);
		va_end(args);
	}
}
#endif //!OLE2ANSI


#ifndef ATLTRACE
#define ATLTRACE            ATL::AtlTrace
#define ATLTRACE2           ATL::AtlTrace2
#endif
#define ATLTRACENOTIMPL(funcname)   ATLTRACE2(atlTraceNotImpl, 2, _T("ATL: %s not implemented.\n"), funcname); return E_NOTIMPL
#else // !DEBUG
inline void _cdecl AtlTrace(LPCSTR , ...){}
inline void _cdecl AtlTrace2(DWORD, UINT, LPCSTR , ...){}
#ifndef OLE2ANSI
inline void _cdecl AtlTrace(LPCWSTR , ...){}
inline void _cdecl AtlTrace2(DWORD, UINT, LPCWSTR , ...){}
#endif //OLE2ANSI
#ifndef ATLTRACE
#define ATLTRACE            1 ? (void)0 : ATL::AtlTrace
#define ATLTRACE2           1 ? (void)0 : ATL::AtlTrace2
#endif //ATLTRACE
#define ATLTRACENOTIMPL(funcname)   return E_NOTIMPL
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// Collection helpers - CSimpleArray & CSimpleMap

template <class T>
class CSimpleArray
{
public:
	T* m_aT;
	int m_nSize;
	int m_nAllocSize;

// Construction/destruction
	CSimpleArray() : m_aT(NULL), m_nSize(0), m_nAllocSize(0)
	{ }

	~CSimpleArray()
	{
		RemoveAll();
	}

// Operations
	int GetSize() const
	{
		return m_nSize;
	}
	BOOL Add(T& t)
	{
		if(m_nSize == m_nAllocSize)
		{
			T* aT;
			int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);
			aT = (T*)realloc(m_aT, nNewAllocSize * sizeof(T));
			if(aT == NULL)
				return FALSE;
			m_nAllocSize = nNewAllocSize;
			m_aT = aT;
		}
		m_nSize++;
		SetAtIndex(m_nSize - 1, t);
		return TRUE;
	}
	BOOL Remove(T& t)
	{
		int nIndex = Find(t);
		if(nIndex == -1)
			return FALSE;
		return RemoveAt(nIndex);
	}
	BOOL RemoveAt(int nIndex)
	{
		if(nIndex != (m_nSize - 1))
		{
			m_aT[nIndex].~T();
			memmove((void*)&m_aT[nIndex], (void*)&m_aT[nIndex + 1], (m_nSize - (nIndex + 1)) * sizeof(T));
		}
		m_nSize--;
		return TRUE;
	}
	void RemoveAll()
	{
		if(m_aT != NULL)
		{
			for(int i = 0; i < m_nSize; i++)
				m_aT[i].~T();
			free(m_aT);
			m_aT = NULL;
		}
		m_nSize = 0;
		m_nAllocSize = 0;
	}
	T& operator[] (int nIndex) const
	{
		ATLASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_aT[nIndex];
	}
	T* GetData() const
	{
		return m_aT;
	}

// Implementation
	class Wrapper
	{
	public:
		Wrapper(T& _t) : t(_t)
		{
		}
		template <class _Ty>
		void *operator new(size_t, _Ty* p)
		{
			return p;
		}
		template <class _Ty>
		void operator delete(void*, _Ty* p)	{}
		T t;
	};
	void SetAtIndex(int nIndex, T& t)
	{
		ATLASSERT(nIndex >= 0 && nIndex < m_nSize);
		new(&m_aT[nIndex]) Wrapper(t);
	}
	int Find(T& t) const
	{
		for(int i = 0; i < m_nSize; i++)
		{
			if(m_aT[i] == t)
				return i;
		}
		return -1;  // not found
	}
};

// for arrays of simple types
template <class T>
class CSimpleValArray : public CSimpleArray< T >
{
public:
	BOOL Add(T t)
	{
		return CSimpleArray< T >::Add(t);
	}
	BOOL Remove(T t)
	{
		return CSimpleArray< T >::Remove(t);
	}
	T operator[] (int nIndex) const
	{
		return CSimpleArray< T >::operator[](nIndex);
	}
};


// intended for small number of simple types or pointers
template <class TKey, class TVal>
class CSimpleMap
{
public:
	TKey* m_aKey;
	TVal* m_aVal;
	int m_nSize;

// Construction/destruction
	CSimpleMap() : m_aKey(NULL), m_aVal(NULL), m_nSize(0)
	{ }

	~CSimpleMap()
	{
		RemoveAll();
	}

// Operations
	int GetSize() const
	{
		return m_nSize;
	}
	BOOL Add(TKey key, TVal val)
	{
		TKey* pKey;
		pKey = (TKey*)realloc(m_aKey, (m_nSize + 1) * sizeof(TKey));
		if(pKey == NULL)
			return FALSE;
		m_aKey = pKey;
		TVal* pVal;
		pVal = (TVal*)realloc(m_aVal, (m_nSize + 1) * sizeof(TVal));
		if(pVal == NULL)
			return FALSE;
		m_aVal = pVal;
		m_nSize++;
		SetAtIndex(m_nSize - 1, key, val);
		return TRUE;
	}
	BOOL Remove(TKey key)
	{
		int nIndex = FindKey(key);
		if(nIndex == -1)
			return FALSE;
		if(nIndex != (m_nSize - 1))
		{
			m_aKey[nIndex].~TKey();
			m_aVal[nIndex].~TVal();
			memmove((void*)&m_aKey[nIndex], (void*)&m_aKey[nIndex + 1], (m_nSize - (nIndex + 1)) * sizeof(TKey));
			memmove((void*)&m_aVal[nIndex], (void*)&m_aVal[nIndex + 1], (m_nSize - (nIndex + 1)) * sizeof(TVal));
		}
		TKey* pKey;
		pKey = (TKey*)realloc(m_aKey, (m_nSize - 1) * sizeof(TKey));
		if(pKey != NULL || m_nSize == 1)
			m_aKey = pKey;
		TVal* pVal;
		pVal = (TVal*)realloc(m_aVal, (m_nSize - 1) * sizeof(TVal));
		if(pVal != NULL || m_nSize == 1)
			m_aVal = pVal;
		m_nSize--;
		return TRUE;
	}
	void RemoveAll()
	{
		if(m_aKey != NULL)
		{
			for(int i = 0; i < m_nSize; i++)
			{
				m_aKey[i].~TKey();
				m_aVal[i].~TVal();
			}
			free(m_aKey);
			m_aKey = NULL;
		}
		if(m_aVal != NULL)
		{
			free(m_aVal);
			m_aVal = NULL;
		}

		m_nSize = 0;
	}
	BOOL SetAt(TKey key, TVal val)
	{
		int nIndex = FindKey(key);
		if(nIndex == -1)
			return FALSE;
		SetAtIndex(nIndex, key, val);
		return TRUE;
	}
	TVal Lookup(TKey key) const
	{
		int nIndex = FindKey(key);
		if(nIndex == -1)
			return NULL;    // must be able to convert
		return GetValueAt(nIndex);
	}
	TKey ReverseLookup(TVal val) const
	{
		int nIndex = FindVal(val);
		if(nIndex == -1)
			return NULL;    // must be able to convert
		return GetKeyAt(nIndex);
	}
	TKey& GetKeyAt(int nIndex) const
	{
		ATLASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_aKey[nIndex];
	}
	TVal& GetValueAt(int nIndex) const
	{
		ATLASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_aVal[nIndex];
	}

// Implementation

	template <typename T>
	class Wrapper
	{
	public:
		Wrapper(T& _t) : t(_t)
		{
		}
		template <typename _Ty>
		void *operator new(size_t, _Ty* p)
		{
			return p;
		}
		template <class _Ty>
		void operator delete(void*, _Ty* p)	{}
		T t;
	};
	void SetAtIndex(int nIndex, TKey& key, TVal& val)
	{
		ATLASSERT(nIndex >= 0 && nIndex < m_nSize);
		new(&m_aKey[nIndex]) Wrapper<TKey>(key);
		new(&m_aVal[nIndex]) Wrapper<TVal>(val);
	}
	int FindKey(TKey& key) const
	{
		for(int i = 0; i < m_nSize; i++)
		{
			if(m_aKey[i] == key)
				return i;
		}
		return -1;  // not found
	}
	int FindVal(TVal& val) const
	{
		for(int i = 0; i < m_nSize; i++)
		{
			if(m_aVal[i] == val)
				return i;
		}
		return -1;  // not found
	}
};


///////////////////////////////////////////////////////////////////////////////
// _ATL_MODULE

typedef GUID CATID;

struct _ATL_CATMAP_ENTRY
{
   int iType;
   const CATID* pcatid;
};

#define _ATL_CATMAP_ENTRY_END 0
#define _ATL_CATMAP_ENTRY_IMPLEMENTED 1
#define _ATL_CATMAP_ENTRY_REQUIRED 2

typedef HRESULT (WINAPI _ATL_CREATORFUNC)(void* pv, REFIID riid, LPVOID* ppv);
typedef HRESULT (WINAPI _ATL_CREATORARGFUNC)(void* pv, REFIID riid, LPVOID* ppv, DWORD dw);
typedef HRESULT (WINAPI _ATL_MODULEFUNC)(DWORD dw);
typedef LPCTSTR (WINAPI _ATL_DESCRIPTIONFUNC)();
typedef const struct _ATL_CATMAP_ENTRY* (_ATL_CATMAPFUNC)();
typedef void (__stdcall _ATL_TERMFUNC)(DWORD dw);

struct _ATL_TERMFUNC_ELEM
{
	_ATL_TERMFUNC* pFunc;
	DWORD dw;
	_ATL_TERMFUNC_ELEM* pNext;
};

struct _ATL_OBJMAP_ENTRY
{
	const CLSID* pclsid;
	HRESULT (WINAPI *pfnUpdateRegistry)(BOOL bRegister);
	_ATL_CREATORFUNC* pfnGetClassObject;
	_ATL_CREATORFUNC* pfnCreateInstance;
	IUnknown* pCF;
	DWORD dwRegister;
	_ATL_DESCRIPTIONFUNC* pfnGetObjectDescription;
	_ATL_CATMAPFUNC* pfnGetCategoryMap;
	HRESULT WINAPI RevokeClassObject()
	{
		return CoRevokeClassObject(dwRegister);
	}
	HRESULT WINAPI RegisterClassObject(DWORD dwClsContext, DWORD dwFlags)
	{
		IUnknown* p = NULL;
		if (pfnGetClassObject == NULL)
			return S_OK;
		HRESULT hRes = pfnGetClassObject((void*)pfnCreateInstance, IID_IUnknown, (LPVOID*) &p);
		if (SUCCEEDED(hRes))
			hRes = CoRegisterClassObject(*pclsid, p, dwClsContext, dwFlags, &dwRegister);
		if (p != NULL)
			p->Release();
		return hRes;
	}
// Added in ATL 3.0
	void (WINAPI *pfnObjectMain)(bool bStarting);
};

struct _ATL_REGMAP_ENTRY
{
	LPCOLESTR     szKey;
	LPCOLESTR     szData;
};

struct _AtlCreateWndData
{
	void* m_pThis;
	DWORD m_dwThreadID;
	_AtlCreateWndData* m_pNext;
};

struct _ATL_MODULE
{
// Attributes
public:
	UINT cbSize;
	HINSTANCE m_hInst;
	HINSTANCE m_hInstResource;
	HINSTANCE m_hInstTypeLib;
	_ATL_OBJMAP_ENTRY* m_pObjMap;
	LONG m_nLockCnt;
#ifdef _ATL_MIN_CRT
#error "not support"
	HANDLE m_hHeap;
#else
	HANDLE m_pad_hHeap;
#endif
	union
	{
		CRITICAL_SECTION m_csTypeInfoHolder;
		CRITICAL_SECTION m_csStaticDataInit;
	};
	CRITICAL_SECTION m_csWindowCreate;
	CRITICAL_SECTION m_csObjMap;
// Original Size = 100
// Stuff added in ATL 3.0
	DWORD dwAtlBuildVer;
	_AtlCreateWndData* m_pCreateWndList;
	bool m_bDestroyHeap;
	GUID* pguidVer;
#ifndef _ATL_NO_MP_HEAP
	DWORD m_dwHeaps;    // Number of heaps we have (-1)
	HANDLE* m_phHeaps;
#else
	DWORD m_unused_2;
	HANDLE* m_unused_3;
#endif
	int m_nHeap;        // Which heap to choose from
	_ATL_TERMFUNC_ELEM* m_pTermFuncs;
};

const int _nAtlModuleVer1Size = 100;



//This define makes debugging asserts easier.
#define _ATL_SIMPLEMAPENTRY ((_ATL_CREATORARGFUNC*)1)

struct _ATL_INTMAP_ENTRY
{
	const IID* piid;       // the interface id (IID)
	DWORD dw;
	_ATL_CREATORARGFUNC* pFunc; //NULL:end, 1:offset, n:ptr
};

/////////////////////////////////////////////////////////////////////////////
// QI Support

ATLAPI AtlInternalQueryInterface(void* pThis,
	const _ATL_INTMAP_ENTRY* pEntries, REFIID iid, void** ppvObject);

/////////////////////////////////////////////////////////////////////////////
// Smart Pointer helpers

ATLAPI_(IUnknown*) AtlComPtrAssign(IUnknown** pp, IUnknown* lp);
ATLAPI_(IUnknown*) AtlComQIPtrAssign(IUnknown** pp, IUnknown* lp, REFIID riid);

/////////////////////////////////////////////////////////////////////////////
// Inproc Marshaling helpers

ATLAPI AtlFreeMarshalStream(IStream* pStream);
ATLAPI AtlMarshalPtrInProc(IUnknown* pUnk, const IID& iid, IStream** ppStream);
ATLAPI AtlUnmarshalPtr(IStream* pStream, const IID& iid, IUnknown** ppUnk);

ATLAPI_(BOOL) AtlWaitWithMessageLoop(HANDLE hEvent);

/////////////////////////////////////////////////////////////////////////////
// Connection Point Helpers

ATLAPI AtlAdvise(IUnknown* pUnkCP, IUnknown* pUnk, const IID& iid, LPDWORD pdw);
ATLAPI AtlUnadvise(IUnknown* pUnkCP, const IID& iid, DWORD dw);

/////////////////////////////////////////////////////////////////////////////
// IDispatch Error handling

ATLAPI AtlSetErrorInfo(const CLSID& clsid, LPCOLESTR lpszDesc,
	DWORD dwHelpID, LPCOLESTR lpszHelpFile, const IID& iid, HRESULT hRes,
	HINSTANCE hInst);

/////////////////////////////////////////////////////////////////////////////
// Module

ATLAPI AtlModuleRegisterClassObjects(_ATL_MODULE* pM, DWORD dwClsContext, DWORD dwFlags);
ATLAPI AtlModuleRevokeClassObjects(_ATL_MODULE* pM);
ATLAPI AtlModuleGetClassObject(_ATL_MODULE* pM, REFCLSID rclsid, REFIID riid, LPVOID* ppv);
ATLAPI AtlModuleRegisterServer(_ATL_MODULE* pM, BOOL bRegTypeLib, const CLSID* pCLSID = NULL);
ATLAPI AtlModuleUnregisterServer(_ATL_MODULE* pM, const CLSID* pCLSID = NULL);
ATLAPI AtlModuleUnregisterServerEx(_ATL_MODULE* pM, BOOL bUnRegTypeLib, const CLSID* pCLSID = NULL);
ATLAPI AtlModuleRegisterTypeLib(_ATL_MODULE* pM, LPCOLESTR lpszIndex);
ATLAPI AtlModuleUnRegisterTypeLib(_ATL_MODULE* pM, LPCOLESTR lpszIndex);
ATLAPI AtlModuleLoadTypeLib(_ATL_MODULE* pM, LPCOLESTR lpszIndex, BSTR* pbstrPath, ITypeLib** ppTypeLib);

ATLAPI AtlModuleInit(_ATL_MODULE* pM, _ATL_OBJMAP_ENTRY* p, HINSTANCE h);
ATLAPI AtlModuleTerm(_ATL_MODULE* pM);
ATLAPI_(DWORD) AtlGetVersion(void* pReserved);
ATLAPI_(void) AtlModuleAddCreateWndData(_ATL_MODULE* pM, _AtlCreateWndData* pData, void* pObject);
ATLAPI_(void*) AtlModuleExtractCreateWndData(_ATL_MODULE* pM);
ATLAPI AtlModuleAddTermFunc(_ATL_MODULE* pM, _ATL_TERMFUNC* pFunc, DWORD dw);


/////////////////////////////////////////////////////////////////////////////
// GUID comparison

inline BOOL InlineIsEqualGUID(REFGUID rguid1, REFGUID rguid2)
{
   return (
	  ((PLONG) &rguid1)[0] == ((PLONG) &rguid2)[0] &&
	  ((PLONG) &rguid1)[1] == ((PLONG) &rguid2)[1] &&
	  ((PLONG) &rguid1)[2] == ((PLONG) &rguid2)[2] &&
	  ((PLONG) &rguid1)[3] == ((PLONG) &rguid2)[3]);
}

inline BOOL InlineIsEqualUnknown(REFGUID rguid1)
{
   return (
	  ((PLONG) &rguid1)[0] == 0 &&
	  ((PLONG) &rguid1)[1] == 0 &&
#ifdef _ATL_BYTESWAP
	  ((PLONG) &rguid1)[2] == 0xC0000000 &&
	  ((PLONG) &rguid1)[3] == 0x00000046);
#else
	  ((PLONG) &rguid1)[2] == 0x000000C0 &&
	  ((PLONG) &rguid1)[3] == 0x46000000);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// QI support

ATLINLINE ATLAPI AtlInternalQueryInterface(void* pThis,
	const _ATL_INTMAP_ENTRY* pEntries, REFIID iid, void** ppvObject)
{
	ATLASSERT(pThis != NULL);
	// First entry in the com map should be a simple map entry
	ATLASSERT(pEntries->pFunc == _ATL_SIMPLEMAPENTRY);
	if (ppvObject == NULL)
		return E_POINTER;
	*ppvObject = NULL;
	if (InlineIsEqualUnknown(iid)) // use first interface
	{
			IUnknown* pUnk = (IUnknown*)((int)pThis+pEntries->dw);
			pUnk->AddRef();
			*ppvObject = pUnk;
			return S_OK;
	}
	while (pEntries->pFunc != NULL)
	{
		BOOL bBlind = (pEntries->piid == NULL);
		if (bBlind || InlineIsEqualGUID(*(pEntries->piid), iid))
		{
			if (pEntries->pFunc == _ATL_SIMPLEMAPENTRY) //offset
			{
				ATLASSERT(!bBlind);
				IUnknown* pUnk = (IUnknown*)((int)pThis+pEntries->dw);
				pUnk->AddRef();
				*ppvObject = pUnk;
				return S_OK;
			}
			else //actual function call
			{
				HRESULT hRes = pEntries->pFunc(pThis,
					iid, ppvObject, pEntries->dw);
				if (hRes == S_OK || (!bBlind && FAILED(hRes)))
					return hRes;
			}
		}
		pEntries++;
	}
	return E_NOINTERFACE;
}

/////////////////////////////////////////////////////////////////////////////
// Smart Pointer helpers

ATLINLINE ATLAPI_(IUnknown*) AtlComPtrAssign(IUnknown** pp, IUnknown* lp)
{
	if (lp != NULL)
		lp->AddRef();
	if (*pp)
		(*pp)->Release();
	*pp = lp;
	return lp;
}

ATLINLINE ATLAPI_(IUnknown*) AtlComQIPtrAssign(IUnknown** pp, IUnknown* lp, REFIID riid)
{
	IUnknown* pTemp = *pp;
	*pp = NULL;
	if (lp != NULL)
		lp->QueryInterface(riid, (void**)pp);
	if (pTemp)
		pTemp->Release();
	return *pp;
}

/////////////////////////////////////////////////////////////////////////////
// Inproc Marshaling helpers

//This API should be called from the same thread that called
//AtlMarshalPtrInProc
ATLINLINE ATLAPI AtlFreeMarshalStream(IStream* pStream)
{
	if (pStream != NULL)
	{
		LARGE_INTEGER l;
		l.QuadPart = 0;
		pStream->Seek(l, STREAM_SEEK_SET, NULL);
		CoReleaseMarshalData(pStream);
		pStream->Release();
	}
	return S_OK;
}

ATLINLINE ATLAPI AtlMarshalPtrInProc(IUnknown* pUnk, const IID& iid, IStream** ppStream)
{
	HRESULT hRes = CreateStreamOnHGlobal(NULL, TRUE, ppStream);
	if (SUCCEEDED(hRes))
	{
		hRes = CoMarshalInterface(*ppStream, iid,
			pUnk, MSHCTX_INPROC, NULL, MSHLFLAGS_TABLESTRONG);
		if (FAILED(hRes))
		{
			(*ppStream)->Release();
			*ppStream = NULL;
		}
	}
	return hRes;
}

ATLINLINE ATLAPI AtlUnmarshalPtr(IStream* pStream, const IID& iid, IUnknown** ppUnk)
{
	*ppUnk = NULL;
	HRESULT hRes = E_INVALIDARG;
	if (pStream != NULL)
	{
		LARGE_INTEGER l;
		l.QuadPart = 0;
		pStream->Seek(l, STREAM_SEEK_SET, NULL);
		hRes = CoUnmarshalInterface(pStream, iid, (void**)ppUnk);
	}
	return hRes;
}

ATLINLINE ATLAPI_(BOOL) AtlWaitWithMessageLoop(HANDLE hEvent)
{
	DWORD dwRet;
	MSG msg;

	while(1)
	{
		dwRet = MsgWaitForMultipleObjects(1, &hEvent, FALSE, INFINITE, QS_ALLINPUT);

		if (dwRet == WAIT_OBJECT_0)
			return TRUE;    // The event was signaled

		if (dwRet != WAIT_OBJECT_0 + 1)
			break;          // Something else happened

		// There is one or more window message available. Dispatch them
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0)
				return TRUE; // Event is now signaled.
		}
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// class CComPtr

template <class T>
class _NoAddRefReleaseOnCComPtr : public T
{
	private:
		STDMETHOD_(ULONG, AddRef)()=0;
		STDMETHOD_(ULONG, Release)()=0;
};

template <class T>
class CComPtr
{
public:
	typedef T _PtrClass;
	CComPtr()
	{
		p=NULL;
	}
	CComPtr(T* lp)
	{
		if ((p = lp) != NULL)
			p->AddRef();
	}
	CComPtr(const CComPtr<T>& lp)
	{
		if ((p = lp.p) != NULL)
			p->AddRef();
	}
	~CComPtr()
	{
		if (p)
			p->Release();
	}
	void Release()
	{
		IUnknown* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}
	operator T*() const
	{
		return (T*)p;
	}
	T& operator*() const
	{
		ATLASSERT(p!=NULL);
		return *p;
	}
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&()
	{
		ATLASSERT(p==NULL);
		return &p;
	}
	_NoAddRefReleaseOnCComPtr<T>* operator->() const
	{
		ATLASSERT(p!=NULL);
		return (_NoAddRefReleaseOnCComPtr<T>*)p;
	}
	T* operator=(T* lp)
	{
		return (T*)AtlComPtrAssign((IUnknown**)&p, lp);
	}
	T* operator=(const CComPtr<T>& lp)
	{
		return (T*)AtlComPtrAssign((IUnknown**)&p, lp.p);
	}
	bool operator!() const
	{
		return (p == NULL);
	}
	bool operator<(T* pT) const
	{
		return p < pT;
	}
	bool operator==(T* pT) const
	{
		return p == pT;
	}
	// Compare two objects for equivalence
	bool IsEqualObject(IUnknown* pOther)
	{
		if (p == NULL && pOther == NULL)
			return true; // They are both NULL objects

		if (p == NULL || pOther == NULL)
			return false; // One is NULL the other is not

		CComPtr<IUnknown> punk1;
		CComPtr<IUnknown> punk2;
		p->QueryInterface(IID_IUnknown, (void**)&punk1);
		pOther->QueryInterface(IID_IUnknown, (void**)&punk2);
		return punk1 == punk2;
	}
	void Attach(T* p2)
	{
		if (p)
			p->Release();
		p = p2;
	}
	T* Detach()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}
	HRESULT CopyTo(T** ppT)
	{
		ATLASSERT(ppT != NULL);
		if (ppT == NULL)
			return E_POINTER;
		*ppT = p;
		if (p)
			p->AddRef();
		return S_OK;
	}
	HRESULT SetSite(IUnknown* punkParent)
	{
		return AtlSetChildSite(p, punkParent);
	}
	HRESULT Advise(IUnknown* pUnk, const IID& iid, LPDWORD pdw)
	{
		return AtlAdvise(p, pUnk, iid, pdw);
	}
	HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		ATLASSERT(p == NULL);
		return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
	}
	HRESULT CoCreateInstance(LPCOLESTR szProgID, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		CLSID clsid;
		HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
		ATLASSERT(p == NULL);
		if (SUCCEEDED(hr))
			hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
		return hr;
	}
	template <class Q>
	HRESULT QueryInterface(Q** pp) const
	{
		ATLASSERT(pp != NULL && *pp == NULL);
		return p->QueryInterface(__uuidof(Q), (void**)pp);
	}
	T* p;
};

inline HRESULT AtlSetChildSite(IUnknown* punkChild, IUnknown* punkParent)
{
	if (punkChild == NULL)
		return E_POINTER;

	HRESULT hr;
	IObjectWithSite* pChildSite = NULL;
	hr = punkChild->QueryInterface(IID_IObjectWithSite, (void**)&pChildSite);
	if (SUCCEEDED(hr) && pChildSite != NULL)
	{
		hr = pChildSite->SetSite(punkParent);
		pChildSite->Release();
	}
	return hr;
}

template <class T, const IID* piid = &__uuidof(T)>
class CComQIPtr
{
public:
	typedef T _PtrClass;
	CComQIPtr()
	{
		p=NULL;
	}
	CComQIPtr(T* lp)
	{
		if ((p = lp) != NULL)
			p->AddRef();
	}
	CComQIPtr(const CComQIPtr<T,piid>& lp)
	{
		if ((p = lp.p) != NULL)
			p->AddRef();
	}
	CComQIPtr(IUnknown* lp)
	{
		p=NULL;
		if (lp != NULL)
			lp->QueryInterface(*piid, (void **)&p);
	}
	~CComQIPtr()
	{
		if (p)
			p->Release();
	}
	void Release()
	{
		IUnknown* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}
	operator T*() const
	{
		return p;
	}
	T& operator*() const
	{
		ATLASSERT(p!=NULL); return *p;
	}
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&()
	{
		ATLASSERT(p==NULL);
		return &p;
	}
	_NoAddRefReleaseOnCComPtr<T>* operator->() const
	{
		ATLASSERT(p!=NULL);
		return (_NoAddRefReleaseOnCComPtr<T>*)p;
	}
	T* operator=(T* lp)
	{
		return (T*)AtlComPtrAssign((IUnknown**)&p, lp);
	}
	T* operator=(const CComQIPtr<T,piid>& lp)
	{
		return (T*)AtlComPtrAssign((IUnknown**)&p, lp.p);
	}
	T* operator=(IUnknown* lp)
	{
		return (T*)AtlComQIPtrAssign((IUnknown**)&p, lp, *piid);
	}
	bool operator!() const
	{
		return (p == NULL);
	}
	bool operator<(T* pT) const
	{
		return p < pT;
	}
	bool operator==(T* pT) const
	{
		return p == pT;
	}
	// Compare two objects for equivalence
	bool IsEqualObject(IUnknown* pOther)
	{
		if (p == NULL && pOther == NULL)
			return true; // They are both NULL objects

		if (p == NULL || pOther == NULL)
			return false; // One is NULL the other is not

		CComPtr<IUnknown> punk1;
		CComPtr<IUnknown> punk2;
		p->QueryInterface(IID_IUnknown, (void**)&punk1);
		pOther->QueryInterface(IID_IUnknown, (void**)&punk2);
		return punk1 == punk2;
	}
	void Attach(T* p2)
	{
		if (p)
			p->Release();
		p = p2;
	}
	T* Detach()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}
	HRESULT CopyTo(T** ppT)
	{
		ATLASSERT(ppT != NULL);
		if (ppT == NULL)
			return E_POINTER;
		*ppT = p;
		if (p)
			p->AddRef();
		return S_OK;
	}
	HRESULT SetSite(IUnknown* punkParent)
	{
		return AtlSetChildSite(p, punkParent);
	}
	HRESULT Advise(IUnknown* pUnk, const IID& iid, LPDWORD pdw)
	{
		return AtlAdvise(p, pUnk, iid, pdw);
	}
	HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		ATLASSERT(p == NULL);
		return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
	}
	HRESULT CoCreateInstance(LPCOLESTR szProgID, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		CLSID clsid;
		HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
		ATLASSERT(p == NULL);
		if (SUCCEEDED(hr))
			hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
		return hr;
	}
	template <class Q>
	HRESULT QueryInterface(Q** pp)
	{
		ATLASSERT(pp != NULL && *pp == NULL);
		return p->QueryInterface(__uuidof(Q), (void**)pp);
	}
	T* p;
};

//Specialization to make it work
template<>
class CComQIPtr<IUnknown, &IID_IUnknown>
{
public:
	typedef IUnknown T;
	typedef IUnknown _PtrClass;
	CComQIPtr()
	{
		p=NULL;
	}
	CComQIPtr(IUnknown* lp)
	{
		//Actually do a QI to get identity
		p=NULL;
		if (lp != NULL)
			lp->QueryInterface(IID_IUnknown, (void **)&p);
	}
	CComQIPtr(const CComQIPtr<IUnknown,&IID_IUnknown>& lp)
	{
		if ((p = lp.p) != NULL)
			p->AddRef();
	}
	~CComQIPtr()
	{
		if (p)
			p->Release();
	}
	void Release()
	{
		IUnknown* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}
	operator IUnknown*() const
	{
		return p;
	}
	IUnknown& operator*() const
	{
		ATLASSERT(p!=NULL);
		return *p;
	}
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	IUnknown** operator&()
	{
		ATLASSERT(p==NULL);
		return &p;
	}
	_NoAddRefReleaseOnCComPtr<T>* operator->() const
	{
		ATLASSERT(p!=NULL);
		return (_NoAddRefReleaseOnCComPtr<T>*)p;
	}
	IUnknown* operator=(IUnknown* lp)
	{
		//Actually do a QI to get identity
		return (IUnknown*)AtlComQIPtrAssign((IUnknown**)&p, lp, IID_IUnknown);
	}
	IUnknown* operator=(const CComQIPtr<IUnknown,&IID_IUnknown>& lp)
	{
		return (IUnknown*)AtlComPtrAssign((IUnknown**)&p, lp.p);
	}
	bool operator!() const
	{
		return (p == NULL);
	}
	bool operator<(IUnknown* pT) const
	{
		return p < pT;
	}
	bool operator==(IUnknown* pT) const
	{
		return p == pT;
	}
	// Compare two objects for equivalence
	bool IsEqualObject(IUnknown* pOther)
	{
		if (p == NULL && pOther == NULL)
			return true; // They are both NULL objects

		if (p == NULL || pOther == NULL)
			return false; // One is NULL the other is not

		CComPtr<IUnknown> punk1;
		CComPtr<IUnknown> punk2;
		p->QueryInterface(IID_IUnknown, (void**)&punk1);
		pOther->QueryInterface(IID_IUnknown, (void**)&punk2);
		return punk1 == punk2;
	}
	IUnknown* Detach()
	{
		IUnknown* pt = p;
		p = NULL;
		return pt;
	}
	HRESULT CopyTo(T** ppT)
	{
		ATLASSERT(ppT != NULL);
		if (ppT == NULL)
			return E_POINTER;
		*ppT = p;
		if (p)
			p->AddRef();
		return S_OK;
	}
	HRESULT SetSite(IUnknown* punkParent)
	{
		return AtlSetChildSite(p, punkParent);
	}
	HRESULT Advise(IUnknown* pUnk, const IID& iid, LPDWORD pdw)
	{
		return AtlAdvise(p, pUnk, iid, pdw);
	}
	HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		ATLASSERT(p == NULL);
		return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
	}
	HRESULT CoCreateInstance(LPCOLESTR szProgID, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		CLSID clsid;
		HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
		ATLASSERT(p == NULL);
		if (SUCCEEDED(hr))
			hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
		return hr;
	}
	template <class Q>
	HRESULT QueryInterface(Q** pp)
	{
		ATLASSERT(pp != NULL && *pp == NULL);
		return p->QueryInterface(__uuidof(Q), (void**)pp);
	}
	IUnknown* p;
};


/////////////////////////////////////////////////////////////////////////////
// Connection Point Helpers

ATLINLINE ATLAPI AtlAdvise(IUnknown* pUnkCP, IUnknown* pUnk, const IID& iid, LPDWORD pdw)
{
	CComPtr<IConnectionPointContainer> pCPC;
	CComPtr<IConnectionPoint> pCP;
	HRESULT hRes = pUnkCP->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC);
	if (SUCCEEDED(hRes))
		hRes = pCPC->FindConnectionPoint(iid, &pCP);
	if (SUCCEEDED(hRes))
		hRes = pCP->Advise(pUnk, pdw);
	return hRes;
}

ATLINLINE ATLAPI AtlUnadvise(IUnknown* pUnkCP, const IID& iid, DWORD dw)
{
	CComPtr<IConnectionPointContainer> pCPC;
	CComPtr<IConnectionPoint> pCP;
	HRESULT hRes = pUnkCP->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC);
	if (SUCCEEDED(hRes))
		hRes = pCPC->FindConnectionPoint(iid, &pCP);
	if (SUCCEEDED(hRes))
		hRes = pCP->Unadvise(dw);
	return hRes;
}

/////////////////////////////////////////////////////////////////////////////
// IDispatch Error handling

ATLINLINE ATLAPI AtlSetErrorInfo(const CLSID& clsid, LPCOLESTR lpszDesc, DWORD dwHelpID,
	LPCOLESTR lpszHelpFile, const IID& iid, HRESULT hRes, HINSTANCE hInst)
{
	USES_CONVERSION;
	TCHAR szDesc[1024];
	szDesc[0] = 0;
	// For a valid HRESULT the id should be in the range [0x0200, 0xffff]
	if (HIWORD(lpszDesc) == 0) //id
	{
		UINT nID = LOWORD((DWORD)lpszDesc);
		ATLASSERT((nID >= 0x0200 && nID <= 0xffff) || hRes != 0);
		if (LoadString(hInst, nID, szDesc, 1024) == 0)
		{
			ATLASSERT(FALSE);
			lstrcpy(szDesc, _T("Unknown Error"));
		}
		lpszDesc = T2OLE(szDesc);
		if (hRes == 0)
			hRes = MAKE_HRESULT(3, FACILITY_ITF, nID);
	}

	CComPtr<ICreateErrorInfo> pICEI;
	if (SUCCEEDED(CreateErrorInfo(&pICEI)))
	{
		CComPtr<IErrorInfo> pErrorInfo;
		pICEI->SetGUID(iid);
		LPOLESTR lpsz;
		ProgIDFromCLSID(clsid, &lpsz);
		if (lpsz != NULL)
			pICEI->SetSource(lpsz);
		if (dwHelpID != 0 && lpszHelpFile != NULL)
		{
			pICEI->SetHelpContext(dwHelpID);
			pICEI->SetHelpFile(const_cast<LPOLESTR>(lpszHelpFile));
		}
		CoTaskMemFree(lpsz);
		pICEI->SetDescription((LPOLESTR)lpszDesc);
		if (SUCCEEDED(pICEI->QueryInterface(IID_IErrorInfo, (void**)&pErrorInfo)))
			SetErrorInfo(0, pErrorInfo);
	}
	return (hRes == 0) ? DISP_E_EXCEPTION : hRes;
}


/////////////////////////////////////////////////////////////////////////////
// Threading Model Support

class CComCriticalSection
{
public:
	void Lock() {EnterCriticalSection(&m_sec);}
	void Unlock() {LeaveCriticalSection(&m_sec);}
	void Init() {InitializeCriticalSection(&m_sec);}
	void Term() {DeleteCriticalSection(&m_sec);}
	CRITICAL_SECTION m_sec;
};

class CComAutoCriticalSection
{
public:
	void Lock() {EnterCriticalSection(&m_sec);}
	void Unlock() {LeaveCriticalSection(&m_sec);}
	CComAutoCriticalSection() {InitializeCriticalSection(&m_sec);}
	~CComAutoCriticalSection() {DeleteCriticalSection(&m_sec);}
	CRITICAL_SECTION m_sec;
};

class CComFakeCriticalSection
{
public:
	void Lock() {}
	void Unlock() {}
	void Init() {}
	void Term() {}
};

class CComMultiThreadModelNoCS
{
public:
	static ULONG WINAPI Increment(LPLONG p) {return InterlockedIncrement(p);}
	static ULONG WINAPI Decrement(LPLONG p) {return InterlockedDecrement(p);}
	typedef CComFakeCriticalSection AutoCriticalSection;
	typedef CComFakeCriticalSection CriticalSection;
	typedef CComMultiThreadModelNoCS ThreadModelNoCS;
};

class CComMultiThreadModel
{
public:
	static ULONG WINAPI Increment(LPLONG p) {return InterlockedIncrement(p);}
	static ULONG WINAPI Decrement(LPLONG p) {return InterlockedDecrement(p);}
	typedef CComAutoCriticalSection AutoCriticalSection;
	typedef CComCriticalSection CriticalSection;
	typedef CComMultiThreadModelNoCS ThreadModelNoCS;
};

class CComSingleThreadModel
{
public:
	static ULONG WINAPI Increment(LPLONG p) {return ++(*p);}
	static ULONG WINAPI Decrement(LPLONG p) {return --(*p);}
	typedef CComFakeCriticalSection AutoCriticalSection;
	typedef CComFakeCriticalSection CriticalSection;
	typedef CComSingleThreadModel ThreadModelNoCS;
};

#if defined(_ATL_SINGLE_THREADED)
	typedef CComSingleThreadModel CComObjectThreadModel;
	typedef CComSingleThreadModel CComGlobalsThreadModel;
#elif defined(_ATL_APARTMENT_THREADED)
	typedef CComSingleThreadModel CComObjectThreadModel;
	typedef CComMultiThreadModel CComGlobalsThreadModel;
#else
	typedef CComMultiThreadModel CComObjectThreadModel;
	typedef CComMultiThreadModel CComGlobalsThreadModel;
#endif


/////////////////////////////////////////////////////////////////////////////

typedef _ATL_OBJMAP_ENTRY _ATL_OBJMAP_ENTRY30;

inline _ATL_OBJMAP_ENTRY* _NextObjectMapEntry(_ATL_MODULE* pM, _ATL_OBJMAP_ENTRY* pEntry)
{
#ifdef _ATL_DLL_IMPL
	if (pM->cbSize == sizeof(_ATL_MODULE20))
		return (_ATL_OBJMAP_ENTRY*)(((BYTE*)pEntry) + sizeof(_ATL_OBJMAP_ENTRY20));
#endif
	return pEntry+1;
}

//Although these functions are big, they are only used once in a module
//so we should make them inline.

ATLINLINE ATLAPI AtlModuleInit(_ATL_MODULE* pM, _ATL_OBJMAP_ENTRY* p, HINSTANCE h)
{
	ATLASSERT(pM != NULL);
	if (pM == NULL)
		return E_INVALIDARG;
#ifdef _ATL_DLL_IMPL
	if ((pM->cbSize != _nAtlModuleVer1Size) && (pM->cbSize != sizeof(_ATL_MODULE)))
		return E_INVALIDARG;
#else
	ATLASSERT(pM->cbSize == sizeof(_ATL_MODULE));
#endif
	pM->m_pObjMap = p;
	pM->m_hInst = pM->m_hInstTypeLib = pM->m_hInstResource = h;
	pM->m_nLockCnt=0L;
#ifdef _ATL_MIN_CRT
	pM->m_hHeap = NULL;
#else
	pM->m_pad_hHeap = NULL;
#endif
	InitializeCriticalSection(&pM->m_csTypeInfoHolder);
	InitializeCriticalSection(&pM->m_csWindowCreate);
	InitializeCriticalSection(&pM->m_csObjMap);
#ifdef _ATL_DLL_IMPL
	if (pM->cbSize > _nAtlModuleVer1Size)
#endif
	{
		pM->m_pCreateWndList = NULL;
		pM->m_bDestroyHeap = true;
		pM->m_dwHeaps = 0;
		pM->m_nHeap = 0;
		pM->m_phHeaps = NULL;
		pM->m_pTermFuncs = NULL;
		if (pM->m_pObjMap != NULL)
		{
			_ATL_OBJMAP_ENTRY* pEntry = pM->m_pObjMap;
			while (pEntry->pclsid != NULL)
			{
				pEntry->pfnObjectMain(true); //initialize class resources
				pEntry = _NextObjectMapEntry(pM, pEntry);
			}
		}
	}

	return S_OK;
}

ATLINLINE ATLAPI AtlModuleTerm(_ATL_MODULE* pM)
{
	ATLASSERT(pM != NULL);
	if (pM == NULL)
		return E_INVALIDARG;
	ATLASSERT(pM->m_hInst != NULL);
	if (pM->m_pObjMap != NULL)
	{
		_ATL_OBJMAP_ENTRY* pEntry = pM->m_pObjMap;
		while (pEntry->pclsid != NULL)
		{
			if (pEntry->pCF != NULL)
				pEntry->pCF->Release();
			pEntry->pCF = NULL;
#ifdef _ATL_DLL_IMPL
			if (pM->cbSize > _nAtlModuleVer1Size)
#endif
				pEntry->pfnObjectMain(false); //cleanup class resources
			pEntry = _NextObjectMapEntry(pM, pEntry);
		}
	}
	DeleteCriticalSection(&pM->m_csTypeInfoHolder);
	DeleteCriticalSection(&pM->m_csWindowCreate);
	DeleteCriticalSection(&pM->m_csObjMap);

#ifdef _ATL_DLL_IMPL
	if (pM->cbSize > _nAtlModuleVer1Size)
#endif
	{
		_ATL_TERMFUNC_ELEM* pElem = pM->m_pTermFuncs;
		_ATL_TERMFUNC_ELEM* pNext = NULL;
		while (pElem != NULL)
		{
			pElem->pFunc(pElem->dw);
			pNext = pElem->pNext;
			delete pElem;
			pElem = pNext;
		}
#ifdef _ATL_MIN_CRT
		if (pM->m_hHeap != NULL && pM->m_bDestroyHeap)
		{
#ifndef _ATL_NO_MP_HEAP
			if (pM->m_phHeaps != NULL)
			{
				for (DWORD i = 0; i <= pM->m_dwHeaps; i++)
					HeapDestroy(pM->m_phHeaps[i]);
			}
#endif
			HeapDestroy(pM->m_hHeap);
		}
#endif
	}
	return S_OK;
}

ATLINLINE ATLAPI AtlModuleAddTermFunc(_ATL_MODULE* pM, _ATL_TERMFUNC* pFunc, DWORD dw)
{
	HRESULT hr = S_OK;
	_ATL_TERMFUNC_ELEM* pNew = NULL;
	ATLTRY(pNew = new _ATL_TERMFUNC_ELEM);
	if (pNew == NULL)
		hr = E_OUTOFMEMORY;
	else
	{
		pNew->pFunc = pFunc;
		pNew->dw = dw;
		EnterCriticalSection(&pM->m_csStaticDataInit);
		pNew->pNext = pM->m_pTermFuncs;
		pM->m_pTermFuncs = pNew;
		LeaveCriticalSection(&pM->m_csStaticDataInit);
	}
	return hr;
}

ATLINLINE ATLAPI AtlRegisterClassCategoriesHelper( REFCLSID clsid,
   const struct _ATL_CATMAP_ENTRY* pCatMap, BOOL bRegister )
{
   CComPtr< ICatRegister > pCatRegister;
   HRESULT hResult;
   const struct _ATL_CATMAP_ENTRY* pEntry;
   CATID catid;

   if( pCatMap == NULL )
   {
	  return( S_OK );
   }

   hResult = CoCreateInstance( CLSID_StdComponentCategoriesMgr, NULL,
	  CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pCatRegister );
   if( FAILED( hResult ) )
   {
	  // Since not all systems have the category manager installed, we'll allow
	  // the registration to succeed even though we didn't register our
	  // categories.  If you really want to register categories on a system
	  // without the category manager, you can either manually add the
	  // appropriate entries to your registry script (.rgs), or you can
	  // redistribute comcat.dll.
	  return( S_OK );
   }

   hResult = S_OK;
   pEntry = pCatMap;
   while( pEntry->iType != _ATL_CATMAP_ENTRY_END )
   {
	  catid = *pEntry->pcatid;
	  if( bRegister )
	  {
		 if( pEntry->iType == _ATL_CATMAP_ENTRY_IMPLEMENTED )
		 {
			hResult = pCatRegister->RegisterClassImplCategories( clsid, 1,
			   &catid );
		 }
		 else
		 {
			ATLASSERT( pEntry->iType == _ATL_CATMAP_ENTRY_REQUIRED );
			hResult = pCatRegister->RegisterClassReqCategories( clsid, 1,
			   &catid );
		 }
		 if( FAILED( hResult ) )
		 {
			return( hResult );
		 }
	  }
	  else
	  {
		 if( pEntry->iType == _ATL_CATMAP_ENTRY_IMPLEMENTED )
		 {
			pCatRegister->UnRegisterClassImplCategories( clsid, 1, &catid );
		 }
		 else
		 {
			ATLASSERT( pEntry->iType == _ATL_CATMAP_ENTRY_REQUIRED );
			pCatRegister->UnRegisterClassReqCategories( clsid, 1, &catid );
		 }
	  }
	  pEntry++;
   }

   return( S_OK );
}

ATLINLINE ATLAPI AtlModuleRegisterServer(_ATL_MODULE* pM, BOOL bRegTypeLib, const CLSID* pCLSID)
{
	ATLASSERT(pM != NULL);
	if (pM == NULL)
		return E_INVALIDARG;
	ATLASSERT(pM->m_hInst != NULL);
	ATLASSERT(pM->m_pObjMap != NULL);
	_ATL_OBJMAP_ENTRY* pEntry = pM->m_pObjMap;
	HRESULT hRes = S_OK;
	for (;pEntry->pclsid != NULL; pEntry = _NextObjectMapEntry(pM, pEntry))
	{
		if (pCLSID == NULL)
		{
			if (pEntry->pfnGetObjectDescription != NULL &&
				pEntry->pfnGetObjectDescription() != NULL)
					continue;
		}
		else
		{
			if (!IsEqualGUID(*pCLSID, *pEntry->pclsid))
				continue;
		}
		hRes = pEntry->pfnUpdateRegistry(TRUE);
		if (FAILED(hRes))
			break;
		if (pM->cbSize == sizeof(_ATL_MODULE))
		{
			hRes = AtlRegisterClassCategoriesHelper( *pEntry->pclsid,
				pEntry->pfnGetCategoryMap(), TRUE );
			if (FAILED(hRes))
				break;
		}
	}
	if (SUCCEEDED(hRes) && bRegTypeLib)
		hRes = AtlModuleRegisterTypeLib(pM, 0);
	return hRes;
}

ATLINLINE ATLAPI AtlModuleUnregisterServerEx(_ATL_MODULE* pM, BOOL bUnRegTypeLib, const CLSID* pCLSID)
{
	ATLASSERT(pM != NULL);
	if (pM == NULL)
		return E_INVALIDARG;
	ATLASSERT(pM->m_hInst != NULL);
	ATLASSERT(pM->m_pObjMap != NULL);
	_ATL_OBJMAP_ENTRY* pEntry = pM->m_pObjMap;
	for (;pEntry->pclsid != NULL; pEntry = _NextObjectMapEntry(pM, pEntry))
	{
		if (pCLSID == NULL)
		{
			if (pEntry->pfnGetObjectDescription != NULL
				&& pEntry->pfnGetObjectDescription() != NULL)
				continue;
		}
		else
		{
			if (!IsEqualGUID(*pCLSID, *pEntry->pclsid))
				continue;
		}
		pEntry->pfnUpdateRegistry(FALSE); //unregister
		if (pM->cbSize == sizeof(_ATL_MODULE) && pEntry->pfnGetCategoryMap != NULL)
			AtlRegisterClassCategoriesHelper( *pEntry->pclsid,
				pEntry->pfnGetCategoryMap(), FALSE );
	}
	if (bUnRegTypeLib)
		AtlModuleUnRegisterTypeLib(pM, 0);
	return S_OK;
}

ATLINLINE ATLAPI AtlModuleUnregisterServer(_ATL_MODULE* pM, const CLSID* pCLSID)
{
	return AtlModuleUnregisterServerEx(pM, FALSE, pCLSID);
}

///////////////////////////////////////////////////////////////////////////////
// class CComModule

class CComModule;

template <class Unused> struct _CComModuleBaseT {
	static CComModule* _pModule;
	static GUID m_libid;
};

typedef _CComModuleBaseT<void> _CComModuleBase;

template <class Unused> CComModule* _CComModuleBaseT<Unused>::_pModule = NULL;
template <class Unused> GUID _CComModuleBaseT<Unused>::m_libid = {0x0,0x0,0x0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

class CComModule : public _ATL_MODULE, public _CComModuleBase
{
public:
	void AddCreateWndData(_AtlCreateWndData* pData, void* pObject)
	{
		AtlModuleAddCreateWndData(this, pData, pObject);
	}
	void* ExtractCreateWndData()
	{
		return AtlModuleExtractCreateWndData(this);
	}

	HRESULT Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, const GUID* plibid = NULL)
	{
		pguidVer = (GUID*)&GUID_ATLVer30;
		_pModule = this;

		cbSize = sizeof(_ATL_MODULE);
		dwAtlBuildVer = _ATL_VER;
		AtlModuleInit(this, p, h);
		if (plibid != NULL)
			memcpy(&m_libid, plibid, sizeof(GUID));
		return S_OK;
	}

	void Term()
	{
		AtlModuleTerm(this);
	}

	HINSTANCE GetModuleInstance() {return m_hInst;}
	HINSTANCE GetResourceInstance() {return m_hInstResource;}
	HINSTANCE GetTypeLibInstance() {return m_hInstTypeLib;}

	HRESULT AddTermFunc(_ATL_TERMFUNC* pFunc, DWORD dw)
	{
		return AtlModuleAddTermFunc(this, pFunc, dw);
	}

	LONG Lock()
	{
		return CComGlobalsThreadModel::Increment(&m_nLockCnt);
	}
	LONG Unlock()
	{
		return CComGlobalsThreadModel::Decrement(&m_nLockCnt);
	}
	LONG GetLockCount()
	{
		return m_nLockCnt;
	}

	// Registry support (helpers)
	HRESULT RegisterTypeLib()
	{
		return AtlModuleRegisterTypeLib(this, NULL);
	}
	HRESULT RegisterTypeLib(LPCTSTR lpszIndex)
	{
		USES_CONVERSION;
		return AtlModuleRegisterTypeLib(this, T2COLE(lpszIndex));
	}
	HRESULT UnRegisterTypeLib()
	{
		return AtlModuleUnRegisterTypeLib(this, NULL);
	}
	HRESULT UnRegisterTypeLib(LPCTSTR lpszIndex)
	{
		USES_CONVERSION;
		return AtlModuleUnRegisterTypeLib(this, T2COLE(lpszIndex));
	}
	HRESULT RegisterServer(BOOL bRegTypeLib = FALSE, const CLSID* pCLSID = NULL)
	{
		return AtlModuleRegisterServer(this, bRegTypeLib, pCLSID);
	}

	HRESULT UnregisterServer(const CLSID* pCLSID = NULL)
	{
		return AtlModuleUnregisterServer(this, pCLSID);
	}
	HRESULT UnregisterServer(BOOL bUnRegTypeLib, const CLSID* pCLSID = NULL)
	{
		return AtlModuleUnregisterServerEx(this, bUnRegTypeLib, pCLSID);
	}

	// Standard Registration
	HRESULT WINAPI UpdateRegistryClass(const CLSID& clsid, LPCTSTR lpszProgID,
		LPCTSTR lpszVerIndProgID, UINT nDescID, DWORD dwFlags, BOOL bRegister);
	HRESULT WINAPI RegisterClassHelper(const CLSID& clsid, LPCTSTR lpszProgID,
		LPCTSTR lpszVerIndProgID, UINT nDescID, DWORD dwFlags);
	HRESULT WINAPI UnregisterClassHelper(const CLSID& clsid, LPCTSTR lpszProgID,
		LPCTSTR lpszVerIndProgID);

	// Register/Revoke All Class Factories with the OS (EXE only)
	HRESULT RegisterClassObjects(DWORD dwClsContext, DWORD dwFlags)
	{
		return AtlModuleRegisterClassObjects(this, dwClsContext, dwFlags);
	}
	HRESULT RevokeClassObjects()
	{
		return AtlModuleRevokeClassObjects(this);
	}

	// Obtain a Class Factory (DLL only)
	HRESULT GetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
	{
		return AtlModuleGetClassObject(this, rclsid, riid, ppv);
	}

	static HRESULT RegisterProgID(LPCTSTR lpszCLSID, LPCTSTR lpszProgID, LPCTSTR lpszUserDesc);

	static void ReplaceSingleQuote(LPOLESTR lpDest, LPCOLESTR lp)
	{
		while (*lp)
		{
			*lpDest++ = *lp;
			if (*lp == OLESTR('\''))
				*lpDest++ = *lp;
			lp++;
		}
		*lpDest = 0;
	}
};

#define _pModule	CComModule::_pModule


/////////////////////////////////////////////////////////////////////////////
// CComBSTR

class CComBSTR
{
public:
	BSTR m_str;
	CComBSTR()
	{
		m_str = NULL;
	}
	/*explicit*/ CComBSTR(int nSize)
	{
		m_str = ::SysAllocStringLen(NULL, nSize);
	}
	/*explicit*/ CComBSTR(int nSize, LPCOLESTR sz)
	{
		m_str = ::SysAllocStringLen(sz, nSize);
	}
	/*explicit*/ CComBSTR(LPCOLESTR pSrc)
	{
		m_str = ::SysAllocString(pSrc);
	}
	/*explicit*/ CComBSTR(const CComBSTR& src)
	{
		m_str = src.Copy();
	}
	/*explicit*/ CComBSTR(REFGUID src)
	{
		LPOLESTR szGuid;
		StringFromCLSID(src, &szGuid);
		m_str = ::SysAllocString(szGuid);
		CoTaskMemFree(szGuid);
	}
	CComBSTR& operator=(const CComBSTR& src)
	{
		if (m_str != src.m_str)
		{
			if (m_str)
				::SysFreeString(m_str);
			m_str = src.Copy();
		}
		return *this;
	}

	CComBSTR& operator=(LPCOLESTR pSrc)
	{
		::SysFreeString(m_str);
		m_str = ::SysAllocString(pSrc);
		return *this;
	}

	~CComBSTR()
	{
		::SysFreeString(m_str);
	}
	unsigned int Length() const
	{
		return (m_str == NULL)? 0 : SysStringLen(m_str);
	}
	operator BSTR() const
	{
		return m_str;
	}
	BSTR* operator&()
	{
		return &m_str;
	}
	BSTR Copy() const
	{
		return ::SysAllocStringLen(m_str, ::SysStringLen(m_str));
	}
	HRESULT CopyTo(BSTR* pbstr)
	{
		ATLASSERT(pbstr != NULL);
		if (pbstr == NULL)
			return E_POINTER;
		*pbstr = ::SysAllocStringLen(m_str, ::SysStringLen(m_str));
		if (*pbstr == NULL)
			return E_OUTOFMEMORY;
		return S_OK;
	}
	void Attach(BSTR src)
	{
		ATLASSERT(m_str == NULL);
		m_str = src;
	}
	BSTR Detach()
	{
		BSTR s = m_str;
		m_str = NULL;
		return s;
	}
	void Empty()
	{
		::SysFreeString(m_str);
		m_str = NULL;
	}
	bool operator!() const
	{
		return (m_str == NULL);
	}
	HRESULT Append(const CComBSTR& bstrSrc)
	{
		return Append(bstrSrc.m_str, SysStringLen(bstrSrc.m_str));
	}
	HRESULT Append(LPCOLESTR lpsz)
	{
		return Append(lpsz, wcslen(lpsz));
	}
	// a BSTR is just a LPCOLESTR so we need a special version to signify
	// that we are appending a BSTR
	HRESULT AppendBSTR(BSTR p)
	{
		return Append(p, SysStringLen(p));
	}
	HRESULT Append(LPCOLESTR lpsz, int nLen)
	{
		int n1 = Length();
		BSTR b;
		b = ::SysAllocStringLen(NULL, n1+nLen);
		if (b == NULL)
			return E_OUTOFMEMORY;
		memcpy(b, m_str, n1*sizeof(OLECHAR));
		memcpy(b+n1, lpsz, nLen*sizeof(OLECHAR));
		b[n1+nLen] = 0;
		SysFreeString(m_str);
		m_str = b;
		return S_OK;
	}
	HRESULT ToLower()
	{
		USES_CONVERSION;
		if (m_str != NULL)
		{
			LPTSTR psz = CharLower(OLE2T(m_str));
			if (psz == NULL)
				return E_OUTOFMEMORY;
			BSTR b = T2BSTR(psz);
			if (psz == NULL)
				return E_OUTOFMEMORY;
			SysFreeString(m_str);
			m_str = b;
		}
		return S_OK;
	}
	HRESULT ToUpper()
	{
		USES_CONVERSION;
		if (m_str != NULL)
		{
			LPTSTR psz = CharUpper(OLE2T(m_str));
			if (psz == NULL)
				return E_OUTOFMEMORY;
			BSTR b = T2BSTR(psz);
			if (psz == NULL)
				return E_OUTOFMEMORY;
			SysFreeString(m_str);
			m_str = b;
		}
		return S_OK;
	}
	bool LoadString(HINSTANCE hInst, UINT nID)
	{
		USES_CONVERSION;
		TCHAR sz[512];
		UINT nLen = ::LoadString(hInst, nID, sz, 512);
		ATLASSERT(nLen < 511);
		SysFreeString(m_str);
		m_str = (nLen != 0) ? SysAllocString(T2OLE(sz)) : NULL;
		return (nLen != 0);
	}
	bool LoadString(UINT nID)
	{
		return LoadString(_pModule->m_hInstResource, nID);
	}

	CComBSTR& operator+=(const CComBSTR& bstrSrc)
	{
		AppendBSTR(bstrSrc.m_str);
		return *this;
	}
	bool operator<(BSTR bstrSrc) const
	{
		if (bstrSrc == NULL && m_str == NULL)
			return false;
		if (bstrSrc != NULL && m_str != NULL)
			return wcscmp(m_str, bstrSrc) < 0;
		return m_str == NULL;
	}
	bool operator==(BSTR bstrSrc) const
	{
		if (bstrSrc == NULL && m_str == NULL)
			return true;
		if (bstrSrc != NULL && m_str != NULL)
			return wcscmp(m_str, bstrSrc) == 0;
		return false;
	}
	bool operator<(LPCSTR pszSrc) const
	{
		if (pszSrc == NULL && m_str == NULL)
			return false;
		USES_CONVERSION;
		if (pszSrc != NULL && m_str != NULL)
			return wcscmp(m_str, A2W(pszSrc)) < 0;
		return m_str == NULL;
	}
	bool operator==(LPCSTR pszSrc) const
	{
		if (pszSrc == NULL && m_str == NULL)
			return true;
		USES_CONVERSION;
		if (pszSrc != NULL && m_str != NULL)
			return wcscmp(m_str, A2W(pszSrc)) == 0;
		return false;
	}
#ifndef OLE2ANSI
	CComBSTR(LPCSTR pSrc)
	{
		m_str = A2WBSTR(pSrc);
	}

	CComBSTR(int nSize, LPCSTR sz)
	{
		m_str = A2WBSTR(sz, nSize);
	}

	void Append(LPCSTR lpsz)
	{
		USES_CONVERSION;
		LPCOLESTR lpo = A2COLE(lpsz);
		Append(lpo, wcslen(lpo));
	}

	CComBSTR& operator=(LPCSTR pSrc)
	{
		::SysFreeString(m_str);
		m_str = A2WBSTR(pSrc);
		return *this;
	}
#endif
	HRESULT WriteToStream(IStream* pStream)
	{
		ATLASSERT(pStream != NULL);
		ULONG cb;
		ULONG cbStrLen = m_str ? SysStringByteLen(m_str)+sizeof(OLECHAR) : 0;
		HRESULT hr = pStream->Write((void*) &cbStrLen, sizeof(cbStrLen), &cb);
		if (FAILED(hr))
			return hr;
		return cbStrLen ? pStream->Write((void*) m_str, cbStrLen, &cb) : S_OK;
	}
	HRESULT ReadFromStream(IStream* pStream)
	{
		ATLASSERT(pStream != NULL);
		ATLASSERT(m_str == NULL); // should be empty
		ULONG cbStrLen = 0;
		HRESULT hr = pStream->Read((void*) &cbStrLen, sizeof(cbStrLen), NULL);
		if ((hr == S_OK) && (cbStrLen != 0))
		{
			//subtract size for terminating NULL which we wrote out
			//since SysAllocStringByteLen overallocates for the NULL
			m_str = SysAllocStringByteLen(NULL, cbStrLen-sizeof(OLECHAR));
			if (m_str == NULL)
				hr = E_OUTOFMEMORY;
			else
				hr = pStream->Read((void*) m_str, cbStrLen, NULL);
		}
		if (hr == S_FALSE)
			hr = E_FAIL;
		return hr;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CComVariant

class CComVariant : public tagVARIANT
{
// Constructors
public:
	CComVariant()
	{
		vt = VT_EMPTY;
	}
	~CComVariant()
	{
		Clear();
	}

	CComVariant(const VARIANT& varSrc)
	{
		vt = VT_EMPTY;
		InternalCopy(&varSrc);
	}

	CComVariant(const CComVariant& varSrc)
	{
		vt = VT_EMPTY;
		InternalCopy(&varSrc);
	}

	CComVariant(BSTR bstrSrc)
	{
		vt = VT_EMPTY;
		*this = bstrSrc;
	}
	CComVariant(LPCOLESTR lpszSrc)
	{
		vt = VT_EMPTY;
		*this = lpszSrc;
	}

#ifndef OLE2ANSI
	CComVariant(LPCSTR lpszSrc)
	{
		vt = VT_EMPTY;
		*this = lpszSrc;
	}
#endif

	CComVariant(bool bSrc)
	{
		vt = VT_BOOL;
#pragma warning(disable: 4310) // cast truncates constant value
		boolVal = bSrc ? VARIANT_TRUE : VARIANT_FALSE;
#pragma warning(default: 4310) // cast truncates constant value
	}

	CComVariant(int nSrc)
	{
		vt = VT_I4;
		lVal = nSrc;
	}
	CComVariant(BYTE nSrc)
	{
		vt = VT_UI1;
		bVal = nSrc;
	}
	CComVariant(short nSrc)
	{
		vt = VT_I2;
		iVal = nSrc;
	}
	CComVariant(long nSrc, VARTYPE vtSrc = VT_I4)
	{
		ATLASSERT(vtSrc == VT_I4 || vtSrc == VT_ERROR);
		vt = vtSrc;
		lVal = nSrc;
	}
	CComVariant(float fltSrc)
	{
		vt = VT_R4;
		fltVal = fltSrc;
	}
	CComVariant(double dblSrc)
	{
		vt = VT_R8;
		dblVal = dblSrc;
	}
	CComVariant(CY cySrc)
	{
		vt = VT_CY;
		cyVal.Hi = cySrc.Hi;
		cyVal.Lo = cySrc.Lo;
	}
	CComVariant(IDispatch* pSrc)
	{
		vt = VT_DISPATCH;
		pdispVal = pSrc;
		// Need to AddRef as VariantClear will Release
		if (pdispVal != NULL)
			pdispVal->AddRef();
	}
	CComVariant(IUnknown* pSrc)
	{
		vt = VT_UNKNOWN;
		punkVal = pSrc;
		// Need to AddRef as VariantClear will Release
		if (punkVal != NULL)
			punkVal->AddRef();
	}

// Assignment Operators
public:
	CComVariant& operator=(const CComVariant& varSrc)
	{
		InternalCopy(&varSrc);
		return *this;
	}
	CComVariant& operator=(const VARIANT& varSrc)
	{
		InternalCopy(&varSrc);
		return *this;
	}

	CComVariant& operator=(BSTR bstrSrc)
	{
		InternalClear();
		vt = VT_BSTR;
		bstrVal = ::SysAllocString(bstrSrc);
		if (bstrVal == NULL && bstrSrc != NULL)
		{
			vt = VT_ERROR;
			scode = E_OUTOFMEMORY;
		}
		return *this;
	}

	CComVariant& operator=(LPCOLESTR lpszSrc)
	{
		InternalClear();
		vt = VT_BSTR;
		bstrVal = ::SysAllocString(lpszSrc);

		if (bstrVal == NULL && lpszSrc != NULL)
		{
			vt = VT_ERROR;
			scode = E_OUTOFMEMORY;
		}
		return *this;
	}

	#ifndef OLE2ANSI
	CComVariant& operator=(LPCSTR lpszSrc)
	{
		USES_CONVERSION;
		InternalClear();
		vt = VT_BSTR;
		bstrVal = ::SysAllocString(A2COLE(lpszSrc));

		if (bstrVal == NULL && lpszSrc != NULL)
		{
			vt = VT_ERROR;
			scode = E_OUTOFMEMORY;
		}
		return *this;
	}
	#endif

	CComVariant& operator=(bool bSrc)
	{
		if (vt != VT_BOOL)
		{
			InternalClear();
			vt = VT_BOOL;
		}
	#pragma warning(disable: 4310) // cast truncates constant value
		boolVal = bSrc ? VARIANT_TRUE : VARIANT_FALSE;
	#pragma warning(default: 4310) // cast truncates constant value
		return *this;
	}

	CComVariant& operator=(int nSrc)
	{
		if (vt != VT_I4)
		{
			InternalClear();
			vt = VT_I4;
		}
		lVal = nSrc;

		return *this;
	}

	CComVariant& operator=(BYTE nSrc)
	{
		if (vt != VT_UI1)
		{
			InternalClear();
			vt = VT_UI1;
		}
		bVal = nSrc;
		return *this;
	}

	CComVariant& operator=(short nSrc)
	{
		if (vt != VT_I2)
		{
			InternalClear();
			vt = VT_I2;
		}
		iVal = nSrc;
		return *this;
	}

	CComVariant& operator=(long nSrc)
	{
		if (vt != VT_I4)
		{
			InternalClear();
			vt = VT_I4;
		}
		lVal = nSrc;
		return *this;
	}

	CComVariant& operator=(float fltSrc)
	{
		if (vt != VT_R4)
		{
			InternalClear();
			vt = VT_R4;
		}
		fltVal = fltSrc;
		return *this;
	}

	CComVariant& operator=(double dblSrc)
	{
		if (vt != VT_R8)
		{
			InternalClear();
			vt = VT_R8;
		}
		dblVal = dblSrc;
		return *this;
	}

	CComVariant& operator=(CY cySrc)
	{
		if (vt != VT_CY)
		{
			InternalClear();
			vt = VT_CY;
		}
		cyVal.Hi = cySrc.Hi;
		cyVal.Lo = cySrc.Lo;
		return *this;
	}

	CComVariant& operator=(IDispatch* pSrc)
	{
		InternalClear();
		vt = VT_DISPATCH;
		pdispVal = pSrc;
		// Need to AddRef as VariantClear will Release
		if (pdispVal != NULL)
			pdispVal->AddRef();
		return *this;
	}

	CComVariant& operator=(IUnknown* pSrc)
	{
		InternalClear();
		vt = VT_UNKNOWN;
		punkVal = pSrc;

		// Need to AddRef as VariantClear will Release
		if (punkVal != NULL)
			punkVal->AddRef();
		return *this;
	}


// Comparison Operators
public:
	bool operator==(const VARIANT& varSrc) const
	{
		if (this == &varSrc)
			return true;

		// Variants not equal if types don't match
		if (vt != varSrc.vt)
			return false;

		// Check type specific values
		switch (vt)
		{
			case VT_EMPTY:
			case VT_NULL:
				return true;

			case VT_BOOL:
				return boolVal == varSrc.boolVal;

			case VT_UI1:
				return bVal == varSrc.bVal;

			case VT_I2:
				return iVal == varSrc.iVal;

			case VT_I4:
				return lVal == varSrc.lVal;

			case VT_R4:
				return fltVal == varSrc.fltVal;

			case VT_R8:
				return dblVal == varSrc.dblVal;

			case VT_BSTR:
				return (::SysStringByteLen(bstrVal) == ::SysStringByteLen(varSrc.bstrVal)) &&
						(::memcmp(bstrVal, varSrc.bstrVal, ::SysStringByteLen(bstrVal)) == 0);

			case VT_ERROR:
				return scode == varSrc.scode;

			case VT_DISPATCH:
				return pdispVal == varSrc.pdispVal;

			case VT_UNKNOWN:
				return punkVal == varSrc.punkVal;

			default:
				ATLASSERT(false);
				// fall through
		}

		return false;
	}
	bool operator!=(const VARIANT& varSrc) const {return !operator==(varSrc);}
	bool operator<(const VARIANT& varSrc) const {return VarCmp((VARIANT*)this, (VARIANT*)&varSrc, LOCALE_USER_DEFAULT, 0)==VARCMP_LT;}
	bool operator>(const VARIANT& varSrc) const {return VarCmp((VARIANT*)this, (VARIANT*)&varSrc, LOCALE_USER_DEFAULT, 0)==VARCMP_GT;}

// Operations
public:
	HRESULT Clear() { return ::VariantClear(this); }
	HRESULT Copy(const VARIANT* pSrc) { return ::VariantCopy(this, const_cast<VARIANT*>(pSrc)); }
	HRESULT Attach(VARIANT* pSrc)
	{
		// Clear out the variant
		HRESULT hr = Clear();
		if (!FAILED(hr))
		{
			// Copy the contents and give control to CComVariant
			memcpy(this, pSrc, sizeof(VARIANT));
			pSrc->vt = VT_EMPTY;
			hr = S_OK;
		}
		return hr;
	}

	HRESULT Detach(VARIANT* pDest)
	{
		// Clear out the variant
		HRESULT hr = ::VariantClear(pDest);
		if (!FAILED(hr))
		{
			// Copy the contents and remove control from CComVariant
			memcpy(pDest, this, sizeof(VARIANT));
			vt = VT_EMPTY;
			hr = S_OK;
		}
		return hr;
	}

	HRESULT ChangeType(VARTYPE vtNew, const VARIANT* pSrc = NULL)
	{
		VARIANT* pVar = const_cast<VARIANT*>(pSrc);
		// Convert in place if pSrc is NULL
		if (pVar == NULL)
			pVar = this;
		// Do nothing if doing in place convert and vts not different
		return ::VariantChangeType(this, pVar, 0, vtNew);
	}

	HRESULT WriteToStream(IStream* pStream);
	HRESULT ReadFromStream(IStream* pStream);

// Implementation
public:
	HRESULT InternalClear()
	{
		HRESULT hr = Clear();
		ATLASSERT(SUCCEEDED(hr));
		if (FAILED(hr))
		{
			vt = VT_ERROR;
			scode = hr;
		}
		return hr;
	}

	void InternalCopy(const VARIANT* pSrc)
	{
		HRESULT hr = Copy(pSrc);
		if (FAILED(hr))
		{
			vt = VT_ERROR;
			scode = hr;
		}
	}
};

inline HRESULT CComVariant::WriteToStream(IStream* pStream)
{
	HRESULT hr = pStream->Write(&vt, sizeof(VARTYPE), NULL);
	if (FAILED(hr))
		return hr;

	int cbWrite = 0;
	switch (vt)
	{
	case VT_UNKNOWN:
	case VT_DISPATCH:
		{
			CComPtr<IPersistStream> spStream;
			if (punkVal != NULL)
			{
				hr = punkVal->QueryInterface(IID_IPersistStream, (void**)&spStream);
				if (FAILED(hr))
					return hr;
			}
			if (spStream != NULL)
				return OleSaveToStream(spStream, pStream);
			else
				return WriteClassStm(pStream, CLSID_NULL);
		}
	case VT_UI1:
	case VT_I1:
		cbWrite = sizeof(BYTE);
		break;
	case VT_I2:
	case VT_UI2:
	case VT_BOOL:
		cbWrite = sizeof(short);
		break;
	case VT_I4:
	case VT_UI4:
	case VT_R4:
	case VT_INT:
	case VT_UINT:
	case VT_ERROR:
		cbWrite = sizeof(long);
		break;
	case VT_R8:
	case VT_CY:
	case VT_DATE:
		cbWrite = sizeof(double);
		break;
	default:
		break;
	}
	if (cbWrite != 0)
		return pStream->Write((void*) &bVal, cbWrite, NULL);

	CComBSTR bstrWrite;
	CComVariant varBSTR;
	if (vt != VT_BSTR)
	{
		hr = VariantChangeType(&varBSTR, this, VARIANT_NOVALUEPROP, VT_BSTR);
		if (FAILED(hr))
			return hr;
		bstrWrite = varBSTR.bstrVal;
	}
	else
		bstrWrite = bstrVal;

	return bstrWrite.WriteToStream(pStream);
}

inline HRESULT CComVariant::ReadFromStream(IStream* pStream)
{
	ATLASSERT(pStream != NULL);
	HRESULT hr;
	hr = VariantClear(this);
	if (FAILED(hr))
		return hr;
	VARTYPE vtRead;
	hr = pStream->Read(&vtRead, sizeof(VARTYPE), NULL);
	if (hr == S_FALSE)
		hr = E_FAIL;
	if (FAILED(hr))
		return hr;

	vt = vtRead;
	int cbRead = 0;
	switch (vtRead)
	{
	case VT_UNKNOWN:
	case VT_DISPATCH:
		{
			punkVal = NULL;
			hr = OleLoadFromStream(pStream,
				(vtRead == VT_UNKNOWN) ? IID_IUnknown : IID_IDispatch,
				(void**)&punkVal);
			if (hr == REGDB_E_CLASSNOTREG)
				hr = S_OK;
			return S_OK;
		}
	case VT_UI1:
	case VT_I1:
		cbRead = sizeof(BYTE);
		break;
	case VT_I2:
	case VT_UI2:
	case VT_BOOL:
		cbRead = sizeof(short);
		break;
	case VT_I4:
	case VT_UI4:
	case VT_R4:
	case VT_INT:
	case VT_UINT:
	case VT_ERROR:
		cbRead = sizeof(long);
		break;
	case VT_R8:
	case VT_CY:
	case VT_DATE:
		cbRead = sizeof(double);
		break;
	default:
		break;
	}
	if (cbRead != 0)
	{
		hr = pStream->Read((void*) &bVal, cbRead, NULL);
		if (hr == S_FALSE)
			hr = E_FAIL;
		return hr;
	}
	CComBSTR bstrRead;

	hr = bstrRead.ReadFromStream(pStream);
	if (FAILED(hr))
		return hr;
	vt = VT_BSTR;
	bstrVal = bstrRead.Detach();
	if (vtRead != VT_BSTR)
		hr = ChangeType(vtRead);
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// CRegKey

class CRegKey
{
public:
	CRegKey();
	~CRegKey();

// Attributes
public:
	operator HKEY() const;
	HKEY m_hKey;

// Operations
public:
	LONG SetValue(DWORD dwValue, LPCTSTR lpszValueName);
	LONG QueryValue(DWORD& dwValue, LPCTSTR lpszValueName);
	LONG QueryValue(LPTSTR szValue, LPCTSTR lpszValueName, DWORD* pdwCount);
	LONG SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);

	LONG SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);
	static LONG WINAPI SetValue(HKEY hKeyParent, LPCTSTR lpszKeyName,
		LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);

	LONG Create(HKEY hKeyParent, LPCTSTR lpszKeyName,
		LPTSTR lpszClass = REG_NONE, DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		REGSAM samDesired = KEY_ALL_ACCESS,
		LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
		LPDWORD lpdwDisposition = NULL);
	LONG Open(HKEY hKeyParent, LPCTSTR lpszKeyName,
		REGSAM samDesired = KEY_ALL_ACCESS);
	LONG Close();
	HKEY Detach();
	void Attach(HKEY hKey);
	LONG DeleteSubKey(LPCTSTR lpszSubKey);
	LONG RecurseDeleteKey(LPCTSTR lpszKey);
	LONG DeleteValue(LPCTSTR lpszValue);
};

inline CRegKey::CRegKey()
{m_hKey = NULL;}

inline CRegKey::~CRegKey()
{Close();}

inline CRegKey::operator HKEY() const
{return m_hKey;}

inline HKEY CRegKey::Detach()
{
	HKEY hKey = m_hKey;
	m_hKey = NULL;
	return hKey;
}

inline void CRegKey::Attach(HKEY hKey)
{
	ATLASSERT(m_hKey == NULL);
	m_hKey = hKey;
}

inline LONG CRegKey::DeleteSubKey(LPCTSTR lpszSubKey)
{
	ATLASSERT(m_hKey != NULL);
	return RegDeleteKey(m_hKey, lpszSubKey);
}

inline LONG CRegKey::DeleteValue(LPCTSTR lpszValue)
{
	ATLASSERT(m_hKey != NULL);
	return RegDeleteValue(m_hKey, (LPTSTR)lpszValue);
}

inline LONG CRegKey::Close()
{
	LONG lRes = ERROR_SUCCESS;
	if (m_hKey != NULL)
	{
		lRes = RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
	return lRes;
}

inline LONG CRegKey::Create(HKEY hKeyParent, LPCTSTR lpszKeyName,
	LPTSTR lpszClass, DWORD dwOptions, REGSAM samDesired,
	LPSECURITY_ATTRIBUTES lpSecAttr, LPDWORD lpdwDisposition)
{
	ATLASSERT(hKeyParent != NULL);
	DWORD dw;
	HKEY hKey = NULL;
	LONG lRes = RegCreateKeyEx(hKeyParent, lpszKeyName, 0,
		lpszClass, dwOptions, samDesired, lpSecAttr, &hKey, &dw);
	if (lpdwDisposition != NULL)
		*lpdwDisposition = dw;
	if (lRes == ERROR_SUCCESS)
	{
		lRes = Close();
		m_hKey = hKey;
	}
	return lRes;
}

inline LONG CRegKey::Open(HKEY hKeyParent, LPCTSTR lpszKeyName, REGSAM samDesired)
{
	ATLASSERT(hKeyParent != NULL);
	HKEY hKey = NULL;
	LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyName, 0, samDesired, &hKey);
	if (lRes == ERROR_SUCCESS)
	{
		lRes = Close();
		ATLASSERT(lRes == ERROR_SUCCESS);
		m_hKey = hKey;
	}
	return lRes;
}

inline LONG CRegKey::QueryValue(DWORD& dwValue, LPCTSTR lpszValueName)
{
	DWORD dwType = 0;
	DWORD dwCount = sizeof(DWORD);
	LONG lRes = RegQueryValueEx(m_hKey, (LPTSTR)lpszValueName, NULL, &dwType,
		(LPBYTE)&dwValue, &dwCount);
	ATLASSERT((lRes!=ERROR_SUCCESS) || (dwType == REG_DWORD));
	ATLASSERT((lRes!=ERROR_SUCCESS) || (dwCount == sizeof(DWORD)));
	return lRes;
}

inline LONG CRegKey::QueryValue(LPTSTR szValue, LPCTSTR lpszValueName, DWORD* pdwCount)
{
	ATLASSERT(pdwCount != NULL);
	DWORD dwType = 0;
	LONG lRes = RegQueryValueEx(m_hKey, (LPTSTR)lpszValueName, NULL, &dwType,
		(LPBYTE)szValue, pdwCount);
	ATLASSERT((lRes!=ERROR_SUCCESS) || (dwType == REG_SZ) ||
			 (dwType == REG_MULTI_SZ) || (dwType == REG_EXPAND_SZ));
	return lRes;
}

inline LONG WINAPI CRegKey::SetValue(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
	ATLASSERT(lpszValue != NULL);
	CRegKey key;
	LONG lRes = key.Create(hKeyParent, lpszKeyName);
	if (lRes == ERROR_SUCCESS)
		lRes = key.SetValue(lpszValue, lpszValueName);
	return lRes;
}

inline LONG CRegKey::SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
	ATLASSERT(lpszValue != NULL);
	CRegKey key;
	LONG lRes = key.Create(m_hKey, lpszKeyName);
	if (lRes == ERROR_SUCCESS)
		lRes = key.SetValue(lpszValue, lpszValueName);
	return lRes;
}

inline LONG CRegKey::SetValue(DWORD dwValue, LPCTSTR lpszValueName)
{
	ATLASSERT(m_hKey != NULL);
	return RegSetValueEx(m_hKey, lpszValueName, 0, REG_DWORD,
		(BYTE * const)&dwValue, sizeof(DWORD));
}

inline LONG CRegKey::SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
	ATLASSERT(lpszValue != NULL);
	ATLASSERT(m_hKey != NULL);
	return RegSetValueEx(m_hKey, lpszValueName, 0, REG_SZ,
		(BYTE * const)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));
}

inline LONG CRegKey::RecurseDeleteKey(LPCTSTR lpszKey)
{
	CRegKey key;
	LONG lRes = key.Open(m_hKey, lpszKey, KEY_READ | KEY_WRITE);
	if (lRes != ERROR_SUCCESS)
		return lRes;
	FILETIME time;
	DWORD dwSize = 256;
	TCHAR szBuffer[256];
	while (RegEnumKeyEx(key.m_hKey, 0, szBuffer, &dwSize, NULL, NULL, NULL,
		&time)==ERROR_SUCCESS)
	{
		lRes = key.RecurseDeleteKey(szBuffer);
		if (lRes != ERROR_SUCCESS)
			return lRes;
		dwSize = 256;
	}
	key.Close();
	return DeleteSubKey(lpszKey);
}

ATLINLINE ATLAPI_(void*) AtlModuleExtractCreateWndData(_ATL_MODULE* pM)
{
        void* pv = NULL;
        ::EnterCriticalSection(&pM->m_csWindowCreate);
        _AtlCreateWndData* pEntry = pM->m_pCreateWndList;
        if(pEntry != NULL)
        {
                DWORD dwThreadID = ::GetCurrentThreadId();
                _AtlCreateWndData* pPrev = NULL;
                while(pEntry != NULL)
                {
                        if(pEntry->m_dwThreadID == dwThreadID)
                        {
                                if(pPrev == NULL)
                                        pM->m_pCreateWndList = pEntry->m_pNext;
                                else
                                        pPrev->m_pNext = pEntry->m_pNext;
                                pv = pEntry->m_pThis;
                                break;
                        }
                        pPrev = pEntry;
                        pEntry = pEntry->m_pNext;
                }
        }
        ::LeaveCriticalSection(&pM->m_csWindowCreate);
        return pv;
}

ATLINLINE ATLAPI_(void) AtlModuleAddCreateWndData(_ATL_MODULE* pM, _AtlCreateWndData* pData, void* pObject)
{
        //if(pData == NULL || pObject == NULL)
        //        _AtlRaiseException((DWORD)EXCEPTION_ACCESS_VIOLATION);

		ATLASSERT(pData != NULL && pObject != NULL);
        pData->m_pThis = pObject;
        pData->m_dwThreadID = ::GetCurrentThreadId();
        ::EnterCriticalSection(&pM->m_csWindowCreate);
        pData->m_pNext = pM->m_pCreateWndList;
        pM->m_pCreateWndList = pData;
        ::LeaveCriticalSection(&pM->m_csWindowCreate);
}

// WM_FORWARDMSG - used to forward a message to another window for processing
// WPARAM - DWORD dwUserData - defined by user
// LPARAM - LPMSG pMsg - a pointer to the MSG structure
// return value - 0 if the message was not processed, nonzero if it was
#define WM_FORWARDMSG       0x037F

// General DLL Version Helpers

inline HRESULT AtlGetDllVersion(HINSTANCE hInstDLL, DLLVERSIONINFO* pDllVersionInfo)
{
        ATLASSERT(pDllVersionInfo != NULL);
        if(pDllVersionInfo == NULL)
                return E_INVALIDARG;

        // We must get this function explicitly because some DLLs don't implement it.
        DLLGETVERSIONPROC pfnDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hInstDLL, "DllGetVersion");
        if(pfnDllGetVersion == NULL)
                return E_NOTIMPL;

        return (*pfnDllGetVersion)(pDllVersionInfo);
}

inline HRESULT AtlGetDllVersion(LPCTSTR lpstrDllName, DLLVERSIONINFO* pDllVersionInfo)
{
        HINSTANCE hInstDLL = ::LoadLibrary(lpstrDllName);
        if(hInstDLL == NULL)
                return E_FAIL;
        HRESULT hRet = AtlGetDllVersion(hInstDLL, pDllVersionInfo);
        ::FreeLibrary(hInstDLL);
        return hRet;
}

// Common Control Versions:
//   Win95/WinNT 4.0    maj=4 min=00
//   IE 3.x     maj=4 min=70
//   IE 4.0     maj=4 min=71
inline HRESULT AtlGetCommCtrlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
{
        ATLASSERT(pdwMajor != NULL && pdwMinor != NULL);
        if(pdwMajor == NULL || pdwMinor == NULL)
                return E_INVALIDARG;

        DLLVERSIONINFO dvi;
        ::ZeroMemory(&dvi, sizeof(dvi));
        dvi.cbSize = sizeof(dvi);
        HRESULT hRet = AtlGetDllVersion(_T("comctl32.dll"), &dvi);

        if(SUCCEEDED(hRet))
        {
                *pdwMajor = dvi.dwMajorVersion;
                *pdwMinor = dvi.dwMinorVersion;
        }
        else if(hRet == E_NOTIMPL)
        {
                // If DllGetVersion is not there, then the DLL is a version
                // previous to the one shipped with IE 3.x
                *pdwMajor = 4;
                *pdwMinor = 0;
                hRet = S_OK;
        }

        return hRet;
}

// Shell Versions:
//   Win95/WinNT 4.0                    maj=4 min=00
//   IE 3.x, IE 4.0 without Web Integrated Desktop  maj=4 min=00
//   IE 4.0 with Web Integrated Desktop         maj=4 min=71
//   IE 4.01 with Web Integrated Desktop        maj=4 min=72
inline HRESULT AtlGetShellVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
{
        ATLASSERT(pdwMajor != NULL && pdwMinor != NULL);
        if(pdwMajor == NULL || pdwMinor == NULL)
                return E_INVALIDARG;

        DLLVERSIONINFO dvi;
        ::ZeroMemory(&dvi, sizeof(dvi));
        dvi.cbSize = sizeof(dvi);
        HRESULT hRet = AtlGetDllVersion(_T("shell32.dll"), &dvi);

        if(SUCCEEDED(hRet))
        {
                *pdwMajor = dvi.dwMajorVersion;
                *pdwMinor = dvi.dwMinorVersion;
        }
        else if(hRet == E_NOTIMPL)
        {
                // If DllGetVersion is not there, then the DLL is a version
                // previous to the one shipped with IE 4.x
                *pdwMajor = 4;
                *pdwMinor = 0;
                hRet = S_OK;
        }

        return hRet;
}

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif

} // namespace ATL

// -------------------------------------------------------------------------

//using ATL::_ATL_OBJMAP_ENTRY;
//using ATL::CComModule;
//using ATL::CComVariant;
//using ATL::CComPtr;
using namespace ATL;
/////////////////////////////////////////////////////////////////////////////

#endif /* __ATLBASE_H__ */
