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
// Module: stdext/memory/AutoArray.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:31:44
// 
// $Id: AutoArray.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MEMORY_AUTOARRAY_H__
#define __STDEXT_MEMORY_AUTOARRAY_H__

#ifndef __STDEXT_MEMORY_AUTOFREEALLOC_H__
#include "AutoFreeAlloc.h"
#endif

#ifndef _STRING_
#include <string>
#endif

__NS_STD_BEGIN

#pragma pack(1)

// -------------------------------------------------------------------------
// class AutoArray

//
// Note: 类型 T 要求是简单类型，没有拷贝构造函数
//
template <class T, class SizeT = UINT>
class AutoArray
{
	struct DataType
	{
		SizeT count;
		T data[1];
	};

private:
	DataType* m_data;

public:
	typedef T value_type;
	typedef T* pointer;

	typedef SizeT size_type;

	typedef const T* iterator;
	typedef const T* const_iterator;

	typedef value_type& reference;
	typedef const value_type& const_reference;

public:
	AutoArray()
	{
		static SizeT zeroArray = 0;
		m_data = (DataType*)&zeroArray;
	}
	
	template <class AllocT>
	AutoArray(const UINT count, AllocT& alloc)
	{
		reserve(count, alloc);
	}

	template <class AllocT>
	AutoArray(const T* data, const UINT count, AllocT& alloc)
	{
		assign(data, count, alloc);
	}

	template <class AllocT>
	AutoArray(const T* itFirst, const T* itLast, AllocT& alloc)
	{
		assign(itFirst, (UINT)(itLast - itFirst), alloc);
	}

	template <class AllocT>
	AutoArray(const T* data, AllocT& alloc)
	{
		assign(data, std::char_traits<T>::length(data), alloc);
	}

public:
	template <class AllocT>
	void winx_call assign(const T* data, const UINT count, AllocT& alloc)
	{
		UINT cb = (SizeT)count * sizeof(T);
		m_data = (DataType*)alloc.allocate(sizeof(SizeT) + cb);
		m_data->count = (SizeT)count;
		memcpy(m_data->data, data, cb);
	}

	template <class AllocT>
	void winx_call assign(const T* itFirst, const T* itLast, AllocT& alloc)
	{
		assign(itFirst, (UINT)(itLast - itFirst), alloc);
	}

	template <class AllocT>
	void winx_call assign(const T* data, AllocT& alloc)
	{
		assign(data, std::char_traits<T>::length(data), alloc);
	}

	template <class AllocT>
	pointer winx_call reserve(const UINT count, AllocT& alloc)
	{
		UINT cb = (SizeT)count * sizeof(T);
		m_data = (DataType*)alloc.allocate(sizeof(SizeT) + cb);
		m_data->count = (SizeT)count;
		return m_data->data;
	}

	template <class StringT>
	void winx_call copyTo(StringT& dest) const
	{
		dest.assign(begin(), end());
	}

	template <class StringT>
	void winx_call appendTo(StringT& dest) const
	{
		dest.insert(dest.end(), begin(), end());
	}

	template <class LogT>
	void winx_call trace(LogT& log) const
	{
		log.printString(begin(), end());
	}

public:
	bool winx_call empty() const				{ return m_data->count == 0; }
	size_type winx_call size() const			{ return m_data->count; }
	
	const_iterator winx_call begin() const		{ return m_data->data; }
	const_iterator winx_call end() const		{ return m_data->data + m_data->count; }

	pointer winx_call head()					{ return m_data->data; }
	pointer winx_call tail()					{ return m_data->data + m_data->count; }

	reference winx_call operator[](const UINT idx)			   { return m_data->data[idx]; }
	const_reference winx_call operator[](const UINT idx) const { return m_data->data[idx]; }
	const_reference winx_call at(const UINT idx) const		   { return m_data->data[idx]; }
};

// -------------------------------------------------------------------------
// class ShortString - 注意：并不以nil作为字符串结尾。

typedef AutoArray<WCHAR, WCHAR> ShortString;

// -------------------------------------------------------------------------
// class TestAutoArray

#ifdef __STDEXT_LOG_H__

template <class LogT>
class TestAutoArray
{
public:
	static void doTest(LogT& log)
	{
		std::AutoFreeAlloc alloc;
		
		std::AutoArray<char> ss("abc", alloc);
		log.printObj(ss).newline();

		ss.assign("2324", alloc);
		log.printObj(ss).newline();
		
		std::string str;
		ss.copyTo(str);
		log.printString(str).newline();

		ss.appendTo(str);
		log.printString(str).newline();
	}
};

#endif

// -------------------------------------------------------------------------
// $Log: AutoArray.h,v $
// Revision 1.1  2006/10/18 12:13:39  xushiwei
// stdext as independent component
//
// Revision 1.3  2006/09/26 07:51:00  xushiwei
// STL-Extension:
//  TestCase(WINX_TEST_APP, WINX_TEST_CLASS, WINX_TEST_SUITE, WINX_TEST, WINX_TEST_SUITE_END)
//  UnitTestAssert(AssertExp, AssertEq, AssertEqBuf)
//
// Revision 1.2  2006/08/19 04:40:48  xushiwei
// STL-Extension:
//   Memory(AutoFreeAlloc, RecycleBuffer, AutoArray, etc)
//   String Algorithm(trim, match, compare, etc), Container(SimpleMultiMap), CharType(isCSymbolFirstChar, etc)
//   Log(OutputLog, ErrorLog, FileLog, StringLog), PerformanceCounter, Diagnost(WINX_ASSERT, WINX_RUN_TEST, etc)
//

#pragma pack()

__NS_STD_END

#endif /* __STDEXT_MEMORY_AUTOARRAY_H__ */
