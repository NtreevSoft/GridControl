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
#include "GridColumn.h"
#include "GridControl.h"
#include "GridCell.h"
#include "GridRow.h"
#include "GridCollection.h"
#include "GridColumnCollection.h"
#include "GridRowCollection.h"
#include "GridResource.h"
#include "GridAttributes.h"

#include <vcclr.h>

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	class CustomRender
	{
	public:
		CustomRender(gcroot<_Column^> column) : m_column(column)
		{
			
		}

		static bool	ColumnBackgroundPaint(GrGridRenderer* pRenderer, const GrColumn* pColumn, GrRect renderRect, void* dwUserData)
		{
			CustomRender* pCustomRenderer = (CustomRender*)dwUserData;
			return pCustomRenderer->OnColumnBackGroundPaint(pRenderer, pColumn, renderRect);
		}

		static bool	ColumnContentsPaint(GrGridRenderer* pRenderer, const GrColumn* pColumn, GrRect renderRect, void* dwUserData)
		{
			CustomRender* pCustomRenderer = (CustomRender*)dwUserData;
			return pCustomRenderer->OnColumnContentsPaint(pRenderer, pColumn, renderRect);
		}

		bool OnColumnBackGroundPaint(GrGridRenderer* pRenderer, const GrColumn* /*pColumn*/, GrRect renderRect)
		{
			System::IntPtr hdc(pRenderer->GetDC());
			_Graphics^ g = _Graphics::FromHdc(hdc);
			Column^ column = m_column;
			ColumnPainter^ columnPainter = column->ColumnPainter;
			bool b = columnPainter->PaintBackground(g, renderRect, column, column);
			delete g;
			pRenderer->ReleaseDC(hdc.ToPointer());
			return b;
		}

		bool OnColumnContentsPaint(GrGridRenderer* pRenderer, const GrColumn* /*pColumn*/, GrRect renderRect)
		{
			System::IntPtr hdc(pRenderer->GetDC());
			_Graphics^ g = _Graphics::FromHdc(hdc);
			Column^ column = m_column;
			ColumnPainter^ columnPainter = column->ColumnPainter;
			bool b = columnPainter->PaintContents(g, renderRect, column, column);
			delete g;
			pRenderer->ReleaseDC(hdc.ToPointer());
			return b;
		}

	private:
		gcroot<_Column^>			m_column;
	};

	ColumnPainter::ColumnPainter()
	{
		try
		{
			m_sortedDownRenderer = gcnew _VisualStyleRenderer(_VisualStyleElement::Header::SortArrow::SortedDown);
		}
		catch(System::Exception^)
		{
			
		}

		try
		{
			m_sortedUpRenderer   = gcnew _VisualStyleRenderer(_VisualStyleElement::Header::SortArrow::SortedUp);
		}
		catch(System::Exception^)
		{
		
		}
	}

	void ColumnPainter::DrawSortArrow(_Graphics^ g, _Rectangle renderRect, SortType sortType)
	{
		switch(sortType)
		{
		case SortType::Up:
			if(m_sortedUpRenderer)
				m_sortedUpRenderer->DrawBackground(g, renderRect);
			else
				_ControlPaint::DrawScrollButton(g, renderRect, _ScrollButton::Up, _ButtonState::Flat);
			break;
		case SortType::Down:
			if(m_sortedDownRenderer)
				m_sortedDownRenderer->DrawBackground(g, renderRect);
			else
				_ControlPaint::DrawScrollButton(g, renderRect, _ScrollButton::Down, _ButtonState::Flat);
			break;
		}
	}

	bool RowComparerUp(GrGridCore* /*pGridCore*/, const GrDataRow* pDataRow1, const GrDataRow* pDataRow2, const GrColumn* pColumn)
	{
		Cell^ cell1 = Cell::FromNative(pDataRow1->GetItem(pColumn));
		Cell^ cell2 = Cell::FromNative(pDataRow2->GetItem(pColumn));

		object^ value1 = cell1->Value;
		object^ value2 = cell2->Value;

		if(value1 == System::DBNull::Value)
			value1 = nullptr;
		if(value2 == System::DBNull::Value)
			value2 = nullptr;

		if(value1 != nullptr && value2 != nullptr)
		{
			int result;
			System::Collections::IComparer^ comparer = cell1->Column->SortComparer;
			if(comparer != nullptr)
			{
				result = comparer->Compare(value1, value2);
			}
			else
			{
				System::IComparable^ comparable = dynamic_cast<System::IComparable^>(value1);
				if(comparable != nullptr)
					result = comparable->CompareTo(value2);
				else
					result = value1->ToString()->CompareTo(value2->ToString());
			}

			if(result == 0)
				return pDataRow1->GetDataRowIndex() < pDataRow2->GetDataRowIndex();

			return result < 0;
		}
		else if(value1 == nullptr)
		{
			if(value2 == nullptr)
				return pDataRow1->GetDataRowIndex() < pDataRow2->GetDataRowIndex();
			return true;
		}

		return false;
	}

	bool RowComparerDown(GrGridCore* pGridCore, const GrDataRow* pDataRow1, const GrDataRow* pDataRow2, const GrColumn* pColumn)
	{
		return RowComparerUp(pGridCore, pDataRow2, pDataRow1, pColumn);
	}

	Column::Column() : m_pColumn(new GrColumn()), CellBase(m_pColumn)
	{
		m_pColumn->SetSortComparer(GrSort::Up, RowComparerUp);
		m_pColumn->SetSortComparer(GrSort::Down, RowComparerDown);

		m_pCustomRender	= new CustomRender(this);
		m_pColumn->ManagedRef = this;

		switch(this->EditingType)
		{
		case _EditingType::DropDown:
			m_pColumn->SetItemType(GrItemType_DropDown);
			break;
		case _EditingType::Modal:
			m_pColumn->SetItemType(GrItemType_Modal);
			break;
		default:
			m_pColumn->SetItemType(GrItemType_Control);
		}
	}

	Column::~Column()
	{
		if((m_site != nullptr) && (m_site->Container != nullptr))
		{
			m_site->Container->Remove(this);
		}

		this->Disposed_IComponent(this, _EventArgs::Empty);
	}

	string^ Column::ToString()
	{
		return this->ColumnName;
	}

	string^ Column::Title::get()
	{
		if(m_title == nullptr)
			return this->ColumnName;
		return m_title;
	}
	
	void Column::Title::set(string^ value)
	{
		m_title = value;
		AsyncDisplayText();
	}

	string^ Column::ColumnName::get()
	{
		if(m_propertyDescriptor != nullptr)
			return m_propertyDescriptor->Name;

		if(m_name == nullptr)
			return string::Empty;

		return m_name;
	}
	
	void Column::ColumnName::set(string^ value)
	{
		if(value == m_name)
			return;

		if(m_propertyDescriptor != nullptr)
			throw gcnew System::InvalidOperationException("바인딩된 컬럼에는 이름을 설정할 수 없습니다.");

		if(value == nullptr || value == string::Empty)
			throw gcnew System::ArgumentException("빈 문자열의 이름은 설정할 수 없습니다.");

		if(GridControl != nullptr)
		{
			if(GridControl->Columns[value] != nullptr)
				throw gcnew System::ArgumentException(string::Format("\"{0}\"의 이름을 가진 항목이 이미 있습니다.", value));
		}

		m_name = value;

		if(m_title == nullptr)
		{
			this->Title = nullptr;
		}
	}

	string^ Column::Tooltip::get()
	{
		if(m_tooltip == nullptr)
		{
			if(m_propertyDescriptor != nullptr)
				return m_propertyDescriptor->Description;
			return string::Empty;
		}
		return m_tooltip;
	}
	
	void Column::Tooltip::set(string^ value)
	{
		if(value == nullptr)
			value = string::Empty;
		m_tooltip = value;
		m_pColumn->SetTooltip(ToNativeString::Convert(value));
	}

	_PropertyDescriptor^ Column::PropertyDescriptor::get()
	{
		return m_propertyDescriptor;
	}

	void Column::propertyDescriptor_ValueChanged(object^ sender, _EventArgs^ /*e*/)
	{
		AsyncDisplayText();
	}

	void Column::PropertyDescriptor::set(_PropertyDescriptor^ value)
	{
		if(value != nullptr)
		{
			if(value->IsBrowsable == false)
				this->IsVisible = false;
			if(value->IsReadOnly == true)
				this->IsReadOnly = true;
			if(m_tooltip == nullptr)
				this->Tooltip = value->Description;

			if(m_defaultValue == nullptr)
			{
				typedef System::ComponentModel::DefaultValueAttribute _DefaultValue;
				_DefaultValue^ attr = dynamic_cast<_DefaultValue^>(value->Attributes[_DefaultValue::typeid]);
				if(attr != nullptr)
				{
					m_defaultValue = attr->Value;
				}
				else if(DataType->IsEnum == true)
				{
					System::Array^ values = System::Enum::GetValues(DataType);
					if(values->Length > 0)
						m_defaultValue = values->GetValue(0);
				}
				else if(DataType->IsValueType == true)
				{
					m_defaultValue = System::Activator::CreateInstance(DataType);
				}
			}

			FrozenAttribute^ frozenAttr = dynamic_cast<FrozenAttribute^>(value->Attributes[FrozenAttribute::typeid]);
			if(frozenAttr != nullptr)
				this->IsFrozen = frozenAttr->Frozen;

			MovableAttribute^ movableAttr = dynamic_cast<MovableAttribute^>(value->Attributes[MovableAttribute::typeid]);
			if(movableAttr != nullptr)
				this->IsMovable = movableAttr->IsMovable;

			ResizableAttribute^ resizableAttr = dynamic_cast<ResizableAttribute^>(value->Attributes[ResizableAttribute::typeid]);
			if(resizableAttr != nullptr)
				this->IsResizable = resizableAttr->Resizable;

			SortableAttribute^ sortableAttr = dynamic_cast<SortableAttribute^>(value->Attributes[SortableAttribute::typeid]);
			if(sortableAttr != nullptr)
				this->CanBeSorted = sortableAttr->Sortable;
		}
		else
		{

		}
	
		m_propertyDescriptor = value;

		AsyncDisplayText();
	}

	int Column::Width::get()
	{
		return m_pColumn->GetWidth();
	}

	void Column::Width::set(int width)
	{
		if(width < 0)
			throw gcnew System::ArgumentOutOfRangeException("width", "width는 0보다 커야 합니다.");
		
		m_pColumn->SetWidth(width);
		Invalidate(this);
	}

	int Column::MinWidth::get()
	{
		return m_pColumn->GetMinWidth();
	}

	void Column::MinWidth::set(int width)
	{
		if(width < 0)
			throw gcnew System::ArgumentOutOfRangeException("width", "width는 0보다 커야 합니다.");
		if(width > MaxWidth)
			throw gcnew System::ArgumentOutOfRangeException("width", "width가 최대 너비보다 큽니다.");
		m_pColumn->SetMinWidth(width);
	}

	int Column::MaxWidth::get()
	{
		return m_pColumn->GetMaxWidth();
	}

	void Column::MaxWidth::set(int width)
	{
		if(width < 0)
			throw gcnew System::ArgumentOutOfRangeException("width", "width는 0보다 커야 합니다.");
		if(width < MinWidth)
			throw gcnew System::ArgumentOutOfRangeException("width", "width가 최소 너비보다 작습니다.");
		m_pColumn->SetMaxWidth(width);
	}
	
	bool Column::IsVisible::get()
	{
		return m_pColumn->GetVisible();
	}

	void Column::IsVisible::set(bool value)
	{
		m_pColumn->SetVisible(value);
		Invalidate();
	}

	bool Column::IsMovable::get()
	{
		return m_pColumn->GetMovable();
	}

	void Column::IsMovable::set(bool value)
	{
		m_pColumn->SetMovable(value);
	}

	bool Column::IsResizable::get()
	{
		return m_pColumn->GetResizable();
	}

	void Column::IsResizable::set(bool value)
	{
		m_pColumn->SetResizable(value);
	}

	bool Column::IsFrozen::get()
	{
		return m_pColumn->GetFrozen();
	}

	void Column::IsFrozen::set(bool value)
	{
		m_pColumn->SetFrozen(value);
	}

	bool Column::IsReadOnly::get()
	{
		return m_pColumn->GetReadOnly();
	}

	void Column::IsReadOnly::set(bool value)
	{
		if(m_propertyDescriptor != nullptr && m_propertyDescriptor->IsReadOnly == true)
		{
			throw gcnew System::InvalidOperationException("읽기 전용으로 바인딩된 컬럼에는 설정할 수 없습니다.");
		}
		m_pColumn->SetReadOnly(value);
	}

	bool Column::CanBeSorted::get()
	{
		return m_pColumn->CanBeSort();
	}

	void Column::CanBeSorted::set(bool value)
	{
		m_pColumn->EnableSort(value);
	}

	_Type^ Column::DataType::get()
	{
		if(m_propertyDescriptor != nullptr)
			return m_propertyDescriptor->PropertyType;

		if(m_dataType == nullptr)
			return string::typeid;
		return m_dataType;
	}
	
	void Column::DataType::set(_Type^ value)
	{
		if(m_propertyDescriptor != nullptr)
			throw gcnew System::InvalidOperationException("바인딩된 컬럼에는 데이터 타입을 설정할 수 없습니다.");

		if(value == nullptr)
			throw gcnew System::ArgumentNullException("value");

		m_dataType = value;
	}

	Column::_TypeConverter^ Column::TypeConverter::get()
	{
		if(m_typeConverter == nullptr)
		{
			if(m_propertyDescriptor != nullptr)
				return m_propertyDescriptor->Converter;

			try
			{
				return System::ComponentModel::TypeDescriptor::GetConverter(this->DataType);
			}
			catch(System::ArgumentNullException^ e)
			{
				throw gcnew System::ArgumentNullException("DataType이 설정되지 않았습니다.", e);
			}
		}

		return m_typeConverter;
	}

	void Column::TypeConverter::set(_TypeConverter^ converter)
	{
		if(converter == nullptr)
			throw gcnew System::ArgumentNullException("converter");
		m_typeConverter = converter;
	}

	_SortType Column::SortType::get()
	{
		return (_SortType)m_pColumn->GetSortType();	
	}

	void Column::SortType::set(_SortType value)
	{
		m_pColumn->SetSortType((GrSort::Type)value);
		Invalidate();
	}

	System::Collections::IComparer^ Column::SortComparer::get()
	{
		return m_sortComparer;
	}

	void Column::SortComparer::set(System::Collections::IComparer^ value)
	{
		m_sortComparer = value;
	}

	int Column::VisibleIndex::get()
	{
		return (int)m_pColumn->GetVisibleIndex();
	}

	int Column::DisplayIndex::get()
	{
		return (int)m_pColumn->GetDisplayIndex();	
	}

	int Column::Index::get()
	{
		return (int)m_pColumn->GetIndex();
	}

	object^ Column::DefaultValue::get()
	{
		if(m_defaultValue == nullptr)
			return nullptr;

		if(m_defaultValue->GetType() == this->DataType)
			return m_defaultValue;

		if(this->TypeConverter->CanConvertFrom(m_defaultValue->GetType()) == false)
			return nullptr;
		return this->TypeConverter->ConvertFrom(m_defaultValue);
	}
	
	void Column::DefaultValue::set(object^ value)
	{
		m_defaultValue = value;
	}

	_EditingResult Column::DefaultEditingResult::get()
	{
		return _EditingResult::Ok; 
	}

	_StringAlignment Column::CellAlignment::get()
	{
		GrHorzAlign align = m_pColumn->GetItemHorzAlign();
		return (_StringAlignment)align;
	}
	
	void Column::CellAlignment::set(_StringAlignment value)
	{
		m_pColumn->SetItemHorzAlign((GrHorzAlign)value);
		Invalidate();
	}

	_StringAlignment Column::CellLineAlignment::get()
	{
		GrVertAlign align = m_pColumn->GetItemVertAlign();
		return (_StringAlignment)align;
	}
	
	void Column::CellLineAlignment::set(_StringAlignment value)
	{
		m_pColumn->SetItemVertAlign((GrVertAlign)value);
		Invalidate();
	}

	bool Column::IsSelected::get()
	{
		return m_pColumn->GetSelected();
	}
	
	void Column::IsSelected::set(bool value)
	{
		m_pColumn->SetSelected(value);
	}

	bool Column::IsFullSelected::get()
	{
		return m_pColumn->GetFullSelected();	
	}

	bool Column::IsGrouped::get()
	{
		return m_pColumn->GetGrouped();
	}

	void Column::IsGrouped::set(bool value)
	{
		m_pColumn->SetGrouped(value);
	}

	bool Column::CanBeGrouped::get()
	{
		return m_pColumn->CanBeGrouped();
	}

	void Column::CanBeGrouped::set(bool value)
	{
		m_pColumn->EnableGrouping(value);
	}

	Column::_ColumnPainter^ Column::ColumnPainter::get()
	{
		return m_columnPainter;
	}

	bool Column::CellWordWrap::get()
	{
		return m_pColumn->GetItemWordWrap();
	}

	void Column::CellWordWrap::set(bool value)
	{
		m_pColumn->SetItemWordWrap(value);
	}

	bool Column::CellMultiline::get()
	{
		return m_pColumn->GetItemMultiline();
	}

	void Column::CellMultiline::set(bool value)
	{
		m_pColumn->SetItemMultiline(value);
	}

	_ColumnState  Column::ColumnState::get()
	{
		if(m_pColumn->GetMousePressed() == true)
			return _ColumnState::Pressed;
		else if(m_pColumn->GetMouseOvered() == true)
			return _ColumnState::Hot;
		return _ColumnState::Normal;
	}

	void Column::ColumnPainter::set(_ColumnPainter^ columnPainter)
	{
		m_columnPainter = columnPainter;
		if(m_columnPainter)
		{
			m_pColumn->m_fnColumnBackgroundRender	= CustomRender::ColumnBackgroundPaint;
			m_pColumn->m_fnColumnContentsRender		= CustomRender::ColumnContentsPaint;
			m_pColumn->m_pColumnRenderData			= (void*)m_pCustomRender;
		}
		else
		{
			m_pColumn->m_fnColumnBackgroundRender	= NULL;
			m_pColumn->m_fnColumnContentsRender		= NULL;
			m_pColumn->m_pColumnRenderData			= NULL;
		}
	}

	_Color Column::CellForeColor::get()
	{
		return m_pColumn->GetItemForeColor();
	}

	void Column::CellForeColor::set(_Color value)
	{
		m_pColumn->SetItemForeColor(value);
	}

	_Color Column::CellBackColor::get()
	{
		return m_pColumn->GetItemBackColor();
	}

	void Column::CellBackColor::set(_Color value)
	{
		m_pColumn->SetItemBackColor(value);
	}

	_Padding Column::CellPadding::get()
	{
		return m_pColumn->GetItemPadding();
	}

	void Column::CellPadding::set(_Padding value)
	{
		m_pColumn->SetItemPadding(value);
	}

	_Font^ Column::CellFont::get()
	{
		GrFont* pFont = m_pColumn->GetItemFont();
		if(pFont == nullptr)
			return nullptr;

		return GrFontManager::ToManagedFont(pFont);
	}

	void Column::CellFont::set(_Font^ value)
	{
		if(value == nullptr)
			m_pColumn->SetItemFont(nullptr);
		else
			m_pColumn->SetItemFont(GrFontManager::GetFontDesc(value->ToHfont().ToPointer()));
	}

	void Column::RefreshAll()
	{
		for each(Row^ item in GridControl->Rows)
		{
			Cell^ cell = item[this];
			cell->UpdateNativeText();
		}
	}

	void Column::ResetCellBackColor()
	{
		this->CellBackColor = _Color::Empty;
	}

	void Column::ResetCellPadding()
	{
		this->CellPadding = GrPadding::Default;
	}

	void Column::ResetCellForeColor()
	{
		this->CellForeColor = _Color::Empty;
	}

	void Column::ResetCellFont()
	{
		m_pColumn->SetItemFont(nullptr);
	}

	Column^ Column::FromNative(const GrColumn* pColumn)
	{
		object^ ref = pColumn->ManagedRef;
		return safe_cast<Column^>(ref);
	}
	
	_EditingResult Column::EditingResult::get()
	{
		return m_editingResult;
	}

	void Column::EditingResult::set(_EditingResult result)
	{
		m_editingResult = result;
		ColumnEventArgs e(this);
		OnEditingResultChanged(this, %e);
	}

	void Column::OnEditingResultChanged(object^ sender, ColumnEventArgs^ e)
	{
		EditingResultChanged(sender, e);
	}

	void Column::OnEditValue(EditValueEventArgs^ e)
	{
		EditValue(this, e);
	}

	uint Column::ColumnID::get()
	{
		return m_pColumn->GetColumnID();	
	}

	void Column::ValidateDefaultValue()
	{
		if(m_dataType == nullptr || m_defaultValue == nullptr)
			return;
	}

	bool Column::ShouldSerializeCellForeColor()
	{
		return m_pColumn->GetItemForeColor() != GrColor::Empty;
	}

	bool Column::ShouldSerializeCellBackColor()
	{
		return m_pColumn->GetItemBackColor() != GrColor::Empty;
	}

	bool Column::ShouldSerializeCellPadding()
	{
		return m_pColumn->GetItemPadding() != GrPadding::Default;
	}
	
	bool Column::ShouldSerializeCellFont()
	{
		return m_pColumn->GetItemFont() != nullptr;
	}

	bool Column::ShouldSerializeTitle()
	{
		return this->Title != this->ColumnName;
	}

	void Column::AsyncDisplayText()
	{
		m_pColumn->SetText(ToNativeString::Convert(this->Title));
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/