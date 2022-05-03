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
// Module: stdext/Log.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 17:57:28
// 
// $Id: Log.h,v 1.7 2007/01/10 09:32:25 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_LOG_H__
#define __STDEXT_LOG_H__

#ifndef __STDEXT_BASIC_H__
#include "Basic.h"
#endif

#ifndef __STDEXT_STORAGE_H__
#include "Storage.h"
#endif

#ifndef _INC_STDIO
#include <stdio.h>
#endif

#ifndef _INC_STDARG
#include <stdarg.h>
#endif

#ifndef _STRING_
#include <string>
#endif

__NS_STD_BEGIN

// =========================================================================
// __STD_LOG_PRINT

#define __STD_LOG_FMT_PRINT_EX(DataT, vData)								\
	Log& winx_call print(DataT v, char* fmt) {								\
		return trace(fmt, vData);											\
	}																		\
	Log& winx_call print(DataT v, WCHAR* fmt) {								\
		return trace(fmt, vData);											\
	}																		\
	Log& winx_call print(DataT v, const WCHAR* fmt) {						\
		return trace(fmt, vData);											\
	}																		\
	Log& winx_call print(DataT v, const char* fmt) {						\
		return trace(fmt, vData);											\
	}

#define __STD_LOG_FMT_PRINT(DataT)											\
	__STD_LOG_FMT_PRINT_EX(DataT, v)

#define __STD_LOG_PRINT_CHAR(DataT)											\
	Log& winx_call print(DataT v) {											\
		return repeat(v);													\
	}																		\
	__STD_LOG_FMT_PRINT(DataT)

#define __STD_LOG_PRINT_INT(DataT)											\
	Log& winx_call print(signed DataT v) {									\
		char buf[32];														\
		return printString(ltoa(v, buf, 10));								\
	}																		\
	Log& winx_call print(unsigned DataT v) {								\
		char buf[32];														\
		return printString(_ultoa(v, buf, 10));								\
	}																		\
	__STD_LOG_FMT_PRINT(signed DataT)										\
	__STD_LOG_FMT_PRINT(unsigned DataT)

#define __STD_LOG_PRINT_STRING(DataT)										\
	Log& winx_call print(DataT v) {											\
		return printString(v);												\
	}																		\
	__STD_LOG_FMT_PRINT(DataT)

#define __STD_LOG_PRINT_STRING_CLS(DataT)									\
	Log& winx_call print(const DataT& v) {									\
		return printString(v);												\
	}																		\
	__STD_LOG_FMT_PRINT_EX(DataT, v.c_str())

#define __STD_LOG_PRINT_INT64(DataT)										\
	Log& winx_call print(signed DataT v) {									\
		char buf[32];														\
		return printString(_i64toa(v, buf, 10));							\
	}																		\
	Log& winx_call print(unsigned DataT v) {								\
		char buf[32];														\
		return printString(_ui64toa(v, buf, 10));							\
	}																		\
	__STD_LOG_FMT_PRINT(signed DataT)										\
	__STD_LOG_FMT_PRINT(unsigned DataT)

#define __STD_LOG_PRINT_FLOAT(DataT, n)										\
	Log& winx_call print(DataT v, int digits = n) {							\
		char buf[64];														\
		return printString(_gcvt(v, digits, buf));							\
	}																		\
	__STD_LOG_FMT_PRINT(DataT)

#define __STD_LOG_PRINT_CTYPES()											\
	__STD_LOG_PRINT_CHAR(char)												\
	__STD_LOG_PRINT_CHAR(signed char)										\
	__STD_LOG_PRINT_CHAR(unsigned char)										\
	__STD_LOG_PRINT_STRING(const char*)										\
	__STD_LOG_PRINT_STRING(const WCHAR*)									\
	__STD_LOG_PRINT_STRING_CLS(std::basic_string<char>)						\
	__STD_LOG_PRINT_STRING_CLS(std::basic_string<WCHAR>)					\
	__STD_LOG_PRINT_INT(int)												\
	__STD_LOG_PRINT_INT(short)												\
	__STD_LOG_PRINT_INT(long)												\
	__STD_LOG_PRINT_INT64(__int64)											\
	__STD_LOG_PRINT_FLOAT(float, 12)										\
	__STD_LOG_PRINT_FLOAT(double, 12)

