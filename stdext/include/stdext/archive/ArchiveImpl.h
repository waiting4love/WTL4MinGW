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
// Module: stdext/archive/ArchiveImpl.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: ArchiveImpl.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_ARCHIVEIMPL_H__
#define __STDEXT_ARCHIVE_ARCHIVEIMPL_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

#ifndef __STDEXT_CHARTYPE_H__
#include "../CharType.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// toInt, toUInt

__forceinline int  winx_call toInt(int li)		{ return li; }
__forceinline UINT winx_call toUInt(UINT li)	{ return li; }

// -------------------------------------------------------------------------
// class ArchiveBuffer

template <class CharT, class SizeT>
class ArchiveBuffer
{
public:
	enum { default_buffer_size = 4096  };
	enum { min_buffer_size = 0x200 };
	
	typedef CharT char_type;
	typedef SizeT size_type;

public:
	ArchiveBuffer(size_type nBufSize = default_buffer_size, char_type* lpBuf = NULL)
	{
		m_lpBufStart = lpBuf;
		if (nBufSize < min_buffer_size)	// if buffer is too small
		{
			m_nBufSize	= min_buffer_size;
			m_lpBufStart = NULL;
		}
		else
		{
			m_nBufSize	 = nBufSize;
		}
		
		if (m_lpBufStart == NULL)
		{
			m_lpBufStart = new char_type[m_nBufSize];
			m_fOwnBuf = 1;
		}
		else
		{
			m_fOwnBuf = 0;
		}
	}
	~ArchiveBuffer()
	{
		if (m_fOwnBuf)
		{
			delete[] m_lpBufStart;
			m_lpBufStart = NULL;
		}
	}

protected:
	static void winx_call _copyMemory(char_type* dest, const char_type* src, size_type n)
	{
		while (n--)
			*dest++ = *src++;
	}
	
protected:
	char_type*	m_lpBufStart;
	size_type	m_nBufSize;
	int			m_fOwnBuf;
};

// -------------------------------------------------------------------------
// class IoException

class IoException : public std::exception
{
private:
	HRESULT m_scode;

public:
	IoException(HRESULT sc, const char* errMsg = "Io Exception")
		: m_scode(sc), std::exception(errMsg) {
	}
	
	HRESULT winx_call getError() const {
		return m_scode;
	}
};

// -------------------------------------------------------------------------
// class WriteArchiveImpl

template <class Handle, class StreamHandle = Handle>
class WriteArchiveImpl : public ArchiveBuffer<typename StreamHandle::char_type, typename StreamHandle::size_type>
{
private:
	typedef ArchiveBuffer<typename StreamHandle::char_type, typename StreamHandle::size_type> BaseClass;
	typedef WriteArchiveImpl<Handle, StreamHandle> ThisClass;

	WriteArchiveImpl(const WriteArchiveImpl&);
	void operator=(const WriteArchiveImpl&);

public:
	typedef typename StreamHandle::int_type	int_type;
	typedef typename StreamHandle::char_type char_type;
	typedef typename StreamHandle::uchar_type uchar_type;
	typedef typename StreamHandle::pos_type	pos_type;
	typedef typename StreamHandle::off_type	off_type;
	typedef typename StreamHandle::size_type size_type;
	
    using BaseClass::default_buffer_size;

protected:
    using BaseClass::m_lpBufStart;
    using BaseClass::m_nBufSize;
    
	char_type* m_lpBufCur;
	char_type* m_lpBufMax;
	StreamHandle m_handle;
	
public:
	WriteArchiveImpl(size_type nBufSize = default_buffer_size, char_type* lpBuf = NULL)
		: BaseClass(nBufSize, lpBuf)
	{
		m_lpBufCur = m_lpBufStart;
		m_lpBufMax = m_lpBufStart + m_nBufSize;
		// m_lpBufCur - m_lpBufStart为已经写入数据的缓存！
	}

