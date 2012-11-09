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
#include "Cell.h"
#include "Column.h"
#include "Row.h"
#include "GridControl.h"
#include "FromNative.h"
#include "ErrorDescriptor.h"
#include "CellBuilder.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    ref class TypeDescriptorContextCore
        : System::ComponentModel::ITypeDescriptorContext
    {
    public: // methods

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

    public: // properties

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

    private: // variables

        Column^ m_column;
        Ntreev::Windows::Forms::Grid::Row^ m_row;
    };

    Cell::Cell(CellBuilder^ builder)
        : CellBase(builder->GridControl, builder->NativeRef), m_pItem(builder->NativeRef), m_errorDescription(System::String::Empty)
    {
        m_column = FromNative::Get(m_pItem->GetColumn());
        m_row = FromNative::Get(m_pItem->GetDataRow());

        m_pItem->ManagedRef = this;
        m_value = nullptr;
        m_oldValue = Cell::NullValue;
        m_displayValue = Cell::NullValue;
    }

    Cell::Cell(_GridControl^ gridControl, GrItem* pItem)
        : CellBase(gridControl, pItem), m_pItem(pItem), m_errorDescription(System::String::Empty)
    {
        m_column = FromNative::Get(pItem->GetColumn());
        m_row = FromNative::Get(pItem->GetDataRow());

        m_pItem->ManagedRef = this;
        m_value = nullptr;
        m_oldValue = Cell::NullValue;
        m_displayValue = Cell::NullValue;
    }

    Column^ Cell::Column::get()
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
        return this->ValueCore;
    }

    void Cell::Value::set(System::Object^ value)
    {
        System::Object^ oldValue = this->Value;

        if(System::Object::Equals(value, oldValue) == true)
            return;

        value = ValidateValue(value);

        if(this->GridControl->InvokeValueChanging(this, value, oldValue) == false)
            return;

        this->ValueCore = value;

        if(m_row->IsBeingEdited == true)
        {
            if(m_oldValue == Cell::NullValue)
            {
                m_row->AddEditedCell();
                m_oldValue = oldValue;
            }
        }

        this->GridControl->InvokeValueChanged(this);
        this->Row->Refresh();
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
            if(m_wrongValue == true)
            {
                System::Object^ sourceValue = this->SourceValue;
                if(sourceValue != nullptr)
                    text = sourceValue->ToString();
            }
            else if(value != nullptr && value != System::DBNull::Value)
            {
                text = m_column->TypeConverter->ConvertToString(typeDescriptorContext, value);
            }
            this->UpdateNativeText(text);
        }
        catch(System::Exception^)
        {
            this->UpdateNativeText(System::String::Empty);
        }
    }

    void Cell::UpdateNativeText(System::String^ text)
    {
        if(m_text != nullptr)
            this->Row->m_textCapacity -= m_text->Length;

        m_text = text;

        if(m_text != nullptr)
        {
            this->Row->m_textCapacity += m_text->Length;
            m_pItem->SetText(ToNativeString::Convert(m_text));
        }
        else
        {
            m_pItem->SetText(L"");
        }
    }

    System::Object^ Cell::ValidateValue(System::Object^ value)
    {
        if(ValueChecker::IsNullOrDBNull(value) == true)
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

    bool Cell::CancelEditInternal()
    {
        if(m_oldValue == Cell::NullValue)
            return false;

        m_row->RemoveEditedCell();
        if(m_wrongValue == false)
        {
            if(this->GridControl->InvokeValueChanging(this, m_oldValue, this->ValueCore) == true)
            {
                this->ValueCore = m_oldValue;
                this->GridControl->InvokeValueChanged(this);
            }
            UpdateNativeText();
        }
        m_oldValue = Cell::NullValue;
        return true;
    }

    bool Cell::EndEditInternal()
    {
        if(m_oldValue == Cell::NullValue)
            return false;

        if(m_value != nullptr)
        {
            System::Object^ value = m_value;
            m_value = nullptr;
            this->ValueCore = value;
        }

        m_row->RemoveEditedCell();
        m_oldValue = Cell::NullValue;
        return true;
    }

    bool Cell::CancelEdit()
    {
        bool result = this->CancelEditInternal();
        this->Row->Refresh();
        return result;
    }

    bool Cell::EndEdit()
    {
        bool result = this->EndEditInternal();
        this->Row->Refresh();
        return result;
    }

    void Cell::Select(Ntreev::Windows::Forms::Grid::SelectionType selectionType)
    {
        this->Selector->SelectItem(m_pItem, (GrSelectionType)selectionType);
    }

    bool Cell::Focus()
    {
        this->Focuser->Set(m_pItem);
        return this->IsFocused;
    }

    void Cell::BringIntoView()
    {
        this->GridControl->BringIntoView(this);
    }

    //void Cell::SetDefaultValue()
    //{
    //    this->ValueCore = this->Column->DefaultValue;
    //    UpdateNativeText();
    //}

    bool Cell::IsEdited::get()
    { 
        return m_oldValue != Cell::NullValue ? true : false;
    }

    bool Cell::IsSelected::get()
    {
        return m_pItem->GetSelected();
    }

    void Cell::IsSelected::set(bool value)
    {
        if(this->Row->Index == INVALID_INDEX)
            throw gcnew System::InvalidOperationException();
        if(this->Row->IsVisible == false)
            throw gcnew System::InvalidOperationException();
        m_pItem->SetSelected(value);
    }

    bool Cell::IsFocused::get()
    {
        return m_pItem->GetFocused();
    }

    bool Cell::IsMouseOvered::get()
    {
        return m_pItem->GetMouseOvered();
    }

    System::String^ Cell::ToString()
    {
        return m_text;
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
        using namespace System::ComponentModel;

        if(m_wrongValue == true)
            return nullptr;

        PropertyDescriptor^ propertyDescriptor = this->Column->PropertyDescriptor;
        System::Object^ component = this->Row->Component;

        if(propertyDescriptor == nullptr || component == nullptr)
        {
            if(m_value == nullptr)
                return nullptr;
            return m_value;
        }
        System::Object^ value = propertyDescriptor->GetValue(component);
        return this->Column->ConvertFromSource(value);
    }

    void Cell::ValueCore::set(System::Object^ value)
    {
        using namespace System::ComponentModel;

        PropertyDescriptor^ propertyDescriptor = this->Column->PropertyDescriptor;
        System::Object^ component = this->Row->Component;

        if(propertyDescriptor == nullptr || component == nullptr)
        {
            m_value = value;
        }
        else if(propertyDescriptor->IsReadOnly == false)
        {
            value = this->Column->ConvertToSource(value);
            try
            {
                //if(propertyDescriptor->ShouldSerializeValue(component) == false)
                    propertyDescriptor->SetValue(component, value);
            }
            catch(System::Exception^ e)
            {
                if(value == nullptr)
                    propertyDescriptor->SetValue(component, System::DBNull::Value);
                else
                    throw e;
            }
        }
    }

    System::Object^ Cell::SourceValue::get()
    {
        System::ComponentModel::PropertyDescriptor^ propertyDescriptor = this->Column->PropertyDescriptor;
        System::Object^ component = this->Row->Component;
        if(propertyDescriptor == nullptr || component == nullptr)
            return nullptr;
        return propertyDescriptor->GetValue(component);
    }

    bool Cell::WrongValue::get()
    {
        return m_wrongValue;
    }

    void Cell::WrongValue::set(bool value)
    {
        m_wrongValue = value;
    }

    System::String^ Cell::DisplayText::get()
    {
        return m_text;
    }

    void Cell::DisplayText::set(System::String^ value)
    {
        using namespace System::ComponentModel;

        this->UpdateNativeText(value);

        if(System::String::IsNullOrEmpty(m_text) == true)
        {
            m_displayValue = Cell::NullValue;
        }
        else
        {
            TypeConverter^ converter = this->Column->TypeConverter;
            try
            {
                m_displayValue = converter->ConvertFromString(m_text);
            }
            catch(System::Exception^ /*e*/)
            {
                m_displayValue = Cell::NullValue;
            }
        }
    }

    System::Object^ Cell::DisplayValue::get()
    {
        if(m_displayValue == Cell::NullValue)
            return this->ValueCore;
        return m_displayValue;
    }

    System::Drawing::Rectangle Cell::TextBound::get()
    {
        return m_pItem->GetTextBounds();
    }

    bool Cell::ShouldSerializeValue()
    {
        using namespace System::ComponentModel;
        PropertyDescriptor^ propertyDescriptor = this->Column->PropertyDescriptor;

        if(propertyDescriptor != nullptr)
        {
            return propertyDescriptor->ShouldSerializeValue(this->Row->Component);
        }

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

    IColumn^ Cell::Column_ICell::get()
    {
        return this->Column;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/