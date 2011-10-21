//=====================================================================================================================
// Ntreev Grid for .Net 1.0.4300.26762
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
#include "GridColumnCollection.h"
#include "GridColumn.h"
#include "GridControl.h"
#include "GridColumnExtension.h"
#include "GridColumnExtension2.h"
#include "GridColumnUITypeEditor.h"

#include <vcclr.h>

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	ColumnCollection::Enumerator::Enumerator(GrColumnList* pColumnList)
		: m_pColumnList(pColumnList), m_index(0)
	{

	}
			
	ColumnCollection::Enumerator::~Enumerator()
	{

	}
	
	bool ColumnCollection::Enumerator::MoveNext()
	{
		m_index++;
		return m_index <= m_pColumnList->GetColumnCount();
	}
			
	void ColumnCollection::Enumerator::Reset()
	{
		m_index = 0;
	}

	Column^ ColumnCollection::Enumerator::Current::get()
	{
		GrColumn* pColumn = m_pColumnList->GetColumn(m_index - 1);
		return Column::FromNative(pColumn);
	}

	ColumnCollection::ColumnCollection(_GridControl^ gridControl)
		: GridObject(gridControl)
	{
		m_pColumnList = GridCore->GetColumnList();

		m_listChangedEventHandler = gcnew System::ComponentModel::ListChangedEventHandler(this, &ColumnCollection::currencyManager_ListChanged);

		gridControl->CurrencyManagerChanging += gcnew CurrencyManagerChangingEventHandler(this, &ColumnCollection::gridControl_CurrencyManagerChanging);
		gridControl->CurrencyManagerChanged += gcnew CurrencyManagerChangedEventHandler(this, &ColumnCollection::gridControl_CurrencyManagerChanged);
		gridControl->Clearing += gcnew ClearEventHandler(this, &ColumnCollection::gridControl_Clearing);
		gridControl->Cleared += gcnew ClearEventHandler(this, &ColumnCollection::gridControl_Cleared);
	}

	void ColumnCollection::Add(_Column^ item)
	{
		Insert(Count, item);
	}

	void ColumnCollection::AddRange(cli::array<_Column^>^ values)
	{
		if(values == nullptr)
			throw gcnew System::ArgumentNullException("values");

		for each(_Column^ item in values)
		{
			Add(item);
		}
	}

	_Column^ ColumnCollection::AddNew()
	{
		return InsertNew(this->Count);
	}

	_Column^ ColumnCollection::AddNew(string^ name)
	{
		return InsertNew(this->Count, name);
	}

	_Column^ ColumnCollection::AddNew(string^ name, _Type^ type)
	{
		return InsertNew(this->Count, name, type);
	}

	void ColumnCollection::Remove(_Column^ item)
	{
		if(item == nullptr)
			throw gcnew System::ArgumentNullException("item");
		if(item->Index == 0xffffffff)
			throw gcnew System::ArgumentException("이미 제거된 항목입니다.");

		item->GridControl = nullptr;
		m_pColumnList->RemoveColumn(item->NativeRef);
		Invalidate();
	}

	int ColumnCollection::IndexOf(_Column^ item)
	{
		if(item == nullptr)
			throw gcnew System::ArgumentNullException("item");
		return item->Index;
	}

	void ColumnCollection::Insert(int index, _Column^ item)
	{
		if(index < 0 || index > this->Count)
			throw gcnew System::ArgumentOutOfRangeException("index");
		if(item == nullptr)
			throw gcnew System::ArgumentNullException("item");
		if(item->Index != 0xffffffff)
			throw gcnew System::ArgumentException("이미 추가된 항목입니다.");

		if(this[item->ColumnName] != nullptr)
			throw gcnew System::ArgumentException(string::Format("\"{0}\"의 이름을 가진 항목이 이미 있습니다.", item->ColumnName));

		if(item->ColumnName == string::Empty)
			item->ColumnName = NewColumnName();

		if(GridControl->InvokeColumnInserting(item) == false)
			return;

		item->GridControl = GridControl;

		m_pColumnList->InsertColumn(item->NativeRef, index);

		GridControl->InvokeColumnInserted(item);
	}

	_Column^ ColumnCollection::InsertNew(int index)
	{
		return InsertNew(index, NewColumnName());
	}

	_Column^ ColumnCollection::InsertNew(int index, string^ name)
	{
		return InsertNew(index, name, string::typeid);
	}

	_Column^ ColumnCollection::InsertNew(int index, string^ name, _Type^ type)
	{
		if(name == nullptr || name == string::Empty)
			name = NewColumnName();
		if(type == nullptr)
			type = string::typeid;

		_Column^ column = CreateColumnInstance(type);
		column->ColumnName = name;
		column->DataType = type;
		Insert(index, column);
		return column;
	}

	void ColumnCollection::RemoveAt(int index)
	{
		Column^ column = this[index];
		Remove(column);
	}

	bool ColumnCollection::Contains(_Column^ item)
	{
		if(item == nullptr)
			throw gcnew System::ArgumentNullException("item");
		return item->Index >= 0;
	}

	_Column^ ColumnCollection::Bind(_PropertyDescriptor^ propertyDescriptor)
	{
		_Column^ existColumn = this[propertyDescriptor->Name];
		ColumnBindingEventArgs e(propertyDescriptor, existColumn);
		GridControl->InvokeColumnBinding(%e);
		_Column^ column = e.BindingColumn;
		if(column == nullptr)
		{
			column = CreateColumnInstance(propertyDescriptor);
		}

		if(existColumn == nullptr)
			column->HasLifeline = true;

		if(column->GridControl == nullptr)
		{
			column->GridControl			= GridControl;
			column->PropertyDescriptor	= propertyDescriptor;
			m_pColumnList->AddColumn(column->NativeRef);
		}
		else
		{
			column->PropertyDescriptor	= propertyDescriptor;
		}

		GridControl->InvokeColumnBinded(gcnew ColumnEventArgs(column));

		return column;
	}

	void ColumnCollection::Unbind(_Column^ column)
	{
		column->PropertyDescriptor = nullptr;

		if(column->HasLifeline == true)
		{
			column->GridControl = nullptr;
			m_pColumnList->RemoveColumn(column->NativeRef);
			delete column;
		}
	}

	void ColumnCollection::DeleteAll()
	{
		while(this->Count > 0)
		{
			Column^ column = this[this->Count - 1];
			delete column;
		}
	}

	void ColumnCollection::SetItemsByDesigner(cli::array<object^>^ values)
	{
		//GrGroupingList* pGroupingList = GridCore->GetGroupingList();
		//std::vector<GrColumn*> groupings;
		//groupings.reserve(pGroupingList->GetGroupingCount());

		//for(uint i=0 ; i<pGroupingList->GetGroupingCount() ; i++)
		//{
		//	GrColumn* pColumn = pGroupingList->GetGrouping(i)->GetColumn();
		//	groupings.push_back(pColumn);
		//}

		//this->Clear_IList();

		//for_stl_const(std::vector<GrColumn*>, groupings, itor)
		//{
		//	std::vector<GrColumn*>::value_type value = *itor;
		//	value->SetGrouped(false);
		//}

		//for each(object^ item in values)
		//{
		//	this->Add_IList(item);
		//}

		//for_stl_const(std::vector<GrColumn*>, groupings, itor)
		//{
		//	std::vector<GrColumn*>::value_type value = *itor;
		//	if(value->GetIndex() == INVALID_INDEX)
		//		continue;
		//	value->SetGrouped(true);
		//}

		for each(object^ item in values)
		{
			Column^ column = dynamic_cast<Column^>(item);
			if(column->Index < 0)
				this->Add_IList(item);
		}
	}

	Column^ ColumnCollection::default::get(int index)
	{
		if(index < 0 || index >= (int)m_pColumnList->GetColumnCount())
			throw gcnew System::ArgumentOutOfRangeException("index");
		GrColumn* pColumn = m_pColumnList->GetColumn((uint)index);
		return Column::FromNative(pColumn);
	}

	int ColumnCollection::Count::get()
	{
		return m_pColumnList->GetColumnCount();
	}

	Column^ ColumnCollection::default::get(string^ columnName)
	{
		if(columnName == nullptr)
			columnName = string::Empty;

		for each(_Column^ column in this)
		{
			if(column->ColumnName == columnName)
				return column;
		}
		return nullptr;
	}

	Column^	ColumnCollection::default::get(GrColumn* pColumn)
	{
		return Column::FromNative(pColumn);
	}

	_Column^ ColumnCollection::CreateColumnInstanceCore(System::IServiceProvider^ serviceProvider, _Type^ columnType)
	{
		using namespace System::ComponentModel;
		using namespace System::ComponentModel::Design;
		if(serviceProvider != nullptr)
		{
			IDesignerHost^ designerHost = dynamic_cast<IDesignerHost^>(serviceProvider->GetService(IDesignerHost::typeid));
			if(designerHost != nullptr)
			{
				return dynamic_cast<Column^>(designerHost->CreateComponent(columnType));
			}
		}

		return dynamic_cast<_Column^>(TypeDescriptor::CreateInstance(serviceProvider, columnType, nullptr, nullptr));
	}
	
	Column^	ColumnCollection::CreateColumnInstance(_PropertyDescriptor^ propertyDescriptor)
	{
		System::Type^ dataType = propertyDescriptor->PropertyType;
		Column^ column = nullptr;

		object^ editor = propertyDescriptor->GetEditor(System::Drawing::Design::UITypeEditor::typeid);
		if(propertyDescriptor->DesignTimeOnly == true)
			editor = nullptr;

		ServiceProvider^ serviceProvider = gcnew ServiceProvider(this->GridControl);

		if(editor != nullptr)
		{
			column = CreateColumnInstanceCore(serviceProvider, ColumnUITypeEditor::typeid);
		}
		else if(dataType == bool::typeid)
		{
			column = CreateColumnInstanceCore(serviceProvider, Columns::ColumnCheckBox::typeid);
		}
		else if(dataType->IsEnum == true)
		{
			if(dataType->GetCustomAttributes(System::FlagsAttribute::typeid, true)->Length != 0)
			{
				column = CreateColumnInstanceCore(serviceProvider, Columns::ColumnFlagControl::typeid);
			}
			else
			{
				column = CreateColumnInstanceCore(serviceProvider, Columns::ColumnComboBox::typeid);
			}
		}
		else
		{
			column = CreateColumnInstanceCore(serviceProvider, Columns::ColumnTextBox::typeid);
		}

		return column;
	}

	_Column^ ColumnCollection::CreateColumnInstance(System::IServiceProvider^ serviceProvider, _Type^ dataType)
	{
		using namespace System::ComponentModel;
		Column^ column = nullptr;

		object^ editor = TypeDescriptor::GetEditor(dataType, System::Drawing::Design::UITypeEditor::typeid);

		if(editor != nullptr)
		{
			column = CreateColumnInstanceCore(serviceProvider, ColumnUITypeEditor::typeid);
		}
		else if(dataType == bool::typeid)
		{
			column = CreateColumnInstanceCore(serviceProvider, Columns::ColumnCheckBox::typeid);
		}
		else if(dataType->IsEnum == true)
		{
			if(dataType->GetCustomAttributes(System::FlagsAttribute::typeid, true)->Length != 0)
			{
				column = CreateColumnInstanceCore(serviceProvider, Columns::ColumnFlagControl::typeid);
			}
			else
			{
				column = CreateColumnInstanceCore(serviceProvider, Columns::ColumnComboBox::typeid);
			}
		}
		else
		{
			column = CreateColumnInstanceCore(serviceProvider, Columns::ColumnTextBox::typeid);
		}

		if(column != nullptr)
			column->DataType = dataType;

		return column;
	}

	_Column^ ColumnCollection::CreateColumnInstance(_Type^ dataType)
	{
		return CreateColumnInstance(gcnew ServiceProvider(this->GridControl), dataType);
	}

	string^ ColumnCollection::NewColumnName()
	{
		int columnIndex = 0;

		string^ name;
		do
		{
			name = string::Format("Column{0}", columnIndex++);
		}
		while(this[name] != nullptr);

		return name;
	}

	void ColumnCollection::currencyManager_ListChanged(object^ sender, System::ComponentModel::ListChangedEventArgs^ e)
	{
		using namespace System::ComponentModel;
		using namespace System::ComponentModel::Design;

		switch(e->ListChangedType)
		{
		case System::ComponentModel::ListChangedType::PropertyDescriptorAdded:
			{
				Bind(e->PropertyDescriptor);
				Invalidate();
			}
			break;
		case System::ComponentModel::ListChangedType::PropertyDescriptorChanged:
			{
				using namespace System::Windows::Forms;
				using namespace System::Collections::Generic;

				CurrencyManager^ currencyManager = dynamic_cast<CurrencyManager^>(sender);

				List<_Column^>^ boundColumns = gcnew List<_Column^>();

				for each(_Column^ item in this)
				{
					if(item->PropertyDescriptor != nullptr)
						boundColumns->Add(item);
				}

				_PropertyDescriptor^ changedPropertyDescriptor = nullptr;

				for each(_PropertyDescriptor^ item in currencyManager->GetItemProperties())
				{
					Column^ column = this[item->Name];
					if(column != nullptr)
					{
						column->PropertyDescriptor = item;
						boundColumns->Remove(column);
					}
					else
					{
						changedPropertyDescriptor = item;
					}
				}

				if(boundColumns->Count == 1 && changedPropertyDescriptor != nullptr)
				{
					boundColumns[0]->PropertyDescriptor = changedPropertyDescriptor;
				}
			}
			break;
		case System::ComponentModel::ListChangedType::PropertyDescriptorDeleted:
			{
				_Column^ column = this[e->PropertyDescriptor->Name];
				System::Diagnostics::Debug::Assert(column != nullptr);
				Unbind(column);
			}
			break;
		case System::ComponentModel::ListChangedType::Reset:
			{
				PropertyDescriptorCollection^ props = m_manager->GetItemProperties();

				for each(PropertyDescriptor^ item in props)
				{
					Bind(item);
				}
			}
			break;
		}
	}

	void ColumnCollection::gridControl_CurrencyManagerChanging(object^ /*sender*/, CurrencyManagerChangingEventArgs^ e)
	{
		System::Windows::Forms::CurrencyManager^ manager = e->CurrecnyManager;

		if(manager == nullptr)
			return;

		if(e->Cancel == true)
			return;

		for each(_PropertyDescriptor^ item in manager->GetItemProperties())
		{
			_Column^ column = this[item->Name];
			if(column == nullptr)
				continue;

			if(column->DataType != item->PropertyType)
			{
				if(column->TypeConverter->CanConvertFrom(item->PropertyType) == true)
					continue;

				System::Text::StringBuilder^ builder = gcnew System::Text::StringBuilder();

				builder->AppendLine(string::Format("{0} 은 연결될 데이터 소스와 데이터 타입이 다릅니다.", item->Name));
				builder->AppendLine(string::Format("    GridControl : {0}", column->DataType));
				builder->AppendLine(string::Format("    DataSource  : {0}", item->PropertyType));

				e->Cancel = true;
				e->CancelReason = builder->ToString();
			}
				
			if(e->Cancel == true)
				return;
		}
	}

	void ColumnCollection::gridControl_CurrencyManagerChanged(object^ /*sender*/, CurrencyManagerChangedEventArgs^ e)
	{
		if(m_manager != nullptr)
		{
			m_manager->ListChanged -= m_listChangedEventHandler;
		}

		m_manager = e->CurrecnyManager;

		if(m_manager == nullptr)
			return;

		for each(_PropertyDescriptor^ item in m_manager->GetItemProperties())
		{
			Bind(item);
		}

		m_manager->ListChanged += m_listChangedEventHandler;
	}

	void ColumnCollection::gridControl_Clearing(object^ /*sender*/, ClearEventArgs^ e)
	{
		if(e->DataSourceOnly == true)
		{
			m_tempBindableColumns = gcnew System::Collections::ArrayList();

			for each(_Column^ item in this)
			{
				if(item->HasLifeline == false || item->PropertyDescriptor == nullptr)
					m_tempBindableColumns->Add(item);
			}

			for each(_Column^ item in m_tempBindableColumns)
			{
				this->Remove(item);
			}
		}
		
		DeleteAll();
	}

	void ColumnCollection::gridControl_Cleared(object^ /*sender*/, ClearEventArgs^ e)
	{
		if(e->DataSourceOnly == true)
		{
			for each(_Column^ item in m_tempBindableColumns)
			{
				this->Add(item);
			}

			delete m_tempBindableColumns;
		}
	}
	
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

	_Column^ SelectedColumnCollection::Enumerator::Current::get()
	{
		const GrColumn* pColumn = (*m_selectedColumns)[m_index - 1];
		return Column::FromNative(pColumn);
	}

	SelectedColumnCollection::SelectedColumnCollection(_GridControl^ gridControl, const GrSelectedColumns* selectedColumns) 
		: GridObject(gridControl), m_selectedColumns(selectedColumns)
	{
	
	}

	void SelectedColumnCollection::Add(_Column^ item)
	{
		if(item->IsSelected == true)
			return;
		item->IsSelected = true;
	}

	bool SelectedColumnCollection::Remove(_Column^ item)
	{
		if(item->IsSelected == false)
			return false;
		item->IsSelected = false;
		return true;
	}

	void SelectedColumnCollection::Clear()
	{
		GridControl->ClearSelection();
	}


	VisibleColumnCollection::VisibleColumnCollection(_GridControl^ gridControl)
		: GridObject(gridControl)
	{
		m_pColumnList = GridCore->GetColumnList();
	}

	Column^ VisibleColumnCollection::default::get(int index)
	{
		if((uint)index >= m_pColumnList->GetVisibleColumnCount())
			throw gcnew System::ArgumentOutOfRangeException("index");
		GrColumn* pColumn = m_pColumnList->GetVisibleColumn(index);
		return Column::FromNative(pColumn);
	}

	int VisibleColumnCollection::Count::get()
	{
		return m_pColumnList->GetVisibleColumnCount();
	}

	VisibleColumnCollection::Enumerator::Enumerator(GrColumnList* pColumnList)
		: m_pColumnList(pColumnList)
	{
		m_index = 0;
	}

	VisibleColumnCollection::Enumerator::~Enumerator()
	{

	}

	bool VisibleColumnCollection::Enumerator::MoveNext()
	{
		m_index++;
		return m_index <= m_pColumnList->GetVisibleColumnCount();
	}

	void VisibleColumnCollection::Enumerator::Reset()
	{
		m_index = 0;
	}

	_Column^ VisibleColumnCollection::Enumerator::Current::get()
	{
		GrColumn* pColumn = m_pColumnList->GetVisibleColumn(m_index - 1);
		return _Column::FromNative(pColumn);
	}

	DisplayableColumnCollection::DisplayableColumnCollection(_GridControl^ gridControl)
		: GridObject(gridControl)
	{
		m_pColumnList = GridCore->GetColumnList();
	}

	Column^ DisplayableColumnCollection::default::get(int index)
	{
		if((uint)index >= m_pColumnList->GetDisplayableColumnCount())
			throw gcnew System::ArgumentOutOfRangeException("index");
		GrColumn* pColumn = m_pColumnList->GetDisplayableColumn(index);
		return Column::FromNative(pColumn);
	}

	int DisplayableColumnCollection::Count::get()
	{
		return m_pColumnList->GetDisplayableColumnCount();
	}

	DisplayableColumnCollection::Enumerator::Enumerator(GrColumnList* pColumnList)
		: m_pColumnList(pColumnList)
	{
		m_index = 0;
	}

	DisplayableColumnCollection::Enumerator::~Enumerator()
	{

	}

	bool DisplayableColumnCollection::Enumerator::MoveNext()
	{
		m_index++;
		return m_index <= m_pColumnList->GetDisplayableColumnCount();
	}

	void DisplayableColumnCollection::Enumerator::Reset()
	{
		m_index = 0;
	}

	_Column^ DisplayableColumnCollection::Enumerator::Current::get()
	{
		GrColumn* pColumn = m_pColumnList->GetDisplayableColumn(m_index - 1);
		object^ ref = pColumn->ManagedRef;
		return safe_cast<_Column^>(ref);
	}

	FrozenColumnCollection::FrozenColumnCollection(_GridControl^ gridControl)
		: GridObject(gridControl)
	{
		m_pColumnList = GridCore->GetColumnList();
	}

	Column^ FrozenColumnCollection::default::get(int index)
	{
		if((uint)index >= m_pColumnList->GetFrozenColumnCount())
			throw gcnew System::ArgumentOutOfRangeException("index");
		GrColumn* pColumn = m_pColumnList->GetFrozenColumn(index);
		return _Column::FromNative(pColumn);
	}

	int FrozenColumnCollection::Count::get()
	{
		return m_pColumnList->GetFrozenColumnCount();
	}

	FrozenColumnCollection::Enumerator::Enumerator(GrColumnList* pColumnList)
		: m_pColumnList(pColumnList)
	{
		m_index = 0;
	}

	FrozenColumnCollection::Enumerator::~Enumerator()
	{

	}

	bool FrozenColumnCollection::Enumerator::MoveNext()
	{
		m_index++;
		return m_index <= m_pColumnList->GetFrozenColumnCount();
	}

	void FrozenColumnCollection::Enumerator::Reset()
	{
		m_index = 0;
	}

	_Column^ FrozenColumnCollection::Enumerator::Current::get()
	{
		GrColumn* pColumn = m_pColumnList->GetFrozenColumn(m_index - 1);
		object^ ref = pColumn->ManagedRef;
		return safe_cast<_Column^>(ref);
	}

	UnfrozenColumnCollection::UnfrozenColumnCollection(_GridControl^ gridControl)
		: GridObject(gridControl)
	{
		m_pColumnList = GridCore->GetColumnList();
	}

	Column^ UnfrozenColumnCollection::default::get(int index)
	{
		if((uint)index >= m_pColumnList->GetUnfrozenColumnCount())
			throw gcnew System::ArgumentOutOfRangeException("index");
		GrColumn* pColumn = m_pColumnList->GetUnfrozenColumn(index);
		return _Column::FromNative(pColumn);
	}

	int UnfrozenColumnCollection::Count::get()
	{
		return m_pColumnList->GetUnfrozenColumnCount();
	}

	UnfrozenColumnCollection::Enumerator::Enumerator(GrColumnList* pColumnList)
		: m_pColumnList(pColumnList)
	{
		m_index = 0;
	}

	UnfrozenColumnCollection::Enumerator::~Enumerator()
	{

	}

	bool UnfrozenColumnCollection::Enumerator::MoveNext()
	{
		m_index++;
		return m_index <= m_pColumnList->GetUnfrozenColumnCount();
	}

	void UnfrozenColumnCollection::Enumerator::Reset()
	{
		m_index = 0;
	}

	_Column^ UnfrozenColumnCollection::Enumerator::Current::get()
	{
		GrColumn* pColumn = m_pColumnList->GetUnfrozenColumn(m_index - 1);
		return _Column::FromNative(pColumn);
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/