	WriteArchiveImpl(Handle hFile, size_type nBufSize = default_buffer_size, char_type* lpBuf = NULL)
		: BaseClass(nBufSize, lpBuf)
	{
		m_handle.open_handle(hFile);
		m_lpBufCur = m_lpBufStart;
		m_lpBufMax = m_lpBufStart + m_nBufSize;
	}

	WriteArchiveImpl(LPCWSTR szFile, size_type nBufSize = default_buffer_size, char_type* lpBuf = NULL)
		: BaseClass(nBufSize, lpBuf)
	{
		m_handle.open_to_write(szFile);
		m_lpBufCur = m_lpBufStart;
		m_lpBufMax = m_lpBufStart + m_nBufSize;
	}

	WriteArchiveImpl(LPCSTR szFile, size_type nBufSize = default_buffer_size, char_type* lpBuf = NULL)
		: BaseClass(nBufSize, lpBuf)
	{
		m_handle.open_to_write(szFile);
		m_lpBufCur = m_lpBufStart;
		m_lpBufMax = m_lpBufStart + m_nBufSize;
	}

	~WriteArchiveImpl() throw(IoException)
	{
		if (m_lpBufCur != m_lpBufStart)
		{
			m_handle.put(m_lpBufStart, (size_type)(m_lpBufCur - m_lpBufStart));
		}
	}

	void winx_call clear_cache() throw(IoException)
	{
		if(m_lpBufCur != m_lpBufStart)
		{
			//@@bugfix: 此流程会引发异常。做出的修改的目的是避免再次引发异常。
			//2005-3-23
			char_type* lpBufCur = m_lpBufCur;
			m_lpBufCur = m_lpBufStart;
			m_handle.put(m_lpBufStart, (UINT)(lpBufCur - m_lpBufStart));
		}
	}

	void winx_call flush() throw(IoException)
	{
		clear_cache();
		m_handle.flush();
	}

	pos_type winx_call tell() const
	{
		return m_handle.tell() + (INT32)(m_lpBufCur - m_lpBufStart);
	}

	void winx_call seek(pos_type offset) throw(IoException)
	{
		clear_cache();
		m_handle.seek(static_cast<off_type>(offset), SEEK_SET);
		m_lpBufCur = m_lpBufStart;
	}

	void winx_call seek_end() throw(IoException)
	{
		clear_cache();
		m_handle.seek(0, SEEK_END);
		m_lpBufCur = m_lpBufStart;
	}

public:
	void winx_call reput(size_type offset, const char_type* lpBuf, size_type cch) throw(IoException)
	{
		WINX_ASSERT((int)offset > 0 && offset >= cch);
		
		if (offset <= (size_type)(UINT)(m_lpBufCur - m_lpBufStart))
		{
			_copyMemory(m_lpBufCur - offset, lpBuf, cch);
		}
		else
		{
			pos_type curpos = tell();
			seek(curpos - offset);
			put(lpBuf, cch);
			seek(curpos);
		}
	}

	size_type winx_call put(const char_type* lpBuf, size_type cch) throw(IoException)
	{
		WINX_ASSERT(lpBuf && m_lpBufStart && m_lpBufCur);
		
		// 1. 如果缓存足够
		size_type cbUnused = (size_type)(m_lpBufMax - m_lpBufCur);
		if (cch <= cbUnused)
		{
			_copyMemory(m_lpBufCur, lpBuf, cch);
			m_lpBufCur += cch;
			return cch;
		}
		
		// 2.1 先将目标数据Copy到缓存，直到填满为止
		_copyMemory(m_lpBufCur, lpBuf, cbUnused);
		lpBuf += cbUnused;
		cch -= cbUnused;

		m_handle.put(m_lpBufStart, m_nBufSize);
		
		// 2.2 写满各整页
		size_type nWritten = cch - (cch % m_nBufSize);
		if (nWritten)
		{
			m_handle.put(lpBuf, nWritten);
			lpBuf += nWritten;
			cch -= nWritten;
		}
		nWritten += cbUnused;
		
		// 2.3 写最后一页
		_copyMemory(m_lpBufStart, lpBuf, cch);
		m_lpBufCur = m_lpBufStart + cch;
		nWritten += cch;
		
		return nWritten;
	}

