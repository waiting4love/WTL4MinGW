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
// Module: stdext/kmp/Finder.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2004-4-5 20:30:40
// 
// $Id: Finder.h,v 1.5 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_KMP_FINDER_H__
#define __STDEXT_KMP_FINDER_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

#ifndef _INC_CTYPE
#include <ctype.h>
#endif

#ifndef _STRING_
#include <string>
#endif

#define __STD_KMP			std::kmp::
#define __STD_KMP_BEGIN		namespace std { namespace kmp {
#define __STD_KMP_END		} }

__STD_KMP_BEGIN

// -------------------------------------------------------------------------
// c++ std::istream adapter

// I use 'istream_type' because I don't want to include <istream>.
template <class istream_type>
class _istream_adapter : public istream_type
{
public:
	enum { endch = -1 };
};

// -------------------------------------------------------------------------
// c style string adapter

template <class _E>
class _cstr_adapter
{
public:
	enum { endch = -1 };
	typedef int int_type;
	
private:
	const _E* m_p;
	
public:
	_cstr_adapter(const _E* p) : m_p(p) {}
	
	int_type winx_call get()
	{
		return (*m_p ? *m_p++ : (int_type)endch);
	}

	const _E* winx_call tell() const
	{
		return m_p;
	}
};

// -------------------------------------------------------------------------
// iterator adapter

template <class iterator_type>
class _iterator_adapter
{
public:
	enum { endch = -1 };

	typedef iterator_type iterator;
	typedef int int_type;
	
private:
	iterator m_it;
	
public:
	_iterator_adapter(iterator it) : m_it(it) {}

	int_type winx_call get()
	{
		return *m_it++;
	}

	iterator winx_call tell() const
	{
		return m_it;
	}
};

// -------------------------------------------------------------------------
// Finder Strategy

/*
@class std::kmp::MatchCase
@brief
	The std::kmp::MatchCase class is provided as a strategy of class \<Finder>. It means the
	searching algorithm will process in case-sensitive mode.
@arg _E
	The data type of a single character. It must be the same as you pass to \<Finder> class.
@see Finder, MatchNoCase
@*/
template <class _E>
struct MatchCase
{
	typedef _E char_type;

	static void winx_call copy(char_type* dest, const char_type* src, int n)
	{
		while (n--)
			dest[n] = src[n];
	}

	template <class ArchiveT>
	static typename ArchiveT::int_type winx_call get(ArchiveT& ar)
	{
		return ar.get();
	}
};

/*
@class std::kmp::MatchNoCase
@brief
	The std::kmp::MatchNoCase class is provided as a strategy of class \<Finder>. It means the
	searching algorithm will process in non case-sensitive mode.
@arg _E
	The data type of a single character. It must be the same as you pass to \<Finder> class.
@see Finder, MatchCase
@*/
template <class _E>
struct MatchNoCase
{
	typedef _E char_type;

	static void winx_call copy(char_type* dest, const char_type* src, int n)
	{
		while (n--)
			dest[n] = toupper( src[n] );
	}

	template <class ArchiveT>
	static typename ArchiveT::int_type winx_call get(ArchiveT& ar)
	{
		return toupper( ar.get() );
	}
};

// -------------------------------------------------------------------------
// class Finder

/*
@class std::kmp::Finder
@brief
	The std::kmp::Finder class implements KMP string searching algorithm.
@arg _E
	The data type of a single character to be found in searching algorithm. It can be char or WCHAR.
@arg _Strategy
	The strategy of searching algorithm. Default is \<MatchCase>, and it also can be \<MatchNoCase>.
@*/
template < class _E, class _Strategy = MatchCase<_E> >
class Finder
{
public:
	typedef _E char_type;
	typedef int size_type;

