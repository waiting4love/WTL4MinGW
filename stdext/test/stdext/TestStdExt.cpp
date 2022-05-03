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
// Module: TestStdExt.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: TestStdExt.cpp,v 1.8 2006/12/03 07:52:55 xushiwei Exp $
// -----------------------------------------------------------------------*/

#define STD_FILTER_TEST_CASE
#define STD_NO_TASKALLOC
#include <stdext.h>
#include <stdext/Archive.h>
#include <stdext/kmp/TestFinder.h>
#include <stdext/memory/Pool.h>
#include <stdext/memory/GC.h>
#pragma comment(lib, "shlwapi.lib")

// -------------------------------------------------------------------------
// main

int main()
{
	WINX_TEST_APP(std::ErrorLog, "TestPriorityArray", "");

	WINX_TEST_CLASS(TestPool);
	WINX_TEST_CLASS(TestFinder);

	WINX_TEST_CLASS(std::TestGC);
	WINX_TEST_CLASS(std::TestPriorityArray);
//	WINX_TEST_CLASS(std::TestStreamArchive);
	WINX_TEST_CLASS(std::TestStdioArchive);
	WINX_TEST_CLASS(std::TestString);
	WINX_TEST_CLASS(std::TestCharType);
	WINX_TEST_CLASS(std::TestLog);
	WINX_TEST_CLASS(std::TestAutoFreeAlloc);
	WINX_TEST_CLASS(std::TestAutoArray);
	WINX_TEST_CLASS(std::TestSimpleMultiMap);
	WINX_TEST_CLASS(std::TestFileBuf);
	
	return 0;
}

// -------------------------------------------------------------------------
// $Log: TestStdExt.cpp,v $
// Revision 1.8  2006/12/03 07:52:55  xushiwei
// TestFinder
//
// Revision 1.6  2006/11/30 03:43:19  xushiwei
// TestStreamArchive, TestStdioArchive, TestPool (boost::pool)
//
// Revision 1.4  2006/09/26 08:05:07  xushiwei
// TestFileBuf
//
// Revision 1.3  2006/09/26 07:51:34  xushiwei
// STL-Extension:
//  TestCase(WINX_TEST_APP, WINX_TEST_CLASS, WINX_TEST_SUITE, WINX_TEST, WINX_TEST_SUITE_END)
//  UnitTestAssert(AssertExp, AssertEq, AssertEqBuf)
//
// Revision 1.2  2006/08/19 04:47:32  xushiwei
// STL-Extension:
//   Memory(AutoFreeAlloc, RecycleBuffer, AutoArray, etc)
//   String Algorithm(trim, match, compare, etc), Container(SimpleMultiMap), CharType(isCSymbolFirstChar, etc)
//   Log(OutputLog, ErrorLog, FileLog, StringLog), PerformanceCounter, Diagnost(WINX_ASSERT, WINX_TEST_CLASS, etc)
//
