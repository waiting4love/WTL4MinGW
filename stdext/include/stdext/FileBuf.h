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
// Module: stdext/FileBuf.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-25 15:33:27
// 
// $Id: FileBuf.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_FILEBUF_H__
#define __STDEXT_FILEBUF_H__

#ifndef __STDEXT_MEMORY_BASIC_H__
#include "memory/Basic.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class AutoBufferT

template <class _Alloc>
class AutoBufferT
{
protected:
	unsigned char* m_data;
	unsigned int m_bytes;

public:
	AutoBufferT() : m_data(NULL), m_bytes(0) {}
	explicit AutoBufferT(unsigned int bytes) : m_data(NULL) {
		allocate(bytes);
	}
	~AutoBufferT() {
		if (m_data)
			_Alloc::deallocate(m_data);
	}

	unsigned char* winx_call allocate(unsigned int bytes) {
		WINX_ASSERT(m_data == NULL);
		m_bytes = bytes;
		m_data = (unsigned char*)_Alloc::allocate(bytes);
		return m_data;
	}

	void winx_call clear() {
		if (m_data) {
			_Alloc::deallocate(m_data, m_bytes);
			m_data = NULL;
		}
	}

	unsigned char* winx_call data() {
		return m_data;
	}
	const unsigned char* winx_call data() const {
		return m_data;
	}

	unsigned char* winx_call begin() {
		return m_data;
	}
	const unsigned char* winx_call begin() const {
		return m_data;
	}
	
	unsigned char* winx_call end() {
		return m_data + m_bytes;
	}
	const unsigned char* winx_call end() const {
		return m_data + m_bytes;
	}

	int good() const {
		return m_data != NULL;
	}

	const unsigned int winx_call size() const {
		return m_bytes;
	}
};

// -------------------------------------------------------------------------
// class WinFileBuf

template <class _Alloc>
class WinFileBufT : public AutoBufferT<_Alloc>
{
private:
	typedef AutoBufferT<_Alloc> Base;
	
	HRESULT winx_call __read(HANDLE hFile)
	{
		WINX_ASSERT(!Base::good());
		
		DWORD dwFileSizeHigh = 0;
		DWORD dwFileSize = ::GetFileSize(hFile, &dwFileSizeHigh);
		if (dwFileSizeHigh > 0)
			return E_OUTOFMEMORY;
		
		::ReadFile(hFile, Base::allocate(dwFileSize), dwFileSize, &dwFileSizeHigh, NULL);
		return (dwFileSizeHigh == dwFileSize ? S_OK : STG_E_READFAULT);
	}

public:
	WinFileBufT() {}

	template <class InputFileT>
	explicit WinFileBufT(InputFileT file) {
		read(file);
	}

	HRESULT winx_call read(LPCSTR file)
	{
		HANDLE hFile = ::CreateFileA(
			file,
			GENERIC_READ, FILE_SHARE_READ, NULL, 
			OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return HRESULT_FROM_WIN32(::GetLastError());
	
		HRESULT hr = __read(hFile);
		::CloseHandle(hFile);
		return hr;
	}

	HRESULT winx_call read(LPCWSTR file)
	{
		HANDLE hFile = ::CreateFileW(
			file,
			GENERIC_READ, FILE_SHARE_READ, NULL, 
			OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return HRESULT_FROM_WIN32(::GetLastError());
		
		HRESULT hr = __read(hFile);
		::CloseHandle(hFile);
		return hr;
	}
};

typedef WinFileBufT<DefaultStaticAlloc> WinFileBuf;

// -------------------------------------------------------------------------
// class FILEFileBuf

template <class _Alloc>
class FILEFileBufT : public AutoBufferT<_Alloc>
{
private:
	typedef AutoBufferT<_Alloc> Base;

	HRESULT winx_call __read(FILE* fp)
	{
		WINX_ASSERT(!Base::good());
		
		::fseek(fp, 0, SEEK_END);
		long lFileSize = ::ftell(fp);
		if (lFileSize < 0)
			return STG_E_READFAULT;

		::fseek(fp, 0, SEEK_SET);
		if (lFileSize != (long)::fread(Base::allocate(lFileSize), 1, lFileSize, fp))
			return STG_E_READFAULT;

		return S_OK;
	}
	
public:
	FILEFileBufT() {}
	
	template <class InputFileT>
	explicit FILEFileBufT(InputFileT file) {
		read(file);
	}
	
	HRESULT winx_call read(LPCSTR file)
	{
		FILE* fp = ::fopen(file, "rb");
		if (fp == NULL)
			return STG_E_ACCESSDENIED;
		
		HRESULT hr = __read(fp);
		::fclose(fp);
		return hr;
	}
	
	HRESULT winx_call read(LPCWSTR file)
	{
		WINX_USES_CONVERSION;
		return read(WINX_W2CA(file));
	}
};

typedef FILEFileBufT<DefaultStaticAlloc> FILEFileBuf;

// -------------------------------------------------------------------------
// class FileBuf

typedef WinFileBuf FileBuf;

// -------------------------------------------------------------------------
// class TestFileBuf

template <class LogT>
class TestFileBuf
{
public:
	WINX_TEST_SUITE(TestFileBuf);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	void test(LogT& log)
	{
		WinFileBuf file(__FILE__);
		log.printString(file.begin(), file.end());

		FILEFileBuf file2(__FILE__);
		AssertEq(file.size(), file2.size());
		AssertEqBuf(file.data(), file2.data(), file.size());
	}
};

// -------------------------------------------------------------------------
// $Log: FileBuf.h,v $
// Revision 1.1  2006/10/18 12:13:39  xushiwei
// stdext as independent component
//
// Revision 1.2  2006/09/26 08:05:56  xushiwei
// TestFileBuf
//
// Revision 1.1  2006/09/25 08:23:36  xushiwei
// STL-Extension: FileBuf(WinFileBuf, FILEFileBuf)
//

__NS_STD_END

#endif /* __STDEXT_FILEBUF_H__ */
