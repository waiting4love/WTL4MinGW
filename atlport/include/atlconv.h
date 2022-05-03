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
// Module: atlconv.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-15 21:12:25
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __ATLCONV_H__
#define __ATLCONV_H__

#ifndef __STDEXT_BASIC_H__
#include "../../stdext/include/stdext/Basic.h"
#endif

// -------------------------------------------------------------------------

#define USES_CONVERSION		WINX_USES_CONVERSION

#define A2W					WINX_A2W
#define W2A					WINX_W2A

#define A2CW				WINX_A2CW
#define W2CA				WINX_W2CA

#define A2T					WINX_A2T
#define W2T					WINX_W2T
#define T2A					WINX_T2A
#define T2W					WINX_T2W

#define A2CT				WINX_A2CT
#define W2CT				WINX_W2CT
#define T2CA				WINX_T2CA
#define T2CW				WINX_T2CW

#define A2COLE				WINX_A2CW
#define A2OLE				WINX_A2W

#define T2OLE				WINX_T2W
#define T2COLE				WINX_T2CW

#define OLE2T				WINX_W2T
#define OLE2CT				WINX_W2CT

// -------------------------------------------------------------------------

inline BSTR A2WBSTR(LPCSTR lp, int nLen = -1)
{
	USES_CONVERSION;
	BSTR str = NULL;
	int nConvertedLen = MultiByteToWideChar(0, 0, lp,
		nLen, NULL, 0)-1;
	str = ::SysAllocStringLen(NULL, nConvertedLen);
	if (str != NULL)
	{
		MultiByteToWideChar(0, 0, lp, -1,
			str, nConvertedLen);
	}
	return str;
}

inline BSTR OLE2BSTR(LPCOLESTR lp) {return ::SysAllocString(lp);}
#if defined(_UNICODE)
// in these cases the default (TCHAR) is the same as OLECHAR
	inline BSTR T2BSTR(LPCTSTR lp) {return ::SysAllocString(lp);}
	inline BSTR A2BSTR(LPCSTR lp) {USES_CONVERSION; return A2WBSTR(lp);}
	inline BSTR W2BSTR(LPCWSTR lp) {return ::SysAllocString(lp);}
#elif defined(OLE2ANSI)
// in these cases the default (TCHAR) is the same as OLECHAR
	inline BSTR T2BSTR(LPCTSTR lp) {return ::SysAllocString(lp);}
	inline BSTR A2BSTR(LPCSTR lp) {return ::SysAllocString(lp);}
	inline BSTR W2BSTR(LPCWSTR lp) {USES_CONVERSION; return ::SysAllocString(W2COLE(lp));}
#else
	inline BSTR T2BSTR(LPCTSTR lp) {USES_CONVERSION; return A2WBSTR(lp);}
	inline BSTR A2BSTR(LPCSTR lp) {USES_CONVERSION; return A2WBSTR(lp);}
	inline BSTR W2BSTR(LPCWSTR lp) {return ::SysAllocString(lp);}
#endif

// -------------------------------------------------------------------------
// Global UNICODE<>ANSI translation helpers

inline LPWSTR WINAPI AtlA2WHelper(LPWSTR lpw, LPCSTR lpa, int nChars, UINT acp)
{
	ATLASSERT(lpa != NULL);
	ATLASSERT(lpw != NULL);
	// verify that no illegal character present
	// since lpw was allocated based on the size of lpa
	// don't worry about the number of chars
	lpw[0] = '\0';
	MultiByteToWideChar(acp, 0, lpa, -1, lpw, nChars);
	return lpw;
}

inline LPSTR WINAPI AtlW2AHelper(LPSTR lpa, LPCWSTR lpw, int nChars, UINT acp)
{
	ATLASSERT(lpw != NULL);
	ATLASSERT(lpa != NULL);
	// verify that no illegal character present
	// since lpa was allocated based on the size of lpw
	// don't worry about the number of chars
	lpa[0] = '\0';
	WideCharToMultiByte(acp, 0, lpw, -1, lpa, nChars, NULL, NULL);
	return lpa;
}
inline LPWSTR WINAPI AtlA2WHelper(LPWSTR lpw, LPCSTR lpa, int nChars)
{
	return AtlA2WHelper(lpw, lpa, nChars, CP_ACP);
}

inline LPSTR WINAPI AtlW2AHelper(LPSTR lpa, LPCWSTR lpw, int nChars)
{
	return AtlW2AHelper(lpa, lpw, nChars, CP_ACP);
}