	enum { npos = -1 };
	enum { nolimit = -1 };

private:
	char_type* m_str_find;
	size_type m_size;
	size_type* m_next;
	
public:
	/*
	@ctor()
	@brief					Default constructor.
	@see Finder, initPattern, getPattern
	@*/
	Finder()
		: m_str_find(NULL), m_next(NULL), m_size(0)
	{
	}
	
	/*
	@ctor(szPattern,cchLen)
	@brief					Construct the finder object with a pattern string.
	@arg [in] szPattern		The start address of pattern string buffer.
	@arg [in] cchLen		The length of pattern string.
	@see Finder, initPattern, getPattern
	@*/
	Finder(const char_type* szPattern, size_type cchLen) : m_str_find(NULL)
	{
		initPattern(szPattern, cchLen);
	}

	/*
	@ctor(szPattern)
	@brief					Construct the finder object with a null-terminated pattern string (C-Style).
	@arg [in] szPattern		The null-terminated pattern string.
	@see Finder, initPattern, getPattern
	@*/
	Finder(const char_type* szPattern) : m_str_find(NULL)
	{
		initPattern(szPattern);
	}

	/*
	@ctor(strPattern)
	@brief					Construct the finder object with a pattern string object (C++ Style).
	@arg [in] strPattern	The pattern string object.
	@see Finder, initPattern, getPattern
	@*/
	Finder(const std::basic_string<_E>& strPattern) : m_str_find(NULL)
	{
		initPattern(strPattern.c_str(), strPattern.size());
	}

	~Finder()
	{
		if (m_str_find)
			free(m_str_find);
	}
	
	/*
	@fn initPattern(szPattern,cchLen)
	@brief					Initialize the finder object with a pattern string.
	@arg [in] szPattern		The start address of pattern string buffer.
	@arg [in] cchLen		The length of pattern string.
	@see Finder, initPattern, getPattern
	@*/
	HRESULT winx_call initPattern(const char_type* szPattern, size_type cchLen)
	{
		if (szPattern == NULL || cchLen <= 0)
			return E_INVALIDARG;

		if (m_str_find)
			free(m_str_find);
		
		m_str_find = (char_type*)malloc( (sizeof(char_type) + sizeof(size_type)) * cchLen);
		m_next = (size_type*)(m_str_find + cchLen);
		m_size = cchLen;
		
		_Strategy::copy(m_str_find, szPattern, cchLen);
		
		size_type k;
		m_next[0] = npos;
		for (size_type j = 1; j != cchLen; ++j)
		{
			k = m_next[j-1];
			while (k != npos && m_str_find[k] != m_str_find[j-1])
				k = m_next[k];
			m_next[j] = k + 1;
		}		
		return S_OK;
	}

	/*
	@fn initPattern(szPattern)
	@brief					Initialize the finder object with a null-terminated pattern string (C-Style).
	@arg [in] szPattern		The null-terminated pattern string.
	@see Finder, initPattern, getPattern
	@*/
	HRESULT winx_call initPattern(const char_type* szPattern)
	{
		if (szPattern == NULL)
			return E_INVALIDARG;
		return initPattern(szPattern, std::char_traits<_E>::length(szPattern));
	}

	/*
	@fn initPattern(strPattern)
	@brief					Initialize the finder object with a pattern string object (C++ Style).
	@arg [in] strPattern	The pattern string object.
	@see Finder, initPattern, getPattern
	@*/
	HRESULT winx_call initPattern(const std::basic_string<_E>& strPattern)
	{
		return initPattern(strPattern.c_str(), strPattern.size());
	}

	/*
	@fn good
	@brief					Indicates the state of the finder object.
	@return
		@val true(nonzero)
			The finder object is initialized with a pattern string.
		@val false(zero)
			The finder object is uninitialized.
	@*/
	int winx_call good() const
	{
		return m_str_find != NULL;
	}

	/*
	@fn size
	@brief					Get the length of the pattern string.
	@return
		Returns the length of the pattern string if the finder object is initialized.
		Returns zero if the finder object is uninitialized.
	@see good
	@*/
	size_type winx_call size() const
	{
		return m_size;
	}

