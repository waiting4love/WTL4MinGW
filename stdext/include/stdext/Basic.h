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
// Module: stdext/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: Basic.h,v 1.6 2007/01/10 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_BASIC_H__
#define __STDEXT_BASIC_H__

#if defined(_WIN32)
#define STD_SUPPORT_TASKALLOC	// for CoTaskMemAlloc
#endif

#if (0)
#define STD_NO_TASKALLOC
#define STD_ISOCPP_FOR
#define STD_EXIT
#define STATIC_ASSERT
#define ASSERT_ONCE
#define ASSERT_DERIVE
#define ASSERT_OK
#define REPORT
#define REPORTA
#define VERIFY_OK
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

// -------------------------------------------------------------------------

#ifndef _INC_STRING
#include <string.h>
#endif

#if defined(STD_SUPPORT_TASKALLOC) && !defined(STD_NO_TASKALLOC)
#ifndef _OBJBASE_H_
#include <objbase.h>		// for CoTaskMemAlloc, CoTaskMemFree
#endif
#endif

#ifndef assert
#include <assert.h>
#endif

#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#if !defined(_W64)
typedef signed char INT8, *PINT8;
typedef signed short INT16, *PINT16;
typedef unsigned char UINT8, *PUINT8;
typedef unsigned short UINT16, *PUINT16;
#endif

// -------------------------------------------------------------------------
// __NS_STD

#ifndef __NS_STD
#define __NS_STD			std::
#define __NS_STD_BEGIN		namespace std {
#define __NS_STD_END		}
#endif

// -------------------------------------------------------------------------
// C++ Compiler

#if defined(_MSC_VER) 
#	if (_MSC_VER > 1200)
#		ifndef WINX_VC_NET
#		define WINX_VC_NET
#		pragma warning(disable:4290)
		// A function is declared using exception specification, which Visual C++
		// accepts but does not implement
#		if (_MSC_VER > 1310)
#			define WINX_VC_NET_GE2005 // Version >= VS.NET 2005
#		endif
#		endif
#	else
#		ifndef WINX_VC6
#		define WINX_VC6
#		endif
#	endif
#else
#	if defined(__GNUG__) || defined(__GNUC__)
#		ifndef WINX_GCC
#		define WINX_GCC
#		endif
#	endif
#endif

// -------------------------------------------------------------------------
// winx_call

#ifndef winx_call
#	if defined(WINX_GCC)
#		define winx_call
#	else
#		define winx_call	__fastcall
#	endif
#endif

// -------------------------------------------------------------------------
// macro __noop - diagnost

#if !defined(__noop)
#	if defined(_MSC_VER)
#		if _MSC_VER <= 1200
#			define __noop 0
#		endif
#	else
inline int __cdecl _winx_null_func(const void* fmt, ...) { return 0; }
inline int __cdecl _winx_null_func(int nLevel, const void* fmt, ...) { return 0; }
#	define __noop  _winx_null_func
#	endif
#endif

// -------------------------------------------------------------------------
// macro for, __forceinline

#if defined(STD_ISOCPP_FOR)
#	if defined(_MSC_VER) && (_MSC_VER <= 1200)
#	undef for
#	define for	if (0); else for
#	endif
#endif

#if !defined(_MSC_VER) && !defined(__forceinline)
#define __forceinline inline
#endif

// -------------------------------------------------------------------------
// msvcrt

#ifndef __STDEXT_MSVC_MSVCRT_H__
#include "msvc/msvcrt.h"
#endif

// -------------------------------------------------------------------------
// countof - for general use

#ifndef	countof
#define countof(array)						(sizeof(array)/sizeof(array[0]))
#endif

// -------------------------------------------------------------------------
// parent_class_ptr - for general use

#ifndef _offsetof
#	if defined(X_CC_GCC)
#		define _offsetof(coclass, member)	( (size_t)&((coclass*)64)->member - 64 )
#	else
#		define _offsetof(coclass, member)	( (size_t)&((coclass*)0)->member )
#	endif
#endif

#ifndef parent_class_ptr
#define parent_class_ptr(ParentClass, member)	\
	( (ParentClass*)((const char*)(this) - _offsetof(ParentClass, member)) )
#endif

// -------------------------------------------------------------------------
// __deprecated(msg) - for code management

