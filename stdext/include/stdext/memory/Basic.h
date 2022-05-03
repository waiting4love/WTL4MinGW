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
// Module: stdext/memory/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 12:25:29
// 
// $Id: Basic.h,v 1.2 2006/12/02 06:44:45 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MEMORY_BASIC_H__
#define __STDEXT_MEMORY_BASIC_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

// -------------------------------------------------------------------------

#ifndef _NEW_
#include <new>
#endif

#ifndef _INC_STDIO
#include <stdio.h>
#endif

#ifndef _INC_STDLIB
#include <stdlib.h>
#endif

#ifndef _INC_MALLOC
#include <malloc.h>
#endif

#ifndef _ALGORITHM_
#include <algorithm>
#endif

#pragma pack() // 为了不受其他头文件的字节对齐干扰

// =========================================================================

#ifndef MEMORY_BLOCK_SIZE
#define MEMORY_BLOCK_SIZE	2048	// 最佳的内存块大小，经验值
#endif

#define MEMORY_ASSERT(e)		WINX_ASSERT(e)
#define MEMORY_STATIC_ASSERT(e)	WINX_STATIC_ASSERT(e)
#define MEMORY_BREAK()			WINX_BREAK()

// =========================================================================
// ConstructorTraits, DestructorTraits

__NS_STD_BEGIN

struct ArrayDestructHeader
{
	size_t count;
};

typedef void winx_call __FnDestructor(void* data);
typedef __FnDestructor* DestructorType;

template <class Type>
struct ConstructorTraits
{
	static Type* winx_call construct(void* data)
	{
		return new(data) Type;
	}

	static Type* winx_call constructArray(Type* array, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
			new(array + i) Type;
		return array;
	}
};

template <class Type>
struct DestructorTraits
{
	static void winx_call destruct(void* data)
	{
		((Type*)data)->~Type();
	}

	static void winx_call _destructArray(Type* array, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
			array[i].~Type;
	}

	static void winx_call destructArray(void* array)
	{
		ArrayDestructHeader* hdr = (ArrayDestructHeader*)array;
		Type* data = (Type*)(hdr + 1);
		for (Type* dataEnd = data + hdr->count; data != dataEnd; ++data)
			data->~Type();
	}
};

template <class AllocT, class Type>
inline Type* winx_call __newArray(AllocT& alloc, Type* zero, size_t count, DestructorType fn)
{
	MEMORY_ASSERT(zero == NULL);
	MEMORY_ASSERT(std::DestructorTraits<Type>::destructArray == fn);

	typedef ArrayDestructHeader HeaderT;

	HeaderT* hdr = (HeaderT*)alloc.allocate(sizeof(HeaderT) + sizeof(Type)*count, fn);
	hdr->count = count;

	return std::ConstructorTraits<Type>::constructArray((Type*)(hdr+1), count);
}

template <class AllocT, class Type>
inline Type* winx_call __newArray(AllocT& alloc, Type* zero, size_t count, int fnZero)
{
	MEMORY_ASSERT(zero == NULL);
	MEMORY_ASSERT(fnZero == 0);

	Type* data = (Type*)alloc.allocate(sizeof(Type)*count);

	return std::ConstructorTraits<Type>::constructArray(data, count);
}

#if defined(_DEBUG)

template <class AllocT, class Type>
inline Type* winx_call __newArray(
	AllocT& alloc, Type* zero, size_t count, DestructorType fn, LPCSTR szFile, int nLine)
{
	MEMORY_ASSERT(zero == NULL);
	MEMORY_ASSERT(std::DestructorTraits<Type>::destructArray == fn);

	typedef ArrayDestructHeader HeaderT;

	HeaderT* hdr = (HeaderT*)alloc.allocate(sizeof(HeaderT) + sizeof(Type)*count, fn, szFile, nLine);
	hdr->count = count;

	return std::ConstructorTraits<Type>::constructArray((Type*)(hdr+1), count);
}

