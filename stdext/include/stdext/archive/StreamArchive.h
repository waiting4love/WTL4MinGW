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
// Module: stdext/archive/StreamArchive.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 20:06:28
// 
// $Id: StreamArchive.h,v 1.3 2006/12/03 05:30:27 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_STREAMARCHIVE_H__
#define __STDEXT_ARCHIVE_STREAMARCHIVE_H__

#ifndef __STDEXT_ARCHIVE_ARCHIVEIMPL_H__
#include "ArchiveImpl.h"
#endif

#ifndef __STDEXT_LARGEINTEGER_H__
#include "../LargeInteger.h"
#endif

#if !defined(WINX_USE_WINSDK)
#if !defined(_MSC_VER) || (_MSC_VER <= 1200)
STDAPI SHCreateStreamOnFileA(LPCSTR pszFile, DWORD grfMode, struct IStream **ppstm);
STDAPI SHCreateStreamOnFileW(LPCWSTR pszFile, DWORD grfMode, struct IStream **ppstm);
#endif
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// toInt, toUInt

__forceinline int winx_call toInt(const LargeInteger& rhs)
{
	return (int)rhs.QuadPart;
}

__forceinline UINT winx_call toUInt(const ULargeInteger& rhs)
{
	return (UINT)rhs.QuadPart;
}

// -------------------------------------------------------------------------

#ifndef _WINX_STREAM_THROW_IO_ERROR
#define _WINX_STREAM_THROW_IO_ERROR(sc)	throw IoException(sc)
#endif

// -------------------------------------------------------------------------
// class IStreamAdapter

class IStreamAdapter
{
private:
	typedef char _E;

public:
	enum { endch = -1 };

	typedef _E				char_type;
	typedef int				int_type;
	typedef unsigned char	uchar_type;
	typedef ULargeInteger	pos_type;
	typedef LargeInteger	off_type;
	typedef unsigned 		size_type;
	
protected:
	IStream* m_pStrm;
	
public:
	IStreamAdapter()
	{
		m_pStrm = NULL;
	}
	IStreamAdapter(const IStreamAdapter& rhs, BOOL do_clone)
	{
		if (do_clone)
		{
			m_pStrm = NULL;
			if (rhs.m_pStrm)
			{
				rhs.m_pStrm->Clone(&m_pStrm);
			}
		}
		else
		{
			m_pStrm = rhs.m_pStrm;
			if (m_pStrm)
				m_pStrm->AddRef();
		}
	}
	~IStreamAdapter()
	{
		if (m_pStrm)
			m_pStrm->Release();
	}

	void winx_call attach(IStream* pStrm)
	{
		if (m_pStrm)
			m_pStrm->Release();
		m_pStrm = pStrm;
	}

	IStream* winx_call detach()
	{
		IStream* p = m_pStrm;
		m_pStrm = NULL;
		return p;
	}

	HRESULT winx_call open(LPCWSTR szFile, UINT nMode)
	{
		WINX_ASSERT(m_pStrm == NULL);
		return SHCreateStreamOnFileW(szFile, nMode, &m_pStrm);
	}

	void winx_call open_handle(IStream* pStrm)
	{
		WINX_ASSERT(m_pStrm == NULL);
		m_pStrm = pStrm;
		if(pStrm)
			pStrm->AddRef();
	}

	HRESULT winx_call open_to_read(LPCWSTR szFile)
	{
		WINX_ASSERT(m_pStrm == NULL);
		return SHCreateStreamOnFileW(szFile, STGM_READ, &m_pStrm);
	}

	HRESULT winx_call open_to_read(LPCSTR szFile)
	{
		WINX_ASSERT(m_pStrm == NULL);
		return SHCreateStreamOnFileA(szFile, STGM_READ, &m_pStrm);
	}

	HRESULT winx_call open_to_write(LPCWSTR szFile)
	{
		WINX_ASSERT(m_pStrm == NULL);
		return SHCreateStreamOnFileW(szFile, STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE, &m_pStrm);
	}

