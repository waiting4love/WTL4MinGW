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
// Module: stdext/String.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:48:49
// 
// $Id: String.h,v 1.3 2007/01/10 09:35:16 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_STRING_H__
#define __STDEXT_STRING_H__

#ifndef __STDEXT_CHARTYPE_H__
#include "CharType.h"
#endif

#ifndef _STRING_
#include <string>
#endif

#ifndef _ALGORITHM_
#include <algorithm>
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------

typedef std::basic_string<char> AnsiString;
typedef std::basic_string<WCHAR> UniString;

// -------------------------------------------------------------------------
// end, length

template <class _It>
inline _It winx_call end(_It it)
{
	while (*it)
		++it;
	return it;
}

template <class _It>
inline _It winx_call length(_It it)
{
	_It first = it;
	while (*it)
		++it;
	return it - first;
}

// -------------------------------------------------------------------------
// trim

template <class _E>
inline void winx_call trim(std::basic_string<_E>& str, const _E* blanks)
{
	typedef std::basic_string<_E> string_type;
	typedef typename string_type::size_type size_type;

	size_type pos1 = str.find_first_not_of(blanks);
	if (pos1 == string_type::npos)
	{
		str.erase();
		return;
	}

	size_type count = str.find_last_not_of(blanks) - pos1 + 1;
	if (str.length() != count)
		str = str.substr(pos1, count);
}

inline void winx_call trim(std::basic_string<char>& str)
{
	const char blanks[] = { ' ', '\t', '\r', '\n', '\0' };
	trim(str, blanks);
}

inline void winx_call trim(std::basic_string<WCHAR>& str)
{
	const WCHAR blanks[] =
	{
		/* 支持中文空格 */
		' ', 12288, '\t', '\r', '\n', '\0'
	};
	trim(str, blanks);
}

// -------------------------------------------------------------------------
// find_first_of, find_first_not_of

template <class _InputIt, class _E>
inline _InputIt winx_call find_first_of(
	_InputIt first, _InputIt last, const _E* cset)
{
	const _E* str;
	for (; first != last; ++first)
	{
		for (str = cset; *str; ++str)
		{
			if (*str == *first)
				return first;
		}
	}
	return first;
}

template <class _InputIt, class _E>
inline _InputIt winx_call find_first_not_of(
	_InputIt first, _InputIt last, const _E* cset)
{
	const _E* str;
	for (; first != last; ++first)
	{
		for (str = cset; *str; ++str)
		{
			if (*str == *first)
				break;
		}
		if (!*str)
			break;
	}
	return first;
}

template <class _InputIt, class _ForwardIt>
inline _InputIt winx_call find_first_not_of(
	_InputIt first, _InputIt last, _ForwardIt set_first, _ForwardIt set_last)
{
	_ForwardIt set_it;
	for (; first != last; ++first)
	{
		for (set_it = set_first; set_it != set_last; ++set_it)
		{
			if (*set_it == *first)
				break;
		}
		if (set_it == set_last)
			break;
	}
	return first;
}

// -------------------------------------------------------------------------
// search

template <class _ForwardIt, class _E>
inline _ForwardIt winx_call search(
	_ForwardIt first, _ForwardIt last, const _E* pattern)
{
	return std::search(
		first, last,
		pattern, end(pattern)
		);
}

// -------------------------------------------------------------------------
// match, match_symbol, match_csymbol

template<class _InputIt1, class _InputIt2>
inline _InputIt1 winx_call match(
	_InputIt1 first1, _InputIt1 last1, _InputIt2 first2, _InputIt2 last2)
{
	_InputIt1 first = first1;
	for (;; ++first1, ++first2)
	{
		if (first2 == last2)
			return first1;
		if (first1 == last1)
			return first;
		if (*first1 != *first2)
			return first;
	}
}

template<class _InputIt, class _E>
inline _InputIt winx_call match(
	_InputIt first, _InputIt last, const _E* str)
{
	_InputIt first1 = first;
	for (;; ++first, ++str)
	{
		if (!*str)
			return first;
		if (first == last)
			return first1;
		if (*first != *str)
			return first1;
	}
}

template <class _InputIt, class _ForwardIt>
inline _InputIt winx_call match_symbol(
	_InputIt first, _InputIt last,
	_ForwardIt set_first, _ForwardIt set_last1, _ForwardIt set_last2)
{
	if (first == last)
		return first;

	if (std::find(set_first, set_last1, *first) == set_last1)
		return first;

	return find_first_not_of(++first, last, set_first, set_last2);
}

