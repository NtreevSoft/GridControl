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
#include "SelectedColumnCollection.h"
#include "GridControl.h"
#include "FromNative.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    SelectedColumnCollection::Enumerator::Enumerator(const GrSelectedColumns* selectedColumns)
        : m_selectedColumns(selectedColumns)
    {
        m_index = 0;
    }

    SelectedColumnCollection::Enumerator::~Enumerator()
    {

    }

    bool SelectedColumnCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_selectedColumns->size();
    }

    void SelectedColumnCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    Column^ SelectedColumnCollection::Enumerator::Current::get()
    {
        GrColumn* pColumn = (*m_selectedColumns)[m_index - 1];
        return FromNative::Get(pColumn);
    }

    System::Object^ SelectedColumnCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return Current; 
    }

    SelectedColumnCollection::SelectedColumnCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, const GrSelectedColumns* selectedColumns) 
        : GridObject(gridControl), m_selectedColumns(selectedColumns)
    {

    }

    void SelectedColumnCollection::Add(Column^ item)
    {
        if(item->IsSelected == true)
            return;
        item->IsSelected = true;
    }

    bool SelectedColumnCollection::Remove(Column^ item)
    {
        if(item->IsSelected == false)
            return false;
        item->IsSelected = false;
        return true;
    }

    void SelectedColumnCollection::Clear()
    {
        this->GridControl->ClearSelection();
    }

    bool SelectedColumnCollection::Contains(Column^ column)
    {
        return column->IsSelected == true;
    }

    int SelectedColumnCollection::Count::get()
    {
        return (int)m_selectedColumns->size();
    }

    System::Collections::Generic::IEnumerator<Column^>^ SelectedColumnCollection::GetEnumerator()
    {
        return gcnew Ntreev::Windows::Forms::Grid::SelectedColumnCollection::Enumerator(m_selectedColumns);
    }

    void SelectedColumnCollection::CopyTo(cli::array<Column^>^ array, int arrayIndex)
    {
        for each(Column^ item in this)
        {
            array->SetValue(item, arrayIndex++);
        }
    }

    cli::array<Column^>^ SelectedColumnCollection::ToArray()
    {
        cli::array<Column^>^ array = gcnew cli::array<Column^>(this->Count);
        this->CopyTo(array, 0);
        return array;
    }

    System::Collections::IEnumerator^ SelectedColumnCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return GetEnumerator();
    }

    void SelectedColumnCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(Column^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    bool SelectedColumnCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true;
    }

    System::Object^ SelectedColumnCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this; 
    }

    int SelectedColumnCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count;
    }

    bool SelectedColumnCollection::IsReadOnly_System_Collections_Generic_ICollection::get()
    {
        return false;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/