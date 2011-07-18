#pragma once

#define _SECURE_SCL 0
#include <vector>
#include <algorithm>
#include "GrGridType.h"

class GrGridCore;

class GrObject
{
public:
	GrObject() : m_pGridCore(0) {}

protected:
	virtual void				OnGridCoreAttached() {};
	virtual void				OnGridCoreDetached() {};

protected:
	GrGridCore*					m_pGridCore;

	friend class GrGridCore;
};


template<class T>
class GrCollection
{
public:
	typedef typename T::value_type value_type;
public:
	GrCollection(const T& container)
		: m_container(container)
	{

	}

	GrCollection(const GrCollection& object)
		: m_container(object.m_container)
	{

	}

	
	GrCollection& operator = (const GrCollection& object)
	{
		m_container = object.m_container;
	}

private:
	const T& m_container;

};