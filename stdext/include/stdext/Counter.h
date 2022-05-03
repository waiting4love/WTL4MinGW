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
// Module: stdext/Counter.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 17:57:28
// 
// $Id: Counter.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_COUNTER_H__
#define __STDEXT_COUNTER_H__

#ifndef __STDEXT_BASIC_H__
#include "Basic.h"
#endif

#ifndef _WINBASE_
#include <winbase.h>
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class PerformanceCounter

class PerformanceCounter
{
public:
	typedef UINT64 value_type;
	enum {
		radix10_cstr_size = 24, // INT64最大的数为：9223372036854775807，计19位。
		radix16_cstr_size = 24,
	};

private:
	value_type m_tick;
	
private:
	static value_type __freq()
	{
		value_type _freq = 0;
		QueryPerformanceFrequency(&(LARGE_INTEGER&)_freq);
		if (_freq == 0)
		{
			WINX_REPORT("QueryPerformanceFrequency is unsupported\n");
			_freq = 1;
		}
		return _freq;
	}
	
public:
	PerformanceCounter()
	{
		freq();
		start();
	}

	void winx_call start()
	{
		QueryPerformanceCounter(&(LARGE_INTEGER&)m_tick);
	}

	value_type winx_call duration() const
	{
		value_type tickNow;
		QueryPerformanceCounter(&(LARGE_INTEGER&)tickNow);
		return tickNow - m_tick;
	}

public:
	static value_type winx_call freq()
	{
		static value_type s_freq = __freq();
		return s_freq;
	}

public:
	static void winx_call toRadix10(
		IN const value_type& value,
		OUT WCHAR radix10[radix10_cstr_size]
		)
	{
		_ui64tow(value, radix10, 10);
	}

	static void winx_call toRadix10(
		IN const value_type& value,
		OUT char radix10[radix10_cstr_size]
		)
	{
		_ui64toa(value, radix10, 10);
	}
	
	static void winx_call toRadix16(
		IN const value_type& value,
		OUT WCHAR radix16[radix16_cstr_size]
		)
	{
		_ui64tow(value, radix16, 16);
	}

	static void winx_call toRadix16(
		IN const value_type& value,
		OUT char radix16[radix16_cstr_size]
		)
	{
		_ui64toa(value, radix16, 16);
	}

public:
	template <class LogT>
	__forceinline static void winx_call trace(LogT& log, const value_type& ticks)
	{
		double msVal = (INT64)ticks * 1000.0 / (INT64)freq();
		char szTicks[radix10_cstr_size];
		toRadix10(ticks, szTicks);
		log.trace(
			"---> Elapse %s ticks (%.2lf ms) (%.2lf min) ...\n",
			szTicks, msVal, msVal/60000.0
			);
	}

	template <class LogT>
	__forceinline void winx_call trace(LogT& log)
	{
		value_type tickNow;
		QueryPerformanceCounter(&(LARGE_INTEGER&)tickNow);
		trace(log, tickNow - m_tick);
	}
};

// -------------------------------------------------------------------------
// $Log: Counter.h,v $
// Revision 1.1  2006/10/18 12:13:39  xushiwei
// stdext as independent component
//
// Revision 1.1  2006/08/19 04:40:48  xushiwei
// STL-Extension:
//   Memory(AutoFreeAlloc, RecycleBuffer, AutoArray, etc)
//   String Algorithm(trim, match, compare, etc), Container(SimpleMultiMap), CharType(isCSymbolFirstChar, etc)
//   Log(OutputLog, ErrorLog, FileLog, StringLog), PerformanceCounter, Diagnost(WINX_ASSERT, WINX_RUN_TEST, etc)
//

__NS_STD_END

#endif /* __STDEXT_COUNTER_H__ */
