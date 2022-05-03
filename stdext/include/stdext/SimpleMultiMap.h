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
// Module: stdext/SimpleMultiMap.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: SimpleMultiMap.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_SIMPLEMULTIMAP_H__
#define __STDEXT_SIMPLEMULTIMAP_H__

#ifndef __STDEXT_MEMORY_H__
#include "Memory.h"
#endif

#ifndef _VECTOR_
#include <vector>
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------

template <class _K, class _Ty, class _Alloc = DefaultStaticAlloc>
class SimpleMultiMap
{
private:
	typedef SimpleMultiMap<_K, _Ty> _Myt;

	struct _Node
	{
		_Node* next;
		_Ty data;
		_Node(const _Ty& dataArg, _Node* nextArg)
			: data(dataArg), next(nextArg) {}
	};
	struct _NullBucket
	{
		_Node* null;
		_Node* first;
		_NullBucket() : null(NULL) { first = (_Node*)this; }
	};
	struct _Bucket : _NullBucket
	{
		_K key;
		_Bucket(const _K& keyArg) : key(keyArg) {}
	};
	typedef void* _BucketPtr;
	typedef std::vector<_BucketPtr> _Buckets;

	SimpleMultiMap(const SimpleMultiMap&);
	void operator=(const SimpleMultiMap&);

private:
	_Buckets m_data;
	_Alloc m_alloc;

public:
	typedef size_t size_type;
	typedef size_t cookie_type;
	typedef std::pair<_K, _Ty> value_type;
	typedef value_type const_reference;

private:
	static _Bucket* winx_call _bucket_node(_Node* it)
	{
		while (it->next != NULL)
			it = it->next;
		return (_Bucket*)it;
	}
	static bool winx_call _bucket_empty(_Bucket* bucket)
	{
		return bucket->first == (_Node*)bucket;
	}
	struct _Eq {
		const _K& key;
		_Eq(const _K& keyArg) : key(keyArg) {}
		bool operator()(_BucketPtr bucket) {
			return ((_Bucket*)bucket)->key == key;
		}
	};
	struct _Clear {
		_Alloc& m_alloc;
		_Clear(_Alloc& alloc) : m_alloc(alloc) {
		}
		void winx_call operator()(_BucketPtr bucketArg) {
			_Bucket* bucket = (_Bucket*)bucketArg;
			_Node* next;
			for (_Node* it = bucket->first; it != (_Node*)bucket; it = next)
			{
				next = it->next;
				STD_DELETE(m_alloc, it);
			}
			STD_DELETE(m_alloc, bucket);
		}
	};

public:
	SimpleMultiMap() {}
	SimpleMultiMap(_Alloc alloc) : m_alloc(alloc) {}
	~SimpleMultiMap()
	{
		clear();
	}
	
	void winx_call clear()
	{
		if (m_data.size())
		{
			std::for_each(m_data.begin(), m_data.end(), _Clear(m_alloc));
			m_data.clear();
		}
	}
	
	void winx_call erase(cookie_type lCookie)
	{
		_Node* it = (_Node*)lCookie;
		_Bucket* bucket = _bucket_node(it);
		_Node** pprev = &bucket->first;
		while (*pprev != it)
		{
			pprev = &it->next;
		}
		*pprev = it->next;
		STD_DELETE(m_alloc, it);
		if (_bucket_empty(bucket))
		{
			m_data.erase(
				std::find(m_data.begin(), m_data.end(), (_BucketPtr)bucket)
				);
			m_alloc.Delete(bucket);
		}
	}

	struct position
	{
		_Node* it;
		_Bucket* owner;
		position() {}
		position(_Bucket* bucket) : owner(bucket), it(bucket->first) {}

		void winx_call operator++() { it = it->next; }
		bool winx_call good() const  { return it->next != NULL; }
		bool winx_call operator==(const position& b) const { return it == b.it; }
		bool winx_call operator!=(const position& b) const { return it != b.it; }
		
		cookie_type winx_call cookie() const { return (cookie_type)it; }

		value_type winx_call operator*() const {
			return value_type(owner->key, it->data);
		}

		template <class LogT>
		void winx_call trace(LogT& log) const {
			while (good()) {
				value_type val = **this;
			}
		}
	};

	position winx_call equal_range(const _K& key) const
	{
		_Buckets::const_iterator it = 
			std::find_if(m_data.begin(), m_data.end(), _Eq(key));
		if (it != m_data.end())
			return (_Bucket*)*it;
		else
		{
			static _NullBucket null;
			return (_Bucket*)&null;
		}
	}

	cookie_type winx_call insert(const value_type& value)
	{
		_Bucket* bucket;
		_Buckets::const_iterator it = 
			std::find_if(m_data.begin(), m_data.end(), _Eq(value.first));
		if (it != m_data.end())
			bucket = (_Bucket*)*it;
		else
		{
			bucket = STD_NEW(m_alloc, _Bucket)(value.first);
			m_data.push_back(bucket);
		}
		bucket->first = STD_NEW(m_alloc, _Node)(value.second, bucket->first);
		return (cookie_type)bucket->first;
	}

	void winx_call swap(_Myt& o)
	{
		m_data.swap(o.m_data);
		m_alloc.swap(o.m_alloc);
	}
};

// -------------------------------------------------------------------------
// class TestSimpleMultiMap

template <class LogT>
class TestSimpleMultiMap
{
public:
	static void doTest(LogT& log)
	{
		typedef std::SimpleMultiMap<int, int> MultiMap;
		std::PerformanceCounter counter;
		{
			MultiMap mm;
			MultiMap::cookie_type c1 = mm.insert(MultiMap::value_type(1, 2));
			MultiMap::cookie_type c2 = mm.insert(MultiMap::value_type(1, 3));
			MultiMap::cookie_type c3 = mm.insert(MultiMap::value_type(1, 4));
		}
		counter.trace(log);
	}
};

// -------------------------------------------------------------------------
// $Log: SimpleMultiMap.h,v $
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

__NS_STD_END

#endif /* __STDEXT_SIMPLEMULTIMAP_H__ */