// -------------------------------------------------------------------------

#ifdef _WINGDI_
/////////////////////////////////////////////////////////////////////////////
// Global UNICODE<>ANSI translation helpers
inline LPDEVMODEW AtlDevModeA2W(LPDEVMODEW lpDevModeW, LPDEVMODEA lpDevModeA)
{
	USES_CONVERSION;
	if (lpDevModeA == NULL)
		return NULL;
	ATLASSERT(lpDevModeW != NULL);
	AtlA2WHelper(lpDevModeW->dmDeviceName, (LPCSTR)lpDevModeA->dmDeviceName, 32*sizeof(WCHAR));
	memcpy(&lpDevModeW->dmSpecVersion, &lpDevModeA->dmSpecVersion,
		offsetof(DEVMODEW, dmFormName) - offsetof(DEVMODEW, dmSpecVersion));
	AtlA2WHelper(lpDevModeW->dmFormName, (LPCSTR)lpDevModeA->dmFormName, 32*sizeof(WCHAR));
	memcpy(&lpDevModeW->dmLogPixels, &lpDevModeA->dmLogPixels,
		sizeof(DEVMODEW) - offsetof(DEVMODEW, dmLogPixels));
	if (lpDevModeA->dmDriverExtra != 0)
		memcpy(lpDevModeW+1, lpDevModeA+1, lpDevModeA->dmDriverExtra);
	lpDevModeW->dmSize = sizeof(DEVMODEW);
	return lpDevModeW;
}

inline LPTEXTMETRICW AtlTextMetricA2W(LPTEXTMETRICW lptmW, LPTEXTMETRICA lptmA)
{
	USES_CONVERSION;
	if (lptmA == NULL)
		return NULL;
	ATLASSERT(lptmW != NULL);
	memcpy(lptmW, lptmA, sizeof(LONG) * 11);
	memcpy(&lptmW->tmItalic, &lptmA->tmItalic, sizeof(BYTE) * 5);
	MultiByteToWideChar(0, 0, (LPCSTR)&lptmA->tmFirstChar, 1, &lptmW->tmFirstChar, 1);
	MultiByteToWideChar(0, 0, (LPCSTR)&lptmA->tmLastChar, 1, &lptmW->tmLastChar, 1);
	MultiByteToWideChar(0, 0, (LPCSTR)&lptmA->tmDefaultChar, 1, &lptmW->tmDefaultChar, 1);
	MultiByteToWideChar(0, 0, (LPCSTR)&lptmA->tmBreakChar, 1, &lptmW->tmBreakChar, 1);
	return lptmW;
}

inline LPTEXTMETRICA AtlTextMetricW2A(LPTEXTMETRICA lptmA, LPTEXTMETRICW lptmW)
{
	USES_CONVERSION;
	if (lptmW == NULL)
		return NULL;
	ATLASSERT(lptmA != NULL);
	memcpy(lptmA, lptmW, sizeof(LONG) * 11);
	memcpy(&lptmA->tmItalic, &lptmW->tmItalic, sizeof(BYTE) * 5);
	WideCharToMultiByte(0, 0, &lptmW->tmFirstChar, 1, (LPSTR)&lptmA->tmFirstChar, 1, NULL, NULL);
	WideCharToMultiByte(0, 0, &lptmW->tmLastChar, 1, (LPSTR)&lptmA->tmLastChar, 1, NULL, NULL);
	WideCharToMultiByte(0, 0, &lptmW->tmDefaultChar, 1, (LPSTR)&lptmA->tmDefaultChar, 1, NULL, NULL);
	WideCharToMultiByte(0, 0, &lptmW->tmBreakChar, 1, (LPSTR)&lptmA->tmBreakChar, 1, NULL, NULL);
	return lptmA;
}

#ifndef ATLDEVMODEA2W
#define ATLDEVMODEA2W AtlDevModeA2W
#define ATLDEVMODEW2A AtlDevModeW2A
#define ATLTEXTMETRICA2W AtlTextMetricA2W
#define ATLTEXTMETRICW2A AtlTextMetricW2A
#endif

#define DEVMODEW2A(lpw)\
	((lpw == NULL) ? NULL : ATLDEVMODEW2A((LPDEVMODEA)alloca(sizeof(DEVMODEA)+lpw->dmDriverExtra), lpw))
