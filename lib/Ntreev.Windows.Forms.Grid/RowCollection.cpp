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


#include "StdAfx.h"
#include "RowCollection.h"
#include "Row.h"
#include "GridControl.h"
#include "Column.h"
#include "Cell.h"
#include "FromNative.h"
#include "Resources.h"

#include <vcclr.h>

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    RowCollection::Enumerator::Enumerator(GrDataRowList* pDataRowList)
        : m_pDataRowList(pDataRowList), m_index(0)
    {

    }

    RowCollection::Enumerator::~Enumerator()
    {

    }

    bool RowCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_pDataRowList->GetDataRowCount();
    }

    void RowCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    Ntreev::Windows::Forms::Grid::Row^ RowCollection::Enumerator::Current::get()
    {
        GrDataRow* pDataRow = m_pDataRowList->GetDataRow(m_index - 1);
        return FromNative::Get(pDataRow);
    }

    System::Object^ RowCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return Current;
    }

    RowCollection::ManagerEventDetach::ManagerEventDetach(Ntreev::Windows::Forms::Grid::RowCollection^ rowCollection)
        : m_rowCollection(rowCollection)
    {
        m_rowCollection->m_manager->ListChanged -= m_rowCollection->m_listChangedEventHandler;
        m_rowCollection->m_manager->CurrentChanged -= m_rowCollection->m_currentChangedEventHandler;
        m_rowCollection->m_locked++;
    }

    RowCollection::ManagerEventDetach::~ManagerEventDetach()
    {
        m_rowCollection->m_locked--;
        m_rowCollection->m_manager->ListChanged += m_rowCollection->m_listChangedEventHandler;
        m_rowCollection->m_manager->CurrentChanged += m_rowCollection->m_currentChangedEventHandler;
    }

    RowCollection::RowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl) 
        : GridObject(gridControl)
    {
        m_pDataRowList = this->GridCore->GetDataRowList();

        m_listChangedEventHandler = gcnew System::ComponentModel::ListChangedEventHandler(this, &RowCollection::currencyManager_ListChanged);
        m_currentChangedEventHandler = gcnew System::EventHandler(this, &RowCollection::currencyManager_CurrentChanged);

        gridControl->CurrencyManagerChanging += gcnew CurrencyManagerChangingEventHandler(this, &RowCollection::gridControl_CurrencyManagerChanging);
        gridControl->CurrencyManagerChanged += gcnew CurrencyManagerChangedEventHandler(this, &RowCollection::gridControl_CurrencyManagerChanged);
    }

    void RowCollection::Bind(System::Object^ component, int componentIndex)
    {
        if(this->GridControl->InvokeRowInserting(component) == false)
            return;

        Ntreev::Windows::Forms::Grid::Row^ row = gcnew Row(GridControl);
        m_pDataRowList->AddDataRow(row->NativeRef);
        row->Component = component;
        row->ComponentIndex = componentIndex;

        this->GridControl->InvokeRowInserted(row);
    }

    void RowCollection::Unbind(int componentIndex)
    {
        for(int i = componentIndex + 1 ; i<this->Count ; i++)
        {
            this->GetAt(i)->ComponentIndex--;
        }

        Ntreev::Windows::Forms::Grid::Row^ row = this->GetAt(componentIndex);
        m_pDataRowList->RemoveDataRow(row->NativeRef);
        this->GridControl->InvokeRowRemoved(gcnew RowRemovedEventArgs(componentIndex));
    }

    void RowCollection::SetItemsByDesigner(cli::array<System::Object^>^ values)
    {
        using namespace System::Collections::Generic;
        for each(System::Object^ item in values)
        {
            Ntreev::Windows::Forms::Grid::Row^ row = dynamic_cast<Ntreev::Windows::Forms::Grid::Row^>(item);

            if(row->Index < 0)
            {
                Add(row);
            }
        }
    }

    bool RowCollection::Contains(Ntreev::Windows::Forms::Grid::Row^ item)
    {
        ArgumentTest(item);
        return item->Index != INVALID_INDEX;
    }

    int RowCollection::IndexOf(Ntreev::Windows::Forms::Grid::Row^ item)
    {
        ArgumentTest(item);
        return item->Index;
    }

    void RowCollection::currencyManager_ListChanged(System::Object^ sender, System::ComponentModel::ListChangedEventArgs^ e)
    {
        switch(e->ListChangedType)
        {
        case System::ComponentModel::ListChangedType::ItemAdded:
            {
                int componentIndex = e->NewIndex;
                Bind(m_manager->List[componentIndex], componentIndex);
            }
            break;
        case System::ComponentModel::ListChangedType::ItemDeleted:
            {
                Unbind(e->NewIndex);
            }
            break;
        case System::ComponentModel::ListChangedType::ItemChanged:
            {
                System::Object^ component = m_manager->List[e->NewIndex];

                Ntreev::Windows::Forms::Grid::Row^ row = this[component];
                if(e->PropertyDescriptor == nullptr)
                {
                    for each(Ntreev::Windows::Forms::Grid::Cell^ item in row->Cells)
                    {
                        item->UpdateNativeText();
                    }
                }
                else
                {
                    row->Cells[e->PropertyDescriptor->Name]->UpdateNativeText();
                }
            }
            break;
        case System::ComponentModel::ListChangedType::ItemMoved:
            {

            }
            break;
        case System::ComponentModel::ListChangedType::Reset:
            {
                m_pDataRowList->Clear();
                for each(System::Object^ item in m_manager->List)
                {
                    Bind(item, this->Count);
                }

                currencyManager_CurrentChanged(sender, System::EventArgs::Empty);
            }
            break;
        }
    }

    void RowCollection::currencyManager_CurrentChanged(System::Object^ /*sender*/, System::EventArgs^ /*e*/)
    {
        if(m_manager->Position < 0)
            return;

        Ntreev::Windows::Forms::Grid::Row^ focusedRow = dynamic_cast<Ntreev::Windows::Forms::Grid::Row^>(this->GridControl->FocusedRow);
        if(focusedRow != nullptr && focusedRow->ComponentIndex == m_manager->Position)
            return;

        Ntreev::Windows::Forms::Grid::Row^ row = this[m_manager->Current];
        if(row != nullptr)
        {
            row->Focus();
            row->BringIntoView();
        }
    }

    void RowCollection::gridControl_CurrencyManagerChanging(System::Object^ /*sender*/, CurrencyManagerChangingEventArgs^ /*e*/)
    {

    }

    void RowCollection::gridControl_CurrencyManagerChanged(System::Object^ /*sender*/, Ntreev::Windows::Forms::Grid::CurrencyManagerChangedEventArgs^ e)
    {
        if(m_manager != nullptr)
        {
            m_manager->ListChanged -= m_listChangedEventHandler;
            m_manager->CurrentChanged -= m_currentChangedEventHandler;
        }

        m_manager = e->CurrecnyManager;

        if(m_manager == nullptr)
            return;

        int componentIndex = 0;
        for each(System::Object^ item in m_manager->List)
        {
            Bind(item, componentIndex++);
        }

        m_manager->ListChanged += m_listChangedEventHandler;
        m_manager->CurrentChanged += m_currentChangedEventHandler;
    }

    void RowCollection::ArgumentTest(Ntreev::Windows::Forms::Grid::Row^ item)
    {
        if(item == nullptr)
            throw gcnew System::ArgumentNullException("item");
        if(item == InsertionRow)
            throw gcnew System::ArgumentException();
    }

    void RowCollection::Clear()
    {
        ManagerEventDetach managerEventDeatch(this);
        m_pDataRowList->Clear();

        for(int i=m_manager->Count-1 ; i>=0 ; i--)
            m_manager->RemoveAt(i);
    }

    void RowCollection::Insert(int index, Ntreev::Windows::Forms::Grid::Row^ item)
    {
        InsertCore(index, item);
    }

    Ntreev::Windows::Forms::Grid::Row^ RowCollection::InsertCore(int index, Ntreev::Windows::Forms::Grid::Row^ item)
    {
        if(index < 0 || index > Count)
            throw gcnew System::ArgumentOutOfRangeException("index");
        if(item == nullptr)
            throw gcnew System::ArgumentNullException("item");
        //if(item == InsertionRow)
        //    throw gcnew System::ArgumentException();
        if(item->Index != 0xffffffff)
            throw gcnew System::ArgumentException();

        ManagerEventDetach managerEventDeatch(this);

        m_manager->AddNew();
        System::Object^ component = m_manager->Current;

        if(item == this->InsertionRow)
        {
            for each(Ntreev::Windows::Forms::Grid::Cell^ cell in item->Cells)
            {
                Ntreev::Windows::Forms::Grid::Column^ column = cell->Column;
                if(column->PropertyDescriptor == nullptr)
                    continue;

                column->PropertyDescriptor->SetValue(component, cell->Value);
            }
        }

        if(this->GridControl->InvokeRowInserting(component) == false)
        {
            m_manager->CancelCurrentEdit();
        }
        else
        {
            m_manager->EndCurrentEdit();

            bool newRow = false;
            if(item == this->InsertionRow)
            {
                item = gcnew Ntreev::Windows::Forms::Grid::Row(this->GridControl);
                newRow = true;
            }

            m_pDataRowList->InsertDataRow(item->NativeRef, index);

            if(newRow == true)
            {
                for(int i=0 ; i<InsertionRow->CellCount ; i++)
                {
                    Ntreev::Windows::Forms::Grid::Cell^ sourceCell = this->InsertionRow->Cells[i];
                    Ntreev::Windows::Forms::Grid::Cell^ cell = item->Cells[i];

                    cell->Value = sourceCell->Value;
                    cell->Tag = sourceCell->Tag;
                }

                this->InsertionRow->SetDefaultValue();
            }

            
            item->Component = component;
            item->ComponentIndex = m_manager->List->Count - 1;

            this->GridControl->InvokeRowInserted(item);
        }
        return item;
    }

    void RowCollection::RemoveAt(int index)
    {
        Ntreev::Windows::Forms::Grid::Row^ row = this[index];
        Remove(row);
    }

    System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Row^>^ RowCollection::GetEnumerator()
    {
        return gcnew Ntreev::Windows::Forms::Grid::RowCollection::Enumerator(m_pDataRowList);
    }

    Ntreev::Windows::Forms::Grid::Row^ RowCollection::default::get(int index)
    {
        if(index < 0 || index >= (int)m_pDataRowList->GetDataRowCount())
            throw gcnew System::ArgumentOutOfRangeException("index");

        GrDataRow* pDataRow = m_pDataRowList->GetDataRow((unsigned int)index);
        return FromNative::Get(pDataRow);
    }

    void RowCollection::default::set(int /*index*/, Ntreev::Windows::Forms::Grid::Row^ /*value*/)
    {

    }

    Ntreev::Windows::Forms::Grid::Row^ RowCollection::GetAt(int index)
    {
        return this[index];
    }

    Ntreev::Windows::Forms::Grid::Row^ RowCollection::Add()
    {
        try
        {
            Ntreev::Windows::Forms::Grid::Row^ row = gcnew Row(GridControl);
            Insert(Count, row);
            return row;
        }
        catch(System::Exception^ e)
        {
            throw e;
        }
    }

    cli::array<Ntreev::Windows::Forms::Grid::Row^>^ RowCollection::Add(int count)
    {
        if(count <= 0)
            throw gcnew System::ArgumentOutOfRangeException("count", "생성 갯수는 0보다 커야 합니다.");
        cli::array<Ntreev::Windows::Forms::Grid::Row^>^ rows = gcnew cli::array<Ntreev::Windows::Forms::Grid::Row^>(count);

        for(int i=0 ; i<count ; i++)
        {
            rows[i] = Add();
        }
        return rows;
    }

    Ntreev::Windows::Forms::Grid::Row^ RowCollection::AddFromInsertion()
    {
        return InsertCore(this->Count, this->InsertionRow);
    }

    void RowCollection::Add(Ntreev::Windows::Forms::Grid::Row^ item)
    {
        Insert(this->Count, item);
    }

    bool RowCollection::Remove(Ntreev::Windows::Forms::Grid::Row^ item)
    {
        ArgumentTest(item);

        if(item->Index == INVALID_INDEX)
            throw gcnew System::ArgumentException("이미 지워지거나 사용되지 않은 row입니다.");

        ManagerEventDetach managerEventDeatch(this);

        if(this->GridControl->InvokeRowRemoving(item) == false)
            return false;

        int index = m_manager->List->IndexOf(item->Component);
        if(index < 0)
            throw gcnew System::ArgumentException("이미 지워지거나 사용되지 않은 row입니다.");

        m_manager->RemoveAt(index);
        m_pDataRowList->RemoveDataRow(item->NativeRef);

        RowRemovedEventArgs eRemoved(0);
        this->GridControl->InvokeRowRemoved(%eRemoved);

        return true;
    }

    int RowCollection::Count::get()
    {
        return (int)m_pDataRowList->GetDataRowCount(); 
    }

    void RowCollection::Count::set(int value)
    {
        int count = this->Count;

        if(value > count)
        {
            Add(value - count);
        }
        else if(value < count)
        {
            for(int i=count-1 ; i>=value ; i--)
            {
                this->RemoveAt(i);
            }
        }
    }

    Ntreev::Windows::Forms::Grid::InsertionRow^ RowCollection::InsertionRow::get()
    {
        return this->GridControl->InsertionRow;
    }

    Ntreev::Windows::Forms::Grid::Row^ RowCollection::default::get(GrDataRow* pDataRow)
    {
        return FromNative::Get(pDataRow);
    }

    Ntreev::Windows::Forms::Grid::Row^ RowCollection::default::get(System::Object^ component)
    {
        for each(Ntreev::Windows::Forms::Grid::Row^ item in this)
        {
            if(item->Component == component)
                return item;
        }
        return nullptr;
    }

    System::Collections::IEnumerator^ RowCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return GetEnumerator();
    }

    void RowCollection::RemoveAt_System_Collections_IList(int index)
    {
        this->RemoveAt(index);
    }

    int RowCollection::Add_System_Collections_IList(System::Object^ value)
    {
        int count = this->Count;
        Add((Ntreev::Windows::Forms::Grid::Row^)value);
        return count;
    }

    bool RowCollection::Contains_System_Collections_IList(System::Object^ value)
    {
        return this->Contains((Ntreev::Windows::Forms::Grid::Row^)value);
    }

    void RowCollection::Clear_System_Collections_IList()
    {
        for(int i=this->Count-1 ; i>=0 ; i--)
        {
            RemoveAt(i);
        }
    }

    int RowCollection::IndexOf_System_Collections_IList(System::Object^ value)
    {
        return this->IndexOf((Ntreev::Windows::Forms::Grid::Row^)value);
    }

    void RowCollection::Insert_System_Collections_IList(int index, System::Object^ value)
    {
        this->Insert(index, (Ntreev::Windows::Forms::Grid::Row^)value);
    }

    void RowCollection::Remove_System_Collections_IList(System::Object^ value)
    {
        this->Remove((Ntreev::Windows::Forms::Grid::Row^)value);
    }

    void RowCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(Ntreev::Windows::Forms::Grid::Row^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    void RowCollection::CopyTo_System_Collections_Generic_ICollection(cli::array<Ntreev::Windows::Forms::Grid::Row^>^ array, int arrayIndex)
    {
        for each(Ntreev::Windows::Forms::Grid::Row^ item in this)
        {
            array->SetValue(item, arrayIndex++);
        }
    }

    System::Object^ RowCollection::default_System_Collections_IList::get(int index)
    {
        return this[index];
    }

    void RowCollection::default_System_Collections_IList::set(int /*index*/, System::Object^ /*value*/)
    {
        throw gcnew System::NotImplementedException();
    }

    bool RowCollection::IsReadOnly_System_Collections_IList::get()
    {
        return false; 
    }

    bool RowCollection::IsFixedSize_System_Collections_IList::get()
    {
        return false;
    }

    bool RowCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true;
    }

    System::Object^ RowCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this;
    }

    int RowCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count;
    }

    bool RowCollection::IsReadOnly_System_Collections_Generic_ICollection::get()
    {
        return false;
    }

} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/