#if !defined(__deprecated)

#if !defined(_MSC_VER) || (_MSC_VER <= 1200)
#define __deprecated(msg)
#elif defined(_MSC_FULL_VER) && (_MSC_FULL_VER >= 140050320)
#define __deprecated(msg)		__declspec(deprecated(msg))
#else
#define __deprecated(msg)		__declspec(deprecated)
#endif

#endif

// -------------------------------------------------------------------------
// __typename

#if !defined(__typename)
#define __typename(Type)		typeid(Type).name()
#endif

// -------------------------------------------------------------------------
// TokenVal

#ifndef TokenVal
#define TokenVal(p, type)		(*((type*&)(p))++)
#endif

// -------------------------------------------------------------------------
// DbgFillMemory

#if !defined(DbgFillMemory)

#ifdef _DEBUG
#define DbgFillMemory(p, len)	memset(p, 0xcd, len)
#else
#define DbgFillMemory(p, len)	0
#endif

#endif

// -------------------------------------------------------------------------
// RELEASE

#ifndef RELEASE
#define RELEASE(pInterface) 												\
	if (pInterface) {														\
		pInterface->Release();												\
		pInterface = NULL;													\
	}
#endif

// -------------------------------------------------------------------------
// CHECK..EXIT

#if (0)
#define EXIT							STD_EXIT
#endif

#ifndef CHECK_BOOLEX
#define CHECK_BOOLEX(result, expr)											\
	if (!(result))															\
	{																		\
		expr;																\
		goto EXIT;															\
	}
#define	CHECK_BOOL(result)				CHECK_BOOLEX(result, 0)
#define CHECK(hr)						CHECK_BOOLEX(SUCCEEDED(hr), 0)
#endif

// -------------------------------------------------------------------------
// WINX_ASSERT/WINX_TRACE - diagnost

#if defined(ASSERT)
#define WINX_ASSERT(e)		ASSERT(e)
#elif defined(_ASSERTE)
#define WINX_ASSERT(e)		_ASSERTE(e)
#else
#ifdef _DEBUG
#define WINX_ASSERT(e)		assert(e)
#else
#define WINX_ASSERT(e)
#endif
#endif

#if defined(ASSERT_ONCE)
#define WINX_ASSERT_ONCE(e)	ASSERT_ONCE(e)
#elif defined(_DEBUG)
#define WINX_ASSERT_ONCE(e)													\
do {																		\
	static int fReported = 0;												\
	if (!fReported && !(e))													\
	{																		\
		WINX_ASSERT(e);														\
		fReported = 1;														\
	}																		\
} while(0)
#else
#define WINX_ASSERT_ONCE(e)	0
#endif

#if defined(REPORTA)
#define WINX_REPORT(msg)	REPORTA(msg)
#elif defined(REPORT)
#define WINX_REPORT(msg)	REPORT(msg)
#else
#define WINX_REPORT(msg)	WINX_ASSERT(!msg)
#endif

#if defined(VERIFY)
#define WINX_VERIFY(e)		VERIFY(e)
#elif defined(_DEBUG)
#define WINX_VERIFY(e)		WINX_ASSERT(e)
#else
#define WINX_VERIFY(e)		e
#endif

#if defined(_DEBUG)
#define WINX_VERIFY_EQ(e, v)	WINX_VERIFY((e) == v)
#else
#define WINX_VERIFY_EQ(e, v)	e
#endif

#if defined(ATLTRACE)
#define WINX_TRACE			ATLTRACE
#elif defined(TRACEA)
#define WINX_TRACE			TRACEA
#elif defined(TRACE)
#define WINX_TRACE			TRACE
#else
#define WINX_TRACE			__noop
#endif

#if defined(ASSERT_OK)
#define WINX_ASSERT_OK(hr)	ASSERT_OK(hr)
#else
#define WINX_ASSERT_OK(hr)	WINX_ASSERT(SUCCEEDED(hr))
#endif

#if defined(VERIFY_OK)
#define WINX_VERIFY_OK(e)	VERIFY_OK(e)
#elif defined(_DEBUG)
#define WINX_VERIFY_OK(e)	WINX_ASSERT_OK(e)
#else
#define WINX_VERIFY_OK(e)	e
#endif

// -------------------------------------------------------------------------
// WINX_DEBUG_ONLY

