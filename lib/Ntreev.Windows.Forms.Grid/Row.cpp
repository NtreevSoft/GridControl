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
#include "Row.h"
#include "GridControl.h"
#include "Cell.h"
#include "Column.h"
#include "ColumnCollection.h"
#include "RowBaseCollection.h"
#include "CellCollection.h"
#include "CellTagCollection.h"
#include "ErrorDescriptor.h"
#include "FromNative.h"
#include "RowBuilder.h"
#include "CellBuilder.h"
#include "NativeGridRow.h"
#include "GridRow.h"

#include "GrGridCell.h"
#include "GrGridCore.h"
#include "GrGridPainter.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	using namespace System::ComponentModel;

	//System::Collections::Generic::Dictionary<Cell^, System::String^>^ Row::emptyErrors = gcnew System::Collections::Generic::Dictionary<Cell^, System::String^>();

	Row::Row(RowBuilder^ rowBuilder)
		: m_pDataRow(rowBuilder->NativeRef), RowBase(rowBuilder->GridControl, rowBuilder->NativeRef)
		, m_error(System::String::Empty)
		, m_sourceError(System::String::Empty)
	{
		m_cells = gcnew CellCollection(this);

		for each(Column^ item in this->GridControl->Columns)
		{
			this->NewCell(item);
		}
	}

	void Row::AttachComponent(System::Object^ component)
	{
		m_component = component;

		for each(Cell^ item in m_cells)
		{
			try
			{
				item->LocalValueToSource(component);
				item->UpdateNativeText();
			}
			catch(System::Exception^)
			{

			}
		}

		System::ComponentModel::IDataErrorInfo^ dataErrorInfo = dynamic_cast<System::ComponentModel::IDataErrorInfo^>(m_component);

		if(dataErrorInfo != nullptr)
		{
			if(System::String::IsNullOrEmpty(dataErrorInfo->Error) == false)
				this->SourceError = dataErrorInfo->Error;

			for each(Cell^ item in m_cells)
			{
				Column^ column = item->Column;
				if(column->PropertyDescriptor == nullptr || column->PropertyDescriptor->PropertyType == IBindingList::typeid)
					continue;

				System::String^ error = dataErrorInfo[column->ColumnName];
				if(System::String::IsNullOrEmpty(error) == false)
					this->SetSourceError(item, error);
			}
		}
	}

	void Row::DetachComponent()
	{
		auto oldComponent = m_component;
		m_component = nullptr;
		for each(Cell^ item in m_cells)
		{
			try
			{
				if(this->HasInvalidValue(item) == true)
					continue;
				item->SourceValueToLocal(oldComponent);
				//item->UpdateNativeText();
			}
			catch(System::Exception^)
			{

			}
		}
	}

	void Row::AttachChildControl()
	{
		using namespace System::ComponentModel;

		for each(Column^ item in this->GridControl->Columns)
		{
			if(item->PropertyDescriptor != nullptr && item->PropertyDescriptor->PropertyType == IBindingList::typeid)
			{
				Native::GrGridRow* pChildRow = new Native::GrGridRow(this->GridControl, item->PropertyDescriptor, this);
				m_pDataRow->AddChild(pChildRow);
			}
		}
	}

	void Row::DetachChildControl()
	{
		for each(RowBase^ item in this->Childs)
		{
			GridRow^ gridRow = dynamic_cast<GridRow^>(item);
			if(gridRow == nullptr)
				continue;

			this->GridControl->Controls->Remove(gridRow->ChildGrid);
			delete gridRow->ChildGrid;
		}
	}

	System::Collections::Generic::Dictionary<Cell^, System::String^>^ Row::Errors::get()
	{
		return m_errors;
	}

	System::Collections::Generic::Dictionary<Cell^, System::String^>^ Row::SourceErrors::get()
	{
		return m_sourceErrors;
	}

	System::Collections::Generic::Dictionary<Cell^, System::String^>^ Row::InvalidValues::get()
	{
		return m_invalidValues;
	}

	GrDataRow* Row::NativeRef::get()
	{
		return m_pDataRow;
	}

	Cell^ Row::NewCell(Column^ column)
	{
		GrItem* pItem = m_pDataRow->GetItem(column->NativeRef);

		Cell^ cell = FromNative::Get(pItem);
		if(cell == nullptr)
		{
			CellBuilder builder;
			builder.GridControl = this->GridControl;
			builder.NativeRef = pItem;
			cell = this->NewCellFromBuilder(%builder);
		}

		return cell;
	}

	Cell^ Row::NewCellFromBuilder(CellBuilder^ builder)
	{
		return gcnew Cell(builder->GridControl, builder->NativeRef);
	}

	System::String^ Row::ToString()
	{
		return this->Index.ToString();
	}

	Ntreev::Windows::Forms::Grid::Cell^ Row::GetAt(int index)
	{
		return this->Cells[index];
	}

	Ntreev::Windows::Forms::Grid::CellCollection^ Row::Cells::get()
	{
		return m_cells;
	}

	Ntreev::Windows::Forms::Grid::CellTagCollection^ Row::CellTags::get()
	{
		if(m_cellTags == nullptr)
		{
			m_cellTags = gcnew Ntreev::Windows::Forms::Grid::CellTagCollection(this);
		}
		return m_cellTags;
	}

	int Row::CellCount::get()
	{
		return this->Cells->Count;
	}

	unsigned int Row::RowID::get()
	{
		return m_pDataRow->GetDataRowID(); 
	}

	System::Object^ Row::default::get(int index)
	{
		return this->Cells[index]->Value;
	}

	void Row::default::set(int index, System::Object^ value)
	{
		this->Cells[index]->Value = value;
	}

	System::Object^ Row::default::get(System::String^ columnName)
	{
		return m_cells[columnName]->Value;
	}

	void Row::default::set(System::String^ columnName, System::Object^ value)
	{
		m_cells[columnName]->Value = value;
	}

	System::Object^ Row::default::get(Column^ column)
	{
		return this->Cells[column]->Value;
	}

	void Row::default::set(Column^ column, System::Object^ value)
	{
		this->Cells[column]->Value = value;
	}

	bool Row::IsVisible::get()
	{
		return m_pDataRow->GetVisible();
	}

	void Row::IsVisible::set(bool value)
	{
		m_pDataRow->SetVisible(value);
	}

	bool Row::IsReadOnly::get()
	{
		return m_pDataRow->GetReadOnly();
	}

	void Row::IsReadOnly::set(bool value)
	{
		m_pDataRow->SetReadOnly(value);
	}

	bool Row::IsSelected::get()
	{
		return m_pDataRow->GetSelected();
	}

	void Row::IsSelected::set(bool value)
	{
		m_pDataRow->SetSelected(value);
	}

	bool Row::IsFullSelected::get()
	{
		return m_pDataRow->GetFullSelected();
	}

	void Row::AddEditedCell()
	{
		m_editedCount++;
	}

	void Row::RemoveEditedCell()
	{
		m_editedCount--;
		if(m_editedCount < 0)
			throw gcnew System::Exception();
	}

	bool Row::IsBeingEdited::get()
	{
		return m_editing;
	}

	bool Row::IsEdited::get()
	{
		return m_editedCount > 0 ? true : false;
	}

	int Row::Index::get()
	{
		return (int)m_pDataRow->GetDataRowIndex();
	}

	void Row::BeginEdit()
	{
		m_editing = true;
	}

	void Row::CancelEdit()
	{
		try
		{
			if(m_editedCount == 0)
				return;

			for each(Cell^ cell in m_cells)
			{
				cell->CancelEditInternal();
			}

			if(m_editedCount < 0)
				throw gcnew System::Exception("먼가 수상합니다.");
		}
		finally
		{
			m_editedCount = 0;
			m_editing = false;
		}

		for each(Cell^ item in m_cells)
		{
			item->UpdateNativeText();
		}
	}

	void Row::EndEdit()
	{
		this->GridControl->EndCurrentEdit(this);
	}

	void Row::EndEditInternal()
	{
		try
		{
			if(m_editedCount == 0)
				return;

			for each(Cell^ cell in m_cells)
			{
				cell->EndEditInternal();
			}

			if(m_editedCount < 0)
				throw gcnew System::Exception("먼가 수상합니다.");
		}
		finally
		{
			m_editedCount = 0;
			m_editing = false;
		}

		for each(Cell^ item in m_cells)
		{
			item->UpdateNativeText();
		}
		this->GridControl->InvokeRowChanged(this);
	}

	void Row::BringIntoView()
	{
		if(m_pDataRow->GetDisplayable() == true)
			return;
		this->GridControl->BringIntoView(this);
	}

	void Row::Select()
	{
		this->Selector->SelectDataRow(m_pDataRow, GrSelectionType_Normal);
	}

	void Row::Select(Ntreev::Windows::Forms::Grid::SelectionType selectionType)
	{
		this->Selector->SelectDataRow(m_pDataRow, (GrSelectionType)selectionType);
	}

	void Row::ResetCellBackColor()
	{
		this->CellBackColor = System::Drawing::Color::Empty;
	}

	void Row::ResetCellForeColor()
	{
		this->CellForeColor = System::Drawing::Color::Empty;
	}

	System::Drawing::Color Row::CellBackColor::get()
	{
		return m_pDataRow->GetItemBackColor();
	}

	void Row::CellBackColor::set(System::Drawing::Color value)
	{
		m_pDataRow->SetItemBackColor(value);
	}

	System::Drawing::Color Row::CellForeColor::get()
	{
		return m_pDataRow->GetItemForeColor();
	}

	System::String^ Row::Error::get()
	{
		return m_error; 
	}

	void Row::Error::set(System::String^ value)
	{
		value = value == nullptr ? System::String::Empty : value;

		if(m_error == value)
			return;

		m_error = value;
		if(m_error == System::String::Empty)
		{
			if(m_errors->Count == 0)
				this->GridControl->ErrorDescriptor->Remove(this);
		}
		else
		{
			this->GridControl->ErrorDescriptor->Add(this);
		}
	}

	System::String^ Row::SourceError::get()
	{
		return m_sourceError;
	}

	void Row::SourceError::set(System::String^ value)
	{
		value = value == nullptr ? System::String::Empty : value;

		if(m_sourceError == value)
			return;

		m_sourceError = value;
		if(m_sourceError == System::String::Empty)
		{
			if(this->HasErrors == false)
				this->GridControl->ErrorDescriptor->Remove(this);
		}
		else
		{
			this->GridControl->ErrorDescriptor->Add(this);
		}
	}

	bool Row::HasErrors::get()
	{
		if(System::String::IsNullOrEmpty(m_error) == false || System::String::IsNullOrEmpty(m_sourceError) == false)
			return true;
		if(m_errors != nullptr && m_errors->Count > 0)
			return true;
		if(m_sourceErrors != nullptr && m_sourceErrors->Count > 0)
			return true;
		if(m_invalidValues != nullptr && m_invalidValues->Count > 0)
			return false;
		return false;
	}

	void Row::CellForeColor::set(System::Drawing::Color value)
	{
		m_pDataRow->SetItemForeColor(value);
	}

	bool Row::ShouldSerializeCellForeColor()
	{
		return m_pDataRow->GetItemForeColor() != GrColor::Empty;
	}

	bool Row::ShouldSerializeCellBackColor()
	{
		return m_pDataRow->GetItemBackColor() != GrColor::Empty;
	}

	bool Row::ShouldSerializeCellFont()
	{
		return m_pDataRow->GetItemFont() != nullptr;
	}

	void Row::InvokeChanged(System::ComponentModel::PropertyDescriptor^ descriptor)
	{
		//if(m_component == nullptr)
		//	throw gcnew System::ArgumentException();

		System::ComponentModel::IDataErrorInfo^ dataErrorInfo = dynamic_cast<System::ComponentModel::IDataErrorInfo^>(m_component);

		if(dataErrorInfo != nullptr)
		{
			this->SourceError = dataErrorInfo->Error;
		}

		if(descriptor == nullptr)
		{
			for each(Cell^ item in m_cells)
			{
				item->UpdateNativeText();

				Column^ column = item->Column;

				if(column->PropertyDescriptor != nullptr && column->PropertyDescriptor->PropertyType == IBindingList::typeid)
				{
					for(uint i=0; i<m_pDataRow->GetChildCount() ; i++)
					{
						Native::GrGridRow* childRow = dynamic_cast<Native::GrGridRow*>(m_pDataRow->GetChild(i));
						if(childRow != nullptr)
						{
							if(childRow->GetPropertyDescriptor() == column->PropertyDescriptor)
							{
								childRow->Update();
							}
						}
					}
				}

				if(dataErrorInfo != nullptr && column->PropertyDescriptor != nullptr && column->PropertyDescriptor->PropertyType != IBindingList::typeid)
				{
					this->SetSourceError(item, dataErrorInfo[column->ColumnName]);
				}
			}
		}
		else
		{
			Cell^ cell =  m_cells[descriptor->Name];
			cell->UpdateNativeText();

			if(dataErrorInfo != nullptr)
			{
				this->SetSourceError(cell, dataErrorInfo[descriptor->Name]);
			}
			this->GridControl->InvokeValueChanged(cell);
		}
	}

	void Row::UpdateNativeText()
	{
		for each(Cell^ item in m_cells)
		{
			item->UpdateNativeText();
		}
	}

	System::String^ Row::GetError(Cell^ cell)
	{
		if(m_errors == nullptr)
			return System::String::Empty;

		if(m_errors->ContainsKey(cell) == false)
			return System::String::Empty;

		return m_errors[cell];
	}

	void Row::SetError(Cell^ cell, System::String^ text)
	{
		text = text == nullptr ? System::String::Empty : text;

		if(text == System::String::Empty)
		{
			if(m_errors == nullptr)
				return;

			if(m_errors->ContainsKey(cell) == true)
				m_errors->Remove(cell);

			if(this->HasErrors == false)
				this->GridControl->ErrorDescriptor->Remove(this);
		}
		else
		{
			if(m_errors == nullptr)
				m_errors = gcnew System::Collections::Generic::Dictionary<Cell^, System::String^>();

			if(m_errors->ContainsKey(cell) == false || m_errors[cell] != text)
			{
				m_errors[cell] = text;
				this->GridControl->ErrorDescriptor->Add(this);
			}
		}
	}

	System::String^ Row::GetSourceError(Cell^ cell)
	{
		if(m_sourceErrors == nullptr)
			return System::String::Empty;

		if(m_sourceErrors->ContainsKey(cell) == false)
			return System::String::Empty;

		return m_sourceErrors[cell];
	}

	void Row::SetSourceError(Cell^ cell, System::String^ text)
	{
		text = text == nullptr ? System::String::Empty : text;

		if(text == System::String::Empty)
		{
			if(m_sourceErrors == nullptr)
				return;

			if(m_sourceErrors->ContainsKey(cell) == true)
				m_sourceErrors->Remove(cell);

			if(this->HasErrors == false)
				this->GridControl->ErrorDescriptor->Remove(this);
		}
		else
		{
			if(m_sourceErrors == nullptr)
				m_sourceErrors = gcnew System::Collections::Generic::Dictionary<Cell^, System::String^>();

			if(m_sourceErrors->ContainsKey(cell) == false || m_sourceErrors[cell] != text)
			{
				m_sourceErrors[cell] = text;
				this->GridControl->ErrorDescriptor->Add(this);
			}
		}
	}

	System::String^ Row::GetInvalidValue(Cell^ cell)
	{
		if(m_invalidValues == nullptr)
			return System::String::Empty;

		if(m_invalidValues->ContainsKey(cell) == false)
			return System::String::Empty;

		return m_invalidValues[cell];
	}

	void Row::SetInvalidValue(Cell^ cell, System::String^ text)
	{
		text = text == nullptr ? System::String::Empty : text;

		if(text == System::String::Empty)
		{
			if(m_invalidValues == nullptr)
				return;

			if(m_invalidValues->ContainsKey(cell) == true)
				m_invalidValues->Remove(cell);

			if(this->HasErrors == false)
				this->GridControl->ErrorDescriptor->Remove(this);
		}
		else
		{
			if(m_invalidValues == nullptr)
				m_invalidValues = gcnew System::Collections::Generic::Dictionary<Cell^, System::String^>();

			if(m_invalidValues->ContainsKey(cell) == false || m_invalidValues[cell] != text)
			{
				m_invalidValues[cell] = text;
				this->GridControl->ErrorDescriptor->Add(this);
			}
		}
	}

	bool Row::HasInvalidValue(Cell^ cell)
	{
		if(m_invalidValues == nullptr)
			return false;

		return m_invalidValues->ContainsKey(cell);
	}

	System::Object^ Row::GetSourceValue(Column^ column)
	{
		PropertyDescriptor^ propertyDescriptor = column->PropertyDescriptor;

		if(propertyDescriptor == nullptr || m_component == nullptr)
			throw gcnew System::ArgumentException();
		System::Object^ value = propertyDescriptor->GetValue(m_component);
		return column->ConvertFromSource(value);
	}

	void Row::SetSourceValue(Column^ column, System::Object^ value)
	{
		PropertyDescriptor^ propertyDescriptor = column->PropertyDescriptor;

		if(propertyDescriptor->IsReadOnly == false)
		{
			value = column->ConvertToSource(value);
			try
			{
				//if(propertyDescriptor->ShouldSerializeValue(component) == false)
				propertyDescriptor->SetValue(m_component, value);
			}
			catch(System::Exception^ e)
			{
				if(value == nullptr)
					propertyDescriptor->SetValue(m_component, System::DBNull::Value);
				else
					throw e;
			}
		}
	}

	bool Row::HasSourceValue(Column^ column)
	{
		PropertyDescriptor^ propertyDescriptor = column->PropertyDescriptor;
		if(propertyDescriptor == nullptr || m_component == nullptr)
			return false;
		return true;
	}

	int Row::GetCellsTextCapacity()
	{
		return m_textCapacity;
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/
