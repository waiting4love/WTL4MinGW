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
// Module: stdext/kmp/TestFinder.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-2 15:53:51
// 
// $Id: TestFinder.h,v 1.2 2006/12/03 07:52:27 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_KMP_TESTFINDER_H__
#define __STDEXT_KMP_TESTFINDER_H__

#ifndef __STDEXT_KMP_REFFINDER_H__
#include "RefFinder.h"
#endif

#ifndef _DEQUE_
#include <deque>
#endif

#ifndef _FSTREAM_
#include <fstream>
#endif

// -------------------------------------------------------------------------

template <class LogT>
class TestFinder : public TestCase
{
public:
	WINX_TEST_SUITE(TestFinder)
		WINX_TEST(testSearchInCStr)
		WINX_TEST(testSearchInArchive)
		WINX_TEST(testSearchInDeque)
		WINX_TEST(testSearchInFStream)
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	void testSearchInArchive(LogT& log)
	{
		std::string line;
		std::StdioReadArchive ar(__FILE__);

		std::kmp::Finder<char> finder("std::kmp::Finder<char>");
		HRESULT hr = finder.next(ar);
		AssertExp(hr == S_OK);

		ar.getline(line);
		log.trace("\nline =%s\n", line.c_str());
	}

	void testSearchInFStream(LogT& log)
	{
		std::string line;
		std::ifstream is(__FILE__);
		
		std::kmp::Finder<char> finder("std::ifstream");
		HRESULT hr = finder.istreamNext(is);
		AssertExp(hr == S_OK);

		std::getline(is, line);
		log.trace("\nline =%s\n", line.c_str());		
	}

	void testSearchInCStr(LogT& log)
	{
		const char* p;
		const char dest[] = "1234ababcde";
		
		std::kmp::Finder<char> finder("abc");
		HRESULT hr = finder.cstrNext(dest, &p);
		AssertExp(hr == S_OK);
		AssertExp(strcmp(p, "de") == 0);
	}

	void testSearchInDeque(LogT& log)
	{
		typedef std::deque<char> Container;

		const char destBuf[] = "1234ababcde";
		Container::iterator itFind;
		Container dest(sizeof(destBuf));
		std::copy(destBuf, destBuf+sizeof(destBuf), dest.begin());

		std::kmp::Finder<char> finder("abc");
		HRESULT hr = finder.iteratorNext(dest.begin(), dest.size(), &itFind);
		AssertExp(hr == S_OK);
		AssertExp(dest.end() - itFind == 3);
	}
};

// -------------------------------------------------------------------------
// $Log: TestFinder.h,v $
// Revision 1.2  2006/12/03 07:52:27  xushiwei
// TestFinder
//
// Revision 1.1  2006/12/02 08:00:45  xushiwei
// STL-Extension:
//  KMP-String-Find-Algorithm(class std::kmp::Finder/CaseFinder/NoCaseFinder)
//

#endif /* __STDEXT_KMP_TESTFINDER_H__ */