	/*
	@fn getPattern
	@brief					Get the pattern string.
	@arg [out] strPattern	Returns the pattern string.
	@see Finder, initPattern
	@*/
	template <class StringT>
	void winx_call getPattern(StringT& strPattern) const
	{
		strPattern.assign(m_str_find, m_size);
	}

	/*
	@fn next
	@brief					Searching the pattern string from current position of an archive object.
	@arg [in] ar			The archive object.
	@arg [in] limit
		The limit scope of searching. Default is <em>nolimit</em>.
		The searching operation will be limited within [cp, cp+limit). Here cp means the current position
		of the archive. If limit is specified <em>nolimit</em>, that means limit to the end of the archive.
	@return
		@val S_OK
			Searching succeeds. The pattern string is found in the archive.
		@val S_FALSE
			Searching fails. The pattern string is not found.
		@val E_ACCESSDENIED
			Searching fails. The finder object is uninitialized.
	@remark
		If searching succeeds (returns S_OK), the new current position of the archive object is the end of
		pattern string found in the archive. For example, assume the archive contents are "1234abcdefg", 
		and the pattern string is "abc", then after the searching operation, the current position of the
		archive pointer to "defg", not "abcdefg". \p
		If searching fails, the new current position of the archive object is undefined.
	@*/
	template <class ArchiveT>
	HRESULT winx_call next(ArchiveT& ar, size_type limit = (size_type)nolimit) const
	{
		if (m_size == 0)
			return E_ACCESSDENIED;

		int j = 0;
		for (; limit != 0; --limit)
		{
			typename ArchiveT::int_type ch = _Strategy::get(ar);
			if (ch == ArchiveT::endch)
				break;

			while (m_str_find[j] != ch)
			{
				j = m_next[j];
				if (j == npos)
					break;
			}
			
			if (++j == m_size)
				return S_OK;
		}
		return S_FALSE;
	}

	/*
	@fn istreamNext
	@brief					Searching the pattern string from current position of an std::istream object.
	@arg [in] is			The istream object.
	@arg [in] limit
		The limit scope of searching. Default is <em>nolimit</em>.
		The searching operation will be limited within [cp, cp+limit). Here cp means the current position
		of the istream. If limit is specified <em>nolimit</em>, that means limit to the end of the istream.
	@return
		@val S_OK
			Searching succeeds. The pattern string is found in the istream.
		@val S_FALSE
			Searching fails. The pattern string is not found.
		@val E_ACCESSDENIED
			Searching fails. The finder object is uninitialized.
	@remark
		If searching succeeds (returns S_OK), the new current position of the istream object is the end of
		pattern string found in the istream. For example, assume the istream contents are "1234abcdefg", 
		and the pattern string is "abc", then after the searching operation, the current position of the
		istream pointer to "defg", not "abcdefg". \p
		If searching fails, the new current position of the istream object is undefined.
	@see next
	@*/
	template <class istream_type>
	HRESULT winx_call istreamNext(istream_type& is, size_type limit = (size_type)nolimit) const
	{
		return next( (_istream_adapter<istream_type>&)is, limit );
	}

	/*
	@fn iteratorNext
	@brief					Searching the pattern string in a text specified by an iterator scope.
	@arg [in] it			Start of the text (iterator begin).
	@arg [in] limit
		The limit scope of searching. It can't be <em>nolimit</em>.
		The searching operation will be limited within [it, it+limit).
	@arg [out] pitFind
		If searching succeeds (returns S_OK), Returns the end of pattern string found in the text.
		For example, assume the text are "1234abcdefg", and the pattern string is "abc", then after
		the searching operation, *pitFind will pointer to "defg", not "abcdefg". \p
		If searching fails, *pitFind is undefined.
	@return
		@val S_OK
			Searching succeeds. The pattern string is found in the text.
		@val S_FALSE
			Searching fails. The pattern string is not found.
		@val E_ACCESSDENIED
			Searching fails. The finder object is uninitialized.
	@see next
	@*/
	template <class iterator_type>
	HRESULT winx_call iteratorNext(iterator_type it, size_type limit, iterator_type* pitFind) const
	{
		WINX_ASSERT(limit >= 0);
		_iterator_adapter<iterator_type> is( it );
		HRESULT hr = next( is, limit );
		*pitFind = is.tell();
		return hr;
	}