	HRESULT winx_call open_to_write(LPCSTR szFile)
	{
		WINX_ASSERT(m_pStrm == NULL);
		return SHCreateStreamOnFileA(szFile, STGM_CREATE|STGM_WRITE|STGM_SHARE_EXCLUSIVE, &m_pStrm);
	}

	void winx_call close()
	{
		if (m_pStrm)
		{
			m_pStrm->Release();
			m_pStrm = NULL;
		}
	}

	IStream** winx_call operator&()
	{
		WINX_ASSERT(m_pStrm == NULL);
		return &m_pStrm;
	}

	operator IStream*() const
	{
		return m_pStrm;
	}
	
	int winx_call operator!() const
	{
		return m_pStrm == NULL;
	}

	int winx_call bad() const
	{
		return m_pStrm == NULL;
	}

	int winx_call good() const
	{
		return m_pStrm != NULL;
	}
	
	void winx_call seek(IN const off_type& pos, IN int dir = SEEK_SET) throw(IoException)
	{
		HRESULT hr = m_pStrm->Seek(pos, dir, NULL);
		if (FAILED(hr))
		{
			_WINX_STREAM_THROW_IO_ERROR(hr);
		}
	}
	
	pos_type winx_call tell() const
	{
		ULARGE_INTEGER dlibPos;
		ULISet32(dlibPos, 0);
		m_pStrm->Seek(*(LARGE_INTEGER*)&dlibPos, SEEK_CUR, &dlibPos);
		return dlibPos;
	}

	pos_type winx_call size() const
	{
		STATSTG statstg;
		ULISet32(statstg.cbSize, 0);
		m_pStrm->Stat(&statstg, STATFLAG_NONAME);
		return statstg.cbSize;
	}

	int_type winx_call get()
	{
		int_type ch = 0;
		ULONG stRead = 0;
		m_pStrm->Read((void*)&ch, sizeof(char_type), &stRead);
		if (!stRead)
			return endch;
		return ch;
	}
	
	size_type winx_call get(char_type* buf, size_type cch)
	{
		ULONG stRead = 0;
		m_pStrm->Read((void*)buf, sizeof(char_type)*cch , &stRead);
		return stRead;
	}
	
	size_type winx_call read(void* buf, size_type cb)
	{
		ULONG stRead = 0L;
		m_pStrm->Read(buf, sizeof(char_type)*cb, &stRead);
		return stRead;
	}

	void winx_call flush() throw(IoException)
	{
		HRESULT hr = m_pStrm->Commit(0);
		if (FAILED(hr))
		{
			_WINX_STREAM_THROW_IO_ERROR(hr);
		}
	}
	
	void winx_call put(const _E* s, size_type cch) throw(IoException)
	{
		ULONG uWrite = 0L;
		HRESULT hr = m_pStrm->Write((void*)s, (ULONG)cch, &uWrite);
		if (SUCCEEDED(hr))
		{
			if (uWrite == cch)
				return;
			//
			//The caller does not have enough permissions for writing to this stream object. 
			//Note:
			//	磁盘写失败，通常出于两种原因：
			//	1）介质满（DiskFull: STG_E_MEDIUMFULL）。
			//	2）介质只读（Permissions: STG_E_ACCESSDENIED）。
			//
			hr = STG_E_ACCESSDENIED;
		}
		_WINX_STREAM_THROW_IO_ERROR(hr);
	}

	void winx_call put(const _E* s)
	{
		const _E* ps = s;
		while(*ps)
			++ps;
		put(s, (size_type)(ps - s));
	}
	
	void winx_call put(size_type cch, _E ch)
	{
		_E* p0 = new _E[cch];
		_E* p = p0;
		while (cch--)
			*p++ = ch;
		put(p0, (size_type)(p - p0));
		delete[] p0;
	}
	
	void winx_call put(_E ch)
	{
		put(&ch, 1);
	}

	size_type winx_call write(const void* buf, size_type cb)
	{
		ULONG uWrite = 0L;
		m_pStrm->Write((void*)buf, (ULONG)cb, &uWrite);
		return uWrite;
	}
};

// -------------------------------------------------------------------------

typedef WriteArchiveImpl<IStream*, IStreamAdapter> StreamWriteArchive;
typedef ReadArchiveImpl<IStream*, IStreamAdapter> StreamReadArchive;

