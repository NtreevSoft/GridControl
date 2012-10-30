#include "stdafx.h"
#include "GridRow.h"
#include "NativeGridRow.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    GridRow::GridRow(Native::GrGridRow* pGridRow)
        : m_pGridRow(pGridRow), RowBase(pGridRow)
    {
    }

    Ntreev::Windows::Forms::Grid::GridControl^ GridRow::ChildGrid::get()
    {
        return m_pGridRow->GetChildGrid();
    }


} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/

