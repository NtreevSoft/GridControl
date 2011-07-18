#include "StdAfx.h"
#include "GridType.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	int HitTest::Width::get()
	{
		return cellRect.Width;
	}

	int	HitTest::Height::get()
	{
		return cellRect.Height;
	}

	_Point HitTest::Location::get()
	{
		return cellRect.Location;
	}

	_Size HitTest::Size::get()
	{
		return cellRect.Size;
	}

	GrCellType HitTest::CellType::get()
	{
		if(pHittedCell == nullptr)
			return GrCellType_Unknown;
		return pHittedCell->GetCellType();
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/