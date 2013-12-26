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
#include "IDisplayTextConverter.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    using namespace System::ComponentModel;

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
        : CellBase(builder->GridControl, builder->NativeRef)
    {
        //m_column = FromNative::Get(m_pItem->GetColumn());
        //m_row = FromNative::Get(m_pItem->GetDataRow());

        m_value = nullptr;
        m_oldValue = Cell::NullValue;
        m_displayValue = Cell::NullValue;

        //this->NativeRef->ManagedRef = this;
    }

    Cell::Cell(_GridControl^ gridControl, GrItem* pItem)
        : CellBase(gridControl, pItem)
    {
        //m_column = FromNative::Get(pItem->GetColumn());
        //m_row = FromNative::Get(pItem->GetDataRow());

        m_value = nullptr;
        m_oldValue = Cell::NullValue;
        m_displayValue = Cell::NullValue;

        //this->NativeRef->ManagedRef = this;
    }

    Column^ Cell::Column::get()
    {
        return FromNative::Get(this->NativeRef->GetColumn());
    }

    unsigned int Cell::ColumnID::get()
    {
        return this->Column->ColumnID;
    }

    Ntreev::Windows::Forms::Grid::Row^ Cell::Row::get()
    {
        return FromNative::Get(this->NativeRef->GetDataRow());
    }

    unsigned int Cell::RowID::get()
    {
        return this->Row->RowID;
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

        if(this->Row->IsBeingEdited == true)
        {
            if(m_oldValue == Cell::NullValue)
            {
                this->Row->AddEditedCell();
                m_oldValue = oldValue;
            }
        }

        this->GridControl->InvokeValueChanged(this);
        this->Row->Refresh();
    }

    System::Object^ Cell::SourceValue::get()
    {
        System::ComponentModel::PropertyDescriptor^ propertyDescriptor = this->Column->PropertyDescriptor;
        System::Object^ component = this->Row->Component;
        if(propertyDescriptor == nullptr || component == nullptr)
            throw gcnew System::ArgumentException();
        return propertyDescriptor->GetValue(component);
    }

    //void Cell::SourceValue::set(System::Object^ value)
    //{
    //    System::ComponentModel::PropertyDescriptor^ propertyDescriptor = this->Column->PropertyDescriptor;
    //    System::Object^ component = this->Row->Component;

    //    if(propertyDescriptor == nullptr || component == nullptr)
    //        throw gcnew System::ArgumentException();

    //    propertyDescriptor->SetValue(component, value);
    //    this->UpdateNativeText();
    //}

    void Cell::UpdateNativeText()
    {
		System::Object^ value = nullptr;

		try
		{
			value = this->ValueCore;
		}
		catch(System::Exception^ e)
		{
			m_wrongValue = true;
		}

        try
        {
            System::String^ text = System::String::Empty;

			if(m_wrongValue == true)
            {
                System::Object^ sourceValue = this->SourceValue;
                if(sourceValue != nullptr)
                    text = sourceValue->ToString();
            }
            
			else if(value != nullptr && value != System::DBNull::Value)
            {
				text = value->ToString();

                if(this->Column->CellMultiline == true)
                    text = text->Replace("\r\n", "\n");
            }
            this->UpdateNativeText(text);
			if(this->NativeRef->IsGridCoreAttached() == true)
				this->Column->OnCellBoundUpdate(this, value);
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
            this->NativeRef->SetText(ToNativeString::Convert(m_text));
        }
        else
        {
            this->NativeRef->SetText(L"");
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

		this->Row->RemoveEditedCell();
		if(m_wrongValue == false)
		{
			if(this->GridControl->InvokeValueChanging(this, m_oldValue, this->ValueCore) == true)
			{
				this->ValueCore = m_oldValue;
				this->GridControl->InvokeValueChanged(this);
			}
		}
		this->UpdateNativeText();
        
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

        this->Row->RemoveEditedCell();
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

    void Cell::SourceValueToLocal(System::Object^ component)
    {
        PropertyDescriptor^ propertyDescriptor = this->Column->PropertyDescriptor;
        if(propertyDescriptor == nullptr)
            return;

        if(component == nullptr)
        {
            m_value = nullptr;
        }
        else
        {
            System::Object^ value = propertyDescriptor->GetValue(component);
            m_value = this->Column->ConvertFromSource(value);
        }
    }

    void Cell::LocalValueToSource(System::Object^ component)
    {
        PropertyDescriptor^ propertyDescriptor = this->Column->PropertyDescriptor;
        if(propertyDescriptor == nullptr)
            return;

        System::Object^ value = propertyDescriptor->GetValue(component);

        //if(ValueChecker::IsNullOrDBNull(value) == true && ValueChecker::IsNullOrDBNull(m_value) == false)
		if(ValueChecker::IsNullOrDBNull(m_value) == false)
        {
            value = this->Column->ConvertToSource(m_value);
            propertyDescriptor->SetValue(component, value);
        }

        m_value = nullptr;        
    }

    void Cell::Select(Ntreev::Windows::Forms::Grid::SelectionType selectionType)
    {
        this->Selector->SelectItem(this->NativeRef, (GrSelectionType)selectionType);
    }

    bool Cell::Focus()
    {
        this->Focuser->Set(this->NativeRef);
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
        return this->NativeRef->GetSelected();
    }

    void Cell::IsSelected::set(bool value)
    {
        //if(this->Row->Index == INVALID_INDEX)
        //    throw gcnew System::InvalidOperationException();
        if(this->Row->IsVisible == false)
            throw gcnew System::InvalidOperationException();
        this->NativeRef->SetSelected(value);
    }

    bool Cell::IsFocused::get()
    {
        return this->NativeRef->GetFocused();
    }

    bool Cell::IsMouseOvered::get()
    {
        return this->NativeRef->GetMouseOvered();
    }

    System::String^ Cell::ToString()
    {
        return m_text;
    }

    bool Cell::IsReadOnly::get()
    {
        return this->NativeRef->GetReadOnly();
    }

    void Cell::IsReadOnly::set(bool value)
    {
        this->NativeRef->SetReadOnly(value);
    }

    bool Cell::IsBeingEdited::get()
    {
        return this->GridControl->EditingCell == this;
    }

    bool Cell::IsSelecting::get()
    {
        return this->NativeRef->IsItemSelecting();
    }

    System::String^ Cell::ErrorDescription::get()
    {
        return this->Row->GetErrorDescription(this);
    }

    void Cell::ErrorDescription::set(System::String^ value)
    {
        this->Row->SetErrorDescription(this, value);
    }

    System::Object^ Cell::ValueCore::get()
    {
        using namespace System::ComponentModel;

		if(m_wrongValue == true)
            return nullptr;

        if(this->HasSourceValue == false)
        {
            return m_value;
        }
        return this->GetValueFromSource();
    }

    void Cell::ValueCore::set(System::Object^ value)
    {
        using namespace System::ComponentModel;

        if(this->HasSourceValue == false)
            m_value = value;
        else
            this->SetValueToSource(value);
    }

    System::Object^ Cell::GetValueFromSource()
    {
        System::ComponentModel::PropertyDescriptor^ propertyDescriptor = this->Column->PropertyDescriptor;
        System::Object^ component = this->Row->Component;
        if(propertyDescriptor == nullptr || component == nullptr)
            throw gcnew System::ArgumentException();
        System::Object^ value = propertyDescriptor->GetValue(component);
        return this->Column->ConvertFromSource(value);
    }

    void Cell::SetValueToSource(System::Object^ value)
    {
        System::ComponentModel::PropertyDescriptor^ propertyDescriptor = this->Column->PropertyDescriptor;
        System::Object^ component = this->Row->Component;
        if(propertyDescriptor->IsReadOnly == false)
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

    bool Cell::HasSourceValue::get()
    {
        System::ComponentModel::PropertyDescriptor^ propertyDescriptor = this->Column->PropertyDescriptor;
        System::Object^ component = this->Row->Component;
        if(propertyDescriptor == nullptr || component == nullptr)
            return false;
        return true;
    }

	bool Cell::WrongValue::get()
    {
        return m_wrongValue;
    }

    void Cell::WrongValue::set(bool value)
    {
        m_wrongValue = value;
    }

    void Cell::DisplayValue::set(System::Object^ value)
    {
        using namespace System::ComponentModel;

        if(value == nullptr || value == System::DBNull::Value)
        {
			m_displayValue = Cell::NullValue;
            this->UpdateNativeText();
        }
        else
        {
			if(value->GetType() != this->Column->DataType)
				throw gcnew System::ArgumentException("설정할 수 없는 타입입니다.");

			m_displayValue = value;
			this->UpdateNativeText(value->ToString());
			//this->Column->OnCellBoundUpdate(this, this->DisplayValue);
            this->Invalidate();
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
        GrSize size = this->NativeRef->GetTextBounds();
        return System::Drawing::Rectangle(0, 0, size.width, size.height);
    }

    bool Cell::ShouldSerializeValue()
    {
        PropertyDescriptor^ propertyDescriptor = this->Column->PropertyDescriptor;
        System::Object^ component = this->Row->Component;
        if(propertyDescriptor == nullptr || component == nullptr)
        {
            return m_value != nullptr;
        }
        return propertyDescriptor->ShouldSerializeValue(this->Row->Component);
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