// =========================================================================
// class Log

template <class Storage>
class Log
{
protected:
	Storage m_stg;
	
public:
	Log() {}
	
	template <class ArgT>
	Log(ArgT arg) : m_stg(arg)
	{
	}

public:
	Storage& winx_call storage()
	{
		return m_stg;
	}

public:
	void winx_call flush()
	{
		if (m_stg)
			m_stg.flush();
	}

	Log& winx_call step(char ch = '.')
	{
		if (m_stg)
			m_stg.put(ch);
		return *this;
	}
	
	Log& winx_call repeat(char ch)
	{
		if (m_stg)
			m_stg.put(ch);
		return *this;
	}

	Log& winx_call repeat(char ch, size_t count)
	{
		if (m_stg)
			m_stg.put(count, ch);
		return *this;
	}
	
	Log& winx_call repeat(WCHAR ch)
	{
		if (m_stg)
			m_stg.putw(ch);
		return *this;
	}

	Log& winx_call repeat(WCHAR ch, size_t count)
	{
		if (m_stg)
			m_stg.putw(count, ch);
		return *this;
	}

	Log& winx_call newline()
	{
		if (m_stg)
			m_stg.put('\n');
		return *this;
	}

	Log& winx_call trace(const char* fmt, ...)
	{
		if (m_stg)
		{
			va_list args;
			va_start(args, fmt);
			m_stg.putv(fmt, args);
			va_end(args);
		}
		return *this;
	}
	
	Log& winx_call trace(const WCHAR* fmt, ...)
	{
		if (m_stg)
		{
			va_list args;
			va_start(args, fmt);
			m_stg.putv(fmt, args);
			va_end(args);
		}
		return *this;
	}
	
private:
	template <class _It>
	Log& winx_call __printString(_It first, _It last, char)
	{
		if (m_stg)
			m_stg.put(&*first, last-first);
		return *this;
	}
		
	template <class _It>
	Log& winx_call __printString(_It first, _It last, unsigned char)
	{
		if (m_stg)
			m_stg.put((char*)&*first, last-first);
		return *this;
	}

	template <class _It>
	Log& winx_call __printString(_It first, _It last, WCHAR)
	{
		std::basic_string<WCHAR> s(first, last);
		return trace("%S", s.c_str());
	}

public:
	template <class _It>
	Log& winx_call printString(_It first, _It last)
	{
		return __printString(first, last, *first);
	}

	Log& winx_call printString(const basic_string<char>& s)
	{
		if (m_stg)
			m_stg.put(s.c_str(), s.size());
		return *this;
	}

	Log& winx_call printString(const char* s)
	{
		if (m_stg)
			m_stg.put(s, strlen(s));
		return *this;
	}
	
	Log& winx_call printString(const basic_string<WCHAR>& s)
	{
		return trace("%S", s.c_str());
	}

	Log& winx_call printString(const WCHAR* s)
	{
		return trace("%S", s);
	}

public:
	Log& winx_call print(const RECT& rc)
	{
		return trace("(%d, %d) - (%d, %d)", rc.left, rc.top, rc.right, rc.bottom);
	}

	Log& winx_call print(const SIZE& sz)
	{
		return trace("(%d, %d)", sz.cx, sz.cy);
	}

public:
	template <class DataT>
	Log& winx_call printObj(const DataT& v)
	{
		v.trace(*this);
		return *this;
	}
	
	template <class DataT, class ArgT1>
	Log& winx_call printObj(const DataT& v, ArgT1 arg1)
	{
		v.trace(*this, arg1);
		return *this;
	}

	template <class DataT, class ArgT1, class ArgT2>
	Log& winx_call printObj(const DataT& v, ArgT1 arg1, ArgT2 arg2)
	{
		v.trace(*this, arg1, arg2);
		return *this;
	}

