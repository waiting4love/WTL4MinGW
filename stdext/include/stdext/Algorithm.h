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
// Module: stdext/Algorithm.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-30 16:43:10
// 
// $Id: Algorithm.h,v 1.1 2006/11/30 08:45:42 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_ALGORITHM_H__
#define __STDEXT_ALGORITHM_H__

#ifndef __STDEXT_BASIC_H__
#include "Basic.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// algorithm - set_intersection_do

template <class _InputIter1, class _InputIter2, class _Op>
inline void set_intersection_do(
	_InputIter1 first1, _InputIter1 last1, _InputIter2 first2, _InputIter2 last2, _Op& op)
{
	for (; first1 != last1 && first2 != last2; )
	{
		if (*first1 < *first2)
			++first1;
		else if (*first2 < *first1)
			++first2;
		else
			op(*first1++, *first2++);
	}
}

template<class _InputIter1, class _InputIter2, class _Op, class _Pred>
inline void set_intersection_do(
	_InputIter1 first1, _InputIter1 last1, _InputIter2 first2, _InputIter2 last2, _Op& op, _Pred pred)
{
	for (; first1 != last1 && first2 != last2; )
		if (pred(*first1, *first2))
			++first1;
		else if (pred(*first2, *first1))
			++first2;
		else
			op(*first1++, *first2++);
}

// -------------------------------------------------------------------------
// $Log: Algorithm.h,v $
// Revision 1.1  2006/11/30 08:45:42  xushiwei
// set_intersection_do
//

__NS_STD_END

#endif /* __STDEXT_ALGORITHM_H__ */
