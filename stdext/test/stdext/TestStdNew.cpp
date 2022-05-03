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
// Module: TestStdNew.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-3 12:59:08
// 
// $Id: TestStdNew.cpp,v 1.1 2006/12/03 05:32:36 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <stdext.h>
#undef STD_NEW

// -------------------------------------------------------------------------

#define STD_MANAGE		operator->*
#define STD_NEW(a)		(a)->*new(TheAllocFlag, (a))

// -------------------------------------------------------------------------

enum AllocFlag { TheAllocFlag = 0 };

class MyAlloc
{
public:
	void* allocate(size_t cb)
	{
		return malloc(cb);
	}

	template <class Type>
	Type* STD_MANAGE(Type* p)
	{
		return p;
	}
};

template <class AllocType>
__forceinline void* operator new(unsigned int cb, AllocFlag, AllocType& alloc)
{
	return alloc.allocate(cb);
}

// -------------------------------------------------------------------------

class A
{
	int a;

public:
	~A()
	{
		a = 0;
	}
};

inline void Test()
{
	MyAlloc a;
	STD_NEW(a) int;
	STD_NEW(a) A;
#if defined(_MSC_VER)
	STD_NEW(a) A[10];
#endif
}

// -------------------------------------------------------------------------
// $Log: TestStdNew.cpp,v $
// Revision 1.1  2006/12/03 05:32:36  xushiwei
// STD_NEW new style (just a try)
//
