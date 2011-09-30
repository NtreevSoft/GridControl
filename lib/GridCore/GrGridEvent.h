//=====================================================================================================================
// Ntreev Grid for .Net 1.0
// https://github.com/NtreevSoft/GridControl
// 
// Released under the MIT License.
// 
// Copyright (c) 2010 Ntreev Soft co., Ltd.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
// persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
// Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//=====================================================================================================================


#pragma once
#include "GrGridBase.h"
#include <set>

template<typename ARG, class OWNER>
class GrEventBase : public GrObject
{
	class EventHandlerBase
	{
	public:
		virtual size_t	obj() const = 0;
		virtual size_t	func() const = 0;

		virtual void	Raise(GrObject* pObject, void* args) const = 0;
	};

	template<typename U, typename ARG>
	class  GrEventHandlerBase : public EventHandlerBase
	{
	public:
		typedef void (U::*MemberFunctionType)(GrObject*,ARG*);

		GrEventHandlerBase(U *obj, void (U::*function)(GrObject*, ARG*))
			: m_func(function), m_obj(obj)
		{

		}

		~GrEventHandlerBase()
		{

		}

		virtual void Raise(GrObject* pObject, void* args) const
		{
			(m_obj->*m_func)(pObject, (ARG*)args);
		}

		virtual size_t obj() const { return (size_t)m_obj; }
		virtual size_t func() const { return *(size_t*)&m_func; }

	private:
		MemberFunctionType	m_func;
		U*					m_obj;
	};

	struct eventless
	{
		bool operator () (const EventHandlerBase* b1, const EventHandlerBase* b2) const
		{
			if(b1->obj() == b2->obj())
				return b1->func() < b2->func();
			return b1->obj() < b2->obj();
		}
	};

	typedef std::set< EventHandlerBase*, eventless > _Handlers;

public:
	GrEventBase() {}

	template<typename T>
	void Add(T *obj, void (T::*function)(GrObject*, ARG*))
	{
		EventHandlerBase* newEvent = new GrEventHandlerBase<T, ARG>(obj, function);
		std::pair< _Handlers::iterator, bool> p = m_setSubscribers.insert(newEvent);
	}

	template<typename T>
	void Remove(T *obj, void (T::*function)(GrObject*, ARG*))
	{
		EventHandlerBase* newEvent = new GrEventHandlerBase<T, ARG>(obj, function);

		GrEventHandlerBase<T, ARG> dm1(obj, function);
		GrEventHandlerBase<T, ARG> dm2 = dm1;

		m_setSubscribers.erase(newEvent);
	}

private:
	void Raise(GrObject* pSender, ARG* e) const
	{
		_Handlers::const_iterator itor = m_setSubscribers.begin();
		while(itor != m_setSubscribers.end())
		{
			const EventHandlerBase* subscriber = *itor;
			subscriber->Raise(pSender, (void*)e);
			itor++;
		}
	}

	void operator () (GrObject* pSender, ARG* e) const
	{
		if(IsEmpty() == true)
			return;
		Raise(pSender, e);
	}

	bool IsEmpty() const
	{
		return m_setSubscribers.size() == 0 ? true : false;
	}

private:
	_Handlers		m_setSubscribers;

	friend OWNER;
};

class GrEventArgs : public GrObject
{
public:
	static GrEventArgs Empty;
};