template <class AllocT, class Type>
inline Type* winx_call __newArray(
	AllocT& alloc, Type* zero, size_t count, int fnZero, LPCSTR szFile, int nLine)
{
	MEMORY_ASSERT(zero == NULL);
	MEMORY_ASSERT(fnZero == 0);

	Type* data = (Type*)alloc.allocate(sizeof(Type)*count, szFile, nLine);

	return std::ConstructorTraits<Type>::constructArray(data, count);
}

#endif

__NS_STD_END

// =========================================================================
// STD_NO_DESTRUCTOR

#define STD_NO_DESTRUCTOR(Type)												\
__NS_STD_BEGIN																\
template <>																	\
struct DestructorTraits< Type >												\
{																			\
	enum { destruct = 0 };													\
	enum { destructArray = 0 };												\
	static void winx_call _destructArray(Type* array, size_t count) {}		\
};																			\
__NS_STD_END

#define STD_INT_NO_DESTRUCTOR(Type)											\
	STD_NO_DESTRUCTOR(unsigned Type);										\
	STD_NO_DESTRUCTOR(signed Type)

// -------------------------------------------------------------------------
// STD_NO_CONSTRUCTOR

#define STD_NO_CONSTRUCTOR(Type)											\
__NS_STD_BEGIN																\
template <>																	\
struct ConstructorTraits< Type >											\
{																			\
	static Type* winx_call construct(void* data) {							\
		return (Type*)data;													\
	}																		\
	static Type* winx_call constructArray(Type* array, size_t count) {		\
		return array;														\
	}																		\
};																			\
__NS_STD_END

#define STD_INT_NO_CONSTRUCTOR(Type)										\
	STD_NO_CONSTRUCTOR(unsigned Type);										\
	STD_NO_CONSTRUCTOR(signed Type)

// -------------------------------------------------------------------------
// C Standard Types Support

#define STD_DECL_CTYPE(Type)												\
	STD_NO_CONSTRUCTOR(Type);												\
	STD_NO_DESTRUCTOR(Type)

#define STD_DECL_INT_CTYPE(Type)											\
	STD_NO_CONSTRUCTOR(Type);												\
	STD_INT_NO_DESTRUCTOR(Type)

// -------------------------------------------------------------------------

STD_DECL_CTYPE(void);
STD_DECL_CTYPE(bool);
STD_DECL_CTYPE(float);
STD_DECL_CTYPE(double);

STD_DECL_INT_CTYPE(int);
STD_DECL_INT_CTYPE(char);
STD_DECL_INT_CTYPE(short);
STD_DECL_INT_CTYPE(long);

// =========================================================================
// MEMORY_DBG_NEW_ARG

#if defined(_DEBUG)
#define MEMORY_FILE_LINE_ARG		,__FILE__, __LINE__
#else
#define MEMORY_FILE_LINE_ARG
#endif

#define MEMORY_NEW_ARG(Type)					sizeof(Type), std::DestructorTraits<Type>::destruct
#define MEMORY_DBG_NEW_ARG(Type)				MEMORY_NEW_ARG(Type) MEMORY_FILE_LINE_ARG

#define MEMORY_NEW_ARRAY_ARG(Type, count)		(Type*)0, (count), std::DestructorTraits<Type>::destructArray
#define MEMORY_DBG_NEW_ARRAY_ARG(Type, count)	MEMORY_NEW_ARRAY_ARG(Type, count) MEMORY_FILE_LINE_ARG

#define MEMORY_DBG_ALLOC_ARG(Type)				sizeof(Type) MEMORY_FILE_LINE_ARG
#define MEMORY_DBG_ALLOC_ARRAY_ARG(Type, count)	sizeof(Type)*(count) MEMORY_FILE_LINE_ARG

// =========================================================================
// NEW, NEW_ARRAY, ALLOC, ALLOC_ARRAY

#define STD_NEW(alloc, Type)					::new((alloc).allocate(MEMORY_DBG_NEW_ARG(Type))) Type
#define STD_NEW_ARRAY(alloc, Type, count) 		std::__newArray(alloc, MEMORY_DBG_NEW_ARRAY_ARG(Type, count))

