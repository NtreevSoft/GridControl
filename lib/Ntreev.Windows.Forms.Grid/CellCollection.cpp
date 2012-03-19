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
#include "CellCollection.h"
#include "Column.h"
#include "Row.h"
#include "Cell.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    CellCollection::Enumerator::Enumerator(GrDataRow* pDataRow, GrColumnList* pColumnList)
        : m_pColumnList(pColumnList), m_pDataRow(pDataRow), m_index(0)
    {

    }

    CellCollection::Enumerator::~Enumerator()
    {

    }

    bool CellCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_pColumnList->GetColumnCount();
    }

    void CellCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    Ntreev::Windows::Forms::Grid::Cell^ CellCollection::Enumerator::Current::get()
    {
        GrColumn* pColumn = m_pColumnList->GetColumn(m_index - 1);
        GrItem* pItem = m_pDataRow->GetItem(pColumn);
        System::Object^ ref = pItem->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::Cell^>(ref);
    }

    System::Object^ CellCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return Current;
    }

    System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Cell^>^ CellCollection::GetEnumerator()
    {
        return gcnew Enumerator(m_row->NativeRef, m_pColumnList);
    }

    CellCollection::CellCollection(Ntreev::Windows::Forms::Grid::Row^ row)
        : m_row(row)
    {
        m_pColumnList = row->GridCore->GetColumnList();
    }

    Ntreev::Windows::Forms::Grid::Cell^ CellCollection::default::get(System::String^ columnName)
    {
        for each(Ntreev::Windows::Forms::Grid::Cell^ cell in this)
        {
            if(cell->Column->ColumnName == columnName)
                return cell;
        }
        throw gcnew System::ArgumentException();
    }

    Ntreev::Windows::Forms::Grid::Cell^ CellCollection::default::get(Ntreev::Windows::Forms::Grid::Column^ column)
    {
        if(column == nullptr)
            throw gcnew System::NullReferenceException();
        for each(Ntreev::Windows::Forms::Grid::Cell^ cell in this)
        {
            if(cell->Column == column)
                return cell;
        }
        throw gcnew System::ArgumentException();
    }

    Ntreev::Windows::Forms::Grid::Cell^ CellCollection::default::get(int index)
    {
        GrColumn* pColumn = m_pColumnList->GetColumn(index);
        GrDataRow* pDataRow = m_row->NativeRef;
        GrItem* pItem = pDataRow->GetItem(pColumn);
        System::Object^ ref = pItem->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::Cell^>(ref);
    }

    int CellCollection::Count::get()
    {
        return (int)m_pColumnList->GetColumnCount();
    }

    Ntreev::Windows::Forms::Grid::Cell^ CellCollection::default::get(GrColumn* pColumn)
    {
        GrDataRow* pDataRow = m_row->NativeRef;
        GrItem* pItem = pDataRow->GetItem(pColumn);
        System::Object^ ref = pItem->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::Cell^>(ref);
    }

    System::Collections::IEnumerator^ CellCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return GetEnumerator();
    }

    void CellCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(Ntreev::Windows::Forms::Grid::Cell^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    bool CellCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true;
    }

    System::Object^ CellCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this;
    }

    int CellCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/