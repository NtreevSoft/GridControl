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
#include "ListBox.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design { namespace Controls
{
    ListBox::ListBox(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService)
        : m_editorService(editorService)
    {
        this->BorderStyle = System::Windows::Forms::BorderStyle::None;
    }

    ListBox::ListBox(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, System::Type^ dataType, System::Object^ value)
        : m_editorService(editorService), m_value(value)
    {
        this->BorderStyle = System::Windows::Forms::BorderStyle::None;

        if(dataType->IsEnum == true)
        {
            for each(System::String^ item in System::Enum::GetNames(dataType))
            {
                this->Items->Add(item);
            }

            if(value != nullptr)
                this->SelectedItem = value->ToString();
        }
        else if(dataType->IsArray == true)
        {
            throw gcnew System::NotImplementedException();
        }

        m_dataType = dataType;
    }

    System::Object^ ListBox::Value::get()
    {
        return m_value;
    }

    void ListBox::OnMouseMove(System::Windows::Forms::MouseEventArgs^ e)
    {
        System::Windows::Forms::ListBox::OnMouseMove(e);
        int selIndex = this->IndexFromPoint(e->Location);
        this->SelectedIndex = selIndex;
    }

    void ListBox::OnMouseClick(System::Windows::Forms::MouseEventArgs^ e)
    {
        System::Windows::Forms::ListBox::OnMouseClick(e);
        if(e->Button == System::Windows::Forms::MouseButtons::Left)
        {
            UpdateValue();
            m_editorService->Close();
        }
    }

    void ListBox::OnKeyDown(System::Windows::Forms::KeyEventArgs^ e)
    {
        System::Windows::Forms::ListBox::OnKeyDown(e);
        switch(e->KeyCode)
        {
        case System::Windows::Forms::Keys::Enter:
            {
                UpdateValue();
                m_editorService->Close();
            }
            break;
        }
    }

    void ListBox::UpdateValue()
    {
        if(this->SelectedItem != nullptr)
        {
            if(m_dataType->IsEnum == true)
            {
                m_value = System::Enum::Parse(m_dataType, (System::String^)this->SelectedItem);
            }
        }
    }
} /*namespace Controls*/ } /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/