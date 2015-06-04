//=====================================================================================================================
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
        //m_currentChangedEventHandler = gcnew System::EventHandler(this, &RowCollection::currencyManager_CurrentChanged);
        
        gridControl->CurrencyManagerChanging += gcnew CurrencyManagerChangingEventHandler(this, &RowCollection::gridControl_CurrencyManagerChanging);
        gridControl->CurrencyManagerChanged += gcnew CurrencyManagerChangedEventHandler(this, &RowCollection::gridControl_CurrencyManagerChanged);
		gridControl->Cleared += gcnew ClearEventHandler(this, &RowCollection::gridControl_Cleared);
        gridControl->FocusedRowChanged += gcnew System::EventHandler(this, &RowCollection::gridControl_FocusedRowChanged);

        m_componentToRow = gcnew System::Collections::Generic::Dictionary<System::Object^, Row^>();
		m_components = gcnew System::Collections::ArrayList();
    }

    void RowCollection::Bind(System::Object^ component, int componentIndex)
    {
		Row^ row;

		if(m_componentToRow->ContainsKey(component) == true)
		{
			row = m_componentToRow[component];
			row->AttachComponent(component);
			m_pDataRowList->RemoveDataRow(row->NativeRef);
			m_pDataRowList->InsertDataRow(row->NativeRef, componentIndex);
			m_components->Remove(component);
			m_components->Insert(componentIndex, component);
			this->GridControl->InvokeRowChanged(row);
			return;
		}

		m_components->Insert(componentIndex, component);

		this->GridControl->InvokeRowBinding(component);

		row = this->GridControl->CreateRow(m_pDataRowList->NewDataRow());
		m_pDataRowList->InsertDataRow(row->NativeRef, componentIndex);
		m_componentToRow->Add(component, row);

		row->AttachComponent(component);
		row->AttachChildControl();

		this->GridControl->InvokeRowBinded(row);
		this->GridControl->InvokeRowChanged(row);
    }

    void RowCollection::Unbind(int componentIndex)
    {
		System::Object^ component = m_components[componentIndex];
		Row^ row = m_componentToRow[component];
		this->GridControl->InvokeRowUnbinding(row);

		m_components->RemoveAt(componentIndex);
		m_componentToRow->Remove(component);
		m_pDataRowList->RemoveDataRow(row->NativeRef);
		row->DetachComponent();
		row->DetachChildControl();
		this->GridControl->InvokeRowUnbinded(row);
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
        ValidateArgument(item);
        return item->Index != INVALID_INDEX;
    }

    int RowCollection::IndexOf(Row^ item)
    {
        ValidateArgument(item);
        return item->Index;
    }

    void RowCollection::currencyManager_ListChanged(System::Object^ /*sender*/, System::ComponentModel::ListChangedEventArgs^ e)
    {
        switch(e->ListChangedType)
        {
        case System::ComponentModel::ListChangedType::ItemAdded:
            {
				int componentIndex = e->NewIndex;
				auto component = m_manager->List[componentIndex];
				if(m_insertion == true)
				{
					m_components->Add(component);
					m_componentToRow->Add(component, this->InsertionRow);
					m_insertion = false;
				}
				else
				{
					this->Bind(component, componentIndex);
				}
            }
            break;
        case System::ComponentModel::ListChangedType::ItemDeleted:
            {
				auto component = m_components[e->NewIndex];
				if(m_insertion == true)
				{
					m_components->RemoveAt(e->NewIndex);
					m_componentToRow->Remove(component);
					m_insertion = false;
				}
				else
				{
					this->Unbind(e->NewIndex);
				}
            }
            break;
        case System::ComponentModel::ListChangedType::ItemChanged:
            {
                System::Object^ component = m_manager->List[e->NewIndex];

				Row^ row = m_componentToRow[component];
				row->InvokeChanged(e->PropertyDescriptor);
                this->GridControl->InvokeRowChanged(row);
            }
            break;
        case System::ComponentModel::ListChangedType::ItemMoved:
            {
				int oldIndex = e->OldIndex;
				int newIndex = e->NewIndex;
				System::Object^ component = m_components[oldIndex];
				m_components->RemoveAt(oldIndex);
				m_components->Insert(newIndex, component);
				
				Row^ row = m_componentToRow[component];

				m_pDataRowList->MoveDataRow(row->NativeRef, oldIndex, newIndex);
            }
            break;
        case System::ComponentModel::ListChangedType::Reset:
            {
				auto components = m_components;
				auto componentToRow = m_componentToRow;
				m_components = gcnew System::Collections::ArrayList(components->Count);
				m_componentToRow = gcnew System::Collections::Generic::Dictionary<System::Object^, Row^>(m_componentToRow->Count);

				if(m_manager->List->Count == 0)
				{
					m_pDataRowList->Clear();
				}
				else
				{
					std::vector<GrDataRow*> nativeRows;
					nativeRows.reserve(m_manager->List->Count);

					for each(System::Object^ item in m_manager->List)
					{
						if(item == this->GridControl->InsertionRow->Component)
							continue;

						Row^ row = componentToRow[item];
						m_components->Add(item);
						m_componentToRow->Add(item, row);
						nativeRows.push_back(row->NativeRef);
					}
					m_pDataRowList->Reset(nativeRows);
				}

				if(this->InsertionRow->Component != nullptr)
				{
					m_components->Add(this->InsertionRow->Component);
					m_componentToRow->Add(this->InsertionRow->Component, this->InsertionRow);
				}

				this->GridControl->InvokeReset();
            }
            break;
        }
    }

    void RowCollection::gridControl_CurrencyManagerChanging(System::Object^ /*sender*/, CurrencyManagerChangingEventArgs^ /*e*/)
    {
        
    }

    void RowCollection::gridControl_CurrencyManagerChanged(System::Object^ /*sender*/, Ntreev::Windows::Forms::Grid::CurrencyManagerChangedEventArgs^ e)
    {
        DetachManagerEvent();

        m_manager = e->CurrecnyManager;
        m_components->Clear();
		m_componentToRow->Clear();

        if(m_manager == nullptr)
            return;

        int componentIndex = 0;
        for each(System::Object^ item in m_manager->List)
        {
            this->Bind(item, componentIndex++);
        }

        AttachManagerEvent();
    }

	void RowCollection::gridControl_Cleared(System::Object^ /*sender*/, Ntreev::Windows::Forms::Grid::ClearEventArgs^ /*e*/)
    {
		m_components->Clear();
		m_componentToRow->Clear();
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

		m_insertion = true;
        m_manager->AddNew();
		if(this->InsertionRow->IsBeingEdited == false)
            this->InsertionRow->BeginEdit();
		this->InsertionRow->AttachComponent(m_manager->Current);
    }

    void RowCollection::EndInsertion()
    {
		if(this->InsertionRow->Component == nullptr)
            return;

		this->InsertionRow->DetachComponent();
		m_insertion = true;
		m_manager->CancelCurrentEdit();
    }

    void RowCollection::ValidateArgument(Row^ item)
    {
        if(item == nullptr)
            throw gcnew System::ArgumentNullException("item");
        if(item == InsertionRow)
            throw gcnew System::ArgumentException();
    }

    void RowCollection::Clear()
    {
        ManagerEventDetach managerEventDeatch(this);
		
		for (int i = m_manager->Count-1; i >= 0; i--)
		{
			m_manager->RemoveAt(i);
		}

		m_components->Clear();
		m_componentToRow->Clear();
        m_pDataRowList->Clear();
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
				m_insertion = true;
                m_manager->AddNew();
                isNew = true;
            }
            catch(System::Exception^)
            {
                return nullptr;
            }
            item->AttachComponent(m_manager->Current);
        }

        if(this->GridControl->InvokeRowInserting(item) == false)
        {
            if(isNew == true)
            {
				item->DetachComponent();
                m_insertion = false;
                m_manager->CancelCurrentEdit();
            }
        }
        else
        {
            try
            {
				m_pDataRowList->InsertDataRow(item->NativeRef, index);
                m_manager->EndCurrentEdit();
             }
            catch(System::Exception^ e)
            {
				m_pDataRowList->RemoveDataRow(item->NativeRef);
                if(isNew == true)
                {
					item->DetachComponent();
					m_insertion = true;
                    m_manager->CancelCurrentEdit();
                }
                this->GridControl->ShowMessage(e->Message, "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
                return nullptr;
            }
			
            item->AttachChildControl();
            item->EndEditInternal();
            this->GridControl->InvokeRowInserted(item);
			this->InsertionRow->UpdateNativeText();
            this->GridControl->InvokeRowChanged(item);
        }
        return item;
    }

    void RowCollection::RemoveAt(int index)
    {
        Row^ row = this[index];
        this->Remove(row);
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
            this->Insert(this->Count, row);
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
        ValidateArgument(item);

        if(item->Index == INVALID_INDEX)
            throw gcnew System::ArgumentException("이미 지워지거나 사용되지 않은 row입니다.");

        if(this->GridControl->InvokeRowRemoving(item) == false)
            return false;

        //ManagerEventDetach managerEventDeatch(this);

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
        this->Add((Row^)value);
        return count;
    }

    bool RowCollection::Contains_System_Collections_IList(System::Object^ value)
    {
        return this->Contains((Row^)value);
    }

    void RowCollection::Clear_System_Collections_IList()
    {
        for(int i = this->Count - 1; i >= 0; i--)
        {
            this->RemoveAt(i);
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
                //m_manager->CurrentChanged -= m_currentChangedEventHandler;
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
                //m_manager->CurrentChanged += m_currentChangedEventHandler;
            }
        }
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/