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
// Module: stdext/msvc/atlconv.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-14 14:55:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MSVC_ATLCONV_H__
#define __STDEXT_MSVC_ATLCONV_H__

#ifndef _INC_WCHAR
#include <wchar.h>
#endif

#ifndef _INC_MALLOC
#include <malloc.h>
#endif

// -------------------------------------------------------------------------
// _winx_A2WHelper/_winx_W2AHelper

inline LPWSTR winx_call _winx_A2WHelper(LPWSTR lpw, LPCSTR lpa, int nWideChars)
{
	// verify that no illegal character present
	// since lpw was allocated based on the size of lpa
	// don't worry about the number of chars
	lpw[0] = '\0';
	MultiByteToWideChar(
		CP_ACP, 0, lpa, nWideChars /* nAnsiChar */, lpw, nWideChars);
	return lpw;
}

inline LPSTR __stdcall _winx_W2AHelper(LPSTR lpa, LPCWSTR lpw, int nWideChars)
{
	// verify that no illegal character present
	// since lpa was allocated based on the size of lpw
	// don't worry about the number of chars
	lpa[0] = '\0';
	WideCharToMultiByte(
		CP_ACP, 0, lpw, nWideChars, lpa, nWideChars<<1 /* nAnsiChar */, NULL, NULL);
	return lpa;
}

// -------------------------------------------------------------------------
// WINX_A2W/WINX_W2A

#if defined(_MSC_VER)
#define WINX_USES_CONVERSION												\
	int _convert; _convert; LPCWSTR _lpw; _lpw; LPCSTR _lpa; _lpa
#else
#define WINX_USES_CONVERSION												\
	int _convert; _convert = 0; LPCWSTR _lpw; _lpw = 0; LPCSTR _lpa; _lpa = 0
#endif

#define WINX_A2W(lpa) (\
	((_lpa = lpa) == NULL) ? NULL : (\
		_convert = (strlen(_lpa)+1),\
		_winx_A2WHelper((LPWSTR)_alloca(_convert<<1), _lpa, _convert)))

#define WINX_W2A(lpw) (\
	((_lpw = lpw) == NULL) ? NULL : (\
		_convert = (wcslen(_lpw)+1), \
		_winx_W2AHelper((LPSTR)_alloca(_convert<<1), _lpw, _convert)))

#define WINX_A2CW(lpa) ((LPCWSTR)WINX_A2W(lpa))
#define WINX_W2CA(lpw) ((LPCSTR)WINX_W2A(lpw))

// -------------------------------------------------------------------------
// WINX_T2A/WINX_A2T/WINX_T2W/WINX_W2T
// WINX_T2CA/WINX_A2CT/WINX_T2CW/WINX_W2CT

#ifdef _UNICODE
	#define WINX_T2A W2A
	#define WINX_A2T A2W
	#define WINX_T2CA W2CA
	#define WINX_A2CT A2CW
	inline LPWSTR WINX_T2W(LPTSTR lp) { return lp; }
	inline LPTSTR WINX_W2T(LPWSTR lp) { return lp; }
	inline LPCWSTR WINX_T2CW(LPCTSTR lp) { return lp; }
	inline LPCTSTR WINX_W2CT(LPCWSTR lp) { return lp; }
#else
	#define WINX_T2W A2W
	#define WINX_W2T W2A
	#define WINX_T2CW A2CW
	#define WINX_W2CT W2CA
	inline LPSTR WINX_T2A(LPTSTR lp) { return lp; }
	inline LPTSTR WINX_A2T(LPSTR lp) { return lp; }
	inline LPCSTR WINX_T2CA(LPCTSTR lp) { return lp; }
	inline LPCTSTR WINX_A2CT(LPCSTR lp) { return lp; }
#endif

// -------------------------------------------------------------------------
// $Log: $

#endif /* __STDEXT_MSVC_ATLCONV_H__ */
