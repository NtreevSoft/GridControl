#include "StdAfx.h"
#include "GridStyleProvider.h"
#include "GridCell.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Private
{
	_Color RenderingStyle::BackColor::get()
	{
		return m_cell->RenderingBackColor;
	}

	_Color RenderingStyle::ForeColor::get()
	{
		return m_cell->RenderingForeColor;
	}

	_Font^ RenderingStyle::Font::get()
	{
		return m_cell->Font;
	}
} /*namespace Private*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/
