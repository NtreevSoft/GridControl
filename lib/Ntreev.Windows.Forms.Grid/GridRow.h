#pragma once
#include "RowBase.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    namespace Native
    {
        class GrGridRow;
    }

    public ref class GridRow : RowBase
    {
    public:
        GridRow(Native::GrGridRow* pGridRow);

    public:
        property Ntreev::Windows::Forms::Grid::GridControl^ ChildGrid
        {
            Ntreev::Windows::Forms::Grid::GridControl^ get();
        }

    private:
        Native::GrGridRow* m_pGridRow;
    };

} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/