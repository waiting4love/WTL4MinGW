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
// Module: stdext.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 14:22:12
// 
// $Id: stdext.h,v 1.3 2007/01/10 09:38:33 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_H__
#define __STDEXT_H__

#if (0)
#define STD_NO_TASKALLOC
#endif

// -------------------------------------------------------------------------

#ifndef __STLPATCH_H__
#include "stlpatch.h"
#endif

#ifndef __STDEXT_BASIC_H__
#include "stdext/Basic.h"
#endif

#ifndef __STDEXT_TESTCASE_H__
#include "stdext/TestCase.h"
#endif

#ifndef __STDEXT_ARRAY_H__
#include "stdext/Array.h"
#endif

#ifndef __STDEXT_STORAGE_H__
#include "stdext/Storage.h"
#endif

#ifndef __STDEXT_LOG_H__
#include "stdext/Log.h"
#endif

#ifndef __STDEXT_COUNTER_H__
#include "stdext/Counter.h"
#endif

#ifndef __STDEXT_MEMORY_H__
#include "stdext/Memory.h"
#endif

#ifndef __STDEXT_CHARTYPE_H__
#include "stdext/CharType.h"
#endif

#ifndef __STDEXT_STRING_H__
#include "stdext/String.h"
#endif

#ifndef __STDEXT_TYPEDPTRARRAY_H__
#include "stdext/TypedPtrArray.h"
#endif

#ifndef __STDEXT_SIMPLEMULTIMAP_H__
#include "stdext/SimpleMultiMap.h"
#endif

#ifndef __STDEXT_FILEBUF_H__
#include "stdext/FileBuf.h"
#endif

// -------------------------------------------------------------------------
// $Log: stdext.h,v $
// Revision 1.3  2007/01/10 09:38:33  xushiwei
// class Array, PriorityArray
//
// Revision 1.2  2006/12/22 10:19:55  xushiwei
// STL-Patch: std::basic_string::_Split function bugfix (vc6)
//
// Revision 1.4  2006/09/26 07:51:00  xushiwei
// STL-Extension:
//  TestCase(WINX_TEST_APP, WINX_TEST_CLASS, WINX_TEST_SUITE, WINX_TEST, WINX_TEST_SUITE_END)
//  UnitTestAssert(AssertExp, AssertEq, AssertEqBuf)
//
// Revision 1.3  2006/09/03 04:30:00  xushiwei
// STL-Extension:
//   Container: TypedPtrArray, InterfaceArray
//
// Revision 1.2  2006/08/19 04:40:48  xushiwei
// STL-Extension:
//   Memory(AutoFreeAlloc, RecycleBuffer, AutoArray, etc)
//   String Algorithm(trim, match, compare, etc), Container(SimpleMultiMap), CharType(isCSymbolFirstChar, etc)
//   Log(OutputLog, ErrorLog, FileLog, StringLog), PerformanceCounter, Diagnost(WINX_ASSERT, WINX_RUN_TEST, etc)
//

#endif /* __STDEXT_H__ */
