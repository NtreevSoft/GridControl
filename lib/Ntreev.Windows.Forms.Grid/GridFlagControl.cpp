//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
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
#include "GridFlagControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Columns
{
	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	int FlagControl::Value::get()
	{
		int value = 0;
		for each(Control^ item in this->Controls)
		{
			CheckBox^ checkBox = dynamic_cast<CheckBox^>(item);
			if (checkBox == nullptr || checkBox->Checked == false)
				continue;
			value |= (int)item->Tag;
		}
		return value;
	}

	void FlagControl::Value::set(int value)
	{
		m_oldValue = this->Value;

		for each (Control^ item in this->Controls)
		{
			CheckBox^ checkBox = dynamic_cast<CheckBox^>(item);
			if (checkBox == nullptr)
				continue;
			int flag = (int)item->Tag;
			checkBox->Checked = (value & flag) != 0 ? true : false;
		}
	}

	void FlagControl::FlagType::set(_Type^ value)
	{
		if (value->IsEnum == false)
			throw gcnew ArgumentException("Enum Type만 가능합니다.");

		cli::array<object^>^ flagAttrs = value->GetCustomAttributes(FlagsAttribute::typeid, true);
		if(flagAttrs->Length == 0)
			throw gcnew ArgumentException("FlagsAttribute를 갖는 Enum Type만 가능합니다.");

		if (m_flagType == value)
			return;

		this->SuspendLayout();

		for (int i = this->Controls->Count-1; i >=0 ; i--)
		{
			CheckBox^ checkBox = dynamic_cast<CheckBox^>(this->Controls[i]);
			if(checkBox != nullptr)
				this->Controls->Remove(checkBox);
		}

		int y = this->Padding.Top;
		int buttonHeight = this->Bottom - this->buttonOk->Top;
		for each(object^ item in Enum::GetValues(value))
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

		m_flagType = value;
	}

	bool FlagControl::ProcessCmdKey(System::Windows::Forms::Message% msg, System::Windows::Forms::Keys keyData)
	{
		if (keyData == Keys::Enter || keyData == Keys::Escape)
		{
			OnPreviewKeyDown(gcnew PreviewKeyDownEventArgs(keyData));
		}
		return UserControl::ProcessCmdKey(msg, keyData);
	}

	System::Void FlagControl::buttonOk_Click(System::Object^ /*sender*/, System::EventArgs^ /*e*/) 
	{
		this->EditOK(this, System::EventArgs::Empty);
	}

	System::Void FlagControl::buttonCancel_Click(System::Object^ /*sender*/, System::EventArgs^ /*e*/) 
	{
		this->Value = m_oldValue;
		this->EditCanceled(this, System::EventArgs::Empty);
	}
} /*namespace Columns*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/