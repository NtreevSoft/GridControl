//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
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
#include "GridRowCollection.h"
#include "GridRow.h"
#include "GridControl.h"
#include "GridColumn.h"
#include "GridCell.h"
#include "GridResource.h"

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

    Row^ RowCollection::Enumerator::Current::get()
    {
        GrDataRow* pDataRow = m_pDataRowList->GetDataRow(m_index - 1);
        return Row::FromNative(pDataRow);
    }

    RowCollection::RowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl) 
        : GridObject(gridControl)
    {
        m_pDataRowList = GridCore->GetDataRowList();

        m_listChangedEventHandler = gcnew System::ComponentModel::ListChangedEventHandler(this, &RowCollection::currencyManager_ListChanged);
        m_currentChangedEventHandler = gcnew System::EventHandler(this, &RowCollection::currencyManager_CurrentChanged);

        gridControl->CurrencyManagerChanging += gcnew CurrencyManagerChangingEventHandler(this, &RowCollection::gridControl_CurrencyManagerChanging);
        gridControl->CurrencyManagerChanged += gcnew CurrencyManagerChangedEventHandler(this, &RowCollection::gridControl_CurrencyManagerChanged);
    }

    void RowCollection::Bind(System::Object^ component, int componentIndex)
    {
        if(GridControl->InvokeRowInserting(component) == false)
            return;

        Row^ row = gcnew Row(GridControl);
        m_pDataRowList->AddDataRow(row->NativeRef);
        row->Component = component;
        row->ComponentIndex = componentIndex;

        GridControl->InvokeRowInserted(row);
    }

    void RowCollection::Unbind(int componentIndex)
    {
        for(int i = componentIndex + 1 ; i<this->Count ; i++)
        {
            this->GetAt(i)->ComponentIndex--;
        }

        Row^ row = this->GetAt(componentIndex);
        m_pDataRowList->RemoveDataRow(row->NativeRef);
        GridControl->InvokeRowRemoved(gcnew RowRemovedEventArgs(componentIndex));
    }

    void RowCollection::SetItemsByDesigner(cli::array<System::Object^>^ values)
    {
        using namespace System::Collections::Generic;
        for each(System::Object^ item in values)
        {
            Row^ row = dynamic_cast<Row^>(item);

            if(row->Index < 0)
            {
                Add(row);
            }
        }
    }

    bool RowCollection::Contains(Row^ item)
    {
        ArgumentTest(item);
        return item->Index != INVALID_INDEX;
    }

    int RowCollection::IndexOf(Row^ item)
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

                Row^ row = this[component];
                if(e->PropertyDescriptor == nullptr)
                {
                    for each(Cell^ item in row->Cells)
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

        Row^ focusedRow = this->GridControl->FocusedRow;
        if(focusedRow != nullptr && focusedRow->ComponentIndex == m_manager->Position)
            return;

        Row^ row = this[m_manager->Current];
        if(row != nullptr)
        {
            row->Focus();
            row->BringIntoView();
        }
    }

    void RowCollection::gridControl_CurrencyManagerChanging(System::Object^ /*sender*/, CurrencyManagerChangingEventArgs^ /*e*/)
    {

    }

    void RowCollection::gridControl_CurrencyManagerChanged(System::Object^ /*sender*/, CurrencyManagerChangedEventArgs^ e)
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

    void RowCollection::ArgumentTest(Row^ item)
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

    void RowCollection::Insert(int index, Row^ item)
    {
        if(index < 0 || index > Count)
            throw gcnew System::ArgumentOutOfRangeException("index");
        if(item == nullptr)
            throw gcnew System::ArgumentNullException("item");
        if(item == InsertionRow)
            throw gcnew System::ArgumentException();
        if(item->Index != 0xffffffff)
            throw gcnew System::ArgumentException();

        ManagerEventDetach managerEventDeatch(this);

        m_manager->AddNew();
        System::Object^ component = m_manager->Current;

        if(GridControl->InvokeRowInserting(component) == false)
        {
            m_manager->CancelCurrentEdit();
        }
        else
        {
            m_manager->EndCurrentEdit();

            m_pDataRowList->InsertDataRow(item->NativeRef, index);
            item->Component = component;
            item->ComponentIndex = m_manager->List->Count - 1;

            GridControl->InvokeRowInserted(item);
        }
    }

    void RowCollection::RemoveAt(int index)
    {
        Row^ row = this[index];
        Remove(row);
    }


    Row^ RowCollection::default::get(int index)
    {
        if(index < 0 || index >= (int)m_pDataRowList->GetDataRowCount())
            throw gcnew System::ArgumentOutOfRangeException("index");

        GrDataRow* pDataRow = m_pDataRowList->GetDataRow((uint)index);
        return Row::FromNative(pDataRow);
    }

    Row^ RowCollection::AddNew()
    {
        try
        {
            Row^ row = gcnew Row(GridControl);
            Insert(Count, row);
            return row;
        }
        catch(System::Exception^ e)
        {
            throw e;
        }
    }

    cli::array<Row^>^ RowCollection::AddNew(int count)
    {
        if(count <= 0)
            throw gcnew System::ArgumentOutOfRangeException("count", "생성 갯수는 0보다 커야 합니다.");
        cli::array<Row^>^ rows = gcnew cli::array<Row^>(count);

        for(int i=0 ; i<count ; i++)
        {
            rows[i] = AddNew();
        }
        return rows;
    }

    Row^ RowCollection::AddNewFromInsertion()
    {
        ManagerEventDetach managerEventDeatch(this);

        try
        {
            m_manager->AddNew();
        }
        catch(System::Exception^ e)
        {
            throw e;
        }

        Row^ row = gcnew Row(GridControl);
        m_pDataRowList->AddDataRow(row->NativeRef);
        row->Component = m_manager->Current;
        row->ComponentIndex = m_manager->List->Count - 1;
        row->IsVisible = false;

        for(int i=0 ; i<InsertionRow->CellCount ; i++)
        {
            Cell^ sourceCell = InsertionRow->Cells[i];
            Cell^ cell   = row->Cells[i];

            cell->Value  = sourceCell->Value;
            cell->Tag  = sourceCell->Tag;
        }

        if(GridControl->InvokeInsertionRowInserting(row) == false)
        {
            m_manager->CancelCurrentEdit(); 
            m_pDataRowList->RemoveDataRow(row->NativeRef);
            row->Component = nullptr;
            row->ComponentIndex = -1;
            return nullptr;
        }

        try
        {
            m_manager->EndCurrentEdit();
            row->IsVisible = true;
            GridControl->InvokeInsertionRowInserted(row);
        }
        catch(System::Exception^ e)
        {
            m_manager->CancelCurrentEdit();
            m_manager->Position = -1;
            m_pDataRowList->RemoveDataRow(row->NativeRef);
            row->Component = nullptr;
            row->ComponentIndex = -1;
            throw e;
        }
        finally
        {
            InsertionRow->SetDefaultValue();
        }

        return row;
    }

    void RowCollection::Add(Row^ item)
    {
        Insert(Count, item);
    }

    bool RowCollection::Remove(Row^ item)
    {
        ArgumentTest(item);

        if(item->Index == INVALID_INDEX)
            throw gcnew System::ArgumentException("이미 지워지거나 사용되지 않은 row입니다.");

        ManagerEventDetach managerEventDeatch(this);

        if(GridControl->InvokeRowRemoving(item) == false)
            return false;

        int index = m_manager->List->IndexOf(item->Component);
        if(index < 0)
            throw gcnew System::ArgumentException("이미 지워지거나 사용되지 않은 row입니다.");

        m_manager->RemoveAt(index);
        m_pDataRowList->RemoveDataRow(item->NativeRef);

        RowRemovedEventArgs eRemoved(0);
        GridControl->InvokeRowRemoved(%eRemoved);

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
            AddNew(value - count);
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
        return GridControl->InsertionRow;
    }

    Row^ RowCollection::default::get(GrDataRow* pDataRow)
    {
        return Row::FromNative(pDataRow);
    }

    Row^ RowCollection::default::get(System::Object^ component)
    {
        for each(Row^ item in this)
        {
            if(item->Component == component)
                return item;
        }
        return nullptr;
    }

    SelectedRowCollection::SelectedRowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, const GrSelectedRows* selectedRows) 
        : GridObject(gridControl), m_selectedRows(selectedRows)
    {

    }

    void SelectedRowCollection::Add(Row^ item)
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

    void SelectedRowCollection::Add(Row^ row, Column^ focusColumn)
    {
        Add(row);

        GrDataRow* pDataRow = row->NativeRef;
        GrItem*  pItem = nullptr;
        if(focusColumn == nullptr)
        {
            GrFocuser* pFocuser = GridCore->GetFocuser();
            GrColumn* pLastFocusedColumn = pFocuser->GetLastFocusedColumn();
            if(pLastFocusedColumn != nullptr)
            {
                pItem = pDataRow->GetItem(pLastFocusedColumn);
            }
            else
            {
                GrColumnList* pColumnList = GridCore->GetColumnList();
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

        Focuser->Set(pItem);
    }

    bool SelectedRowCollection::Remove(Row^ row)
    {
        if(row->IsSelected == false)
            return false;
        row->IsSelected = false;
        return true;
    }

    void SelectedRowCollection::Clear()
    {
        GridControl->ClearSelection();
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

    Row^ SelectedRowCollection::Enumerator::Current::get()
    {
        const GrDataRow* pDataRow = m_selectedRows->at(m_index-1);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<Row^>(ref);
    }

    Row^ SelectedRowCollection::default::get(int index)
    {
        const GrDataRow* pDataRow = m_selectedRows->at(index);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<Row^>(ref);
    }

    VisibleRowCollection::VisibleRowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_pDataRowList = GridCore->GetDataRowList();
    }

    RowBase^ VisibleRowCollection::default::get(int index)
    {
        if((uint)index >= m_pDataRowList->GetVisibleRowCount())
            throw gcnew System::ArgumentOutOfRangeException("index");
        IDataRow* pDataRow = m_pDataRowList->GetVisibleRow(index);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<RowBase^>(ref);
    }

    int VisibleRowCollection::Count::get()
    {
        return m_pDataRowList->GetVisibleRowCount();
    }

    VisibleRowCollection::Enumerator::Enumerator(GrDataRowList* pDataRowList)
        : m_pDataRowList(pDataRowList)
    {
        m_index = 0;
    }

    VisibleRowCollection::Enumerator::~Enumerator()
    {

    }

    bool VisibleRowCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_pDataRowList->GetVisibleRowCount();
    }

    void VisibleRowCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    RowBase^ VisibleRowCollection::Enumerator::Current::get()
    {
        IDataRow* pDataRow = m_pDataRowList->GetVisibleRow(m_index - 1);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<RowBase^>(ref);
    }

    DisplayableRowCollection::DisplayableRowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_pDataRowList = GridCore->GetDataRowList();
    }

    RowBase^ DisplayableRowCollection::default::get(int index)
    {
        if((uint)index >= m_pDataRowList->GetDisplayableRowCount())
            throw gcnew System::ArgumentOutOfRangeException("index");
        IDataRow* pDataRow = m_pDataRowList->GetDisplayableRow(index);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<RowBase^>(ref);
    }

    int DisplayableRowCollection::Count::get()
    {
        return m_pDataRowList->GetDisplayableRowCount();
    }


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

    RowBase^ DisplayableRowCollection::Enumerator::Current::get()
    {
        IDataRow* pDataRow = m_pDataRowList->GetDisplayableRow(m_index - 1);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<RowBase^>(ref);
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/