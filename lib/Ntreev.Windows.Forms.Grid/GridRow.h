#pragma once
#include "RowBase.h"

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
        property _GridControl^ ChildGrid
        {
            _GridControl^ get();
        }

    private:
        Native::GrGridRow* m_pGridRow;
    };

} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/