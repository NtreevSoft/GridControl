//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5792.31442
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


#include "stdafx.h"
#include "GroupRowCollection.h"
#include "GridControl.h"
#include "FromNative.h"
#include "GroupRow.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    GroupRowCollection::Enumerator::Enumerator(_GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_index = 0;
    }

    GroupRowCollection::Enumerator::~Enumerator()
    {

    }

    bool GroupRowCollection::Enumerator::MoveNext()
    {
        GrGridCore* pGridCore = this->GridCore;
        unsigned int count = 0;
        if(pGridCore->IsGrouped() == true)
        {
            count = pGridCore->GetDataRowList()->GetChildCount();
        }
        m_index++;
        return m_index <= count;
    }

    void GroupRowCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    GroupRow^ GroupRowCollection::Enumerator::Current::get()
    {
        GrDataRowList* pDataRowList = this->GridCore->GetDataRowList();
        GrGroupRow* pGroupRow = (GrGroupRow*)pDataRowList->GetChild(m_index - 1);

        return FromNative::Get(pGroupRow);
    }

    System::Object^ GroupRowCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return this->Current; 
    }

    GroupRowCollection::GroupRowCollection(_GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_pDataRowList = this->GridCore->GetDataRowList();
    }

    System::Collections::Generic::IEnumerator<GroupRow^>^ GroupRowCollection::GetEnumerator()
    {
        return gcnew Ntreev::Windows::Forms::Grid::GroupRowCollection::Enumerator(this->GridControl); 
    }

    GroupRow^ GroupRowCollection::default::get(int index)
    {
        if(index >= this->Count)
            throw gcnew System::ArgumentOutOfRangeException("index");

        GrGroupRow* pGroupRow = (GrGroupRow*)m_pDataRowList->GetChild(index);
        return FromNative::Get(pGroupRow);
    }

    GroupRow^ GroupRowCollection::default::get(System::String^ text)
    {
        for each(GroupRow^ item in this)
        {
            if(item->Text == text)
                return item;
        }
        return nullptr;
    }

    int GroupRowCollection::Count::get()
    {
        if(this->GridControl->IsGrouped == false)
            return 0;

        return m_pDataRowList->GetChildCount();
    }

    System::Collections::IEnumerator^ GroupRowCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return GetEnumerator();
    }

    void GroupRowCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(GroupRow^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    bool GroupRowCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true; 
    }

    System::Object^ GroupRowCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this;
    }

    int GroupRowCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count; 
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/