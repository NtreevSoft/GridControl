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
		m_value = value;

		if(ValueChecker::IsNullOrDBNull(m_value) == true)
			return;

		this->UpdateValueToControl(m_value->GetHashCode(), nullptr);
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
			{
				checkBox->CheckedChanged -= gcnew System::EventHandler(this, &FlagControl::checkBox_CheckChanged);
				this->Controls->Remove(checkBox);
			}
		}

		int y = this->Padding.Top;
		int buttonHeight = this->Bottom - this->buttonOk->Top;
		for each(System::String^ item in Enum::GetNames(m_flagType))
		{
			System::Object^ value = Enum::Parse(m_flagType, item);
			CheckBox^ checkBox = gcnew CheckBox();
			checkBox->Name = item;
			checkBox->Text = item;
			checkBox->Tag = value;

			int code = value->GetHashCode();

			if(code == 0)
				checkBox->ForeColor = System::Drawing::Color::Red;
			else if(this->IsSingleBit(code) == false)
				checkBox->Font = gcnew System::Drawing::Font(checkBox->Font, System::Drawing::FontStyle::Bold);

			checkBox->AutoSize = true;
			checkBox->Location = Point(this->Padding.Left, y);

			this->Controls->Add(checkBox);
			m_checkBoxes.Add(checkBox);
			checkBox->CheckedChanged += gcnew System::EventHandler(this, &FlagControl::checkBox_CheckChanged);

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

	System::Void FlagControl::checkBox_CheckChanged(System::Object^ sender, System::EventArgs^ e)
	{
		CheckBox^ checkBox = dynamic_cast<CheckBox^>(sender);
		int controlValue = checkBox->Tag->GetHashCode();
		int value = this->GetInt32Value();

		if(checkBox->Checked == true)
		{
			if(controlValue == 0)
				value = 0;
			else
				value |= checkBox->Tag->GetHashCode();
		}
		else
		{
			value &= ~checkBox->Tag->GetHashCode();
		}
	
		m_value = System::Enum::ToObject(m_flagType, value);
		this->UpdateValueToControl(value, controlValue != 0 ? checkBox : nullptr);
	}

	void FlagControl::UpdateValue()
	{
		m_value = System::Enum::ToObject(m_flagType, this->GetInt32Value());
	}

	void FlagControl::UpdateValueToControl(int value, CheckBox^ exception)
	{
		for each (CheckBox^ item in m_checkBoxes)
		{
			item->CheckedChanged -= gcnew System::EventHandler(this, &FlagControl::checkBox_CheckChanged);
		}

		for each (CheckBox^ item in m_checkBoxes)
		{
			if(item == exception)
				continue;

			int itemValue = item->Tag->GetHashCode();

			if(value == itemValue)
			{
				item->CheckState = System::Windows::Forms::CheckState::Checked;
			}
			else if((value & itemValue) != 0)
			{
				if(this->IsSingleBit(itemValue) == true || (value & itemValue) == itemValue)
					item->CheckState = System::Windows::Forms::CheckState::Checked;
				else
					item->CheckState = System::Windows::Forms::CheckState::Indeterminate;
			}
			else
			{
				item->CheckState = System::Windows::Forms::CheckState::Unchecked;
			}
		}

		for each (CheckBox^ item in m_checkBoxes)
		{
			item->CheckedChanged += gcnew System::EventHandler(this, &FlagControl::checkBox_CheckChanged);
		}
	}

	int FlagControl::GetInt32Value()
	{
		int value = 0;
		for each(CheckBox^ item in m_checkBoxes)
		{
			int controlValue = item->Tag->GetHashCode();
			if(item->CheckState != CheckState::Checked || this->IsSingleBit(controlValue) == false)
				continue;
			value |= controlValue;
		}
		return value;
	}

	bool FlagControl::IsSingleBit(int value)
	{
		for(int i=0; i <32 ; i++)
		{
			if(value == 1 << i)
			{
				return true;
			}
		}
		return false;
	}
} /*namespace Controls*/ } /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/