	/*
	@fn cstrNext
	@brief					Searching the pattern string in a text specified by a C-Style string.
	@arg [in] text			The text. It's a null-terminated string.
	@arg [out] ppFind
		If searching succeeds (returns S_OK), Returns the end of pattern string found in the text.
		For example, assume the text are "1234abcdefg", and the pattern string is "abc", then after
		the searching operation, *ppFind will pointer to "defg", not "abcdefg". \p
		If searching fails, *ppFind is undefined.
	@return
		@val S_OK
			Searching succeeds. The pattern string is found in the text.
		@val S_FALSE
			Searching fails. The pattern string is not found.
		@val E_ACCESSDENIED
			Searching fails. The finder object is uninitialized.
	@see next
	@*/
	HRESULT winx_call cstrNext(const char_type* text, const char_type** ppFind) const
	{
		_cstr_adapter<char_type> is( text );
		HRESULT hr = next( is );
		*ppFind = is.tell();
		return hr;
	}
};

// -------------------------------------------------------------------------
// class NoCaseFinder

/*
@class std::kmp::NoCaseFinder
@brief
	The std::kmp::NoCaseFinder class is a simple class which sets \<Finder> to non case-sensitive mode.
@arg _E
	The data type of a single character to be found in searching algorithm. It can be char or WCHAR.
@see Finder
@*/
template <class _E>
class NoCaseFinder : public Finder< _E, MatchNoCase<_E> >
{
private:
	typedef Finder< _E, MatchNoCase<_E> > BaseClass;

public:
    typedef typename BaseClass::size_type size_type;
    typedef typename BaseClass::char_type char_type;
    
	/*
	@ctor()
	@brief					Default constructor.
	@see std::kmp::Finder
	@*/
	NoCaseFinder() : BaseClass() {}

	/*
	@ctor(szPattern,cchLen)
	@brief					Construct the finder object with a pattern string.
	@arg [in] szPattern		The start address of pattern string buffer.
	@arg [in] cchLen		The length of pattern string.
	@see std::kmp::Finder
	@*/
	NoCaseFinder(const char_type* szPattern, size_type cchLen) : BaseClass(szPattern, cchLen) {}

	/*
	@ctor(szPattern)
	@brief					Construct the finder object with a null-terminated pattern string (C-Style).
	@arg [in] szPattern		The null-terminated pattern string.
	@see std::kmp::Finder
	@*/
	NoCaseFinder(const char_type* szPattern) : BaseClass(szPattern) {}

	/*
	@ctor(strPattern)
	@brief					Construct the finder object with a pattern string object (C++ Style).
	@arg [in] strPattern	The pattern string object.
	@see std::kmp::Finder
	@*/
	NoCaseFinder(const std::basic_string<_E>& strPattern) : BaseClass(strPattern) {}
};

// -------------------------------------------------------------------------
// $Log: Finder.h,v $
// Revision 1.4  2006/12/24 10:50:20  xushiwei
// Documentation: MatchCase, MatchNoCase, Finder, NoCaseFinder
//
// Revision 1.1  2006/12/02 08:00:44  xushiwei
// STL-Extension:
//  KMP-String-Find-Algorithm(class std::kmp::Finder/CaseFinder/NoCaseFinder)
//

__STD_KMP_END

#endif /* __STDEXT_KMP_FINDER_H__ */
