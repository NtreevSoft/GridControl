//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4646.22417
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
#include "CellCollection.h"
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

    Row^ RowCollection::Enumerator::Current::get()
    {
        GrDataRow* pDataRow = m_pDataRowList->GetDataRow(m_index - 1);
        return FromNative::Get(pDataRow);
    }

    System::Object^ RowCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return Current;
    }

    RowCollection::ManagerEventDetach::ManagerEventDetach(RowCollection^ rows)
        : m_rows(rows)
    {
        m_rows->DetachManagerEvent();
    }

    RowCollection::ManagerEventDetach::~ManagerEventDetach()
    {
        m_rows->AttachManagerEvent();
    }

    RowCollection::RowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl) 
        : GridObject(gridControl)
    {
        m_pDataRowList = this->GridCore->GetDataRowList();

        m_listChangedEventHandler = gcnew System::ComponentModel::ListChangedEventHandler(this, &RowCollection::currencyManager_ListChanged);
        m_currentChangedEventHandler = gcnew System::EventHandler(this, &RowCollection::currencyManager_CurrentChanged);
        
        gridControl->CurrencyManagerChanging += gcnew CurrencyManagerChangingEventHandler(this, &RowCollection::gridControl_CurrencyManagerChanging);
        gridControl->CurrencyManagerChanged += gcnew CurrencyManagerChangedEventHandler(this, &RowCollection::gridControl_CurrencyManagerChanged);
        gridControl->FocusedRowChanged += gcnew System::EventHandler(this, &RowCollection::gridControl_FocusedRowChanged);

        m_componentToRow = gcnew System::Collections::Generic::Dictionary<System::Object^, Row^>();
		m_components = gcnew System::Collections::ArrayList();
    }

    void RowCollection::Bind(System::Object^ component, int componentIndex)
    {
		Row^ row;
		System::ComponentModel::IDataErrorInfo^ dataErrorInfo = dynamic_cast<System::ComponentModel::IDataErrorInfo^>(component);

		if(m_componentToRow->ContainsKey(component) == true)
		{
			row = m_componentToRow[component];
			row->Component = component;
			m_pDataRowList->RemoveDataRow(row->NativeRef);
			m_pDataRowList->InsertDataRow(row->NativeRef, componentIndex);
			m_components->Remove(component);
			m_components->Insert(componentIndex, component);
			this->GridControl->InvokeRowChanged(row);
			return;
		}

		m_components->Insert(componentIndex, component);

		if(this->GridControl->InvokeRowBinding(component) == false)
			return;

		row = this->GridControl->CreateRow(m_pDataRowList->NewDataRow());

		m_pDataRowList->InsertDataRow(row->NativeRef, componentIndex);
		m_componentToRow->Add(component, row);

		row->Component = component;
		row->ProcessChildControl();

		if(dataErrorInfo != nullptr)
		{
			System::String^ error = dataErrorInfo->Error;

			if(System::String::IsNullOrEmpty(error) == false)
				row->ErrorDescription = error;

			for each(System::ComponentModel::PropertyDescriptor^ item in m_manager->GetItemProperties())
			{
				if(item->PropertyType == System::ComponentModel::IBindingList::typeid)
					continue;

				System::String^ error = dataErrorInfo[item->Name];

				if(System::String::IsNullOrEmpty(error) == false)
					row->Cells[item->Name]->ErrorDescription = error;
			}
		}

		this->GridControl->InvokeRowBinded(row);
		this->GridControl->InvokeRowChanged(row);

    }

    void RowCollection::Unbind(int componentIndex)
    {
		System::Object^ component = m_components[componentIndex];
		Row^ row = m_componentToRow[component];
		m_components->RemoveAt(componentIndex);
		m_componentToRow->Remove(component);
		
		this->GridControl->InvokeRowUnbinding(row);
		m_pDataRowList->RemoveDataRow(row->NativeRef);
		this->GridControl->InvokeRowUnbinded(row);
		row->DetachChildControl();
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
                this->Bind(m_manager->List[componentIndex], componentIndex);
            }
            break;
        case System::ComponentModel::ListChangedType::ItemDeleted:
            {
                this->Unbind(e->NewIndex);
            }
            break;
        case System::ComponentModel::ListChangedType::ItemChanged:
            {
                System::Object^ component = m_manager->List[e->NewIndex];
				System::ComponentModel::IDataErrorInfo^ dataErrorInfo = dynamic_cast<System::ComponentModel::IDataErrorInfo^>(component);


                Row^ row = this[component];
                System::Collections::Generic::List<Cell^> cells;

				if(dataErrorInfo != nullptr)
				{
					row->ErrorDescription = dataErrorInfo->Error;
				}

                if(e->PropertyDescriptor == nullptr)
                {
					for each(System::ComponentModel::PropertyDescriptor^ item in m_manager->GetItemProperties())
					{
						if(item->PropertyType == System::ComponentModel::IBindingList::typeid)
							continue;
						cells.Add(row->Cells[item->Name]);
					}
                }
                else
                {
                    cells.Add(row->Cells[e->PropertyDescriptor->Name]);
                }

                for each(Cell^ item in cells)
                {
					item->UpdateNativeText();

					if(dataErrorInfo != nullptr)
					{
						item->ErrorDescription = dataErrorInfo[item->Column->ColumnName];
					}

					this->GridControl->InvokeValueChanged(item);
                }

                this->GridControl->InvokeRowChanged(row);
            }
            break;
        case System::ComponentModel::ListChangedType::ItemMoved:
            {

            }
            break;
        case System::ComponentModel::ListChangedType::Reset:
            {
                m_pDataRowList->Clear();
                m_components->Clear();
				m_componentToRow->Clear();
                for each(System::Object^ item in m_manager->List)
                {
					if(item == this->GridControl->InsertionRow->Component)
						continue;
					this->Bind(item, this->Count);
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

        Row^ focusedRow = dynamic_cast<Row^>(this->GridControl->FocusedRow);
		if(focusedRow != nullptr && focusedRow->Component == m_manager->Current)
            return;

        //Row^ row = this[m_manager->Current];
        //if(row != nullptr)
        //{
        //    row->Select();
        //    row->Focus();
        //    row->BringIntoView();
        //}
    }

    void RowCollection::gridControl_CurrencyManagerChanging(System::Object^ /*sender*/, CurrencyManagerChangingEventArgs^ /*e*/)
    {
        
    }

    void RowCollection::gridControl_CurrencyManagerChanged(System::Object^ /*sender*/, Ntreev::Windows::Forms::Grid::CurrencyManagerChangedEventArgs^ e)
    {
        DetachManagerEvent();

        m_manager = e->CurrecnyManager;
        m_components->Clear();

        if(m_manager == nullptr)
            return;

        int componentIndex = 0;
        for each(System::Object^ item in m_manager->List)
        {
            Bind(item, componentIndex++);
        }

        //this->SetDefaultValue();

        AttachManagerEvent();
    }

    void RowCollection::gridControl_FocusedRowChanged(System::Object^ /*sender*/, System::EventArgs^ /*e*/)
    {
        ManagerEventDetach managerEventDeatch(this);

        Row^ row = dynamic_cast<Row^>(this->GridControl->FocusedRow);

        if(row != nullptr && row != this->InsertionRow)
        {
			if(m_manager->Position >= 0 && m_manager->Current != row->Component)
            {
				m_manager->Position = m_manager->List->IndexOf(row->Component);
            }
        }
    }

    void RowCollection::BeginInsertion()
    {
        if(this->InsertionRow->Component != nullptr)
            return;

        ManagerEventDetach managerEventDeatch(this);

        m_manager->AddNew();
        this->InsertionRow->Component = m_manager->Current;
    }

    void RowCollection::EndInsertion()
    {
        ManagerEventDetach managerEventDeatch(this);

        this->InsertionRow->Component = nullptr;
        m_manager->CancelCurrentEdit();
    }

    Row^ RowCollection::GetByComponentIndex(int index)
    {
        //for each(Row^ item in this)
        //{
        //    if(item->ComponentIndex == index)
        //        return item;
        //}
        return nullptr;
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
        this->InsertCore(index, item);
    }

    Row^ RowCollection::InsertCore(int index, Row^ item)
    {
        if(index < 0 || index > Count)
            throw gcnew System::ArgumentOutOfRangeException("index");
        if(item == nullptr)
            throw gcnew System::ArgumentNullException("item");
        if(item->Index != 0xffffffff)
            throw gcnew System::ArgumentException();

        bool isNew = false;
        if(item->Component == nullptr)
        {
            try
            {
                ManagerEventDetach managerEventDeatch(this);
                m_manager->AddNew();
                isNew = true;
            }
            catch(System::Exception^)
            {
                return nullptr;
            }
            item->Component = m_manager->Current;
        }

        //bool fromInsertion = item == this->InsertionRow;

        if(this->GridControl->InvokeRowInserting(item) == false)
        {
            if(isNew == true)
            {
                item->Component = nullptr;
                ManagerEventDetach managerEventDeatch(this);
                m_manager->CancelCurrentEdit();
            }
        }
        else
        {
            try
            {
                //item->ComponentIndex = m_manager->List->Count - 1;
                ManagerEventDetach managerEventDeatch(this);
                m_manager->EndCurrentEdit();
            }
            catch(System::Exception^ e)
            {
                if(isNew == true)
                {
                    item->Component = nullptr;
                    ManagerEventDetach managerEventDeatch(this);
                    m_manager->CancelCurrentEdit();
                }
                this->GridControl->ShowMessage(e->Message, "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
                return nullptr;
            }

            item->ProcessChildControl();
            m_pDataRowList->InsertDataRow(item->NativeRef, index);

            item->EndEdit();
            item->Refresh();
            this->GridControl->InvokeRowInserted(item);
            this->InsertionRow->Refresh();
            this->GridControl->InvokeRowChanged(item);
        }
        return item;
    }

    void RowCollection::RemoveAt(int index)
    {
        Row^ row = this[index];
        Remove(row);
    }

    System::Collections::Generic::IEnumerator<Row^>^ RowCollection::GetEnumerator()
    {
        return gcnew Ntreev::Windows::Forms::Grid::RowCollection::Enumerator(m_pDataRowList);
    }

    Row^ RowCollection::default::get(int index)
    {
        if(index < 0 || index >= (int)m_pDataRowList->GetDataRowCount())
            throw gcnew System::ArgumentOutOfRangeException("index");

        GrDataRow* pDataRow = m_pDataRowList->GetDataRow((unsigned int)index);
        return FromNative::Get(pDataRow);
    }

    void RowCollection::default::set(int /*index*/, Row^ /*value*/)
    {

    }

    Row^ RowCollection::GetAt(int index)
    {
        return this[index];
    }

    Row^ RowCollection::Add()
    {
        try
        {
            Row^ row = this->GridControl->CreateRow(m_pDataRowList->NewDataRow());
            Insert(this->Count, row);
            return row;
        }
        catch(System::Exception^ e)
        {
            throw e;
        }
    }

    cli::array<Row^>^ RowCollection::Add(int count)
    {
        if(count <= 0)
            throw gcnew System::ArgumentOutOfRangeException("count", "생성 갯수는 0보다 커야 합니다.");
        cli::array<Row^>^ rows = gcnew cli::array<Row^>(count);

        for(int i=0 ; i<count ; i++)
        {
            rows[i] = Add();
        }
        return rows;
    }

    Row^ RowCollection::AddFromInsertion()
    {
        return InsertCore(this->Count, this->InsertionRow);
    }

    void RowCollection::Add(Row^ item)
    {
        Insert(this->Count, item);
    }

    bool RowCollection::Remove(Row^ item)
    {
        ArgumentTest(item);

        if(item->Index == INVALID_INDEX)
            throw gcnew System::ArgumentException("이미 지워지거나 사용되지 않은 row입니다.");

        if(this->GridControl->InvokeRowRemoving(item) == false)
            return false;

        ManagerEventDetach managerEventDeatch(this);

        int index = m_manager->List->IndexOf(item->Component);
        if(index < 0)
            throw gcnew System::ArgumentException("이미 지워지거나 사용되지 않은 row입니다.");

        try
        {
            m_manager->RemoveAt(index);
        }
        catch(System::Exception^ e)
        {
            this->GridControl->ShowMessage(e->Message, "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
            return false;
        }

        item->Component = nullptr;
        m_pDataRowList->RemoveDataRow(item->NativeRef);

        RowRemovedEventArgs eRemoved(0);
        this->GridControl->InvokeRowRemoved(%eRemoved);
        item->DetachChildControl();
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

    Row^ RowCollection::InsertionRow::get()
    {
        GrDataRow* pDataRow = m_pDataRowList->GetInsertionRow();
        Row^ row = Ntreev::Windows::Forms::Grid::FromNative::Get(pDataRow);
        if(row == nullptr)
            row = this->GridControl->CreateRow(pDataRow);
        return row;
    }

    Row^ RowCollection::default::get(GrDataRow* pDataRow)
    {
        return FromNative::Get(pDataRow);
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
        Add((Row^)value);
        return count;
    }

    bool RowCollection::Contains_System_Collections_IList(System::Object^ value)
    {
        return this->Contains((Row^)value);
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
        return this->IndexOf((Row^)value);
    }

    void RowCollection::Insert_System_Collections_IList(int index, System::Object^ value)
    {
        this->Insert(index, (Row^)value);
    }

    void RowCollection::Remove_System_Collections_IList(System::Object^ value)
    {
        this->Remove((Row^)value);
    }

    void RowCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(Row^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    void RowCollection::CopyTo_System_Collections_Generic_ICollection(cli::array<Row^>^ array, int arrayIndex)
    {
        for each(Row^ item in this)
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

    void RowCollection::DetachManagerEvent()
    {
        if(m_lockRef == 0)
        {
            if(m_manager != nullptr)
            {
                m_manager->ListChanged -= m_listChangedEventHandler;
                m_manager->CurrentChanged -= m_currentChangedEventHandler;
            }
        }
        m_lockRef--;
    }

    void RowCollection::AttachManagerEvent()
    {
         m_lockRef++;
        if(m_lockRef == 0)
        {
            if(m_manager != nullptr)
            {
                m_manager->ListChanged += m_listChangedEventHandler;
                m_manager->CurrentChanged += m_currentChangedEventHandler;
            }
        }
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/