	size_type winx_call put(const char_type* str) throw(IoException)
	{
		const char_type* endptr = str;
		while ( *endptr	)
			++endptr;
		return put(str, (size_type)(endptr - str));
	}

	size_type winx_call put(char_type ch) throw(IoException)
	{
		if (m_lpBufCur < m_lpBufMax)
		{
			*m_lpBufCur++ = ch;
			return 1;
		}
		return put(&ch, 1);
	}

public:
	size_type winx_call write(const void* lpBuf, size_type nMax) throw(IoException)
	{
		return put( (const char*)lpBuf, nMax );
	}

	ThisClass& winx_call put16i(IN UINT16 val) throw(IoException)
	{
		_WinxByteSwap16(val);
		put( (const char*)&val, sizeof(val) );
		return *this;
	}
	ThisClass& winx_call put32i(IN UINT32 val) throw(IoException)
	{
		_WinxByteSwap32(val);
		put( (const char*)&val, sizeof(val) );
		return *this;
	}
	
	template <class StrucType>
	ThisClass& winx_call put_struct(IN const StrucType& struc) throw(IoException)
	{
		_WinxByteSwapStruct(struc);
		put( (const char*)&struc, sizeof(struc) );
		return *this;
	}

#if defined(WINX_BYTESWAP)
	ThisClass& put16i(IN const UINT16 warray[], IN UINT count) throw(IoException)
	{
		for (UINT i = 0; i < count; ++i)
			put16i(warray[i]);
		return *this;
	}
	ThisClass& put16i(IN const INT16 warray[], IN UINT count) throw(IoException)
	{
		for (UINT i = 0; i < count; ++i)
			put16i(warray[i]);
		return *this;
	}
	ThisClass& put32i(IN const UINT32 dwarray[], IN UINT count) throw(IoException)
	{
		for (UINT i = 0; i < count; ++i)
			put32i(dwarray[i]);
		return *this;
	}
	ThisClass& put32i(IN const INT32 dwarray[], IN UINT count) throw(IoException)
	{
		for (UINT i = 0; i < count; ++i)
			put32i(dwarray[i]);
		return *this;
	}
	template <class StrucType>
	ThisClass& put_struct(const StrucType* array, UINT count) throw(IoException)
	{
		for (UINT i = 0; i < count; ++i)
			put_struct(array[i]);
		return *this;
	}
#else
	ThisClass& put16i(IN const UINT16 warray[], IN UINT count) throw(IoException)
	{
		put( (const char*)warray, sizeof(UINT16)*count );
		return *this;
	}
	ThisClass& put16i(IN const INT16 warray[], IN UINT count) throw(IoException)
	{
		put( (const char*)warray, sizeof(INT16)*count );
		return *this;
	}
	ThisClass& put32i(IN const UINT32 dwarray[], IN UINT count) throw(IoException)
	{
		put( (const char*)dwarray, sizeof(UINT32)*count );
		return *this;
	}
	ThisClass& put32i(IN const INT32 dwarray[], IN UINT count) throw(IoException)
	{
		put( (const char*)dwarray, sizeof(INT32)*count );
		return *this;
	}
	template <class StrucType>
	ThisClass& put_struct(const StrucType* array, UINT count) throw(IoException)
	{
		put( (const char*)array, sizeof(StrucType)*count );
		return *this;
	}
#endif // !defined(WINX_BYTESWAP)

public:
	HRESULT winx_call open(LPCWSTR szFile)
	{
		if (m_handle.good())
			return E_ACCESSDENIED;
		return m_handle.open_to_write(szFile);
	}

	HRESULT winx_call open(LPCSTR szFile)
	{
		if (m_handle.good())
			return E_ACCESSDENIED;
		return m_handle.open_to_write(szFile);
	}

	void winx_call open_handle(Handle hFile)
	{
		if (!m_handle.good())
			m_handle.open_handle(hFile);
	}

	void winx_call close() throw(IoException)
	{
		clear_cache();
		m_handle.close();
	}

