//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4478.19833
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
#include "FlagControl.h"
#include "Utilities.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design { namespace Controls
{
    using namespace System;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;

    FlagControl::FlagControl()
    {
        InitializeComponent();
    }

    FlagControl::FlagControl(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, System::Type^ flagType)
        : m_editorService(editorService), m_flagType(flagType)
    {
        InitializeComponent();
        InitializeCheckBox();
    }

    System::Object^ FlagControl::Value::get()
    {
        return m_value;
    }

    void FlagControl::Value::set(System::Object^ value)
    {
        int enumValue = ValueChecker::IsNullOrDBNull(value) == false ? (int)value : 0;

        for each (Control^ item in this->Controls)
        {
            CheckBox^ checkBox = dynamic_cast<CheckBox^>(item);
            if (checkBox == nullptr)
                continue;
            int flag = (int)item->Tag;
            checkBox->Checked = (enumValue & flag) != 0 ? true : false;
        }
        m_value = value;
    }

    void FlagControl::InitializeCheckBox()
    {
        cli::array<System::Object^>^ flagAttrs = m_flagType->GetCustomAttributes(FlagsAttribute::typeid, true);
        if(flagAttrs->Length == 0)
            throw gcnew ArgumentException("FlagsAttribute를 갖는 Enum Type만 가능합니다.");

        this->SuspendLayout();

        for (int i = this->Controls->Count-1; i >=0 ; i--)
        {
            CheckBox^ checkBox = dynamic_cast<CheckBox^>(this->Controls[i]);
            if(checkBox != nullptr)
                this->Controls->Remove(checkBox);
        }

        int y = this->Padding.Top;
        int buttonHeight = this->Bottom - this->buttonOk->Top;
        for each(System::Object^ item in Enum::GetValues(m_flagType))
        {
            CheckBox^ checkBox = gcnew CheckBox();
            checkBox->Name = item->ToString();
            checkBox->Text = item->ToString();
            checkBox->Tag = item;
            checkBox->AutoSize = true;
            checkBox->Location = Point(this->Padding.Left, y);

            this->Controls->Add(checkBox);

            y += checkBox->Height + 1;
        }

        this->Height = y + buttonHeight;
        this->ResumeLayout(true);
    }

    bool FlagControl::ProcessCmdKey(System::Windows::Forms::Message% msg, System::Windows::Forms::Keys keyData)
    {
        if (keyData == Keys::Enter)
        {
            UpdateValue();
            m_editorService->Close();
            return true;
        }
        return UserControl::ProcessCmdKey(msg, keyData);
    }

    System::Void FlagControl::buttonOk_Click(System::Object^ /*sender*/, System::EventArgs^ /*e*/) 
    {
        UpdateValue();
        m_editorService->Close();
    }

    System::Void FlagControl::buttonCancel_Click(System::Object^ /*sender*/, System::EventArgs^ /*e*/) 
    {
        m_editorService->Close();
    }

    void FlagControl::UpdateValue()
    {
        int value = 0;
        for each(Control^ item in this->Controls)
        {
            CheckBox^ checkBox = dynamic_cast<CheckBox^>(item);
            if (checkBox == nullptr || checkBox->Checked == false)
                continue;
            value |= (int)item->Tag;
        }
        m_value = System::Enum::ToObject(m_flagType, value);
    }
} /*namespace Controls*/ } /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/