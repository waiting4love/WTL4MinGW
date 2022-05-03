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
// Module: stdext/archive/SimpleArchive.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-26 0:45:33
// 
// $Id: SimpleArchive.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_SIMPLEARCHIVE_H__
#define __STDEXT_ARCHIVE_SIMPLEARCHIVE_H__

#ifndef _VECTOR_
#include <vector>
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class SimpleWriteArchive

class SimpleWriteArchive
{
private:
	FILE* m_fp;

public:
	typedef char char_type;
	typedef unsigned size_type;

public:
	SimpleWriteArchive() : m_fp(NULL) {
	}

	SimpleWriteArchive(
		IN LPCSTR szFileA,
		IN UINT nMode = STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE)
	{
		WINX_ASSERT(nMode & STGM_CREATE);
		m_fp = fopen(szFileA, "wb");
	}

	SimpleWriteArchive(
		IN LPCWSTR szFile,
		IN UINT nMode = STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE)
	{
		WINX_ASSERT(nMode & STGM_CREATE);
		WINX_USES_CONVERSION;
		LPCSTR szFileA = WINX_W2A(szFile);
		m_fp = fopen(szFileA, "wb");
	}

	~SimpleWriteArchive() {
		if (m_fp)
			fclose(m_fp);
	}
	
	void winx_call close()
	{
		if (m_fp) {
			fclose(m_fp);
			m_fp = NULL;
		}
	}
	
	HRESULT winx_call open(
		IN LPCSTR szFileA,
		IN UINT nMode = STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE)
	{
		WINX_ASSERT(nMode & STGM_CREATE);
		if (good())
			return E_ACCESSDENIED;
		m_fp = fopen(szFileA, "wb");
		return m_fp ? S_OK : E_INVALIDARG;
	}

	HRESULT winx_call open(
		IN LPCWSTR szFile,
		IN UINT nMode = STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE)
	{
		WINX_ASSERT(nMode & STGM_CREATE);
		WINX_USES_CONVERSION;
		if (good())
			return E_ACCESSDENIED;
		LPCSTR szFileA = WINX_W2A(szFile);
		m_fp = fopen(szFileA, "wb");
		return m_fp ? S_OK : E_INVALIDARG;
	}

	HRESULT winx_call put(const UniString& str)
	{
		typedef UniString::value_type CharType;
		const UINT32 cchStr = str.size();
		put((const char*)&cchStr, sizeof(cchStr));
		put((const char*)str.c_str(), cchStr*sizeof(CharType));
		return S_OK;
	}
	
	size_type winx_call put(const char_type* lpBuf, size_type cch)
	{
		return fwrite(lpBuf, sizeof(char_type), cch, m_fp);
	}

	int winx_call good() const {
		return m_fp != NULL;
	}
	
	void winx_call flush() {
		fflush(m_fp);
	}
};

// -------------------------------------------------------------------------
// class SimpleReadArchive

class SimpleReadArchive
{
private:
	FILE* m_fp;
	
public:
	typedef char char_type;
	typedef unsigned size_type;
	
public:
	SimpleReadArchive() : m_fp(NULL) {
	}
	
	SimpleReadArchive(
		IN LPCSTR szFileA,
		IN UINT nMode = STGM_READ|STGM_SHARE_EXCLUSIVE)
	{
		WINX_ASSERT(!(nMode & STGM_CREATE));
		m_fp = fopen(szFileA, "rb");
	}

	SimpleReadArchive(
		IN LPCWSTR szFile,
		IN UINT nMode = STGM_READ|STGM_SHARE_EXCLUSIVE)
	{
		WINX_ASSERT(!(nMode & STGM_CREATE));
		WINX_USES_CONVERSION;
		LPCSTR szFileA = WINX_W2A(szFile);
		m_fp = fopen(szFileA, "rb");
	}

	~SimpleReadArchive() {
		if (m_fp)
			fclose(m_fp);
	}

	void winx_call close()
	{
		if (m_fp) {
			fclose(m_fp);
			m_fp = NULL;
		}
	}
	
	HRESULT winx_call open(
		IN LPCSTR szFileA,
		IN UINT nMode = STGM_READ|STGM_SHARE_EXCLUSIVE)
	{
		WINX_ASSERT(!(nMode & STGM_CREATE));
		if (good())
			return E_ACCESSDENIED;
		m_fp = fopen(szFileA, "rb");
		return m_fp ? S_OK : E_INVALIDARG;
	}

	HRESULT winx_call open(
		IN LPCWSTR szFile,
		IN UINT nMode = STGM_READ|STGM_SHARE_EXCLUSIVE)
	{
		WINX_ASSERT(!(nMode & STGM_CREATE));
		WINX_USES_CONVERSION;
		if (good())
			return E_ACCESSDENIED;
		LPCSTR szFileA = WINX_W2A(szFile);
		m_fp = fopen(szFileA, "rb");
		return m_fp ? S_OK : E_INVALIDARG;
	}
	
	HRESULT winx_call get(UniString& str)
	{
		UINT32 cchStr;
		if (get((char*)&cchStr, sizeof(cchStr)) != sizeof(cchStr))
			return S_FALSE;

		typedef UniString::value_type CharType;
		UINT32 cbStr = cchStr*sizeof(CharType);
		str.resize(cchStr);
		if (get((char*)&str[0], cbStr) != cbStr)
		{
			WINX_REPORT("SimpleReadArchive::get(UniString& str) Error - Unexpected Data");
			return E_UNEXPECTED;
		}
		return S_OK;
	}
	
	size_type winx_call get(char_type* lpBuf, size_type cch)
	{
		return fread(lpBuf, sizeof(char_type), cch, m_fp);
	}
	
	int winx_call good() const {
		return m_fp != NULL;
	}
};

// -------------------------------------------------------------------------
// class MemSeqWriteArchive

class MemSeqWriteArchive : public std::vector<char>
{
public:
	typedef char char_type;
	typedef unsigned size_type;
	typedef int int_type;
	
private:
	MemSeqWriteArchive(const MemSeqWriteArchive&);
	void operator=(const MemSeqWriteArchive&);
	
public:
	MemSeqWriteArchive() {}
	explicit MemSeqWriteArchive(UINT nReserveSize) {
		reserve(nReserveSize);
	}
	
	size_type winx_call put(const char_type* lpBuf, size_type cch)
	{
		insert(end(), lpBuf, lpBuf + cch);
		return cch;
	}
	
	void winx_call put(const char_type* lpBuf)
	{
		insert( end(), lpBuf, static_cast<const char_type*>(strchr(lpBuf, '\0')) );
	}
	
	size_type winx_call put(int_type ch)
	{
		push_back((char)ch);
		return 1;
	}
	
	size_type winx_call write(const void* lpBuf, size_type cbBuf)
	{
		insert(end(), (const char*)lpBuf, (const char*)lpBuf + cbBuf);
		return cbBuf;
	}
	
	const char* winx_call data() const {
		return &*begin();
	}
};

// -------------------------------------------------------------------------
// $Log: SimpleArchive.h,v $
// Revision 1.1  2006/10/18 12:13:39  xushiwei
// stdext as independent component
//
// Revision 1.1  2006/08/26 03:19:44  xushiwei
// STL-Extension:
//   Archive(SimpleWriteArchive, SimpleReadArchive, MemSeqWriteArchive, RegWriteArchive, RegReadArchive)
//

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_SIMPLEARCHIVE_H__ */
