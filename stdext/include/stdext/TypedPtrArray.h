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
// Module: stdext/TypedPtrArray.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-3 2:18:33
// 
// $Id: TypedPtrArray.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_TYPEDPTRARRAY_H__
#define __STDEXT_TYPEDPTRARRAY_H__

#ifndef __STDEXT_BASIC_H__
#include "Basic.h"
#endif

#ifndef _VECTOR_
#include <vector>
#endif

#ifndef _ALGORITHM_
#include <algorithm>
#endif

#ifndef __NS_STD
#define __NS_STD			std::
#define __NS_STD_BEGIN		namespace std {
#define __NS_STD_END		}
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// _ConvIt - vector iterator convert

#if defined(_MSC_VER) && (_MSC_VER > 1200) // visual c++ .net

template <class _It>
__forceinline typename _It::pointer _ConvIt(_It it)
	{ return it._Myptr; }

#else

template <class _It>
__forceinline _It _ConvIt(_It it)
	{ return it; }

#endif

// -------------------------------------------------------------------------
// class TypedPtrArray

template <class _PtrType>
class TypedPtrArray : private std::vector<void*>
{
private:
	typedef std::vector<void*> BaseClass;
	typedef TypedPtrArray<_PtrType> _Myt;

	TypedPtrArray(const TypedPtrArray&);
	void operator=(const TypedPtrArray&);

public:
	typedef BaseClass::size_type size_type;
	typedef _PtrType value_type;
	
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	
	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef pointer iterator;
	typedef const_pointer const_iterator;

public:
	using BaseClass::size;
	using BaseClass::resize;
	using BaseClass::empty;
	using BaseClass::reserve;
	using BaseClass::pop_back;
	using BaseClass::clear;

#if (0)
	size_t size();
	bool empty();
	void resize(size_type n, value_type val = NULL);
	void reserve(size_type n);
	void pop_back();
	void clear();
#endif

	TypedPtrArray() {}

	VOID winx_call swap(_Myt& o)
	{
		BaseClass::swap((BaseClass&)o);
	}

	void winx_call binary_insert(value_type val)
	{
		BaseClass::insert(
			std::lower_bound(BaseClass::begin(), BaseClass::end(), (void*)val), val);
	}
	
	const_iterator winx_call binary_find(value_type val) const
	{
		BaseClass::iterator itEnd = BaseClass::end();
		BaseClass::iterator it = std::lower_bound(BaseClass::begin(), itEnd, (void*)val);
		if (it != itEnd && (*it) == val)
			return it;
		else
			return itEnd;
	}

	size_type winx_call erase_unique(value_type val, size_type iEraseFrom = 0)
	{
		for (size_type i = iEraseFrom; i < BaseClass::size(); ++i)
		{
			if (BaseClass::at(i) == (void*)val)
			{
				BaseClass::erase(BaseClass::begin() + i);
				return 1;
			}
		}
		return 0;
	}

	template <class _Ty, class _Equal>
	size_type winx_call erase_unique(_Ty val, _Equal eq, size_type iEraseFrom = 0)
	{
		for (size_type i = iEraseFrom; i < BaseClass::size(); ++i)
		{
			if (eq((value_type)BaseClass::at(i), val))
			{
				BaseClass::erase(BaseClass::begin() + i);
				return 1;
			}
		}
		return 0;
	}

	bool winx_call pop_back(value_type* val)
	{
		if (BaseClass::empty())
			return false;
		else
		{
			*val = (value_type)BaseClass::back();
			BaseClass::pop_back();
			return true;
		}
	}

	void winx_call push_back(value_type val)
		{ BaseClass::push_back(val); }
	
	const_reference winx_call at(size_type i) const
		{ return (const_reference)BaseClass::at(i); }

	reference winx_call at(size_type i)
		{ return (reference)BaseClass::at(i); }

	reference winx_call front()
		{ return (reference)BaseClass::front(); }

	const_reference winx_call front() const
		{ return (const_reference)BaseClass::front(); }

	reference winx_call back()
		{ return (reference)BaseClass::back(); }

	const_reference winx_call back() const
		{ return (const_reference)BaseClass::back(); }

	iterator winx_call begin()
		{ return (iterator)_ConvIt(BaseClass::begin()); }

	iterator winx_call end()
		{ return (iterator)_ConvIt(BaseClass::end()); }

	const_iterator winx_call begin() const
		{ return (const_iterator)_ConvIt(BaseClass::begin()); }

	const_iterator winx_call end() const
		{ return (const_iterator)_ConvIt(BaseClass::end()); }
};

// -------------------------------------------------------------------------
// class InterfaceArray

template <class Interface>
class InterfaceArray : private std::vector<void*>
{
private:
	typedef std::vector<void*> BaseClass;
	typedef Interface* _PtrType;
	typedef InterfaceArray<Interface> _Myt;

	InterfaceArray(const InterfaceArray&);
	void operator=(const InterfaceArray&);

public:
	typedef BaseClass::size_type size_type;
	typedef _PtrType value_type;
	
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	
	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef pointer iterator;
	typedef const_pointer const_iterator;

public:
	using BaseClass::size;
	using BaseClass::empty;
	using BaseClass::reserve;

#if (0)
	size_t size();
	bool empty();
	void reserve(size_type n);
#endif

	InterfaceArray() {}
	~InterfaceArray() { clear(); }

	void winx_call swap(const _Myt& o)
	{
		BaseClass::swap((BaseClass&)o);
	}