	void winx_call attach(Handle hFile) throw(IoException)
	{
		clear_cache();
		m_handle.attach(hFile);
	}

	Handle winx_call detach() throw(IoException)
	{
		clear_cache();
		return m_handle.detach();
	}

	int winx_call operator!() const
	{
		return m_handle.bad();
	}

	int winx_call good() const
	{
		return m_handle.good();
	}

	int winx_call bad() const
	{
		return m_handle.bad();
	}
};

// -------------------------------------------------------------------------
// class ReadArchiveImpl

template <class Handle, class StreamHandle = Handle>
class ReadArchiveImpl : public ArchiveBuffer<typename StreamHandle::char_type, typename StreamHandle::size_type>
{
private:
	typedef ArchiveBuffer<typename StreamHandle::char_type, typename StreamHandle::size_type> BaseClass;
	typedef ReadArchiveImpl<Handle, StreamHandle> ThisClass;

public:
	enum { endch = StreamHandle::endch };

   	typedef typename StreamHandle::int_type	int_type;
	typedef typename StreamHandle::char_type char_type;
	typedef typename StreamHandle::uchar_type uchar_type;
	typedef typename StreamHandle::pos_type	pos_type;
	typedef typename StreamHandle::off_type	off_type;
	typedef typename StreamHandle::size_type size_type;
	
    using BaseClass::default_buffer_size;

protected:
    using BaseClass::m_lpBufStart;
    using BaseClass::m_nBufSize;

	char_type* m_lpBufCur;
	char_type* m_lpBufMax;
	StreamHandle m_handle;

private:
	ReadArchiveImpl(const ReadArchiveImpl&);
	void operator=(const ReadArchiveImpl&);
	
public:
	ReadArchiveImpl(size_type nBufSize = default_buffer_size, char_type* lpBuf = NULL)
		: BaseClass(nBufSize, lpBuf)
	{
		// 读状态：m_lpBufMax - m_lpBufCur 为已经读入预读的数据！
		m_lpBufCur = m_lpBufMax = m_lpBufStart;
	}

	ReadArchiveImpl(Handle hFile, size_type nBufSize = default_buffer_size, char_type* lpBuf = NULL)
		: BaseClass(nBufSize, lpBuf)
	{
		m_handle.open_handle(hFile);
		m_lpBufCur = m_lpBufMax = m_lpBufStart;
	}

	ReadArchiveImpl(LPCWSTR szFile, size_type nBufSize = default_buffer_size, char_type* lpBuf = NULL)
		: BaseClass(nBufSize, lpBuf)
	{
		m_handle.open_to_read(szFile);
		m_lpBufCur = m_lpBufMax = m_lpBufStart;
	}

	ReadArchiveImpl(LPCSTR szFile, size_type nBufSize = default_buffer_size, char_type* lpBuf = NULL)
		: BaseClass(nBufSize, lpBuf)
	{
		m_handle.open_to_read(szFile);
		m_lpBufCur = m_lpBufMax = m_lpBufStart;
	}

	ReadArchiveImpl(const ReadArchiveImpl& rhs, BOOL fClone, size_type nBufSize = default_buffer_size, char_type* lpBuf = NULL)
		: BaseClass(nBufSize, lpBuf), m_handle(rhs.m_handle, fClone)
	{
		m_lpBufCur = m_lpBufMax = m_lpBufStart;
	}
	
	void winx_call clear_cache()
	{
		m_lpBufCur = m_lpBufMax = m_lpBufStart;
	}

	pos_type winx_call tell() const
	{
		return m_handle.tell() - (INT32)(m_lpBufMax - m_lpBufCur);
	}

	pos_type winx_call size() const
	{
		return m_handle.size();
	}

	void winx_call seek(pos_type offset)
	{
		// 读状态：m_lpBufMax - m_lpBufStart不是m_nBufSize，而是有效数据范围！
		// ??			--- m_lpBufStart
		// position		--- m_lpBufMax
		// offset		--- m_lpBufCur
		pos_type position = m_handle.tell();
		pos_type delta = position - offset;
		if (delta <= (pos_type)(INT)(m_lpBufMax - m_lpBufStart))
		{
			m_lpBufCur = m_lpBufMax - toUInt(delta);
		}
		else
		{
			m_handle.seek(static_cast<off_type>(offset), SEEK_SET);
			m_lpBufCur = m_lpBufMax = m_lpBufStart;
		}
	}

