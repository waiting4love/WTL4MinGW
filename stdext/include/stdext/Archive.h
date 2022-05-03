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
// Module: stdext/Archive.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-25 9:47:19
// 
// $Id: Archive.h,v 1.2 2006/11/30 03:19:17 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ARCHIVE_H__
#define __STDEXT_ARCHIVE_H__

#if (0)
#define BYTESWAP
#define __BYTESWAP__
#endif

#ifndef __STDEXT_BASIC_H__
#include "Basic.h"
#endif

// -------------------------------------------------------------------------
// WINX_BYTESWAP

#if defined(BYTESWAP) || defined(__BYTESWAP__)
#define WINX_BYTESWAP
#endif

#if !defined(WINX_BYTESWAP)
	#define _WinxByteSwap(val)
	#define _WinxByteSwap16(val)
	#define _WinxByteSwap32(val)
	#define _WinxByteSwapStruct(val)
	#define _WinxByteSwap16Array(val)
	#define _WinxByteSwap32Array(val)
	#define _WinxByteSwapStructArray(val)
#else
	#error "todo: BYTESWAP support"
#endif

// -------------------------------------------------------------------------

#ifndef __STDEXT_ARCHIVE_SIMPLEARCHIVE_H__
#include "archive/SimpleArchive.h"
#endif

#if defined(_ATL_VER)
#ifndef __STDEXT_ARCHIVE_REGARCHIVE_H__
#include "archive/RegArchive.h"
#endif
#endif

#ifndef __STDEXT_ARCHIVE_STREAMARCHIVE_H__
#include "archive/StreamArchive.h"
#endif

#ifndef __STDEXT_ARCHIVE_STDIOARCHIVE_H__
#include "archive/StdioArchive.h"
#endif

#ifndef __STDEXT_ARCHIVE_RECORDARCHIVE_H__
#include "archive/RecordArchive.h"
#endif

// -------------------------------------------------------------------------
// isStgWritable

__NS_STD_BEGIN

inline BOOL winx_call isStgWritable(IStorage* pstg)
{
	STATSTG statstg;
	pstg->Stat(&statstg, STATFLAG_NONAME);
	return statstg.grfMode & (STGM_READWRITE|STGM_WRITE);
}

inline BOOL winx_call isStgWritable(IStream* pstm)
{
	STATSTG statstg;
	pstm->Stat(&statstg, STATFLAG_NONAME);
	return statstg.grfMode & (STGM_READWRITE|STGM_WRITE);
}

__NS_STD_END

// -------------------------------------------------------------------------
// WriteArchive/ReadArchive

__NS_STD_BEGIN

typedef SimpleWriteArchive WriteArchive;
typedef SimpleReadArchive ReadArchive;

__NS_STD_END

// -------------------------------------------------------------------------
// $Log: Archive.h,v $
// Revision 1.2  2006/11/30 03:19:17  xushiwei
// STL-Extension:
//  ULargeInteger, LargeInteger, ReadArchiveImpl, WriteArchiveImpl
//  StreamArchive(StreamWriteArchive, StreamReadArchive, MemStreamWriteArchive, MemStreamReadArchive)
//  StdioArchive(StdioWriteArchive, StdioReadArchive)
//
// Revision 1.1  2006/10/18 12:13:39  xushiwei
// stdext as independent component
//
// Revision 1.1  2006/08/26 03:19:44  xushiwei
// STL-Extension:
//   Archive(SimpleWriteArchive, SimpleReadArchive, MemSeqWriteArchive, RegWriteArchive, RegReadArchive)
//

#endif /* __STDEXT_ARCHIVE_H__ */
