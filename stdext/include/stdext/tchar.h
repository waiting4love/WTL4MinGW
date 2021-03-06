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
// Module: stdext/tchar.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-23 12:39:31
// 
// $Id: tchar.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_TCHAR_H__
#define __STDEXT_TCHAR_H__

#ifndef __STDEXT_STRING_H__
#include "String.h"
#endif

#ifndef _INC_LIMITS
#include <limits.h>
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// template char type functions

#define __template_xchar		template <class xchar> __forceinline
#define __template_xchar_inline	__template_xchar
#define __xcharfunc(name)		name
#define __xdecl

namespace tchar {

#include "tchar/itoa.h"
#include "tchar/makepath.h"
#include "tchar/memop.h"
#include "tchar/splitpath.h"
#include "tchar/strcat.h"
#include "tchar/strchr.h"
#include "tchar/strcmp.h"
#include "tchar/strcspn.h"
#include "tchar/strdup.h"
#include "tchar/stricmp.h"
#include "tchar/strlen.h"
#include "tchar/strlwr.h"
#include "tchar/strncmp.h"
#include "tchar/strncpy.h"
#include "tchar/strnicmp.h"
#include "tchar/strrchr.h"
#include "tchar/strspn.h"
#include "tchar/strstr.h"
#include "tchar/strupr.h"

using std::strecpy;

}; // namespace tchar

#undef __xdecl
#undef __xcharfunc
#undef __template_xchar
#undef __template_xchar_inline

// <--- template char type functions

// -------------------------------------------------------------------------
// $Log: tchar.h,v $
// Revision 1.1  2006/10/18 12:13:39  xushiwei
// stdext as independent component
//
// Revision 1.2  2006/08/23 05:42:04  xushiwei
// std::strecpy
//
// Revision 1.1  2006/08/23 04:48:32  xushiwei
// STL-Extension:
//   namespace std::tchar(std::tchar::strlen, std::tchar::strlwr, etc)
//

__NS_STD_END

#endif /* __STDEXT_TCHAR_H__ */
