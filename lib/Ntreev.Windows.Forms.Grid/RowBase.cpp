//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4478.19833
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
    RowBase::RowBase(_GridControl^ gridControl, IDataRow* pDataRow)
        : CellBase(gridControl, pDataRow), m_pDataRow(pDataRow)
    {

    }

    RowBase::RowBase(IDataRow* pDataRow)
        : CellBase(pDataRow), m_pDataRow(pDataRow)
    {

    }

    void RowBase::Focus()
    {
        this->Focuser->Set(m_pDataRow);
    }

	int RowBase::Height::get()
	{
		return m_pDataRow->GetHeight();
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

    int RowBase::DisplayIndex::get()
    {
        return (int)m_pDataRow->GetDisplayIndex();
    }

    int RowBase::VisibleIndex::get()
    {
        return (int)m_pDataRow->GetVisibleIndex();
    }

    bool RowBase::IsFocused::get()
    {
        return m_pDataRow->HasFocused();
    }

    bool RowBase::IsExpanded::get()
    {
        return m_pDataRow->IsExpanded();
    }

    void RowBase::IsExpanded::set(bool value)
    {
        m_pDataRow->Expand(value);
    }

    RowBaseCollection^ RowBase::Childs::get()
    {
        if(m_childs == nullptr)
        {
            m_childs = gcnew RowBaseCollection(m_pDataRow);
        }
        return m_childs;
    }

    RowBase^ RowBase::Parent::get()
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