// -------------------------------------------------------------------------
// class MemStreamWriteArchive

class MemStreamWriteArchive : public StreamWriteArchive
{
private:
	typedef StreamWriteArchive BaseClass;

public:
	MemStreamWriteArchive(size_type nBufSize = default_buffer_size, char_type* lpBuf = NULL)
		: BaseClass(nBufSize, lpBuf)
	{
		CreateStreamOnHGlobal(NULL, FALSE, &m_handle);
	}
	~MemStreamWriteArchive()
	{
		close();
	}

public:
	size_type winx_call write(const void* lpBuf, size_type nMax)
	{
		return put((const char_type*)lpBuf, nMax);
	}

	void winx_call close()
	{
		if (m_handle.good())
		{
			HGLOBAL hgbl = detach();
			if (hgbl)
				GlobalFree(hgbl);
		}
	}

	HGLOBAL winx_call detach()
	{
		HGLOBAL hgbl = NULL;
		clear_cache();
		GetHGlobalFromStream(m_handle, &hgbl);
		m_handle.close();
		return hgbl;
	}

	HGLOBAL winx_call get_handle()
	{
		HGLOBAL hgbl = NULL;
		clear_cache();
		GetHGlobalFromStream(m_handle, &hgbl);
		return hgbl;
	}
};

// -------------------------------------------------------------------------
// class MemStreamReadArchive

class MemStreamReadArchive : public StreamReadArchive
{
private:
	typedef StreamReadArchive BaseClass;

public:
	MemStreamReadArchive(HGLOBAL hgbl, size_type nBufSize = default_buffer_size, char_type* lpBuf = NULL)
		: BaseClass(nBufSize, lpBuf)
	{
		CreateStreamOnHGlobal(hgbl, FALSE, &m_handle);
	}

	MemStreamReadArchive(size_type nBufSize = default_buffer_size, char_type* lpBuf = NULL)
		: BaseClass(nBufSize, lpBuf)
	{
	}
	
public:
	HRESULT winx_call open(HGLOBAL hgbl, BOOL fDeleteOnRelease = FALSE)
	{
		if (good())
			return E_ACCESSDENIED;
		return CreateStreamOnHGlobal(hgbl, fDeleteOnRelease, &m_handle);
	}

	size_type winx_call read(void* lpBuf, size_type nMax)
	{
		return get((char_type*)lpBuf, nMax);
	}
};

// -------------------------------------------------------------------------
// class TestStreamArchive

template <class LogT>
class TestStreamArchive
{
public:
	void doTest(LogT& log)
	{
		{
			StreamWriteArchive ar(L"/__test__.txt");
			ar.put("hello world\n");
		}
		{
			char szBuf[100];
			StreamReadArchive ar("/__test__.txt");
			ar.seek(6);
			size_t cch = ar.get(szBuf, countof(szBuf));
			szBuf[cch] = '\0';
			printf(szBuf);
		}
		{
			StreamWriteArchive ar;
			ar.open("/__test__.txt");
			ar.put("you're welcome!\n");
			ar.seek(3);
			ar.put(" are welcome!\n");
		}
		{
			char szBuf[100];
			StreamReadArchive ar;
			ar.open(L"/__test__.txt");
			size_t cch = ar.get(szBuf, countof(szBuf));
			szBuf[cch] = '\0';
			printf(szBuf);
		}
	}
};

// -------------------------------------------------------------------------
// $Log: StreamArchive.h,v $
// Revision 1.3  2006/12/03 05:30:27  xushiwei
// WINX_USE_DEFSDK support
//
// Revision 1.2  2006/12/02 06:47:16  xushiwei
// modify details
//
// Revision 1.1  2006/11/30 03:19:24  xushiwei
// STL-Extension:
//  ULargeInteger, LargeInteger, ReadArchiveImpl, WriteArchiveImpl
//  StreamArchive(StreamWriteArchive, StreamReadArchive, MemStreamWriteArchive, MemStreamReadArchive)
//  StdioArchive(StdioWriteArchive, StdioReadArchive)
//

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_STREAMARCHIVE_H__ */
