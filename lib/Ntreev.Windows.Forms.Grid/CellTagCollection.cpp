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
#include "CellTagCollection.h"
#include "CellCollection.h"
#include "Column.h"
#include "Row.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    CellTagCollection::Enumerator::Enumerator(GrDataRow* pDataRow, GrColumnList* pColumnList)
        : m_pColumnList(pColumnList), m_pDataRow(pDataRow), m_index(0)
    {

    }

    CellTagCollection::Enumerator::~Enumerator()
    {

    }

    bool CellTagCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_pColumnList->GetColumnCount();
    }

    void CellTagCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    System::Object^ CellTagCollection::Enumerator::Current::get()
    {
        GrColumn* pColumn = m_pColumnList->GetColumn(m_index - 1);
        GrItem* pItem = m_pDataRow->GetItem(pColumn);
        System::Object^ ref = pItem->ManagedRef;
        Ntreev::Windows::Forms::Grid::Cell^ cell = safe_cast<Ntreev::Windows::Forms::Grid::Cell^>(ref);
        return cell->Tag;
    }

    System::Collections::IEnumerator^ CellTagCollection::GetEnumerator()
    {
        return gcnew Enumerator(m_row->NativeRef, m_pColumnList);
    }

    CellTagCollection::CellTagCollection(Ntreev::Windows::Forms::Grid::Row^ row)
        : m_row(row)
    {
        m_pColumnList = row->GridCore->GetColumnList();
    }

    System::Object^ CellTagCollection::default::get(System::String^ columnName)
    {
        return m_row->Cells[columnName]->Tag;
    }

    void CellTagCollection::default::set(System::String^ columnName, System::Object^ value)
    {
        m_row->Cells[columnName]->Tag = value;
    }

    System::Object^ CellTagCollection::default::get(Column^ column)
    {
        return m_row->Cells[column]->Tag;
    }

    void CellTagCollection::default::set(Column^ column, System::Object^ value)
    {
        m_row->Cells[column]->Tag = value;
    }

    System::Object^ CellTagCollection::default::get(int index)
    {
        return m_row->Cells[index]->Tag;
    }

    void CellTagCollection::default::set(int index, System::Object^ value)
    {
        m_row->Cells[index]->Tag = value;
    }

    int CellTagCollection::Count::get()
    {
        return m_row->Cells->Count;
    }

    void CellTagCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(System::Object^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    bool CellTagCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true;
    }

    System::Object^ CellTagCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this;
    }

    int CellTagCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/