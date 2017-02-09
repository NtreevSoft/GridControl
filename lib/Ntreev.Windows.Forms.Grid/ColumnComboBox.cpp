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
#include "ColumnComboBox.h"
#include "GridControl.h"
#include "Row.h"
#include "ICell.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Columns
{
    ColumnComboBox::ColumnComboBox()
    {
        using namespace System::Windows::Forms;

        m_nMaxDropDownItems = DefaultMaxDropDownItems;
        System::Windows::Forms::ListBox^ listBox = this->Control;

        listBox->BorderStyle = BorderStyle::None;
        //listBox->IntegralHeight = false;

        listBox->PreviewKeyDown += gcnew PreviewKeyDownEventHandler(this, &ColumnComboBox::listBox_PreviewKeyDown);
        listBox->MouseMove += gcnew MouseEventHandler(this, &ColumnComboBox::listBox_OnMouseMove);
        listBox->MouseClick += gcnew MouseEventHandler(this, &ColumnComboBox::listBox_OnMouseClick);
        listBox->VisibleChanged += gcnew System::EventHandler(this, &ColumnComboBox::listBox_OnVisibleChanged);
    }

    System::Object^ ColumnComboBox::DataSource::get()
    {
        return this->Control->DataSource;
    }

    void ColumnComboBox::DataSource::set(System::Object^ value)
    {
        using namespace System::Windows::Forms;
        BindingContext bindingContext;
        CurrencyManager^ manager = dynamic_cast<CurrencyManager^>(bindingContext[value]);
        if(manager == nullptr)
            throw gcnew System::ArgumentException("Not supported", "value");

        for each(System::Object^ item in manager->List)
        {
            this->Control->Items->Add(item);
        }
        this->Control->SelectedIndex = -1;
    }

    System::Windows::Forms::ListBox::ObjectCollection^ ColumnComboBox::Items::get()
    {
        return this->Control->Items;
    }

    void ColumnComboBox::MaxDropDownItems::set(int count)
    {
        if(count < 1)
            throw gcnew System::ArgumentOutOfRangeException("count");
        m_nMaxDropDownItems = count;
    }

    int ColumnComboBox::MaxDropDownItems::get()
    {
        return m_nMaxDropDownItems;
    }

    void ColumnComboBox::listBox_PreviewKeyDown(System::Object^ /*sender*/, System::Windows::Forms::PreviewKeyDownEventArgs^ e)
    {
        if(e->KeyCode == System::Windows::Forms::Keys::Enter)
        {
            System::Windows::Forms::ListBox^ control = this->Control;
            if(control->SelectedItem != nullptr)
                m_value = control->SelectedItem;
        }
    }

    void ColumnComboBox::listBox_OnMouseMove(System::Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e)
    {
        int selIndex = this->Control->IndexFromPoint(e->Location);
        if(selIndex < 0)
            return;
        this->Control->SelectedIndex = selIndex;
    }

    void ColumnComboBox::listBox_OnMouseClick(System::Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ /*e*/)
    {
        System::Windows::Forms::ListBox^ control = this->Control;
        if(control->SelectedItem != nullptr)
            m_value = control->SelectedItem;
        CloseDropDown();
    }

    void ColumnComboBox::listBox_OnVisibleChanged(System::Object^ /*sender*/, System::EventArgs^ /*e*/)
    {
        //if(this->Control->Visible == false)
        //    this->Control->SelectedIndex = -1;
        //else
        //    this->Control->SelectedItem = m_value;
    }

    bool ColumnComboBox::ShouldSerializeMaxDropDownItems()
    {
        return m_nMaxDropDownItems != DefaultMaxDropDownItems;
    }

    System::Object^ ColumnComboBox::GetControlValue()
    {
        return m_value;
    }

    void ColumnComboBox::SetControlValue(System::Object^ value)
    {
        this->Control->SelectedItem = value;
        m_value = value;
    }

    void ColumnComboBox::SetControlLayout(Ntreev::Windows::Forms::Grid::ICell^ cell)
    {
        using namespace System::Windows::Forms;
        System::Windows::Forms::ListBox^ control = this->Control;
        ColumnDropDown::SetControlLayout(cell);
        delete control->CreateGraphics();

        const int itemHeight = control->ItemHeight;
        int height = 0;
        const int itemCount = control->Items->Count;

        if(itemCount > this->MaxDropDownItems)
        {
            height = itemHeight * (this->MaxDropDownItems);
        }
        else
        {
            height = itemHeight * (itemCount);
        }

        control->Height = height;
        delete control->CreateGraphics();
    }
} /*namespace Extensiton*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/