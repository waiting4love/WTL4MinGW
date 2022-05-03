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
// Module: stdext/RegEx.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-10 16:10:02
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_REGEX_H__
#define __STDEXT_REGEX_H__

#if (0)
#define WINX_USE_DEFBOOST
#endif

// -------------------------------------------------------------------------

#if defined(WINX_USE_DEFBOOST)

#ifndef BOOST_RE_REGEX_HPP
#include <boost/regex.hpp>
#endif

#else

#ifndef BOOST_RE_REGEX_HPP
#include "../../../boost/boost/regex.hpp"
#endif

#endif

// -------------------------------------------------------------------------
// class TestRegEx

template <class LogT>
class TestRegEx : public TestCase
{
public:
	WINX_TEST_SUITE(TestRegEx);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	void test(LogT& log)
	{
	}
};

// -------------------------------------------------------------------------
// $Log: $

#endif /* __STDEXT_REGEX_H__ */