template <class _InputIt>
inline _InputIt winx_call match_csymbol(
	_InputIt first, _InputIt last)
{
	if (first == last)
		return first;

	if (!CharType::isCSymbolFirstChar(*first))
		return first;

	return std::find_if(++first, last, CharType::NotIsCSymbolNextChar());
}

// -------------------------------------------------------------------------
// find_not_csymbol, find_digit_or_csymbol

template <class _InputIt>
inline _InputIt winx_call find_not_csymbol(
	_InputIt first, _InputIt last)
{
	return std::find_if(first, last, CharType::NotIsCSymbolNextChar());
}

template <class _InputIt>
inline _InputIt winx_call find_digit_or_csymbol(
	_InputIt first, _InputIt last)
{
	return std::find_if(first, last, CharType::IsCSymbolNextChar());
}

// -------------------------------------------------------------------------
// compare

template<class _InputIt1, class _InputIt2>
inline int winx_call compare(
	_InputIt1 first1, _InputIt1 last1, _InputIt2 first2, _InputIt2 last2)
{
	for (;; ++first1, ++first2)
	{
		if (first1 == last1)
			return first2 == last2 ? 0 : -1;
		if (first2 == last2)
			return 1;
		if (*first1 != *first2)
			return (int)*first1 - (int)*first2;
	}
}

template<class _InputIt, class _E>
inline int winx_call compare(
	_InputIt first, _InputIt last, const _E* str)
{
	for (;; ++first, ++str)
	{
		if (first == last)
			return *str == 0 ? 0 : -1;
		if (*first != *str)
			return (int)*first - (int)*str;
		if (*str == 0)
			return 1;
	}
}

__forceinline int compare(const char* src, const char* dst)
{
	return strcmp(src, dst);
}

__forceinline int compare(const wchar_t* src, const wchar_t* dst)
{
	return wcscmp(src, dst);
}

// -------------------------------------------------------------------------
// strupper, strlower

template <class string_type>
inline void winx_call strupper(string_type& str)
{
    typename string_type::iterator itEnd = str.end();
    for (typename string_type::iterator it = str.begin(); it != itEnd; ++it)
    {
		*it = ::toupper(*it);
    }
}

template <class string_type>
inline void winx_call strlower(string_type& str)
{
    typename string_type::iterator itEnd = str.end();
    for (typename string_type::iterator it = str.begin(); it != itEnd; ++it)
    {
		*it = ::tolower(*it);
    }
}

// -------------------------------------------------------------------------
// strecpy

template <class xchar>
inline xchar* winx_call strecpy(xchar* pszDest, const xchar* pszSrc)
{
	while ((*pszDest = *pszSrc) != 0)
	{
		++pszDest;
		++pszSrc;
	}
	return pszDest;
}

// -------------------------------------------------------------------------
// class TestString

template <class LogT>
class TestString : public TestCase
{
public:
	WINX_TEST_SUITE(TestString);
		WINX_TEST(testTrim);
		WINX_TEST(testUpperLower);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	void testTrim(LogT& log)
	{
		std::string s = " \t  abc 123  \r\n";
		std::trim(s);
		AssertExp(s == "abc 123");
	}

	void testUpperLower(LogT& log)
	{
		std::string s = "abc123";
		std::strupper(s);
		AssertExp(s == "ABC123");
		std::strlower(s);
		AssertExp(s == "abc123");
	}
};

// -------------------------------------------------------------------------
// $Log: String.h,v $
// Revision 1.3  2007/01/10 09:35:16  xushiwei
// std::compare
//
// Revision 1.5  2006/09/26 07:51:00  xushiwei
// STL-Extension:
//  TestCase(WINX_TEST_APP, WINX_TEST_CLASS, WINX_TEST_SUITE, WINX_TEST, WINX_TEST_SUITE_END)
//  UnitTestAssert(AssertExp, AssertEq, AssertEqBuf)
//
// Revision 1.4  2006/08/26 03:19:44  xushiwei
// STL-Extension:
//   Archive(SimpleWriteArchive, SimpleReadArchive, MemSeqWriteArchive, RegWriteArchive, RegReadArchive)
//
// Revision 1.3  2006/08/23 05:42:04  xushiwei
// std::strecpy
//
// Revision 1.2  2006/08/19 04:40:48  xushiwei
// STL-Extension:
//   Memory(AutoFreeAlloc, RecycleBuffer, AutoArray, etc)
//   String Algorithm(trim, match, compare, etc), Container(SimpleMultiMap), CharType(isCSymbolFirstChar, etc)
//   Log(OutputLog, ErrorLog, FileLog, StringLog), PerformanceCounter, Diagnost(WINX_ASSERT, WINX_RUN_TEST, etc)
//

__NS_STD_END

#endif /* __STDEXT_STRING_H__ */
