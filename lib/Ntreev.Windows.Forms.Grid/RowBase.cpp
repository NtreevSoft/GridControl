//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4461.30274
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


#include "StdAfx.h"
#include "RowBase.h"
#include "RowBaseCollection.h"
#include "Row.h"
#include "GroupRow.h"
#include "FromNative.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    RowBase::RowBase(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, IDataRow* pDataRow)
        : CellBase(gridControl, pDataRow), m_pDataRow(pDataRow)
    {

    }

    RowBase::RowBase(IDataRow* pDataRow)
        : CellBase(pDataRow), m_pDataRow(pDataRow)
    {

    }

    void RowBase::Focus()
    {
        Focuser->Set(m_pDataRow);
    }

    void RowBase::Height::set(int value)
    {
        if(value < 0)
            throw gcnew System::ArgumentOutOfRangeException("value");
        m_pDataRow->SetHeight(value);
    }

    bool RowBase::IsVisible::get()
    {
        return m_pDataRow->GetVisible();
    }

    unsigned int RowBase::DisplayIndex::get()
    {
        return m_pDataRow->GetDisplayIndex();
    }

    unsigned int RowBase::VisibleIndex::get()
    {
        return m_pDataRow->GetVisibleIndex();
    }

    bool RowBase::Focused::get()
    {
        return m_pDataRow->HasFocused();
    }

    Ntreev::Windows::Forms::Grid::RowBaseCollection^ RowBase::Childs::get()
    {
        if(m_childs == nullptr)
        {
            m_childs = gcnew Ntreev::Windows::Forms::Grid::RowBaseCollection(m_pDataRow);
        }
        return m_childs;
    }

    Ntreev::Windows::Forms::Grid::RowBase^ RowBase::Parent::get()
    {
        IDataRow* pParent = dynamic_cast<IDataRow*>(m_pDataRow->GetParent());
        if(pParent == nullptr)
            return nullptr;
        
        return FromNative::Get(pParent);
    }

    bool RowBase::ShouldSerializeHeight()
    {
        return m_pDataRow->GetHeight() != GrRow::DefaultHeight;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/