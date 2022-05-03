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
// Module: stdext/msvc/msvcrt.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-14 15:24:12
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MSVC_MSVCRT_H__
#define __STDEXT_MSVC_MSVCRT_H__

#ifndef __STDEXT_MSVC_ATLCONV_H__
#include "atlconv.h"
#endif

// -------------------------------------------------------------------------
// crtdbg.h

#if defined(WINX_GCC)

#ifndef _INC_CRTDBG
#include "crtdbg.h"
#endif

#ifndef _ASSERT_H_
#include <assert.h>
#endif

#undef _ASSERTE
#define _ASSERTE(e)	assert(e)

#undef _CrtDbgBreak
#define _CrtDbgBreak()	_assert("_CrtDbgBreak", __FILE__, __LINE__)

#undef _CrtSetDbgFlag
#define _CrtSetDbgFlag(f)

#undef _malloc_dbg
#undef _calloc_dbg
#undef _realloc_dbg
#undef _expand_dbg
#undef _free_dbg
#undef _msize_dbg

#define _malloc_dbg(s, t, f, l)         malloc(s)
#define _calloc_dbg(c, s, t, f, l)      calloc(c, s)
#define _realloc_dbg(p, s, t, f, l)     realloc(p, s)
#define _expand_dbg(p, s, t, f, l)      _expand(p, s)
#define _free_dbg(p, t)                 free(p)
#define _msize_dbg(p, t)                _msize(p)

#undef NULL
#define NULL 0

#else

#ifndef _INC_CRTDBG
#include <crtdbg.h>
#endif

#endif // defined(WINX_GCC)

// -------------------------------------------------------------------------
// std::exception

#if defined(WINX_GCC)

#ifndef __EXCEPTION__
#include <exception>
#endif

namespace std
{
	class msvc_exception : public exception
	{
	private:
		const char* _m_what;
	public:
		msvc_exception() : _m_what("") {}
		msvc_exception(const char* msg) : _m_what(msg) {}
		virtual const char* what() const throw() { return _m_what; }
	};
}

#define exception msvc_exception

#endif // defined(WINX_GCC)

// -------------------------------------------------------------------------
// $Log: $

#endif /* __STDEXT_MSVC_MSVCRT_H__ */
