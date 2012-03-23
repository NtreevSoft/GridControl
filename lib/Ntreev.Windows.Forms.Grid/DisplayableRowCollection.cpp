//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4464.32161
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
#include "DisplayableRowCollection.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    DisplayableRowCollection::Enumerator::Enumerator(GrDataRowList* pDataRowList)
        : m_pDataRowList(pDataRowList)
    {
        m_index = 0;
    }

    DisplayableRowCollection::Enumerator::~Enumerator()
    {

    }

    bool DisplayableRowCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_pDataRowList->GetDisplayableRowCount();
    }

    void DisplayableRowCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    Ntreev::Windows::Forms::Grid::RowBase^ DisplayableRowCollection::Enumerator::Current::get()
    {
        IDataRow* pDataRow = m_pDataRowList->GetDisplayableRow(m_index - 1);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::RowBase^>(ref);
    }

    System::Object^ DisplayableRowCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return Current; 
    }

    DisplayableRowCollection::DisplayableRowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_pDataRowList = this->GridCore->GetDataRowList();
    }

    System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::RowBase^>^ DisplayableRowCollection::GetEnumerator()
    {
        return gcnew Ntreev::Windows::Forms::Grid::DisplayableRowCollection::Enumerator(m_pDataRowList); 
    }

    Ntreev::Windows::Forms::Grid::RowBase^ DisplayableRowCollection::default::get(int index)
    {
        if((unsigned int)index >= m_pDataRowList->GetDisplayableRowCount())
            throw gcnew System::ArgumentOutOfRangeException("index");
        IDataRow* pDataRow = m_pDataRowList->GetDisplayableRow(index);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::RowBase^>(ref);
    }

    int DisplayableRowCollection::Count::get()
    {
        return m_pDataRowList->GetDisplayableRowCount();
    }

    System::Collections::IEnumerator^ DisplayableRowCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return GetEnumerator();
    }

    void DisplayableRowCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(Ntreev::Windows::Forms::Grid::RowBase^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    bool DisplayableRowCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true; 
    }

    System::Object^ DisplayableRowCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this;
    }

    int DisplayableRowCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count; 
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/