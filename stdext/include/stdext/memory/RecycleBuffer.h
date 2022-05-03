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
// Module: stdext/memory/RecycleBuffer.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:46:39
// 
// $Id: RecycleBuffer.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MEMORY_RECYCLEBUFFER_H__
#define __STDEXT_MEMORY_RECYCLEBUFFER_H__

#ifndef __STDEXT_MEMORY_BASIC_H__
#include "Basic.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class RecycleBuffer

template <class _Ty, int _RoundSize = 64>
class RecycleBuffer
{
private:
	_Ty* m_buf;
	size_t m_size;
	
public:
	typedef _Ty* pointer;

public:
	RecycleBuffer() : m_size(0), m_buf(NULL) {}
	explicit RecycleBuffer(size_t nInitial)
		: m_size(nInitial)
	{
		m_buf = (_Ty*)malloc(sizeof(_Ty)*m_size);
	}
	~RecycleBuffer()
	{
		if (m_buf)
			free(m_buf);
	}
	
	pointer winx_call data() const {
		return m_buf;
	}
	
	pointer winx_call begin() const {
		return m_buf;
	}
	
	pointer winx_call reserve(size_t nSize)
	{
		if (nSize > m_size) {
			free(m_buf);
			m_size = ROUND(nSize, _RoundSize);
			m_buf = (_Ty*)malloc(sizeof(_Ty)*m_size);
		}
		return m_buf;
	}
};

// -------------------------------------------------------------------------
// class RecycleBlockAlloc

class RecycleBlockAlloc
{
private:
	struct _Block {
		_Block* next;
	};
	_Block* m_freeList;

public:
	RecycleBlockAlloc() : m_freeList(NULL) {}
	~RecycleBlockAlloc()
	{
		clear();
	}

public:
	void* winx_call allocate(size_t cb)
	{
		if (m_freeList)
		{
			MEMORY_ASSERT(_msize(m_freeList) == cb);
			_Block* blk = m_freeList;
			m_freeList = blk->next;
			return blk;
		}
		return malloc(cb);
	}

	void winx_call deallocate(void* p)
	{
		_Block* blk = (_Block*)p;
		blk->next = m_freeList;
		m_freeList = blk;
	}

	void winx_call clear()
	{
		while (m_freeList)
		{
			_Block* blk = m_freeList;
			m_freeList = blk->next;
			free(blk);
		}
	}
};

// -------------------------------------------------------------------------
// $Log: RecycleBuffer.h,v $
// Revision 1.1  2006/10/18 12:13:39  xushiwei
// stdext as independent component
//
// Revision 1.2  2006/08/19 04:40:48  xushiwei
// STL-Extension:
//   Memory(AutoFreeAlloc, RecycleBuffer, AutoArray, etc)
//   String Algorithm(trim, match, compare, etc), Container(SimpleMultiMap), CharType(isCSymbolFirstChar, etc)
//   Log(OutputLog, ErrorLog, FileLog, StringLog), PerformanceCounter, Diagnost(WINX_ASSERT, WINX_RUN_TEST, etc)
//

__NS_STD_END

#endif /* __STDEXT_MEMORY_RECYCLEBUFFER_H__ */