	void winx_call seek_end()
	{
		m_handle.seek(0, SEEK_END);
		m_lpBufCur = m_lpBufMax = m_lpBufStart;
	}

public:
	size_type winx_call skip(size_type nMax)
	{
		if (nMax + m_lpBufCur <= m_lpBufMax)
		{
			// 1）在缓存数据足够时，读入数据并返回
			m_lpBufCur  += nMax;
		}
		else
		{
			// 2）在缓存数据不足时
			// ??			--- m_lpBufStart
			// position		--- m_lpBufMax		= 文件指针对应处
			// offset		--- m_lpBufCur
			m_handle.seek(nMax - (m_lpBufMax - m_lpBufCur), SEEK_CUR);
			m_lpBufCur = m_lpBufMax = m_lpBufStart;
		}
		return nMax;
	}

	size_type winx_call get(char_type* lpBuf, size_type nMax)
	{
		WINX_ASSERT(lpBuf && m_lpBufStart && m_lpBufCur);
		
		// 1）在缓存数据足够时，读入数据并返回
		if (nMax + m_lpBufCur <= m_lpBufMax)
		{
			_copyMemory(lpBuf, m_lpBufCur, nMax);
			m_lpBufCur += nMax;
			return nMax;
		}
		
		// 2）在缓存数据不足时
		
		// nRead	--- 读入的总字节数
		// nMax		--- 剩余字节数
		// nBlkRead --- 文件操作的返回值
		size_type nBlkRead, nRead;
		
		// a）先读入缓存中的数据
		nRead = (size_type)(m_lpBufMax - m_lpBufCur);
		_copyMemory(lpBuf, m_lpBufCur, nRead);
		nMax -= nRead;
		m_lpBufCur = m_lpBufMax;
		
		// b）将m_nBufSize的整数倍内容读入
		size_type nTemp = nMax - nMax % m_nBufSize;
		if (nTemp)
		{
			nBlkRead = m_handle.get(lpBuf + nRead, nTemp);
			nRead += nBlkRead;
			if (nBlkRead < nTemp)
			{
				// 可能已经到了文件末端：
				return nRead;
			}
			nMax -= nTemp;
		}
		WINX_ASSERT(nMax < m_nBufSize);
		
		// c）剩余的先读入到缓冲区中，再写入目标内存
		nBlkRead	= m_handle.get(m_lpBufStart, m_nBufSize);
		m_lpBufMax	= (m_lpBufCur = m_lpBufStart) + nBlkRead;
		if (nBlkRead < nMax)
			nMax = nBlkRead;
		_copyMemory(lpBuf + nRead, m_lpBufCur, nMax);
		m_lpBufCur += nMax;
		nRead += nMax;
		
		return nRead;
	}

	int_type winx_call get()
	{
		if (m_lpBufCur < m_lpBufMax)
			return (uchar_type)*m_lpBufCur++;

		char_type ch;
		return get(&ch, 1) ? (uchar_type)ch : (int_type)endch;
	}

	void winx_call reget(size_type offset, char_type* lpBuf, size_type cch)
	{
		WINX_ASSERT((int)offset > 0 && offset >= cch);
		
		if (offset <= (size_type)(UINT)(m_lpBufCur - m_lpBufStart))
		{
			_copyMemory(lpBuf, m_lpBufCur - offset, cch);
		}
		else
		{
			pos_type curpos = tell();
			seek(curpos - offset);
			get(lpBuf, cch);
			seek(curpos);
		}
	}

