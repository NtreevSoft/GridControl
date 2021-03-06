﻿//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5190.32793
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
#include "SelectedRowCollection.h"
#include "Column.h"
#include "GridControl.h"
#include "Resources.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    SelectedRowCollection::SelectedRowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, const GrSelectedRows* selectedRows) 
        : GridObject(gridControl), m_selectedRows(selectedRows)
    {

    }

    void SelectedRowCollection::Add(Ntreev::Windows::Forms::Grid::Row^ item)
    {
        using namespace Ntreev::Windows::Forms::Grid::Properties;

        if(item == nullptr)
            throw gcnew System::ArgumentNullException("item");

        if(item->Index == INSERTION_ROW)
            throw gcnew System::Exception(Resources::RemoveInsertionRowException);

        if(item->Index == INVALID_INDEX)
            throw gcnew System::ArgumentException("이미 지워지거나 사용되지 않은 row입니다.");

        if(item->IsSelected == true)
            return;
        item->IsSelected = true;
    }

    void SelectedRowCollection::Add(Ntreev::Windows::Forms::Grid::Row^ row, Column^ focusColumn)
    {
        Add(row);

        GrDataRow* pDataRow = row->NativeRef;
        GrItem* pItem = nullptr;
        if(focusColumn == nullptr)
        {
            GrFocuser* pFocuser = this->GridCore->GetFocuser();
            GrColumn* pLastFocusedColumn = pFocuser->GetLastFocusedColumn();
            if(pLastFocusedColumn != nullptr)
            {
                pItem = pDataRow->GetItem(pLastFocusedColumn);
            }
            else
            {
                GrColumnList* pColumnList = this->GridCore->GetColumnList();
                if(pColumnList->GetDisplayableColumnCount() != 0)
                {
                    GrColumn* pColumn = pColumnList->GetDisplayableColumn(0);
                    pItem = pDataRow->GetItem(pColumn);
                }
            }
        }
        else
        {
            pItem = pDataRow->GetItem(focusColumn->NativeRef);
        }

        this->Focuser->Set(pItem);
    }

    bool SelectedRowCollection::Remove(Ntreev::Windows::Forms::Grid::Row^ row)
    {
        if(row->IsSelected == false)
            return false;
        row->IsSelected = false;
        return true;
    }

    void SelectedRowCollection::Clear()
    {
        this->GridControl->ClearSelection();
    }

    bool SelectedRowCollection::Contains(Ntreev::Windows::Forms::Grid::Row^ row)
    {
        return row->IsSelected == true;
    }

    SelectedRowCollection::Enumerator::Enumerator(const GrSelectedRows* selectedRows)
        : m_selectedRows(selectedRows), m_index(0)
    {

    }

    SelectedRowCollection::Enumerator::~Enumerator()
    {

    }

    bool SelectedRowCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_selectedRows->size();
    }

    void SelectedRowCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    Ntreev::Windows::Forms::Grid::Row^ SelectedRowCollection::Enumerator::Current::get()
    {
        const GrDataRow* pDataRow = m_selectedRows->at(m_index-1);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::Row^>(ref);
    }

    System::Object^ SelectedRowCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return Current; 
    }

    Ntreev::Windows::Forms::Grid::Row^ SelectedRowCollection::default::get(int index)
    {
        const GrDataRow* pDataRow = m_selectedRows->at(index);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::Row^>(ref);
    }

    void SelectedRowCollection::CopyTo(cli::array<Ntreev::Windows::Forms::Grid::Row^>^ array, int arrayIndex)
    {
        for each(Ntreev::Windows::Forms::Grid::Row^ item in this)
        {
            array->SetValue(item, arrayIndex++);
        }
    }

    cli::array<Ntreev::Windows::Forms::Grid::Row^>^ SelectedRowCollection::ToArray()
    {
        cli::array<Ntreev::Windows::Forms::Grid::Row^>^ array = gcnew cli::array<Ntreev::Windows::Forms::Grid::Row^>(this->Count);
        this->CopyTo(array, 0);
        return array;
    }

    System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Row^>^ SelectedRowCollection::GetEnumerator()
    {
        return gcnew Ntreev::Windows::Forms::Grid::SelectedRowCollection::Enumerator(m_selectedRows);
    }

    int SelectedRowCollection::Count::get()
    {
        return (int)m_selectedRows->size();
    }

    System::Collections::IEnumerator^ SelectedRowCollection::GetEnumerator_System_Collections_IEnumerablen()
    {
        return GetEnumerator();
    }

    void SelectedRowCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(Ntreev::Windows::Forms::Grid::Row^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    bool SelectedRowCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true;
    }

    System::Object^ SelectedRowCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this;
    }

    int SelectedRowCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count;
    }

    bool SelectedRowCollection::IsReadOnly_System_Collections_Generic_ICollection::get()
    {
        return false;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/