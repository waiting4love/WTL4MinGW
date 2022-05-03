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
// Module: stdext/archive/RegArchive.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-26 0:34:01
// 
// $Id: RegArchive.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_REGARCHIVE_H__
#define __STDEXT_ARCHIVE_REGARCHIVE_H__

#ifndef __STDEXT_STRING_H__
#include "../String.h" // for UniString type
#endif

#ifndef __ATLBASE_H__
#include <atlbase.h>
#endif

#ifndef _WINERROR_
#include <winerror.h>
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// RegWriteString
// --> 千万注意 (对于ATL低版本而言): 
//	由于使用了A2W/W2A，RegReadString/RegWriteString这两个函数不能inline

#define _WINX_REG_STRING_PREFIX	'N'

template <class RegKeyType>
HRESULT winx_call RegWriteString(
	RegKeyType& regKey, UINT index, const UniString& str)
{
	TCHAR szName[16];
	szName[0] = _WINX_REG_STRING_PREFIX;

#if defined(UNICODE)
	_itow(index, szName+1, 10);
	#if (_ATL_VER >= 0x0710)
		return HRESULT_FROM_WIN32(regKey.SetStringValue(str.c_str(), szName));
	#else
		return HRESULT_FROM_WIN32(regKey.SetValue(str.c_str(), szName));
	#endif
#else
	_itoa(index, szName+1, 10);
	#if (_ATL_VER >= 0x0710)
		USES_CONVERSION;
		return HRESULT_FROM_WIN32(regKey.SetStringValue(W2A(str.c_str()), szName));
	#else
		USES_CONVERSION;
		return HRESULT_FROM_WIN32(regKey.SetValue(W2A(str.c_str()), szName));
	#endif
#endif
}

// -------------------------------------------------------------------------
// RegReadString

template <class RegKeyType>
HRESULT winx_call RegReadString(RegKeyType& regKey, UINT index, UniString& str)
{
	TCHAR szName[16];
	szName[0] = _WINX_REG_STRING_PREFIX;

#if defined(UNICODE)
	_itow(index, szName+1, 10);
#else
	_itoa(index, szName+1, 10);
#endif

	DWORD dwCount = 0;
#if (_ATL_VER >= 0x0710)
	LONG lRes = regKey.QueryStringValue(szName, NULL, &dwCount);
#else
	LONG lRes = regKey.QueryValue(NULL, szName, &dwCount);
#endif
	if (lRes != 0)
		return HRESULT_FROM_WIN32(lRes);

	TCHAR* szValue = (TCHAR*)_alloca(dwCount*sizeof(TCHAR));
#if (_ATL_VER >= 0x0710)
	lRes = regKey.QueryStringValue(szName, szValue, &dwCount);
#else
	lRes = regKey.QueryValue(szValue, szName, &dwCount);
#endif
	WINX_ASSERT(lRes == 0);
	if (lRes != 0)
		return HRESULT_FROM_WIN32(lRes);
	
#if defined(UNICODE)
	str.assign(szValue);
	return S_OK;
#else
	USES_CONVERSION;
	str.assign(A2W(szValue));
	return S_OK;
#endif
}

// -------------------------------------------------------------------------
// class RegWriteArchive

class RegWriteArchive
{
private:
	UINT m_nCount;
	CRegKey m_regKey;

public:
	RegWriteArchive() : m_nCount(0) {
	}

	RegWriteArchive(
		HKEY hKeyParent, LPCTSTR lpszKeyName,
		LPTSTR lpszClass = REG_NONE, DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		REGSAM samDesired = KEY_ALL_ACCESS,
		LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
		LPDWORD lpdwDisposition = NULL) : m_nCount(0)
	{
		::RegDeleteKey(hKeyParent, lpszKeyName);
		m_regKey.Create(
			hKeyParent, lpszKeyName, 
			lpszClass, dwOptions, samDesired, lpSecAttr, lpdwDisposition);
	}

	void winx_call close() {
		m_regKey.Close();
	}
	
	HRESULT winx_call open(
		HKEY hKeyParent, LPCTSTR lpszKeyName,
		LPTSTR lpszClass = REG_NONE, DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		REGSAM samDesired = KEY_ALL_ACCESS,
		LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
		LPDWORD lpdwDisposition = NULL)
	{
		if (good())
			return E_ACCESSDENIED;
		m_nCount = 0;
		::RegDeleteKey(hKeyParent, lpszKeyName);
		return HRESULT_FROM_WIN32(
			m_regKey.Create(hKeyParent, lpszKeyName, 
			lpszClass, dwOptions, samDesired, lpSecAttr, lpdwDisposition));
	}

	HRESULT winx_call put(const UniString& str) {
		return RegWriteString(m_regKey, ++m_nCount, str);
	}

	int winx_call good() const {
		return m_regKey != NULL;
	}
};

// -------------------------------------------------------------------------
// class RegReadArchive

class RegReadArchive
{
private:
	UINT m_nCount;
	CRegKey m_regKey;
	
public:
	RegReadArchive() : m_nCount(0) {
	}
	
	RegReadArchive(
		HKEY hKeyParent, LPCTSTR lpszKeyName,
		REGSAM samDesired = KEY_ALL_ACCESS) : m_nCount(0)
	{
		m_regKey.Open(hKeyParent, lpszKeyName, samDesired);
	}
	
	void winx_call close() {
		m_regKey.Close();
	}
	
	HRESULT winx_call open(
		HKEY hKeyParent, LPCTSTR lpszKeyName,
		REGSAM samDesired = KEY_ALL_ACCESS)
	{
		if (good())
			return E_ACCESSDENIED;
		m_nCount = 0;
		return HRESULT_FROM_WIN32(m_regKey.Open(hKeyParent, lpszKeyName));
	}
	
	HRESULT winx_call get(UniString& str) {
		return RegReadString(m_regKey, ++m_nCount, str);
	}
	
	int winx_call good() const {
		return m_regKey != NULL;
	}
};

// -------------------------------------------------------------------------
// $Log: RegArchive.h,v $
// Revision 1.1  2006/10/18 12:13:39  xushiwei
// stdext as independent component
//
// Revision 1.2  2006/08/26 09:12:37  xushiwei
// vs2005 support
//
// Revision 1.1  2006/08/26 03:19:44  xushiwei
// STL-Extension:
//   Archive(SimpleWriteArchive, SimpleReadArchive, MemSeqWriteArchive, RegWriteArchive, RegReadArchive)
//

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_REGARCHIVE_H__ */
