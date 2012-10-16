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
#include "Row.h"
#include "GridControl.h"
#include "Cell.h"
#include "Column.h"
#include "ColumnCollection.h"
#include "ErrorDescriptor.h"
#include "FromNative.h"
#include "RowBuilder.h"
#include "NativeGridRow.h"

#include "GrGridCell.h"
#include "GrGridCore.h"
#include "GrGridPainter.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    //Row::Row(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, GrDataRow* pDataRow) 
    //    : m_pDataRow(pDataRow), RowBase(gridControl, m_pDataRow), m_errorDescription(System::String::Empty)

    Row::Row(Ntreev::Windows::Forms::Grid::RowBuilder^ rowBuilder)
        : m_pDataRow(rowBuilder->NativeRef), RowBase(rowBuilder->GridControl, rowBuilder->NativeRef), m_errorDescription(System::String::Empty)
    {
        m_cells = gcnew Ntreev::Windows::Forms::Grid::CellCollection(this);
        m_componentIndex = -1;

        for each(Ntreev::Windows::Forms::Grid::Column^ item in this->GridControl->Columns)
        {
            NewCell(item);
        }
    }

    void Row::Component::set(System::Object^ value)
    {
        m_component = value;

        //if(IsBeingEdited == true)
        //{
        //    EndEdit();
        //}

        for each(Ntreev::Windows::Forms::Grid::Column^ item in this->GridControl->Columns)
        {
            try
            {
                Ntreev::Windows::Forms::Grid::Cell^ cell = NewCell(item);
                cell->UpdateNativeText();

                if(item->PropertyDescriptor->PropertyType == System::ComponentModel::IBindingList::typeid)
                {
                    Native::GrGridRow* pGridRow = new Native::GrGridRow(this->GridControl);
                    m_pDataRow->AddChild(pGridRow);
                    pGridRow->SetDataSource(cell->Value);
                }
            }
            catch(System::Exception^)
            {

            }

            
        }
    }

    GrDataRow* Row::NativeRef::get()
    {
        return m_pDataRow;
    }

    Ntreev::Windows::Forms::Grid::Cell^ Row::NewCell(Ntreev::Windows::Forms::Grid::Column^ column)
    {
        GrItem* pItem = m_pDataRow->GetItem(column->NativeRef);

        Ntreev::Windows::Forms::Grid::Cell^ cell = FromNative::Get(pItem);
        if(cell == nullptr)
            cell = gcnew Ntreev::Windows::Forms::Grid::Cell(this->GridControl, pItem);

        return cell;
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
        return m_cells->Count;
    }

    unsigned int Row::RowID::get()
    {
        return m_pDataRow->GetDataRowID(); 
    }

    System::Object^ Row::default::get(int index)
    {
        return m_cells[index]->Value;
    }

    void Row::default::set(int index, System::Object^ value)
    {
        m_cells[index]->Value = value;
    }

    System::Object^ Row::default::get(System::String^ columnName)
    {
        return m_cells[columnName]->Value;
    }

    void Row::default::set(System::String^ columnName, System::Object^ value)
    {
        m_cells[columnName]->Value = value;
    }

    System::Object^ Row::default::get(Ntreev::Windows::Forms::Grid::Column^ column)
    {
        return m_cells[column]->Value;
    }

    void Row::default::set(Ntreev::Windows::Forms::Grid::Column^ column, System::Object^ value)
    {
        m_cells[column]->Value = value;
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

    void Row::AddErrorCell()
    {
        m_errorCell++;
    }

    void Row::RemoveErrorCell()
    {
        m_errorCell--;
        if(m_errorCell < 0)
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
                cell->CancelEdit();
            }

            if(m_editedCount < 0)
                throw gcnew System::Exception("먼가 수상합니다.");
        }
        finally
        {
            m_editedCount = 0;
            m_editing = false;
        }
    }

    void Row::EndEdit()
    {
        try
        {
            if(m_editedCount == 0)
                return;

            for each(Ntreev::Windows::Forms::Grid::Cell^ cell in m_cells)
            {
                cell->EndEdit();
            }

            if(m_editedCount < 0)
                throw gcnew System::Exception("먼가 수상합니다.");
        }
        finally
        {
            m_editedCount = 0;
            m_editing = false;
        }
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
        CellBackColor = System::Drawing::Color::Empty;
    }

    void Row::ResetCellForeColor()
    {
        CellForeColor = System::Drawing::Color::Empty;
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
        if(value == nullptr)
            value = System::String::Empty;

        if(m_errorDescription == value)
            return;

        m_errorDescription = value;
        if(m_errorDescription == System::String::Empty)
        {
            this->GridControl->ErrorDescriptor->Remove(this);
        }
        else
        {
            this->GridControl->ErrorDescriptor->Add(this);
        }
    }

    bool Row::HasErrorCell::get()
    {
        return m_errorCell > 0;
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

    //void Row::SetDefaultValue(System::Windows::Forms::CurrencyManager^ manager)
    //{
    //    try
    //    {
    //        manager->AddNew();

    //        for each(Ntreev::Windows::Forms::Grid::Cell^ cell in this->Cells)
    //        {
    //            Ntreev::Windows::Forms::Grid::Column^ column = cell->Column;
    //            if(column->PropertyDescriptor == nullptr || column->DefaultValue != nullptr)
    //            {
    //                cell->ValueCore = column->DefaultValue;
    //            }
    //            else
    //            {
    //                cell->ValueCore = column->PropertyDescriptor->GetValue(manager->Current);
    //            }
    //            cell->UpdateNativeText();
    //        }
    //    }
    //    catch(System::Exception^)
    //    {

    //    }
    //    finally
    //    {
    //        manager->CancelCurrentEdit();
    //    }
    //}

    void Row::ValueToSource(System::Object^ component)
    {
        for each(Ntreev::Windows::Forms::Grid::Cell^ cell in this->Cells)
        {
            Ntreev::Windows::Forms::Grid::Column^ column = cell->Column;
            System::ComponentModel::PropertyDescriptor^ propertyDescriptor = column->PropertyDescriptor;
            if(propertyDescriptor == nullptr)
                continue;

            System::Object^ value = column->ConvertToSource(cell->Value);
            try
            {
                propertyDescriptor->SetValue(component, value);
            }
            catch(System::Exception^ /*e*/)
            {
                propertyDescriptor->SetValue(component, System::DBNull::Value);
            }
        }
    }

    int Row::GetCellsTextCapacity()
    {
        return m_textCapacity;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/