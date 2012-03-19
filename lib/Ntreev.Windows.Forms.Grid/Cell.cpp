//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4461.30274
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
#include "Cell.h"
#include "Column.h"
#include "Row.h"
#include "GridControl.h"
#include "FromNative.h"
#include "ErrorDescriptor.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    ref class TypeDescriptorContextCore : System::ComponentModel::ITypeDescriptorContext
    {
    public:
        TypeDescriptorContextCore(Ntreev::Windows::Forms::Grid::Cell^ cell)
            : m_column(cell->Column), m_row(cell->Row)
        {

        }

        virtual System::Object^ GetService(System::Type^ serviceType)
        {
            System::IServiceProvider^ serviceProvider = dynamic_cast<System::IServiceProvider^>(m_column);
            if(serviceProvider != nullptr)
            {
                return serviceProvider->GetService(serviceType);
            }

            return nullptr;
        }

        virtual void OnComponentChanged()
        {

        }

        virtual bool OnComponentChanging()
        {
            return false;
        }

    public:
        property System::ComponentModel::IContainer^ Container
        {
            virtual System::ComponentModel::IContainer^ get()
            {
                System::ComponentModel::ISite^ site = dynamic_cast<System::ComponentModel::ISite^>(m_column);
                if(site != nullptr)
                {
                    return site->Container;
                }

                return nullptr;
            }
        }

        property System::Object^ Instance
        {
            virtual System::Object^ get()
            {
                return m_row;
            }
        }

        property System::ComponentModel::PropertyDescriptor^ PropertyDescriptor
        {
            virtual System::ComponentModel::PropertyDescriptor^ get()
            {
                return m_column->PropertyDescriptor;
            }
        }

    private:
        Ntreev::Windows::Forms::Grid::Column^ m_column;
        Ntreev::Windows::Forms::Grid::Row^ m_row;
    };

    Cell::Cell(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, GrItem* pItem)
        : CellBase(gridControl, pItem), m_pItem(pItem), m_errorDescription(System::String::Empty)
    {
        m_column = FromNative::Get(pItem->GetColumn());
        m_row = FromNative::Get(pItem->GetDataRow());

        m_pItem->ManagedRef = this;

        UpdateNativeText();
    }

    Ntreev::Windows::Forms::Grid::Column^ Cell::Column::get()
    {
        return m_column;
    }

    unsigned int Cell::ColumnID::get()
    {
        return m_column->ColumnID;
    }

    Ntreev::Windows::Forms::Grid::Row^ Cell::Row::get()
    {
        return m_row;
    }

    unsigned int Cell::RowID::get()
    {
        return m_row->RowID;
    }

    System::Object^ Cell::Value::get()
    {
        return ValueCore;
    }

    void Cell::Value::set(System::Object^ value)
    {
        System::Object^ oldValue = this->Value;

        if(System::Object::Equals(value, oldValue) == true)
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

        this->GridControl->InvokeValueChanged(this);
    }

    void Cell::UpdateNativeText()
    {
        UpdateNativeText(this->ValueCore);
    }

    void Cell::UpdateNativeText(System::Object^ value)
    {
        try
        {
            TypeDescriptorContextCore^ typeDescriptorContext = gcnew TypeDescriptorContextCore(this);
            System::String^ text = System::String::Empty;
            if(value != nullptr && value != System::DBNull::Value)
                text = m_column->TypeConverter->ConvertToString(typeDescriptorContext, value);
            m_pItem->SetText(ToNativeString::Convert(text));
        }
        catch(System::Exception^)
        {
            m_pItem->SetText(L"");
        }
    }

    System::Object^ Cell::ValidateValue(System::Object^ value)
    {
        if(value == nullptr || value == System::DBNull::Value)
            return value;

        System::Type^ dataType = this->Column->DataType;
        System::Type^ valueType = value->GetType();

        if(dataType == valueType)
            return value;

        if(dataType == System::String::typeid)
            return value->ToString();

        System::ComponentModel::TypeConverter^ typeConverter = this->Column->TypeConverter;
        TypeDescriptorContextCore^ typeDescriptorContext = gcnew TypeDescriptorContextCore(this);
        if(typeConverter->CanConvertFrom(typeDescriptorContext, valueType) == false)
        {
            System::String^ reason = System::String::Format("{0}형식을 {1}으로 변환할 수 없습니다.", valueType, dataType);
            throw gcnew System::ArgumentException(reason);
        }

        return typeConverter->ConvertFrom(typeDescriptorContext, System::Windows::Forms::Application::CurrentCulture, value);
    }

    //Ntreev::Windows::Forms::Grid::Cell^ FromNative::Get(GrItem* pItem)
    //{
    //    System::Object^ ref = pItem->ManagedRef;
    //    return safe_cast<Ntreev::Windows::Forms::Grid::Cell^>(ref);
    //}

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

    void Cell::Select(Ntreev::Windows::Forms::Grid::SelectionType selectionType)
    {
        Selector->SelectItem(m_pItem, (GrSelectionType)selectionType);
    }

    void Cell::Focus()
    {
        Focuser->Set(m_pItem);
    }

    void Cell::BringIntoView()
    {
        this->GridControl->BringIntoView(this);
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

    System::String^ Cell::ToString()
    {
        if(this->Value == nullptr)
            return System::String::Empty;
        TypeDescriptorContextCore^ typeDescriptorContext = gcnew TypeDescriptorContextCore(this);
        return m_column->TypeConverter->ConvertToString(typeDescriptorContext, Value);
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
        return this->GridControl->EditingCell == this;
    }

    //bool Cell::IsDisplayable::get()
    //{
    // return m_pItem->GetDisplayable(); 
    //}

    bool Cell::IsSelecting::get()
    {
        return m_pItem->IsItemSelecting();
    }

    System::String^ Cell::ErrorDescription::get()
    {
        return m_errorDescription;
    }

    void Cell::ErrorDescription::set(System::String^ value)
    {
        if(value == nullptr)
            value = System::String::Empty;

        if(m_errorDescription == value)
            return;

        m_errorDescription = value;
        if(m_errorDescription == System::String::Empty)
        {
            this->GridControl->ErrorDescriptor->Remove(this);
            this->Row->RemoveErrorCell();
        }
        else
        {
            this->GridControl->ErrorDescriptor->Add(this);
            this->Row->AddErrorCell();
        }
    }

    System::Object^ Cell::ValueCore::get()
    {
        System::ComponentModel::PropertyDescriptor^ propertyDescriptor = Column->PropertyDescriptor;
        if(propertyDescriptor == nullptr)
            return m_value;
        System::Object^ value = propertyDescriptor->GetValue(Row->Component);
        return this->Column->ConvertFromSource(value);
    }

    void Cell::ValueCore::set(System::Object^ value)
    {
        System::ComponentModel::PropertyDescriptor^ propertyDescriptor = Column->PropertyDescriptor;
        if(propertyDescriptor == nullptr)
        {
            m_value = value;
        }
        else if(propertyDescriptor->IsReadOnly == false)
        {
            value = this->Column->ConvertToSource(value);
            try
            {
                propertyDescriptor->SetValue(Row->Component, value);
            }
            catch(System::Exception^)
            {
                propertyDescriptor->SetValue(Row->Component, System::DBNull::Value);
            }
        }
    }

    System::Drawing::Rectangle Cell::TextBound::get()
    {
        return m_pItem->GetTextBounds();
    }

    bool Cell::ShouldSerializeValue()
    {
        //if(this->Column->PropertyDescriptor != nullptr)
        // return false;

        if(this->ValueCore == nullptr || this->ValueCore->ToString() == "")
            return false;

        return true;
    }

    System::Object^ Cell::Value_ICell::get()
    {
        return this->Value;
    }

    System::Object^ Cell::Tag_ICell::get()
    {
        return this->Tag;
    }

    InsertionCell::InsertionCell(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, GrItem* pItem, System::Object^ defaultValue)
        : m_value(defaultValue), Cell(gridControl, pItem)
    {

    }

    System::Object^ InsertionCell::ValueCore::get()
    {
        return m_value;
    }

    void InsertionCell::ValueCore::set(System::Object^ value)
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