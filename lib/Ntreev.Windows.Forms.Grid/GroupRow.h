#pragma once
#include "RowBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    public ref class GroupRow : Ntreev::Windows::Forms::Grid::RowBase
    {
    public:

    public:

        //property bool IsExpanded
        //{
        //    bool get();
        //    void set(bool);
        //}

        //property System::String^ Text
        //{
        //    System::String^ get();
        //}



    internal: // methods

        GroupRow(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, GrGroupRow* pGroupRow);


    private: // variables

        GrGroupRow* m_pGroupRow;

    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/