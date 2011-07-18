#include "StdAfx.h"
#include "GridCell.h"
#include "GridDebug.h"
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
		
		try
		{
			object^ value = ValueCore;
			string^ text = "";
			if(value != nullptr || value != System::DBNull::Value)
				text = m_column->TypeConverter->ConvertToString(value);
			pItem->SetText(ToNativeString::Convert(text));
		}
		catch(System::Exception^ e)
		{
			Debug::WriteLine(e);
			pItem->SetText(L"");
		}
	}

	//_Color Cell::ForeColor::get()
	//{
	//	return m_pItem->GetForeColor();
	//}

	//_Color Cell::BackColor::get()
	//{
	//	return m_pItem->GetBackColor();
	//}

	//_Font^ Cell::Font::get()
	//{
	//	GrFont* pFont = m_pItem->GetRenderingFont();
	//	return GrFontManager::ToManagedFont(pFont);
	//	//System::IntPtr fontPtr(m_pItem->GetRenderingFont()->GetFont());
	//	//return _Font::FromHfont(fontPtr);
	//}

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

		if(value != nullptr && value->GetType() != this->Column->DataType)
		{
			if(this->Column->DataType == string::typeid)
			{
				value = value->ToString();
			}
			else
			{
				System::ComponentModel::TypeConverter^ typeConverter = Column->TypeConverter;
				if(typeConverter->CanConvertFrom(value->GetType()) == false)
					throw gcnew System::ArgumentException("타입이 잘못되었습니다");
				value = typeConverter->ConvertFrom(value);
			}
		}

		if(GridControl->InvokeValueChanging(this, value, oldValue) == false)
			return;

		if(this->Row->IsEditing == true)
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
		UpdateNativeText(Value);
	}

	void Cell::UpdateNativeText(object^ value)
	{
		string^ text = m_column->TypeConverter->ConvertToString(value);
		m_pItem->SetText(ToNativeString::Convert(text));
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
		return propertyDescriptor->GetValue(Row->Component);
	}

	void Cell::ValueCore::set(object^ value)
	{
		_PropertyDescriptor^ propertyDescriptor = Column->PropertyDescriptor;
		if(propertyDescriptor == nullptr)
			m_value = value;
		else
			propertyDescriptor->SetValue(Row->Component, value);
	}

	_Rectangle Cell::TextBound::get()
	{
		return *m_pItem->GetTextBound();
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
		catch(System::Exception^ e)
		{
			Debug::WriteLine(e->Message);
			return;
		}
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/