	template <class DataT, class ArgT1, class ArgT2, class ArgT3>
	Log& winx_call printObj(const DataT& v, ArgT1 arg1, ArgT2 arg2, ArgT3 arg3)
	{
		v.trace(*this, arg1, arg2, arg3);
		return *this;
	}

public:
	template <class DataIt>
	Log& winx_call printArray(
		DataIt array, size_t count,
		const char* bracketL = "{ ", const char* bracketR = " }\n",
		const char* dataSep = ", ")
	{
		if (m_stg)
		{
			m_stg.put(bracketL, strlen(bracketL));
			if (count)
			{
				for (size_t i = 0;;)
				{
					print(*array++);
					if (++i >= count)
						break;
					m_stg.put(dataSep, strlen(dataSep));
				}
			}
			m_stg.put(bracketR, strlen(bracketR));
		}
		return *this;
	}

	template <class ArgT, class DataIt>
	Log& winx_call printArray(
		ArgT fmt, DataIt array, size_t count,
		const char* bracketL = "{ ", const char* bracketR = " }\n",
		const char* dataSep = ", ")
	{
		if (m_stg)
		{
			m_stg.put(bracketL, strlen(bracketL));
			if (count)
			{
				for (size_t i = 0;;)
				{
					print(*array++, fmt);
					if (++i >= count)
						break;
					m_stg.put(dataSep, strlen(dataSep));
				}
			}
			m_stg.put(bracketR, strlen(bracketR));
		}
		return *this;
	}

public:
	template <class CharT, class VectorT1, class VectorT2>
	Log& winx_call traceDim(const CharT* fmt, const VectorT1& x, const VectorT2& y)
	{
		size_t count = x.size();
		if (y.size() < count)
		{
			print("\t[ Exception: Size of vector doesn't match! ]\n");
		}
		else
		{
			for (size_t i = 0; i < count; ++i)
				trace(fmt, x[i], y[i]);
		}
		return *this;
	}
	
	template <class CharT, class VectorT1, class VectorT2, class VectorT3>
	Log& winx_call traceDim(const CharT* fmt, const VectorT1& x, const VectorT2& y, const VectorT3& z)
	{
		size_t count = x.size();
		if (y.size() < count || z.size() < count)
		{
			print("\t[ Exception: Size of vector doesn't match! ]\n");
		}
		else
		{
			for (size_t i = 0; i < count; ++i)
				trace(fmt, x[i], y[i], z[i]);
		}
		return *this;
	}

	template <class CharT, class VectorT1, class VectorT2, class VectorT3, class VectorT4>
	Log& winx_call traceDim(const CharT* fmt, const VectorT1& x, const VectorT2& y, const VectorT3& z, const VectorT4& u)
	{
		size_t count = x.size();
		if (y.size() < count || z.size() < count || u.size() < count)
		{
			print("\t[ Exception: Size of vector doesn't match! ]\n");
		}
		else
		{
			for (size_t i = 0; i < count; ++i)
				trace(fmt, x[i], y[i], z[i], u[i]);
		}
		return *this;
	}

public:
	template <class VectorT1, class VectorT2>
	Log& winx_call printDim(const VectorT1& x, const VectorT2& y)
	{
		size_t count = x.size();
		if (y.size() < count)
		{
			print("\t[ Exception: Size of vector doesn't match! ]\n");
		}
		else
		{
			for (size_t i = 0; i < count; ++i)
				print("\t[ ").
				print(x[i]).print(", ").
				print(y[i]).print(" ]\n");
		}
		return *this;
	}

	template <class VectorT1, class VectorT2, class VectorT3>
	Log& winx_call printDim(const VectorT1& x, const VectorT2& y, const VectorT3& z)
	{
		size_t count = x.size();
		if (y.size() < count || z.size() < count)
		{
			print("\t[ Exception: Size of vector doesn't match! ]\n");
		}
		else
		{
			for (size_t i = 0; i < count; ++i)
				print("\t[ ").
				print(x[i]).print(", ").
				print(y[i]).print(", ").
				print(z[i]).print(" ]\n");
		}
		return *this;
	}

	template <class VectorT1, class VectorT2, class VectorT3, class VectorT4>
	Log& winx_call printDim(const VectorT1& x, const VectorT2& y, const VectorT3& z, const VectorT4& u)
	{
		size_t count = x.size();
		if (y.size() < count || z.size() < count || u.size() < count)
		{
			print("\t[ Exception: Size of vector doesn't match! ]\n");
		}
		else
		{
			for (size_t i = 0; i < count; ++i)
				print("\t[ ").
				print(x[i]).print(", ").
				print(y[i]).print(", ").
				print(z[i]).print(", ").
				print(u[i]).print(" ]\n");
		}
		return *this;
	}

public:
	void winx_call reportTestCaseError(
		const char* testClass,
		const char* testCase,
		const char* msg,
		const char* file, int line)
	{
		trace(
			"\n%s(%d):\n%s::%s()\n  ---> %s\n", 
			file, line, testClass, testCase, msg
			);
	}