#define STD_ALLOC(alloc, Type)					((Type*)(alloc).allocate(MEMORY_DBG_ALLOC_ARG(Type)))
#define STD_ALLOC_ARRAY(alloc, Type, count)		((Type*)(alloc).allocate(MEMORY_DBG_ALLOC_ARRAY_ARG(Type, count)))

// =========================================================================
// DELETE, DELETE_ARRAY

__NS_STD_BEGIN

template <class AllocT, class Type>
inline void winx_call __delete(AllocT& alloc, Type* o)
{
	o->~Type();
	alloc.deallocate((void*)o, sizeof(Type));
}

template <class AllocT, class Type>
inline void winx_call __deleteArray(AllocT& alloc, Type* array, size_t count)
{
	std::DestructorTraits<Type>::_destructArray(array, count);
	alloc.deallocate((void*)array, sizeof(Type)*count);
}

inline void winx_call enableMemoryLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

__NS_STD_END

#undef	STD_DELETE
#define STD_DELETE(alloc, o)					std::__delete(alloc, o)
#define STD_DELETE_ARRAY(alloc, array, count)	std::__deleteArray(alloc, array, count)

// =========================================================================

__NS_STD_BEGIN

inline void winx_call swap(void* a, void* b, size_t cb)
{
	void* t = _alloca(cb);
	memcpy(t, a, cb);
	memcpy(a, b, cb);
	memcpy(b, t, cb);
}

struct StdLibAlloc
{
	static void* winx_call allocate(size_t cb)						{ return malloc(cb); }
	static void* winx_call allocate(size_t cb, DestructorType fn)	{ return malloc(cb); }
	static void* winx_call allocate(size_t cb, int fnZero)			{ return malloc(cb); }

	static void winx_call deallocate(void* p)			{ free(p); }
	static void winx_call deallocate(void* p, size_t)	{ free(p); }
	static void winx_call swap(StdLibAlloc& o)			{}

#if defined(_DEBUG)
	static void* winx_call allocate(size_t cb, LPCSTR szFile, int nLine)
		{ return _malloc_dbg(cb, _NORMAL_BLOCK, szFile, nLine); }

	static void* winx_call allocate(size_t cb, DestructorType fn, LPCSTR szFile, int nLine)
		{ return _malloc_dbg(cb, _NORMAL_BLOCK, szFile, nLine); }
	
	static void* winx_call allocate(size_t cb, int fnZero, LPCSTR szFile, int nLine)
		{ return _malloc_dbg(cb, _NORMAL_BLOCK, szFile, nLine); }
#endif
};

typedef StdLibAlloc DefaultStaticAlloc;

// -------------------------------------------------------------------------

#if defined(STD_SUPPORT_CRTDBG) && !defined(STD_NO_TASKALLOC)

struct CoTaskAlloc
{
	static void* winx_call allocate(size_t cb)						{ return CoTaskMemAlloc(cb); }
	static void* winx_call allocate(size_t cb, DestructorType fn)	{ return CoTaskMemAlloc(cb); }
	static void* winx_call allocate(size_t cb, int fnZero)			{ return CoTaskMemAlloc(cb); }
	
	static void winx_call deallocate(void* p)			{ CoTaskMemFree(p); }
	static void winx_call deallocate(void* p, size_t)	{ CoTaskMemFree(p); }
	static void winx_call swap(CoTaskAlloc& o)			{}
	
#if defined(_DEBUG)
	static void* winx_call allocate(size_t cb, LPCSTR szFile, int nLine)
		{ return CoTaskMemAlloc(cb); }
	
	static void* winx_call allocate(size_t cb, DestructorType fn, LPCSTR szFile, int nLine)
		{ return CoTaskMemAlloc(cb); }
	
	static void* winx_call allocate(size_t cb, int fnZero, LPCSTR szFile, int nLine)
		{ return CoTaskMemAlloc(cb); }
#endif
};

