#include "StdAfx.h"
#include "GroupRow.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    GroupRow::GroupRow(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, GrGroupRow* pGroupRow)
        : RowBase(gridControl, pGroupRow), m_pGroupRow(pGroupRow)
    {

    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/