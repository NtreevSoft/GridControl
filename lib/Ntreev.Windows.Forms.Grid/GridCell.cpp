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
#include "GridCell.h"
#include "GridUtil.h"
#include "GridColumn.h"
#include "GridRow.h"
#include "GridControl.h"
#include "GridResource.h"
#include "GridErrorDescriptor.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	Cell::Cell(_GridControl^ gridControl, GrItem* pItem)
		: CellBase(gridControl, pItem), m_pItem(pItem)
	{
		m_column = _Column::FromNative(pItem->GetColumn());
		m_row = _Row::FromNative(pItem->GetDataRow());

		m_pItem->ManagedRef = this;
		

		UpdateNativeText();
	}

	_Column^ Cell::Column::get()
	{
		return m_column;
	}

	uint Cell::ColumnID::get()
	{
		return m_column->ColumnID;
	}

	_Row^ Cell::Row::get()
	{
		return m_row;
	}

	uint Cell::RowID::get()
	{
		return m_row->RowID;
	}

	object^ Cell::Value::get()
	{
		return ValueCore;
	}

	void Cell::Value::set(object^ value)
	{
		object^ oldValue = this->Value;

		if(object::Equals(value, oldValue) == true)
			return;

		value = ValidateValue(value);

		if(GridControl->InvokeValueChanging(this, value, oldValue) == false)
			return;

		if(this->Row->IsBeingEdited == true)
		{
			if(m_oldValue == nullptr)
			{
				m_oldValue = oldValue;
				m_row->AddEditedCell();
			}
		}

		try
		{
			this->ValueCore = value;
		}
		catch(System::ArgumentException^)
		{
			this->ValueCore = System::DBNull::Value;
		}

		UpdateNativeText(value);
		
		GridControl->InvokeValueChanged(this);
	}

	void Cell::UpdateNativeText()
	{
		UpdateNativeText(this->ValueCore);
	}

	void Cell::UpdateNativeText(object^ value)
	{
		try
		{
			string^ text = string::Empty;
			if(value != nullptr && value != System::DBNull::Value)
				text = m_column->TypeConverter->ConvertToString(value);
			m_pItem->SetText(ToNativeString::Convert(text));
		}
		catch(System::Exception^)
		{
			m_pItem->SetText(L"");
		}
	}

	object^ Cell::ValidateValue(object^ value)
	{
		if(value == nullptr || value == System::DBNull::Value)
			return value;

		_Type^ dataType = this->Column->DataType;
		_Type^ valueType = value->GetType();

		if(dataType == valueType)
			return value;

		if(dataType == string::typeid)
			return value->ToString();

		System::ComponentModel::TypeConverter^ typeConverter = this->Column->TypeConverter;
		if(typeConverter->CanConvertFrom(valueType) == false)
		{
			string^ reason = string::Format("{0}형식을 {1}으로 변환할 수 없습니다.", valueType, dataType);
			throw gcnew System::ArgumentException(reason);
		}

		return typeConverter->ConvertFrom(value);
	}

	Cell^ Cell::FromNative(GrItem* pItem)
	{
		object^ ref = pItem->ManagedRef;
		return safe_cast<Cell^>(ref);
	}

	bool Cell::CancelEdit()
	{
		if(m_oldValue == nullptr)
			return false;

		m_row->RemoveEditedCell();
		Value = m_oldValue;
		m_oldValue = nullptr;
		return true;
	}

	bool Cell::ApplyEdit()
	{
		if(m_oldValue == nullptr)
			return false;

		m_row->RemoveEditedCell();
		m_oldValue = nullptr;
		return true;
	}

	void Cell::Select(_SelectionType selectionType)
	{
		Selector->SelectItem(m_pItem, (GrSelectionType)selectionType);
	}

	void Cell::Focus()
	{
		Focuser->Set(m_pItem);
	}

	void Cell::EnsureVisible()
	{
		GridControl->EnsureVisible(this);
	}

	bool Cell::IsEdited::get()
	{ 
		return m_oldValue != nullptr ? true : false;
	}

	bool Cell::IsSelected::get()
	{
		return m_pItem->GetSelected();
	}

	void Cell::IsSelected::set(bool value)
	{
		if(Row->Index == INVALID_INDEX)
			throw gcnew System::InvalidOperationException();
		if(Row->IsVisible == false)
			throw gcnew System::InvalidOperationException();
		m_pItem->SetSelected(value);
	}

	bool Cell::IsFocused::get()
	{
		return m_pItem->GetFocused();
	}

	void Cell::IsFocused::set(bool value)
	{
		if(Row->Index == INVALID_INDEX)
			throw gcnew System::InvalidOperationException();
		if(Row->IsVisible == false)
			throw gcnew System::InvalidOperationException();
		m_pItem->SetFocused(value);
	}

	bool Cell::IsMouseOvered::get()
	{
		return m_pItem->GetMouseOvered();
	}

	string^ Cell::ToString()
	{
		if(this->Value == nullptr)
			return string::Empty;
		return m_column->TypeConverter->ConvertToString(Value);
	}

	bool Cell::IsReadOnly::get()
	{
		return m_pItem->GetReadOnly();
	}

	void Cell::IsReadOnly::set(bool value)
	{
		m_pItem->SetReadOnly(value);
	}

	bool Cell::IsBeingEdited::get()
	{
		return GridControl->EditingCell == this;
	}

	void Cell::LeaveEdit()
	{
		GridControl->EndEditCell();
	}

	bool Cell::IsDisplayed::get()
	{
		return m_pItem->GetDisplayable();	
	}

	bool Cell::IsSelecting::get()
	{
		return m_pItem->IsItemSelecting();
	}

	string^ Cell::ErrorDescription::get()
	{
		if(m_errorDescription == nullptr)
			return string::Empty;
		return m_errorDescription;
	}

	void Cell::ErrorDescription::set(string^ value)
	{
		m_errorDescription = value;
		if(m_errorDescription == string::Empty)
		{
			GridControl->ErrorDescriptor->Remove(this);
		}
		else
		{
			GridControl->ErrorDescriptor->Add(this);
		}
	}

	object^	Cell::ValueCore::get()
	{
		_PropertyDescriptor^ propertyDescriptor = Column->PropertyDescriptor;
		if(propertyDescriptor == nullptr)
			return m_value;
		object^ value = propertyDescriptor->GetValue(Row->Component);
		return this->Column->ConvertFromSource(value);
	}

	void Cell::ValueCore::set(object^ value)
	{
		_PropertyDescriptor^ propertyDescriptor = Column->PropertyDescriptor;
		if(propertyDescriptor == nullptr)
		{
			m_value = value;
		}
		else if(propertyDescriptor->IsReadOnly == false)
		{
			value = this->Column->ConvertToSource(value);
			propertyDescriptor->SetValue(Row->Component, value);
		}
	}

	_Rectangle Cell::TextBound::get()
	{
		return *m_pItem->GetTextBound();
	}

	bool Cell::ShouldSerializeValue()
	{
		//if(this->Column->PropertyDescriptor != nullptr)
		//	return false;
		
		if(this->ValueCore == nullptr || this->ValueCore->ToString() == "")
			return false;

		return true;
	}

	InsertionCell::InsertionCell(_GridControl^ gridControl, GrItem* pItem, object^ defaultValue)
		: m_value(defaultValue), Cell(gridControl, pItem)
	{
		
	}
	
	object^	InsertionCell::ValueCore::get()
	{
		return m_value;
	}

	void InsertionCell::ValueCore::set(object^ value)
	{
		m_value = value;
	}

	void InsertionCell::SetDefaultValue()
	{
		try
		{
			m_value = Column->DefaultValue;
			UpdateNativeText(m_value);
		}
		catch(System::Exception^)
		{

		}
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/