#endif

// -------------------------------------------------------------------------

template <DWORD uFlags = HEAP_GENERATE_EXCEPTIONS>
class HeapMemAllocBase
{
public:
	HANDLE m_hHeap;

public:
	HeapMemAllocBase(HANDLE hHeap) : m_hHeap(hHeap) {}

	void* winx_call allocate(size_t cb)					   { return HeapAlloc(m_hHeap, uFlags, cb); }
	void* winx_call allocate(size_t cb, DestructorType fn) { return HeapAlloc(m_hHeap, uFlags, cb); }
	void* winx_call allocate(size_t cb, int fnZero)		   { return HeapAlloc(m_hHeap, uFlags, cb); }
	
	void winx_call deallocate(void* p)					{ HeapFree(m_hHeap, uFlags, p); }
	void winx_call deallocate(void* p, size_t)			{ HeapFree(m_hHeap, uFlags, p); }	
	void winx_call swap(HeapMemAllocBase& o)			{ std::swap(m_hHeap, o.m_hHeap); }
	
#if defined(_DEBUG)
	void* winx_call allocate(size_t cb, LPCSTR szFile, int nLine)
		{ return HeapAlloc(m_hHeap, uFlags, cb); }
	
	void* winx_call allocate(size_t cb, DestructorType fn, LPCSTR szFile, int nLine)
		{ return HeapAlloc(m_hHeap, uFlags, cb); }
	
	void* winx_call allocate(size_t cb, int fnZero, LPCSTR szFile, int nLine)
		{ return HeapAlloc(m_hHeap, uFlags, cb); }
#endif
};

class HeapMemAlloc
{
public:
	static HANDLE hProcessHeap;

	static void* winx_call allocate(size_t cb)					  { return HeapAlloc(hProcessHeap, 0, cb); }
	static void* winx_call allocate(size_t cb, DestructorType fn) { return HeapAlloc(hProcessHeap, 0, cb); }
	static void* winx_call allocate(size_t cb, int fnZero)		  { return HeapAlloc(hProcessHeap, 0, cb); }
	
	static void winx_call deallocate(void* p)				{ HeapFree(hProcessHeap, 0, p); }
	static void winx_call deallocate(void* p, size_t)		{ HeapFree(hProcessHeap, 0, p); }
	static void winx_call swap(HeapMemAlloc& o)				{}
	
#if defined(_DEBUG)
	static void* winx_call allocate(size_t cb, LPCSTR szFile, int nLine)
		{ return HeapAlloc(hProcessHeap, 0, cb); }
	
	static void* winx_call allocate(size_t cb, DestructorType fn, LPCSTR szFile, int nLine)
		{ return HeapAlloc(hProcessHeap, 0, cb); }
	
	static void* winx_call allocate(size_t cb, int fnZero, LPCSTR szFile, int nLine)
		{ return HeapAlloc(hProcessHeap, 0, cb); }
#endif
};

typedef HeapMemAlloc DefaultDynamicAlloc;

__NS_STD_END

// =========================================================================
// $Log: Basic.h,v $
// Revision 1.2  2006/12/02 06:44:45  xushiwei
// bugfix
//
// Revision 1.3  2006/09/02 01:45:57  xushiwei
// STL-Extension:
//   Log: print RECT, SIZE
//   Memory: HeapMemAlloc - DefaultDynamicAlloc
//
// Revision 1.2  2006/08/19 04:40:48  xushiwei
// STL-Extension:
//   Memory(AutoFreeAlloc, RecycleBuffer, AutoArray, etc)
//   String Algorithm(trim, match, compare, etc), Container(SimpleMultiMap), CharType(isCSymbolFirstChar, etc)
//   Log(OutputLog, ErrorLog, FileLog, StringLog), PerformanceCounter, Diagnost(WINX_ASSERT, WINX_RUN_TEST, etc)
//

#endif /* __STDEXT_MEMORY_BASIC_H__ */