#define DEVMODEA2W(lpa)\
	((lpa == NULL) ? NULL : ATLDEVMODEA2W((LPDEVMODEW)alloca(sizeof(DEVMODEW)+lpa->dmDriverExtra), lpa))
#define TEXTMETRICW2A(lptmw)\
	((lptmw == NULL) ? NULL : ATLTEXTMETRICW2A((LPTEXTMETRICA)alloca(sizeof(TEXTMETRICA)), lptmw))
#define TEXTMETRICA2W(lptma)\
	((lptma == NULL) ? NULL : ATLTEXTMETRICA2W((LPTEXTMETRICW)alloca(sizeof(TEXTMETRICW)), lptma))

#ifdef OLE2ANSI
	#define DEVMODEOLE DEVMODEA
	#define LPDEVMODEOLE LPDEVMODEA
	#define TEXTMETRICOLE TEXTMETRICA
	#define LPTEXTMETRICOLE LPTEXTMETRICA
#else
	#define DEVMODEOLE DEVMODEW
	#define LPDEVMODEOLE LPDEVMODEW
	#define TEXTMETRICOLE TEXTMETRICW
	#define LPTEXTMETRICOLE LPTEXTMETRICW
#endif

#if defined(_UNICODE)
// in these cases the default (TCHAR) is the same as OLECHAR
	inline LPDEVMODEW DEVMODEOLE2T(LPDEVMODEOLE lp) { return lp; }
	inline LPDEVMODEOLE DEVMODET2OLE(LPDEVMODEW lp) { return lp; }
	inline LPTEXTMETRICW TEXTMETRICOLE2T(LPTEXTMETRICOLE lp) { return lp; }
	inline LPTEXTMETRICOLE TEXTMETRICT2OLE(LPTEXTMETRICW lp) { return lp; }
#elif defined(OLE2ANSI)
// in these cases the default (TCHAR) is the same as OLECHAR
	inline LPDEVMODE DEVMODEOLE2T(LPDEVMODEOLE lp) { return lp; }
	inline LPDEVMODEOLE DEVMODET2OLE(LPDEVMODE lp) { return lp; }
	inline LPTEXTMETRIC TEXTMETRICOLE2T(LPTEXTMETRICOLE lp) { return lp; }
	inline LPTEXTMETRICOLE TEXTMETRICT2OLE(LPTEXTMETRIC lp) { return lp; }
#else
	#define DEVMODEOLE2T(lpo) DEVMODEW2A(lpo)
	#define DEVMODET2OLE(lpa) DEVMODEA2W(lpa)
	#define TEXTMETRICOLE2T(lptmw) TEXTMETRICW2A(lptmw)
	#define TEXTMETRICT2OLE(lptma) TEXTMETRICA2W(lptma)
#endif

#endif //_WINGDI_

#ifndef _ATL_DLL_IMPL
#ifndef _ATL_DLL
#define _ATLCONV_IMPL
#endif
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef _ATLCONV_IMPL

#ifdef _WINGDI_

ATLINLINE ATLAPI_(LPDEVMODEA) AtlDevModeW2A(LPDEVMODEA lpDevModeA, LPDEVMODEW lpDevModeW)
{
	USES_CONVERSION;
	if (lpDevModeW == NULL)
		return NULL;
	ATLASSERT(lpDevModeA != NULL);
	AtlW2AHelper((LPSTR)lpDevModeA->dmDeviceName, lpDevModeW->dmDeviceName, 32*sizeof(char));
	memcpy(&lpDevModeA->dmSpecVersion, &lpDevModeW->dmSpecVersion,
		offsetof(DEVMODEA, dmFormName) - offsetof(DEVMODEA, dmSpecVersion));
	AtlW2AHelper((LPSTR)lpDevModeA->dmFormName, lpDevModeW->dmFormName, 32*sizeof(char));
	memcpy(&lpDevModeA->dmLogPixels, &lpDevModeW->dmLogPixels,
		sizeof(DEVMODEA) - offsetof(DEVMODEA, dmLogPixels));
	if (lpDevModeW->dmDriverExtra != 0)
		memcpy(lpDevModeA+1, lpDevModeW+1, lpDevModeW->dmDriverExtra);
	lpDevModeA->dmSize = sizeof(DEVMODEA);
	return lpDevModeA;
}

#endif //_WINGDI

//Prevent pulling in second time
#undef _ATLCONV_IMPL

#endif // _ATLCONV_IMPL

// -------------------------------------------------------------------------
// $Log: $

#endif /* __ATLCONV_H__ */
