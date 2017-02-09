//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5792.31442
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
#include "Events.h"
#include "Cell.h"
#include "Row.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    using namespace System::Drawing;
    using namespace System::Windows::Forms;

    CellEventArgs::CellEventArgs(_Cell^ cell) 
        : m_cell(cell) 
    { 

    }

    _Cell^ CellEventArgs::Cell::get()
    {
        return m_cell;
    }

    ValueChangingEventArgs::ValueChangingEventArgs(_Cell^ cell, System::Object^ newValue, System::Object^ oldValue) 
        : CellEventArgs(cell), m_cancel(false), m_newValue(newValue), m_oldValue(oldValue)
    {

    }

    bool ValueChangingEventArgs::Cancel::get()
    {
        return m_cancel; 
    } 

    void ValueChangingEventArgs::Cancel::set(bool cancel)
    {
        m_cancel = cancel; 
    }

    System::Object^ ValueChangingEventArgs::NewValue::get()
    {
        return m_newValue; 
    }

    System::Object^ ValueChangingEventArgs::OldValue::get()
    {
        return m_oldValue; 
    }

    RowEventArgs::RowEventArgs(_Row^ row)
        : m_row(row)
    {

    }

    _Row^ RowEventArgs::Row::get()
    {
        return m_row; 
    }

    void RowEventArgs::Row::set(_Row^ value) 
    {
        m_row = value; 
    }

    RowEventArgs::RowEventArgs()
    {

    }

	RowMovingEventArgs::RowMovingEventArgs(_Row^ row, int index)
        : RowEventArgs(row), m_component(row->Component), m_cancel(false), m_index(index)
    {

    }

    void RowMovingEventArgs::Cancel::set(bool value)
    {
        m_cancel = value; 
    }

    bool RowMovingEventArgs::Cancel::get()
    {
        return m_cancel;
    }

	int RowMovingEventArgs::Index::get()
    {
        return m_index;
    }

    System::Object^ RowMovingEventArgs::Component::get()
    {
        return m_component; 
    } 

    void RowMovingEventArgs::Component::set(System::Object^ value)
    {
        m_component = value; m_cancel = false; 
    }

    RowMovingEventArgs::RowMovingEventArgs()
    {

    }

    RowInsertingEventArgs::RowInsertingEventArgs(_Row^ row)
        : RowEventArgs(row), m_component(row->Component), m_cancel(false)
    {

    }

    void RowInsertingEventArgs::Cancel::set(bool value)
    {
        m_cancel = value; 
    }

    bool RowInsertingEventArgs::Cancel::get()
    {
        return m_cancel;
    }

    System::Object^ RowInsertingEventArgs::Component::get()
    {
        return m_component; 
    } 

    void RowInsertingEventArgs::Component::set(System::Object^ value)
    {
        m_component = value; m_cancel = false; 
    }

    RowInsertingEventArgs::RowInsertingEventArgs()
    {

    }

    RowInsertedEventArgs::RowInsertedEventArgs(_Row^ row)
        : m_row(row)
    {

    }

    _Row^ RowInsertedEventArgs::Row::get()
    {
        return m_row; 
    } 

    System::Object^ RowInsertedEventArgs::Component::get()
    {
        return m_row->Component;
    }

    RowRemovingEventArgs::RowRemovingEventArgs(_Row^ row) 
        : RowEventArgs(row), m_cancel(false)
    {

    }

    void RowRemovingEventArgs::Cancel::set(bool value)
    {
        m_cancel = value;
    } 

    bool RowRemovingEventArgs::Cancel::get()
    {
        return m_cancel;
    }

    RowRemovedEventArgs::RowRemovedEventArgs(unsigned int nNewIndex)
        : m_newSelect(false), m_newIndex(nNewIndex) 
    { 

    }

    void RowRemovedEventArgs::NewSelect::set(bool value)
    {
        m_newSelect = value;
    }

    void RowRemovedEventArgs::NewSelectionIndex::set(unsigned int value)
    {
        m_newIndex = value;
    }

    RowBindingEventArgs::RowBindingEventArgs(System::Object^ component)
        : m_component(component)
    {

    }

    System::Object^ RowBindingEventArgs::Component::get()
    {
        return m_component;
    }

    bool RowBindingEventArgs::Cancel::get()
    {
        return m_cancel;
    }
    
    void RowBindingEventArgs::Cancel::set(bool value)
    {
        m_cancel = value;
    }
    
    ColumnBindingEventArgs::ColumnBindingEventArgs(System::ComponentModel::PropertyDescriptor^ propertyDescriptor, _Column^ column)
        : m_propertyDescriptor(propertyDescriptor), m_column(column), m_cancel(false)
    {

    }

    System::ComponentModel::PropertyDescriptor^ ColumnBindingEventArgs::PropertyDescriptor::get()
    {
        return m_propertyDescriptor;
    }

    _Column^ ColumnBindingEventArgs::BindingColumn::get()
    {
        return m_column;
    }

    void ColumnBindingEventArgs::BindingColumn::set(_Column^ value)
    {
        m_column = value;
    }

	bool ColumnBindingEventArgs::Cancel::get()
    {
        return m_cancel;
    }
    
    void ColumnBindingEventArgs::Cancel::set(bool value)
    {
        m_cancel = value;
    }

    ColumnEventArgs::ColumnEventArgs(_Column^ column)
        : m_column(column)
    {
    }

    _Column^ ColumnEventArgs::Column::get()
    {
        return m_column; 
    } 

    ColumnInsertingEventArgs::ColumnInsertingEventArgs(_Column^ column)
        : ColumnEventArgs(column), m_cancel(false)
    {

    }

    void ColumnInsertingEventArgs::Cancel::set(bool value)
    {
        m_cancel = value;
    } 

    bool ColumnInsertingEventArgs::Cancel::get()
    {
        return m_cancel;
    }

    ColumnMouseEventArgs::ColumnMouseEventArgs(_Column^ column, Point location)
        : ColumnEventArgs(column), m_location(location), m_handled(false)
    {

    }

    int ColumnMouseEventArgs::X::get()
    {
        return m_location.X;
    } 

    int ColumnMouseEventArgs::Y::get()
    {
        return m_location.Y;
    }

    System::Drawing::Point ColumnMouseEventArgs::Location::get()
    {
        return m_location;
    }

    bool ColumnMouseEventArgs::Handled::get()
    {
        return m_handled;
    }

    void ColumnMouseEventArgs::Handled::set(bool value)
    {
        m_handled = value;
    } 

    CellMouseEventArgs::CellMouseEventArgs(_Cell^ cell, Point location)
        : CellEventArgs(cell), m_location(location), m_handled(false)
    {

    }

    int CellMouseEventArgs::X::get()
    {
        return m_location.X;
    } 

    int CellMouseEventArgs::Y::get()
    {
        return m_location.Y;
    }

    System::Drawing::Point CellMouseEventArgs::Location::get()
    {
        return m_location;
    }

    bool CellMouseEventArgs::Handled::get()
    {
        return m_handled;
    }

    void CellMouseEventArgs::Handled::set(bool value)
    {
        m_handled = value;
    } 

    EditBegunEventArgs::EditBegunEventArgs(_Cell^ cell)
        : m_cancel(false), CellEventArgs(cell)
    {

    }

    bool EditBegunEventArgs::Cancel::get()
    {
        return m_cancel;
    }

    void EditBegunEventArgs::Cancel::set(bool value)
    {
        m_cancel = value;
    } 

    ClearEventArgs::ClearEventArgs(bool dataSourceOnly)
        : m_dataSourceOnly(dataSourceOnly)
    {

    }

    bool ClearEventArgs::DataSourceOnly::get()
    {
        return m_dataSourceOnly;
    }

    CurrencyManagerChangedEventArgs::CurrencyManagerChangedEventArgs(CurrencyManager^ currencyManager)
        : m_manager(currencyManager)
    {

    }

    CurrencyManager^ CurrencyManagerChangedEventArgs::CurrecnyManager::get()
    {
        return m_manager;
    }

    CurrencyManagerChangingEventArgs::CurrencyManagerChangingEventArgs(CurrencyManager^ currencyManager)
        : m_cancel(false), CurrencyManagerChangedEventArgs(currencyManager)
    {
        this->CancelReason = System::String::Empty;
    }

    bool CurrencyManagerChangingEventArgs::Cancel::get()
    {
        return m_cancel;
    }

    void CurrencyManagerChangingEventArgs::Cancel::set(bool value)
    {
        m_cancel = value;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/