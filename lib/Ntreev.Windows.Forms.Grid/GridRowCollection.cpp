#include "StdAfx.h"
#include "GridRowCollection.h"
#include "GridRow.h"
#include "GridControl.h"
#include "GridColumn.h"
#include "GridCell.h"

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

	RowCollection::RowCollection(_GridControl^ gridControl) 
		: GridObject(gridControl)
	{
		m_pDataRowList = GridCore->GetDataRowList();

		m_listChangedEventHandler = gcnew System::ComponentModel::ListChangedEventHandler(this, &RowCollection::currencyManager_ListChanged);
		m_list = gcnew System::Collections::ArrayList();
		
		gridControl->CurrencyManagerChanged += gcnew CurrencyManagerChangedEventHandler(this, &RowCollection::gridControl_CurrencyManagerChanged);
	}

	Row^ RowCollection::BindNew(object^ component)
	{
		if(GridControl->InvokeRowInserting(component) == false)
			return nullptr;

		Row^ row = gcnew Row(GridControl);
		row->Component = component;

		m_pDataRowList->AddDataRow(row->NativeRef);
		row->RefreshCells();

		GridControl->InvokeRowInserted(row);
		return row;
	}

	bool RowCollection::Unbind(int /*componentIndex*/)
	{
		return false;
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

	void RowCollection::currencyManager_ListChanged(object^ /*sender*/, System::ComponentModel::ListChangedEventArgs^ /*e*/)
	{
#if 0 // unused, but
		switch(e->ListChangedType)
		{
		case System::ComponentModel::ListChangedType::ItemAdded:
			{
				object^ component = m_currencyManager->List[e->NewIndex];
				BindNew(component);
				System::Diagnostics::Debug::Assert(m_list->Count == e->NewIndex);
				m_list->Add(component);
				Invalidate();
			}
			break;
		case System::ComponentModel::ListChangedType::ItemDeleted:
			{
				object^ component = m_list[e->NewIndex];

				m_list->RemoveAt(e->NewIndex);

				Row^ row = this[component];
				if(row == nullptr)
					return;
		
				GridControl->InvokeRowRemoving(row);

				m_pDataRowList->RemoveDataRow(row->NativeRef);

				RowRemovedEventArgs eRemoved(0);
				GridControl->InvokeRowRemoved(%eRemoved);
				Invalidate();
			}
			break;
		case System::ComponentModel::ListChangedType::ItemChanged:
			{
				//object^ component = m_list[e->NewIndex];

				//Row^ row = this[component];
				//row[e->PropertyDescriptor2->Name]->UpdateNativeText();
			}
			break;
		case System::ComponentModel::ListChangedType::ItemMoved:
			{
				
			}
			break;
		case System::ComponentModel::ListChangedType::Reset:
			{
				m_pDataRowList->ClearDataRow();
				m_list->Clear();
				Invalidate();
			}
			break;
		}
#endif
	}

	void RowCollection::gridControl_CurrencyManagerChanged(object^ /*sender*/, CurrencyManagerChangedEventArgs^ e)
	{
		if(m_currencyManager != nullptr)
			m_currencyManager->ListChanged -= m_listChangedEventHandler;

		m_currencyManager = e->CurrecnyManager;
		m_list->Clear();

		if(m_currencyManager == nullptr)
			return;

		m_list->Capacity = m_currencyManager->List->Count;
		for each(object^ item in m_currencyManager->List)
		{
			BindNew(item);
		}

		m_list->AddRange(m_currencyManager->List);

		m_currencyManager->ListChanged += m_listChangedEventHandler;
	}

	void RowCollection::ArgumentTest(_Row^ item)
	{
		if(item == nullptr)
			throw gcnew System::ArgumentNullException("item");
		if(item == InsertionRow)
			throw gcnew System::ArgumentException();
	}

	void RowCollection::Clear()
	{
		m_pDataRowList->ClearDataRow();

		m_currencyManager->ListChanged -= m_listChangedEventHandler;
		for(int i=m_currencyManager->Count-1 ; i>=0 ; i--)
			m_currencyManager->RemoveAt(i);
		
		m_currencyManager->ListChanged += m_listChangedEventHandler;
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

		m_currencyManager->ListChanged -= m_listChangedEventHandler;

		try
		{
			m_currencyManager->AddNew();
			object^ component = m_currencyManager->Current;
			
			if(GridControl->InvokeRowInserting(component) == false)
			{
				m_currencyManager->CancelCurrentEdit();
			}
			else
			{
				m_currencyManager->EndCurrentEdit();
			
				m_pDataRowList->InsertDataRow(item->NativeRef, index);
				item->Component = component;
				item->RefreshCells();

				GridControl->InvokeRowInserted(item);
			}
		}
		catch(System::Exception^ e)
		{
			throw e;
		}
		finally
		{
			m_currencyManager->ListChanged += m_listChangedEventHandler;
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
		m_currencyManager->ListChanged -= m_listChangedEventHandler;

		try
		{
			m_currencyManager->AddNew();
		}
		catch(System::Exception^ e)
		{
			m_currencyManager->ListChanged += m_listChangedEventHandler;
			throw e;
		}

		Row^ row = gcnew Row(GridControl);
		m_pDataRowList->AddDataRow(row->NativeRef);
		row->Component = m_currencyManager->Current;
		row->RefreshCells();
		row->IsVisible = false;

		for(int i=0 ; i<InsertionRow->CellCount ; i++)
		{
			Cell^ sourceCell = InsertionRow[i];
			Cell^ cell		 = row[i];

			cell->Value		= sourceCell->Value;
			cell->Tag		= sourceCell->Tag;
		}

		try
		{
			if(GridControl->InvokeInsertionRowInserting(row) == false)
			{
				m_currencyManager->CancelCurrentEdit();	
				m_pDataRowList->RemoveDataRow(row->NativeRef);
				return nullptr;
			}
			
			m_currencyManager->EndCurrentEdit();
			row->IsVisible = true;
			
			InsertionRow->SetDefaultValue();
			GridControl->InvokeInsertionRowInserted(row);
		}
		finally
		{
			m_currencyManager->ListChanged += m_listChangedEventHandler;
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

		m_currencyManager->ListChanged -= m_listChangedEventHandler;

		try
		{
			if(GridControl->InvokeRowRemoving(item) == false)
				return false;
			
			int index = m_currencyManager->List->IndexOf(item->Component);
			if(index < 0)
				throw gcnew System::ArgumentException("이미 지워지거나 사용되지 않은 row입니다.");

			m_currencyManager->RemoveAt(index);
			m_pDataRowList->RemoveDataRow(item->NativeRef);


			RowRemovedEventArgs eRemoved(0);
			GridControl->InvokeRowRemoved(%eRemoved);
			Invalidate();
		}
		finally
		{
			m_currencyManager->ListChanged += m_listChangedEventHandler;
		}

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

	_InsertionRow^ RowCollection::InsertionRow::get()
	{
		return GridControl->InsertionRow;
	}

	Row^ RowCollection::default::get(GrDataRow* pDataRow)
	{
		return Row::FromNative(pDataRow);
	}

	Row^ RowCollection::default::get(object^ component)
	{
		for each(Row^ item in this)
		{
			if(item->Component == component)
				return item;
		}
		return nullptr;
	}

	SelectedRowCollection::SelectedRowCollection(_GridControl^ gridControl, const GrSelectedRows* selectedRows) 
		: GridObject(gridControl), m_selectedRows(selectedRows)
	{
	
	}

	void SelectedRowCollection::Add(Row^ item)
	{
		if(item == nullptr)
			throw gcnew System::ArgumentNullException("item");

		if(item->Index == INSERTION_ROW)
			throw gcnew System::Exception(Properties::Resources::RemoveInsertionRowException);

		if(item->Index == INVALID_INDEX)
			throw gcnew System::ArgumentException("이미 지워지거나 사용되지 않은 row입니다.");

		if(item->IsSelected == true)
			return;
		item->IsSelected = true;
	}

	void SelectedRowCollection::Add(Row^ row, Column^ focusColumn)
	{
		Add(row);
		
		GrDataRow*	pDataRow = row->NativeRef;
		GrItem*		pItem = nullptr;
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

	_Row^ SelectedRowCollection::Enumerator::Current::get()
	{
		const GrDataRow* pDataRow = m_selectedRows->at(m_index-1);
		object^ ref = pDataRow->ManagedRef;
		return safe_cast<Row^>(ref);
	}

	Row^ SelectedRowCollection::default::get(int index)
	{
		const GrDataRow* pDataRow = m_selectedRows->at(index);
		object^ ref = pDataRow->ManagedRef;
		return safe_cast<Row^>(ref);
	}

	VisibleRowCollection::VisibleRowCollection(_GridControl^ gridControl)
		: GridObject(gridControl)
	{
		m_pDataRowList = GridCore->GetDataRowList();
	}
	
	RowBase^ VisibleRowCollection::default::get(int index)
	{
		if((uint)index >= m_pDataRowList->GetVisibleRowCount())
			throw gcnew System::ArgumentOutOfRangeException("index");
		IDataRow* pDataRow = m_pDataRowList->GetVisibleRow(index);
		object^ ref = pDataRow->ManagedRef;
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

	_RowBase^ VisibleRowCollection::Enumerator::Current::get()
	{
		IDataRow* pDataRow = m_pDataRowList->GetVisibleRow(m_index - 1);
		object^ ref = pDataRow->ManagedRef;
		return safe_cast<RowBase^>(ref);
	}

	DisplayableRowCollection::DisplayableRowCollection(_GridControl^ gridControl)
		: GridObject(gridControl)
	{
		m_pDataRowList = GridCore->GetDataRowList();
	}
	
	RowBase^ DisplayableRowCollection::default::get(int index)
	{
		if((uint)index >= m_pDataRowList->GetDisplayableRowCount())
			throw gcnew System::ArgumentOutOfRangeException("index");
		IDataRow* pDataRow = m_pDataRowList->GetDisplayableRow(index);
		object^ ref = pDataRow->ManagedRef;
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

	_RowBase^ DisplayableRowCollection::Enumerator::Current::get()
	{
		IDataRow* pDataRow = m_pDataRowList->GetDisplayableRow(m_index - 1);
		object^ ref = pDataRow->ManagedRef;
		return safe_cast<RowBase^>(ref);
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/