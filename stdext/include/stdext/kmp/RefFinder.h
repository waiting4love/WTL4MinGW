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
// Module: stdext/kmp/RefFinder.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2005-4-29 16:09:26
// 
// $Id: RefFinder.h,v 1.1 2006/12/02 08:00:45 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_KMP_REFFINDER_H__
#define __STDEXT_KMP_REFFINDER_H__

#ifndef __STDEXT_KMP_FINDER_H__
#include "Finder.h"
#endif

#ifndef _ALGORITHM_
#include <algorithm>
#endif

__STD_KMP_BEGIN

// -------------------------------------------------------------------------
// class RefFinder - just for testing class Finder

template < class _E, class _Strategy = MatchCase<_E> >
class RefFinder : public _Strategy
{
public:
	typedef _E char_type;
	typedef int size_type;

	enum { npos = -1 };
	enum { nolimit = -1 };
	
private:
	char_type* m_str_find;
	size_type m_size;

public:
	RefFinder()
		: m_str_find(NULL), m_size(0)
	{
	}
	RefFinder(const char_type* p, size_type len) : m_str_find(NULL)
	{
		initPattern(p, len);
	}
	RefFinder(const char_type* p) : m_str_find(NULL)
	{
		initPattern(p);
	}
	RefFinder(const std::basic_string<_E>& str) : m_str_find(NULL)
	{
		initPattern(str.c_str(), str.size());
	}
	~RefFinder()
	{
		if (m_str_find)
			free(m_str_find);
	}

	size_type winx_call size() const
	{
		return m_size;
	}

	HRESULT winx_call initPattern(const char_type* p, size_type len)
	{
		if (p == NULL || len <= 0)
			return E_INVALIDARG;

		if (m_str_find)
			free(m_str_find);
		
		m_str_find = (char_type*)malloc( (sizeof(char_type) + sizeof(size_type)) * len);
		m_size = len;
		
		_Strategy::Copy(m_str_find, p, len);
		
		return S_OK;
	}

	HRESULT winx_call initPattern(const char_type* p)
	{
		if (p == NULL)
			return E_INVALIDARG;
		return initPattern(p, std::char_traits<_E>::length(p));
	}

	HRESULT winx_call initPattern(const std::basic_string<_E>& str)
	{
		return initPattern(str.c_str(), str.size());
	}

	template <class iterator_type>
	HRESULT winx_call iteratorNext(iterator_type it, size_type limit, iterator_type* pitFind) const
	{
		iterator_type itEnd = it;
		std::advance(itEnd, limit);
		
		iterator_type itFind = std::search(it, itEnd, m_str_find, m_str_find + m_size);
		if (itFind == itEnd)
		{
			*pitFind = itEnd;
			return S_FALSE;
		}

		std::advance(itFind, m_size);
		*pitFind = itFind;
		return S_OK;
	}
};

// -------------------------------------------------------------------------
// $Log: RefFinder.h,v $
// Revision 1.1  2006/12/02 08:00:45  xushiwei
// STL-Extension:
//  KMP-String-Find-Algorithm(class std::kmp::Finder/CaseFinder/NoCaseFinder)
//

__STD_KMP_END

#endif /* __STDEXT_KMP_REFFINDER_H__ */
