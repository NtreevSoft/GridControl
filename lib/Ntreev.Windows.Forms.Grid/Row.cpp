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

    Row::Row(RowBuilder^ rowBuilder)
        : m_pDataRow(rowBuilder->NativeRef), RowBase(rowBuilder->GridControl, rowBuilder->NativeRef), m_errorDescription(System::String::Empty)
    {
        m_cells = gcnew CellCollection(this);
        //m_componentIndex = -1;

        for each(Column^ item in this->GridControl->Columns)
        {
            this->NewCell(item);
        }

        m_cellErrorDescriptions = gcnew System::Collections::Generic::Dictionary<Cell^, System::String^>();
    }

    void Row::Component::set(System::Object^ value)
    {
        System::Object^ oldComponent = m_component;
        m_component = value;

        for each(Column^ item in this->GridControl->Columns)
        {
            try
            {
                Cell^ cell = this->NewCell(item);
                if(value != nullptr)
                {
                    cell->LocalValueToSource(value);
                }
                else
                {
                    cell->SourceValueToLocal(oldComponent);
                }
                cell->UpdateNativeText();
            }
            catch(System::Exception^)
            {

            }
        }
    }

    void Row::ProcessChildControl()
    {
        using namespace System::ComponentModel;

        for each(Column^ item in this->GridControl->Columns)
        {
            if(item->PropertyDescriptor != nullptr && item->PropertyDescriptor->PropertyType == IBindingList::typeid)
            {
                Native::GrGridRow* pChildRow = new Native::GrGridRow(this->GridControl, item->PropertyDescriptor, this, this[item]);
                m_pDataRow->AddChild(pChildRow);
                //this->GridControl->InvokeNewChildGridControl(item->PropertyDescriptor, this, this[item]);
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

	System::Collections::Generic::Dictionary<Cell^, System::String^>^ Row::CellErrors::get()
	{
		return m_cellErrorDescriptions;
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
        if(m_cells == nullptr)
        {
            m_cells = gcnew CellCollection(this);
        }
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

            for each(Ntreev::Windows::Forms::Grid::Cell^ cell in m_cells)
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

        this->Refresh();
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

            for each(Ntreev::Windows::Forms::Grid::Cell^ cell in m_cells)
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

        this->Refresh();
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

    System::String^ Row::ErrorDescription::get()
    {
        return m_errorDescription; 
    }

    void Row::ErrorDescription::set(System::String^ value)
    {
		value = value == nullptr ? System::String::Empty : value;

        if(m_errorDescription == value)
            return;

        m_errorDescription = value;
		if(m_errorDescription == System::String::Empty)
        {
			if(m_cellErrorDescriptions->Count == 0)
				this->GridControl->ErrorDescriptor->Remove(this);
        }
        else
        {
            this->GridControl->ErrorDescriptor->Add(this);
        }
    }

    bool Row::HasErrorCell::get()
    {
		return m_cellErrorDescriptions->Count > 0;
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

    void Row::Refresh()
    {
        //return;
        for each(Column^ item in this->GridControl->Columns)
        {
            try
            {
                Cell^ cell = NewCell(item);
                cell->UpdateNativeText();
            }
            catch(System::Exception^)
            {

            }
        }
    }

    System::String^ Row::GetErrorDescription(Cell^ cell)
    {
        if(m_cellErrorDescriptions->ContainsKey(cell) == false)
            return System::String::Empty;

        return m_cellErrorDescriptions[cell];
    }
     
    void Row::SetErrorDescription(Cell^ cell, System::String^ text)
    {
		text = text == nullptr ? System::String::Empty : text;

		if(text == System::String::Empty)
		{
			if(m_cellErrorDescriptions->ContainsKey(cell) == true)
				m_cellErrorDescriptions->Remove(cell);

			if(m_cellErrorDescriptions->Count == 0 && m_errorDescription == System::String::Empty)
				this->GridControl->ErrorDescriptor->Remove(this);
		}
		else
		{
			m_cellErrorDescriptions[cell] = text;
			this->GridControl->ErrorDescriptor->Add(this);
		}
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