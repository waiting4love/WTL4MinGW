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
// Module: stdext/archive/StdioArchive.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 21:07:06
// 
// $Id: StdioArchive.h,v 1.3 2007/01/10 09:36:12 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_STDIOARCHIVE_H__
#define __STDEXT_ARCHIVE_STDIOARCHIVE_H__

// -------------------------------------------------------------------------

#ifndef _INC_IO
#include <io.h>
#endif

#ifndef __STDEXT_ARCHIVE_ARCHIVEIMPL_H__
#include "ArchiveImpl.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class StdioAdapter

class StdioAdapter
{
private:
	typedef char _E;

public:
	enum { endch = -1 };

	typedef _E				char_type;
	typedef int				int_type;
	typedef unsigned char	uchar_type;
	typedef unsigned long	pos_type;
	typedef signed long		off_type;
	typedef unsigned		size_type;

protected:
	FILE* m_pFile;
	
public:
	StdioAdapter() : m_pFile(NULL) {}
	~StdioAdapter()
	{
		if (m_pFile)
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
	}
	
	void winx_call attach(FILE* pFile)
	{
		m_pFile = pFile;
	}

	FILE* winx_call detach()
	{
		FILE* pFile = m_pFile;
		m_pFile = NULL;
		return pFile;
	}

	void winx_call open_handle(FILE* pFile)
	{
		WINX_ASSERT(m_pFile == NULL);
		m_pFile = pFile;
	}

	HRESULT winx_call open_to_write(LPCWSTR szFile)
	{
		WINX_ASSERT(m_pFile == NULL);
		WINX_USES_CONVERSION;
		m_pFile = fopen(WINX_W2CA(szFile), "wb");
		return m_pFile ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open_to_write(LPCSTR szFile)
	{
		WINX_ASSERT(m_pFile == NULL);
		m_pFile = fopen(szFile, "wb");
		return m_pFile ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open_to_read(LPCWSTR szFile)
	{
		WINX_ASSERT(m_pFile == NULL);
		WINX_USES_CONVERSION;
		m_pFile = fopen(WINX_W2CA(szFile), "rb");
		return m_pFile ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open_to_read(LPCSTR szFile)
	{
		WINX_ASSERT(m_pFile == NULL);
		m_pFile = fopen(szFile, "rb");
		return m_pFile ? S_OK : E_ACCESSDENIED;
	}

	void winx_call seek(const off_type& pos, int dir = SEEK_SET)
	{
		fseek(m_pFile, pos, dir);
	}
	
	pos_type winx_call tell() const
	{
		return ftell(m_pFile);
	}

	pos_type winx_call size() const
	{
		return _filelength(m_pFile->_file);
	}
	
	size_type winx_call get(_E* buf, size_type cch)
	{	
		return fread(buf, sizeof(char_type), cch, m_pFile);
	}

	size_type winx_call put(const _E* s, size_type cch)
	{
		return fwrite(s, sizeof(char_type), cch, m_pFile);
	}

	void winx_call flush()
	{
		fflush(m_pFile);
	}

	void winx_call close()
	{
		if (m_pFile)
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
	}

	operator FILE*() const
	{
		return m_pFile;
	}

	int winx_call operator!() const
	{
		return m_pFile == NULL;
	}

	int winx_call good() const
	{
		return m_pFile != NULL;
	}

	int winx_call bad() const
	{
		return m_pFile == NULL;
	}
};

// -------------------------------------------------------------------------

typedef WriteArchiveImpl<FILE*, StdioAdapter> StdioWriteArchive;
typedef ReadArchiveImpl<FILE*, StdioAdapter> StdioReadArchive;

// -------------------------------------------------------------------------
// class TestStdioArchive

template <class LogT>
class TestStdioArchive
{
public:
	void doTest(LogT& log)
	{
		{
			StdioWriteArchive ar(L"/__test__.txt");
			ar.put("hello\n");
		}
		{
			char szBuf[100];
			StdioReadArchive ar("/__test__.txt");
			size_t cch = ar.get(szBuf, countof(szBuf));
			szBuf[cch] = '\0';
			log.print(szBuf);
		}
		{
			StdioWriteArchive ar;
			ar.open("/__test__.txt");
			ar.put("you're welcome!\n");
		}
		{
			char szBuf[100];
			StdioReadArchive ar;
			ar.open(L"/__test__.txt");
			size_t cch = ar.get(szBuf, countof(szBuf));
			szBuf[cch] = '\0';
			log.print(szBuf);
		}
		{
			char szBuf[100];
			StdioWriteArchive ar("/__test__.txt");
			ar.put(itoa(13242, szBuf, 10));
			ar.put(' ');
			ar.put(itoa(1111, szBuf, 10));
		}
		{
			StdioReadArchive ar("/__test__.txt");
			unsigned val;
			ar.scan_uint(val);
			log.print(val).newline();
			ar.scan_uint(val, 2);
			log.print(val).newline();
		}
	}
};

// -------------------------------------------------------------------------
// $Log: StdioArchive.h,v $
// Revision 1.3  2007/01/10 09:36:12  xushiwei
// StdioArchive::size
//
// Revision 1.2  2006/12/01 05:38:28  xushiwei
// STL-Extension: class ArchiveImpl -
//  put16i/get16i, put32i/get32i, put_struct/get_struct, read/write
//  scan_uint, get_uint, get_not_if/getnws, skip_if/skipws
//
// Revision 1.1  2006/11/30 03:19:24  xushiwei
// STL-Extension:
//  ULargeInteger, LargeInteger, ReadArchiveImpl, WriteArchiveImpl
//  StreamArchive(StreamWriteArchive, StreamReadArchive, MemStreamWriteArchive, MemStreamReadArchive)
//  StdioArchive(StdioWriteArchive, StdioReadArchive)
//

__NS_STD_END

#endif /* __STDEXT_ARCHIVE_STDIOARCHIVE_H__ */
