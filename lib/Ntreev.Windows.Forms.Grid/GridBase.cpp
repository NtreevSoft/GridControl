#include "StdAfx.h"
#include "GridBase.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	GridObject::GridObject()
	{
		
	}

	GridObject::GridObject(_GridControl^ gridControl)
		: m_gridControl(gridControl), 
		m_pGridCore(gridControl->GridCore), 
		m_pGridRenderer(gridControl->GridRenderer), 
		m_pItemSelector(m_pGridCore->GetItemSelector()),
		m_pFocuser(m_pGridCore->GetFocuser())
	{

		
	}

	void GridObject::GridControl::set(_GridControl^ gridControl)
	{
		if(gridControl != nullptr)
		{
			m_gridControl	= gridControl;
			m_pGridCore		= gridControl->GridCore;
			m_pGridRenderer = gridControl->GridRenderer;
			m_pItemSelector = m_pGridCore->GetItemSelector();
			m_pFocuser		= m_pGridCore->GetFocuser();
		}
		else
		{
			m_gridControl	= nullptr;
			m_pGridCore		= nullptr;
			m_pGridRenderer = nullptr;
			m_pItemSelector = nullptr;
			m_pFocuser		= nullptr;
		}
	}

	void GridObject::Invalidate()
	{
		if(m_gridControl == nullptr)
			return;
		m_gridControl->Invalidate(false);
	}

	void GridObject::Invalidate(int left, int top, int right, int bottom)
	{
		m_gridControl->Invalidate(_Rectangle::FromLTRB(left, top, right, bottom), false);
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/