	void winx_call reportError(
		const char* msg, const char* file, int line)
	{
		trace("\n%s(%d):\n  ---> %s\n", file, line, msg);
	}

	__STD_LOG_PRINT_CTYPES()
};

// =========================================================================
// class OutputLog, ErrorLog, FileLog

class OutputLog : public Log<FILEStorage>
{
public:
	OutputLog() : Log<FILEStorage>(stdout) {}
};

class ErrorLog : public Log<FILEStorage>
{
public:
	ErrorLog() : Log<FILEStorage>(stderr) {}
};

class FileLog : public Log<FILEStorage>
{
public:
	FileLog() {}
	FileLog(LPCSTR szFile)
		: Log<FILEStorage>(fopen(szFile, "w"))
	{
	}
	~FileLog()
	{
		m_stg.close();
	}

	void winx_call open(LPCSTR szFile)
	{
		WINX_ASSERT(!m_stg.good());
		m_stg.assign(fopen(szFile, "w"));
	}

	int winx_call good() const
	{
		return m_stg.good();
	}
};

// =========================================================================
// class StringLog

class StringLog : 
	public std::basic_string< char >,
	public Log< StringStorage< std::basic_string<char> > >
{
private:
	typedef std::basic_string< char > StorageT;
	typedef Log< StringStorage< std::basic_string<char> > > LogT;

public:
	StringLog() 
	{
		m_stg.assign(this);
	}

	template <class LogT>
	void winx_call trace(LogT& log) const
	{
		log.printString(*static_cast<const StorageT*>(this));
	}
};

// =========================================================================
// class MultiLog

template <UINT nStorage = 0, class StorageContainer = Array<ILogStorage*, nStorage> >
class MultiLog : public Log< MultiStorage<nStorage, StorageContainer> >
{
};

// =========================================================================
// class NullLog

class NullLog
{
public:
	typedef NullLog Log;

	void winx_call flush()							{}
	Log& winx_call step()							{ return *this; }
	Log& winx_call repeat(char ch)					{ return *this; }
	Log& winx_call repeat(char ch, size_t count)	{ return *this; }
	Log& winx_call repeat(WCHAR ch)					{ return *this; }
	Log& winx_call repeat(WCHAR ch, size_t count)	{ return *this; }
	Log& winx_call newline()						{ return *this; }

	Log& winx_call trace(const char* fmt, ...)		{ return *this; }
	Log& winx_call trace(const WCHAR* fmt, ...)		{ return *this; }

	template <class DataT>
	Log& winx_call print(const DataT& v)		
	{
		return *this;
	}

	template <class DataT, class ArgT1>
	Log& winx_call print(const DataT& v, ArgT1 arg1)
	{
		return *this;
	}
	
	template <class DataT, class ArgT1, class ArgT2>
	Log& winx_call print(const DataT& v, ArgT1 arg1, ArgT2 arg2)
	{
		return *this;
	}
	
	template <class DataT, class ArgT1, class ArgT2, class ArgT3>
	Log& winx_call print(const DataT& v, ArgT1 arg1, ArgT2 arg2, ArgT3 arg3)
	{
		return *this;
	}

	template <class _It>
	Log& winx_call printString(_It first, _It last)	{ return *this; }
	
	template <class _String>
	Log& winx_call printString(const _String& s)
	{
		return *this;
	}
	
	template <class ArgT, class DataIt>
	Log& winx_call printArray(
		ArgT fmt, DataIt array, size_t count,
		const char* bracketL = "{ ", const char* bracketR = " }\n",
		const char* dataSep = ", ")
	{
		return *this;
	}
	
public:
	void winx_call reportTestCaseError(
		const char* testClass,
		const char* testCase,
		const char* msg,
		const char* file, int line)
	{
	}
	