	int winx_call unget()
	{
		if (m_lpBufCur <= m_lpBufStart)
		{
			pos_type delta = m_nBufSize >> 2;
			pos_type current = tell();
			if (current < delta)
				delta = current;
			m_handle.seek(current-delta, SEEK_SET);
			m_lpBufMax = m_lpBufStart + m_handle.get(m_lpBufStart, m_nBufSize);
			m_lpBufCur = m_lpBufStart + toUInt(delta);

			if (m_lpBufCur <= m_lpBufStart)
				return 0;
		}
		--m_lpBufCur;
		return 1;
	}

	void winx_call unget(int_type ch)
	{
		if (ch != endch)
			unget();
	}

	int_type winx_call peek()
	{
		if (m_lpBufCur < m_lpBufMax)
			return (uchar_type)*m_lpBufCur;

		char_type ch;
		if (!get(&ch, 1))
			return endch;

		--m_lpBufCur;
		return (uchar_type)ch;
	}

public:
	size_type winx_call read(void* lpBuf, size_type nMax)
	{
		return get( (char*)lpBuf, nMax );
	}

	HRESULT winx_call get16i(UINT16& val)
	{
		UINT cbRead = get( (char*)&val, sizeof(val) );
		if (cbRead != sizeof(val))
			return STG_E_READFAULT;

		_WinxByteSwap16(val);
		return S_OK;
	}
	
	HRESULT winx_call get16i(INT16& val)
	{
		UINT cbRead = get( (char*)&val, sizeof(val) );
		if (cbRead != sizeof(val))
			return STG_E_READFAULT;
		
		_WinxByteSwap16(val);
		return S_OK;
	}

	HRESULT winx_call get32i(UINT32& val)
	{
		UINT cbRead = get( (char*)&val, sizeof(val) );
		if (cbRead != sizeof(val))
			return STG_E_READFAULT;
		
		_WinxByteSwap32(val);
		return S_OK;
	}

	HRESULT winx_call get32i(INT32& val)
	{
		UINT cbRead = get( (char*)&val, sizeof(val) );
		if (cbRead != sizeof(val))
			return STG_E_READFAULT;
		
		_WinxByteSwap32(val);
		return S_OK;
	}
	
