//=====================================================================================================================
// Ntreev Grid for .Net 2.0.0.0
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

class IEventHandler
{
public:
    virtual size_t    obj() const = 0;
    virtual size_t    func() const = 0;
    virtual size_t    size() const = 0;

    virtual void    Raise(GrObject* pObject, void* args) const = 0;
};

template<typename T, typename ARG>
class  GrEventHandler : public IEventHandler
{
public:
    typedef void (T::*Delegate)(GrObject*, ARG*);

    GrEventHandler(T* obj, void (T::*dele)(GrObject*, ARG*))
        : m_delegate(dele), m_obj(obj)
    {
        m_size = sizeof(*obj);
    }

    GrEventHandler(const GrEventHandler<T, ARG>& handler)
        : m_delegate(handler.m_delegate), m_obj(handler.m_obj), m_size(handler.m_size)
    {
        
    }

    virtual ~GrEventHandler()
    {

    }

    virtual void Raise(GrObject* pObject, void* args) const
    {
        (m_obj->*m_delegate)(pObject, (ARG*)args);
    }

    virtual size_t obj() const { return (size_t)m_obj; }
    virtual size_t func() const { return *(size_t*)&m_delegate; }
    virtual size_t size() const { return m_size; }

private:
    Delegate    m_delegate;
    T*          m_obj;
    size_t      m_size;
};

template<typename ARG, class OWNER>
class GrEvent : public GrObject
{
    struct GrLess
    {
        bool operator () (const IEventHandler* b1, const IEventHandler* b2) const
        {
            if(b1->obj() == b2->obj())
                return b1->size() < b2->size();
            return b1->obj() < b2->obj();
        }
    };
    typedef std::set< IEventHandler*, GrLess > _Handlers;

public:
    GrEvent()
    {
        
    }

    virtual ~GrEvent()
    {
        typename _Handlers::iterator itor = m_handlers.begin();
        while(itor != m_handlers.end())
        {
            delete (*itor);
            itor++;
        }
    }

    template<typename T>
    void Add(T* obj, void (T::*function)(GrObject*, ARG*))
    {
        Add(GrEventHandler<T, ARG>(obj, function));
    }

    template<typename T>
    void Add(GrEventHandler<T, ARG> handler)
    {
        GrEventHandler<T, ARG>* pEventHandler = new GrEventHandler<T, ARG>(handler);
        std::pair< typename _Handlers::iterator, bool> p = m_handlers.insert(pEventHandler);
        if(p.second == false)
            throw new std::exception();
    }

    template<typename T>
    void Remove(T *obj, void (T::*function)(GrObject*, ARG*))
    {
        Remove(GrEventHandler<T, ARG>(obj, function));
    }

    template<typename T>
    void Remove(GrEventHandler<T, ARG> handler)
    {
        typename _Handlers::iterator itor = m_handlers.find(&handler);
        if(itor == m_handlers.end())
            throw new std::exception();
        delete *itor;
        m_handlers.erase(itor);
    }

#ifdef _MSC_VER
private:
#else
public:
#endif
    void Raise(GrObject* pSender, ARG* e) const
    {
        typename _Handlers::const_iterator itor = m_handlers.begin();
        while(itor != m_handlers.end())
        {
            const IEventHandler* subscriber = *itor;
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
        return m_handlers.size() == 0 ? true : false;
    }

private:
    _Handlers        m_handlers;

#ifdef _MSC_VER 
    friend OWNER;
#endif

};

class GrEventArgs : public GrObject
{
public:
    static GrEventArgs Empty;
};