	void winx_call binary_insert(value_type val)
	{
		BaseClass::insert(
			std::lower_bound(BaseClass::begin(), BaseClass::end(), (void*)val), val);
		if (val)
			val->AddRef();
	}
	
	const_iterator winx_call binary_find(value_type val) const
	{
		BaseClass::const_iterator itEnd = BaseClass::end();
		BaseClass::const_iterator it = std::lower_bound(BaseClass::begin(), itEnd, (void*)val);
		if (it != itEnd && (*it) == val)
			return it;
		else
			return itEnd;
	}

	bool winx_call exist(value_type val) const
	{
		BaseClass::const_iterator itEnd = BaseClass::end();
		return std::find(BaseClass::begin(), itEnd, (void*)val) != itEnd;
	}

	const_iterator winx_call find(value_type val) const
	{
		return std::find(BaseClass::begin(), BaseClass::end(), (void*)val);
	}

	size_type winx_call insert_unique(value_type val)
	{
		BaseClass::iterator itEnd = BaseClass::end();
		BaseClass::iterator it = std::find(BaseClass::begin(), itEnd, (void*)val);
		if (it != itEnd)
			return 0;

		BaseClass::push_back(val);
		if (val)
			val->AddRef();
		return 1;
	}

	size_type winx_call erase_unique(value_type val)
	{
		BaseClass::iterator itEnd = BaseClass::end();
		BaseClass::iterator it = std::find(BaseClass::begin(), itEnd, (void*)val);
		if (it == itEnd)
			return 0;

		BaseClass::erase(it);
		if (val)
			val->Release();
		return 1;
	}
	
	size_type winx_call erase_unique(value_type val, size_type iEraseFrom)
	{
		for (size_type i = iEraseFrom; i < BaseClass::size(); ++i)
		{
			if (BaseClass::at(i) == (void*)val)
			{
				if (val)
					val->Release();
				BaseClass::erase(BaseClass::begin() + i);
				return 1;
			}
		}
		return 0;
	}

	template <class _Ty, class _Equal>
	size_type winx_call erase_unique(
		_Ty val, _Equal eq, size_type iEraseFrom = 0, value_type* itemDetach = NULL)
	{
		for (size_type i = iEraseFrom; i < BaseClass::size(); ++i)
		{
			value_type item = (value_type)BaseClass::at(i);
			if (eq(item, val))
			{
				if (itemDetach)
					*itemDetach = item;
				else if (item)
					item->Release();
				BaseClass::erase(BaseClass::begin() + i);
				return 1;
			}
		}
		return 0;
	}

	bool winx_call pop_back(value_type* val)
	{
		if (BaseClass::empty())
			return false;
		else
		{
			*val = (value_type)BaseClass::back();
			BaseClass::pop_back();
			return true;
		}
	}
	
	void winx_call pop_back()
	{
		value_type val = (value_type)BaseClass::back();
		BaseClass::pop_back();
		if (val)
			val->Release();
	}

	void winx_call push_back(value_type val)
	{
		BaseClass::push_back(val);
		if (val)
			val->AddRef();
	}

	void winx_call attach_push_back(value_type val)
	{
		BaseClass::push_back(val);
	}
	
	void winx_call attach_insert(size_type pos, value_type val)
	{
		BaseClass::insert(BaseClass::begin() + pos, val);
	}

	void winx_call resize(size_type n)
	{
		for (size_type i = n; i < BaseClass::size(); ++i)
		{
			value_type item = (value_type)BaseClass::at(i);
			if (item)
				item->Release();
		}
		BaseClass::resize(n);
	}

	void winx_call clear()
	{
		for (BaseClass::iterator it = BaseClass::begin(); it != BaseClass::end(); ++it)
		{
			value_type item = (value_type)*it;
			if (item)
				item->Release();
		}
		BaseClass::clear();
	}

	template <class _Ty>
	long winx_call item(size_type i, _Ty* val)
	{
		if (i < BaseClass::size())
		{
			*val = (value_type)BaseClass::at(i);
			if (*val)
				(*val)->AddRef();
			return 0;
		}
		return 0x80000003L; // E_INVALIDARG - no need include <winerror.h>
	}

	const_reference winx_call at(size_type i) const
		{ return (const_reference)BaseClass::at(i); }

	reference winx_call at(size_type i)
		{ return (reference)BaseClass::at(i); }

	reference winx_call front()
		{ return (reference)BaseClass::front(); }

	const_reference winx_call front() const
		{ return (const_reference)BaseClass::front(); }

	reference winx_call back()
		{ return (reference)BaseClass::back(); }

	const_reference winx_call back() const
		{ return (const_reference)BaseClass::back(); }

	iterator winx_call begin()
		{ return (iterator)_ConvIt(BaseClass::begin()); }

	iterator winx_call end()
		{ return (iterator)_ConvIt(BaseClass::end()); }

	const_iterator winx_call begin() const
		{ return (const_iterator)_ConvIt(BaseClass::begin()); }

	const_iterator winx_call end() const
		{ return (const_iterator)_ConvIt(BaseClass::end()); }
};

// -------------------------------------------------------------------------
// $Log: TypedPtrArray.h,v $
// Revision 1.1  2006/10/18 12:13:39  xushiwei
// stdext as independent component
//
// Revision 1.1  2006/09/03 04:30:02  xushiwei
// STL-Extension:
//   Container: TypedPtrArray, InterfaceArray
//

__NS_STD_END

#endif /* __STDEXT_TYPEDPTRARRAY_H__ */
