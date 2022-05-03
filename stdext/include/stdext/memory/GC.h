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
// Module: stdext/memory/GC.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-13 21:39:51
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MEMORY_GC_H__
#define __STDEXT_MEMORY_GC_H__

#ifndef __STDEXT_BASIC_H__
#include "../Basic.h"
#endif

__NS_STD_BEGIN

// -------------------------------------------------------------------------
// class _GCPtrBase

class _GCPtrBase
{
protected:
	_GCPtrBase* m_next;
	_GCPtrBase* m_prev;

protected:
	template <class T>
	void winx_call __init(T* obj) {
		if (obj) obj->gcListenPtr(this);
		else m_next = m_prev = this;
	}

	void winx_call __destroy() {
		m_prev->m_next = m_next;
		m_next->m_prev = m_prev;
	}

public:
	template <class T>
	_GCPtrBase(T* obj) { __init(obj); }
	_GCPtrBase() { m_next = m_prev = this; }
	~_GCPtrBase() { __destroy(); }

	template <class T>
	void winx_call assign(T* obj) {
		__destroy();
		__init(obj);
	}

	void winx_call swap(_GCPtrBase& b) {
		std::swap(m_prev, b.m_prev);
		std::swap(m_next, b.m_next);
	}

	void winx_call insertAfter(_GCPtrBase* node) {
		node->m_next = m_next;
		node->m_prev = this;
		m_next->m_prev = node;
		m_next = node;
	}

	void winx_call insertBefore(_GCPtrBase* node) {
		node->m_next = this;
		node->m_prev = m_prev;
		m_prev->m_next = node;
		m_prev = node;
	}
};

// -------------------------------------------------------------------------
// class GCPtr

template <class T>
class GCPtr : private _GCPtrBase
{
private:
	T* m_obj;

public:
	GCPtr() {}
	GCPtr(T* obj) : _GCPtrBase(obj), m_obj(obj) {}
	GCPtr(const GCPtr& lp) : _GCPtrBase(lp.m_obj), m_obj(lp.m_obj) {}

	operator T*() const {
		return m_obj;
	}

	bool winx_call operator!() const {
		return (m_obj != NULL);
	}

	bool winx_call operator<(T* obj) const {
		return m_obj < obj;
	}

	bool winx_call operator==(T* obj) const {
		return m_obj == obj;
	}

	void winx_call swap(GCPtr& b) {
		_GCPtrBase::swap(b);
		std::swap(m_obj, b.m_obj);
	}

	void winx_call assign(T* obj) {
		_GCPtrBase::assign(obj);
		m_obj = obj;
	}

	template <class AllocT>
	void winx_call gc(AllocT& alloc) {
		if (m_obj)
			m_obj->gcClone(alloc);
	}

	T* winx_call operator=(T* obj) {
		_GCPtrBase::assign(obj);
		return m_obj = obj;
	}

	T* winx_call operator=(const GCPtr& lp) {
		_GCPtrBase::assign(lp.m_obj);
		return m_obj = lp.m_obj;
	}

	T* winx_call operator->() const {
		return m_obj;
	}

	T& winx_call operator*() const {
		return *m_obj;
	}
};

// -------------------------------------------------------------------------
// class _GCObjectBase

class GCAlloc;
class _GCObjectBase : protected _GCPtrBase
{
protected:
	mutable GCAlloc* m_owner;
	friend class GCAlloc;
};

// -------------------------------------------------------------------------
// class GCAlloc

class GCAlloc : private AutoFreeAlloc
{
public:
	void* winx_call alloc_obj(size_t cb) {
		_GCObjectBase* obj = (_GCObjectBase*)AutoFreeAlloc::allocate(cb);
		obj->m_owner = this;
		return obj;
	}

	template <class DestructorT>
	void* winx_call alloc_obj(size_t cb, DestructorT fn) {
		_GCObjectBase* obj = (_GCObjectBase*)AutoFreeAlloc::allocate(cb, fn);
		obj->m_owner = this;
		return obj;
	}
};

#define STD_GC_NEW(alloc, Type)												\
	::new((alloc).alloc_obj(MEMORY_NEW_ARG(Type))) Type

// -------------------------------------------------------------------------
// class GCObject

template <class T>
class GCObject : private _GCObjectBase
{
public:
	T* winx_call gcClone(GCAlloc& newAlloc)
	{
		T* pThis = static_cast<T*>(this);
		if (m_owner == &newAlloc)
			return pThis;

		T* obj = STD_GC_NEW(newAlloc, T)(*pThis);
		obj->gc(newAlloc);
		while (m_next != m_prev) {
			((GCPtr<T>*)(void*)m_next)->assign(obj);
		}
		return obj;
	}

	void winx_call gcListenPtr(_GCPtrBase* p)
	{
		_GCPtrBase::insertAfter(p);
	}

	void winx_call gc(GCAlloc& newAlloc)
	{
	}
};

// -------------------------------------------------------------------------
// WINX_GC_BEGIN..WINX_GC_END

#define WINX_GC_BEGIN()			 											\
public:																		\
	void winx_call gc(GCAlloc& _gc_newAlloc) {

#define WINX_GC(member)														\
		member.gc(_gc_newAlloc);

#define WINX_GC_END()														\
	}

// -------------------------------------------------------------------------
// class TestGC

template <class LogT>
class TestGC : public TestCase
{
public:
	WINX_TEST_SUITE(TestGC);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	class SubA : public GCObject<SubA>
	{
	private:
		int a;
	public:
		SubA() : a(12) {}
	};

	class A : public GCObject<A>
	{
		WINX_GC_BEGIN()
			WINX_GC(m_pSubA)
			WINX_GC(m_pSubA2)
			WINX_GC(m_a)
		WINX_GC_END();
	private:
		GCPtr<SubA> m_pSubA;
		GCPtr<SubA> m_pSubA2;
		SubA m_a;

	public:
		A(GCAlloc& alloc) {
			m_pSubA = STD_GC_NEW(alloc, SubA);
			m_pSubA2 = m_pSubA;
		}
	};

	void test(LogT& log)
	{
		GCAlloc alloc;
		GCAlloc alloc2;

		GCPtr<A> p = STD_GC_NEW(alloc, A)(alloc);
		p.gc(alloc2);
	}
};

// -------------------------------------------------------------------------
// $Log: $

__NS_STD_END

#endif /* __STDEXT_MEMORY_GC_H__ */