	void winx_call reportError(
		const char* msg, const char* file, int line)
	{
	}
};

// =========================================================================
// class TestLog

template <class LogT>
class TestLog : public TestCase
{
	WINX_TEST_SUITE(TestLog);
		WINX_TEST(testLog);
		WINX_TEST(testStringLog);
		WINX_TEST(testMultiLog);
	WINX_TEST_SUITE_END();

public:
	struct Obj
	{
		int m_data;
		Obj(int data) : m_data(data) {}

		template <class LogT2>
		void trace(LogT2& log) const {
			log.trace("%d", m_data);
		}
		
		template <class LogT2>
		void trace(LogT2& log, const char* fmt) const {
			log.trace(fmt, m_data);
		}
	};

	void testLog(LogT& log)
	{
		Obj obj(0x4224);

		log.print("\n---> print int\n");
		log.print(12).newline();
		log.print((short)123).newline();
		
		log.print("\n---> print int64\n");
		log.print((INT64)-1).newline();
		log.print((UINT64)-1).newline();
		log.print((INT64)-1, "0x%I64X").newline();

		log.print("\n---> print float\n");
		log.print(12.2).newline();
		log.print(1.0/3).newline();
		log.print(1.0/3, 1).newline();
		log.print(1.0/3, "%.8f").newline();

		log.print("\n---> print char\n");
		log.print('c').newline();
		log.print('c', "%d").newline();
		
		log.print("\n---> print string\n");
		log.print("abc").newline();
		log.print(L"abc").newline();
		log.print("abc", "%8s").newline();
		
		log.print("\n---> print string class\n");
		std::string s = "class";
		log.print(s).newline();
		log.print(s, "%8s").newline();

		log.print("\n---> print object\n");
		log.printObj(obj, "0x%.4x").newline();
		log.printObj(obj).newline();
	}

	void testStringLog(LogT& log)
	{
		std::StringLog slog;
		slog.print("\n--> print to string log\n");
		slog.print(1.0/3).newline();
		slog.print(1.0/3, 1).newline();
		slog.newline();
		log.print(slog);
	}

	void testMultiLog(LogT& log)
	{
		std::FILEStorageT<true> file("/__temp__.log");
		std::FILEStorageT<false> fout(stdout);
		std::MultiLog<2> mlog;
		mlog.storage().add(file);
		mlog.storage().add(fout);
		mlog.trace("Hello, MultiLog!!!\n");

		int sel = mlog.storage().select(0);
		mlog.trace("Message to file!!!\n");
		mlog.storage().select(sel);
	}
};

// =========================================================================
// $Log: Log.h,v $
// Revision 1.7  2007/01/10 09:32:25  xushiwei
// move UnitTest Assert(AssertExp, AssertFail, etc) from Log.h to TestCase.h
//
// Revision 1.6  2006/12/24 10:16:34  xushiwei
// STL-Extension: MultiStorage(select)
//
// Revision 1.3  2006/12/20 08:58:51  xushiwei
// STL-Extension: Container(Array), Log(MultiStorage, MultiLog)
//
// Revision 1.6  2006/09/26 08:34:56  xushiwei
// STL-Extension:
//  TestCase(WINX_TEST_APP, WINX_TEST_CLASS, WINX_TEST_SUITE, WINX_TEST, WINX_TEST_SUITE_END)
//  UnitTestAssert(AssertExp, AssertEq, AssertEqBuf, AssertMsg, AssertFail)
//
// Revision 1.3  2006/09/02 01:45:55  xushiwei
// STL-Extension:
//   Log: print RECT, SIZE
//   Memory: HeapMemAlloc - DefaultDynamicAlloc
//
// Revision 1.2  2006/08/19 04:40:48  xushiwei
// STL-Extension:
//   Memory(AutoFreeAlloc, RecycleBuffer, AutoArray, etc)
//   String Algorithm(trim, match, compare, etc), Container(SimpleMultiMap), CharType(isCSymbolFirstChar, etc)
//   Log(OutputLog, ErrorLog, FileLog, StringLog), PerformanceCounter, Diagnost(WINX_ASSERT, WINX_RUN_TEST, etc)
//

__NS_STD_END

#endif /* __STDEXT_LOG_H__ */