#if defined(_DEBUG)
#define WINX_DEBUG_ONLY(e)	e
#else
#define WINX_DEBUG_ONLY(e)	NULL
#endif

// -------------------------------------------------------------------------
// WINX_ASSERT_DERIVE - diagnost

#if defined(ASSERT_DERIVE)
#define WINX_ASSERT_DERIVE(TestClass, BaseClass)	ASSERT_DERIVE(TestClass, BaseClass)
#else
#define WINX_ASSERT_DERIVE(TestClass, BaseClass)	0
#endif

// -------------------------------------------------------------------------
// WINX_STATIC_ASSERT

#if defined(STATIC_ASSERT)
#define WINX_STATIC_ASSERT(e)	STATIC_ASSERT(e)
#else
#define WINX_STATIC_ASSERT(e)	WINX_ASSERT(e)
#endif

// -------------------------------------------------------------------------
// WINX_TRY { ... } WINX_TRY_END;

#define WINX_TRY				try
#define WINX_TRY_END			catch (...) { WINX_ASSERT_ONCE(!"WINX_TRY_END"); }

// -------------------------------------------------------------------------
// WINX_BREAK

#if defined(_CrtDbgBreak)
#define WINX_BREAK()			_CrtDbgBreak()
#else
#define WINX_BREAK()			__asm { int 3 }
#endif

// -------------------------------------------------------------------------
// WINX_DBG_BREAK

#if defined(_DEBUG)
#define WINX_DBG_BREAK()		WINX_BREAK()
#else
#define WINX_DBG_BREAK()
#endif

// -------------------------------------------------------------------------
// ABS, ROUND, MAX, MIN - for general use

#ifndef ABS
#define ABS(x)			((x) >= 0 ? (x) : -(x))
#endif

#ifndef ROUND
#define ROUND(x, y)		(((x)+((y)-1)) & ~((y)-1))
#endif

#ifndef MAX
#define MAX(x, y)		((x) > (y) ? (x) : (y))
#define MIN(x, y)		((x) < (y) ? (x) : (y))
#endif

// -------------------------------------------------------------------------
// WINX_PTHIS - for general use

#ifndef WINX_PTHIS
#define WINX_PTHIS			(static_cast<FinalClass*>(this))
#define WINX_NULL_PTHIS		(static_cast<FinalClass*>(0))
#endif

// -------------------------------------------------------------------------
// class NullClass

__NS_STD_BEGIN

class NullClass {};

__NS_STD_END

// -------------------------------------------------------------------------
// uuidof

#ifndef __STDEXT_MSVC_UUIDOF_H__
#include "msvc/uuidof.h"
#endif

// -------------------------------------------------------------------------
// TestCase class

#ifndef __STDEXT_TESTCASE_H__
#include "TestCase.h"
#endif

typedef std::TestCase TestCase;

// -------------------------------------------------------------------------
// Log class

#ifndef __STDEXT_LOG_H__
#include "Log.h"
#endif

// -------------------------------------------------------------------------
// $Log: Basic.h,v $
// Revision 1.6  2007/01/10 09:38:10  xushiwei
// STD_ISOCPP_FOR
//
// Revision 1.5  2006/12/20 08:55:14  xushiwei
// STL-Extension: Container(Array), Log(MultiStorage, MultiLog)
//
// Revision 1.4  2006/09/26 07:51:00  xushiwei
// STL-Extension:
//  TestCase(WINX_TEST_APP, WINX_TEST_CLASS, WINX_TEST_SUITE, WINX_TEST, WINX_TEST_SUITE_END)
//  UnitTestAssert(AssertExp, AssertEq, AssertEqBuf)
//
// Revision 1.3  2006/08/26 09:12:37  xushiwei
// vs2005 support
//
// Revision 1.2  2006/08/19 04:40:48  xushiwei
// STL-Extension:
//   Memory(AutoFreeAlloc, RecycleBuffer, AutoArray, etc)
//   String Algorithm(trim, match, compare, etc), Container(SimpleMultiMap), CharType(isCSymbolFirstChar, etc)
//   Log(OutputLog, ErrorLog, FileLog, StringLog), PerformanceCounter, Diagnost(WINX_ASSERT, WINX_RUN_TEST, etc)
//

#endif /* __STDEXT_BASIC_H__ */