	template <class StrucType>
	HRESULT winx_call get_struct(StrucType& val)
	{
		UINT cbRead = get( (char*)&val, sizeof(val) );
		if (cbRead != sizeof(val))
			return STG_E_READFAULT;
		
		_WinxByteSwapStruct(val);
		return S_OK;
	}

#if defined(WINX_BYTESWAP)
	HRESULT winx_call get16i(IN UINT16 warray[], IN UINT count)
	{
		HRESULT hr = S_OK;
		for (UINT i = 0; i < count; ++i)
			hr = get16u(warray[i]);
		return hr;
	}
	HRESULT winx_call get16i(IN INT16 warray[], IN UINT count)
	{
		HRESULT hr = S_OK;
		for (UINT i = 0; i < count; ++i)
			hr = get16i(warray[i]);
		return hr;
	}
	HRESULT winx_call get32i(IN UINT32 dwarray[], IN UINT count)
	{
		HRESULT hr = S_OK;
		for (UINT i = 0; i < count; ++i)
			hr = get32u(warray[i]);
		return hr;
	}
	HRESULT winx_call get32i(IN INT32 dwarray[], IN UINT count)
	{
		HRESULT hr = S_OK;
		for (UINT i = 0; i < count; ++i)
			hr = get32i(warray[i]);
		return hr;
	}
	template <class StrucType>
	HRESULT winx_call get_struct(StrucType* array, UINT count)
	{
		HRESULT hr = S_OK;
		for (UINT i = 0; i < count; ++i)
			hr = get_struct(warray[i]);
		return hr;
	}
#else
	HRESULT winx_call get16i(IN UINT16 warray[], IN UINT count)
	{
		UINT cbToRead = sizeof(UINT16) * count;
		UINT cbRead = get( (char*)warray, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
	HRESULT winx_call get16i(IN INT16 warray[], IN UINT count)
	{
		UINT cbToRead = sizeof(INT16) * count;
		UINT cbRead = get( (char*)warray, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
	HRESULT get32i(IN UINT32 dwarray[], IN UINT count)
	{
		UINT cbToRead = sizeof(UINT32) * count;
		UINT cbRead = get( (char*)dwarray, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
	HRESULT get32i(IN INT32 dwarray[], IN UINT count)
	{
		UINT cbToRead = sizeof(INT32) * count;
		UINT cbRead = get( (char*)dwarray, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
	template <class StrucType>
	HRESULT get_struct(StrucType* array, UINT count)
	{
		UINT cbToRead = sizeof(StrucType) * count;
		UINT cbRead = get( (char*)array, cbToRead );
		if (cbRead != cbToRead)
			return STG_E_READFAULT;
		return S_OK;
	}
#endif // !defined(WINX_BYTESWAP)

public:
	HRESULT winx_call scan_uint(unsigned& val, unsigned radix = 10)
	{
		int_type ch = getnws();
		unsigned dig = DigitTable::toDigit(ch);
		if (dig < radix)
		{
			val = get_uint(radix, dig);
			return S_OK;
		}
		else
		{
			unget(ch);
			return STG_E_READFAULT;
		}
	}

	unsigned winx_call get_uint(unsigned radix = 10, unsigned preval = 0)
	{
		unsigned dig;
		int_type ch;
		while ( (dig = DigitTable::toDigit(ch = get())) < radix )
			preval = preval * radix + dig;
		unget(ch);
		return preval;
	}

	template <class _Cond>
	int_type winx_call get_not_if(const _Cond& cond)
	{
		int_type ch;
		while (	cond(ch = get()) );
		return ch;
	}

	int_type winx_call getnws()
	{
		int_type ch;
		while (	::isspace(ch = get()) );
		return ch;
	}

	template <class _Cond>
	size_type winx_call skip_if(const _Cond& cond)
	{
		WINX_ASSERT( !cond(endch) );

		size_type count = 0;
		int_type ch;
		while ( cond(ch = get()) )
			++count;
		unget(ch);
		return count;
	}

	void winx_call skipws()
	{
		int_type ch;
		while (	::isspace(ch = get()) );
		unget(ch);
	}

	void winx_call skip_eol()
	{
		int_type ch = get();
		if (ch == 0x0a)
		{
			ch = get();
			if (ch != 0x0d)
				unget(ch);
		}
		else if (ch == 0x0d)
		{
			ch = get();
			if (ch != 0x0a)
				unget(ch);
		}
	}

	template <class StringT>
	void winx_call getline(StringT& str)
	{
		size_type count = skip_if(CharType::NotIsEOL());
		str.resize(count);
		reget(count, &str[0], count);
		skip_eol();
	}

public:
	HRESULT winx_call open(LPCWSTR szFile)
	{
		if (m_handle.good())
			return E_ACCESSDENIED;
		return m_handle.open_to_read(szFile);
	}

	HRESULT winx_call open(LPCSTR szFile)
	{
		if (m_handle.good())
			return E_ACCESSDENIED;
		return m_handle.open_to_read(szFile);
	}

	void winx_call open_handle(Handle hFile)
	{
		if (!m_handle.good())
			m_handle.open_handle(hFile);
	}

	void winx_call close()
	{
		clear_cache();
		m_handle.close();
	}

	void winx_call attach(Handle hFile)
	{
		clear_cache();
		m_handle.attach(hFile);
	}

	Handle winx_call detach()
	{
		clear_cache();
		return m_handle.detach();
	}

	int winx_call operator!() const
	{
		return m_handle.bad();
	}

	int winx_call good() const
	{
		return m_handle.good();
	}

	int winx_call bad() const
	{
		return m_handle.bad();
	}
};

// -------------------------------------------------------------------------
// $Log: ArchiveImpl.h,v $
// Revision 1.4  2006/12/14 09:15:04  xushiwei
// work together with boost
//
// Revision 1.3  2006/12/03 07:51:46  xushiwei
// getline, reget, skip_eol
//
// Revision 1.2  2006/12/01 05:38:02  xushiwei
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

#endif /* __STDEXT_ARCHIVE_ARCHIVEIMPL_H__ */
