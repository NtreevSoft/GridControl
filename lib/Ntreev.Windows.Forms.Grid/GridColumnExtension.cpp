//=====================================================================================================================
// Ntreev Grid for .Net 1.0.4300.26762
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
#include "GridColumnExtension.h"
#include "GridControl.h"
#include "GridRow.h"
#include "GridCell.h"
#include "GridColumnDropDown.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Columns
{
	ColumnTextBox::ColumnTextBox()
	{

	}

	void ColumnTextBox::OnAttaching(AttachEventArgs^ e)
	{
		ColumnControl::OnAttaching(e);
		this->EditingControl->ReadOnly	= IsReadOnly;
		this->EditingControl->Multiline	= CellMultiline;
		this->EditingControl->WordWrap	= CellWordWrap;
	} 

	void ColumnTextBox::OnAttached(AttachEventArgs^ e)
	{
		ColumnControl::OnAttached(e);

		System::Windows::Forms::TextBox^ textBox = this->EditingControl;
		switch(e->By->EditingReasonType)
		{
		case EditingReasonType::Char:
			{
				textBox->Text = e->By->Char.ToString();
				textBox->Select(textBox->TextLength, 0);
			}
			break;
		case EditingReasonType::Ime:
			{
				textBox->Text = string::Empty;
			}
			break;
		}
	}

	void ColumnTextBox::OnDetaching(DetachEventArgs^ e)
	{
		ColumnControl::OnDetaching(e);
		this->EditingControl->Select(0, 0);
	}

	void ColumnTextBox::textBox_OnKeyPress(object^ /*sender*/, System::Windows::Forms::KeyPressEventArgs^ e)
	{
		System::Windows::Forms::TextBox^ textBox = this->EditingControl;
		switch(e->KeyChar)
		{
		case '\r':
		case 27: // escape key
			if(textBox->Multiline == false)
				e->Handled = true;
			textBox->Select(0, 0);
			break;
		case '\n':
			{
				if(textBox->Multiline == true)
				{
					Row^ row = GridControl->EditingCell->Row;
					textBox->Height = (textBox->Lines->Length + 1) * (textBox->Font->Height);

					row->Height = textBox->Height + GridControl->EditingCell->Padding.Vertical;
					GridControl->Refresh();
				}
			}
			break;
		}
	}

	void ColumnTextBox::SetControlLayout(System::Windows::Forms::TextBox^ control, _Point location, _Size size)
	{
		System::Windows::Forms::TextBox^ textBox = this->EditingControl;
		_Point pt = textBox->GetPositionFromCharIndex(0);
		_Point newPoint = location - (_Size)pt;

		_Size newSize(size.Width + pt.X, size.Height);

		ColumnControl::SetControlLayout(control, newPoint, newSize);
	}

	void ColumnTextBox::SetEditingValue(System::Windows::Forms::TextBox^ control, object^ value)
	{
		control->Text = TypeConverter->ConvertToString(value);
	}

	void ColumnTextBox::OnControlCreated(System::Windows::Forms::TextBox^ control)
	{
		using namespace System::Windows::Forms;

		control->BorderStyle = BorderStyle::None;
		control->KeyPress += gcnew KeyPressEventHandler(this, &ColumnTextBox::textBox_OnKeyPress);
	}

	object^ ColumnTextBox::GetEditingValue(System::Windows::Forms::TextBox^ control)
	{
		if(this->DataType != string::typeid && control->Text == string::Empty)
			return nullptr;

		return TypeConverter->ConvertFrom(control->Text);
	}

	ColumnComboBox::ColumnComboBox()
	{
		using namespace System::Windows::Forms;

		m_nMaxDropDownItems = DefaultMaxDropDownItems;
		System::Windows::Forms::ListBox^ listBox = EditingControl;

		listBox->BorderStyle = BorderStyle::None;

		listBox->MouseMove   += gcnew MouseEventHandler(this, &ColumnComboBox::listBox_OnMouseMove);
		listBox->MouseClick  += gcnew MouseEventHandler(this, &ColumnComboBox::listBox_OnMouseClick);
	}

	_Size ColumnComboBox::GetPreferredSize(_Size proposedSize)
	{
		using namespace System::Windows::Forms;

		ListBox^ listBox = this->EditingControl;

		int itemHeight = listBox->Font->Height - 1;
		_Size size;

		size.Width = proposedSize.Width;

		int itemCount = listBox->Items->Count;

		if(listBox->DataSource != nullptr)
		{
			BindingContext^ bindingContext = gcnew BindingContext();
			BindingManagerBase^ managerBase = bindingContext[listBox->DataSource];
			if(managerBase != nullptr)
			{
				itemCount = managerBase->Count;
			}
		}

		if(itemCount > this->MaxDropDownItems)
		{
			size.Height = itemHeight * (this->MaxDropDownItems);
		}
		else
		{
			size.Height = itemHeight * (itemCount);
		}

		return size;
	}

	object^ ColumnComboBox::DataSource::get()
	{
		return this->EditingControl->DataSource;
	}

	void ColumnComboBox::DataSource::set(object^ value)
	{
		this->EditingControl->DataSource = value;
	}

	System::Windows::Forms::ListBox::ObjectCollection^ ColumnComboBox::Items::get()
	{
		return this->EditingControl->Items;
	}

	void ColumnComboBox::MaxDropDownItems::set(int count)
	{
		m_nMaxDropDownItems = count;
	}

	int ColumnComboBox::MaxDropDownItems::get()
	{
		return m_nMaxDropDownItems;
	}

	bool ColumnComboBox::KeyTest(System::Windows::Forms::Keys key)
	{
		if(key == System::Windows::Forms::Keys::Space)
			return true;
		return false;
	}

	void ColumnComboBox::listBox_OnMouseMove(object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e)
	{
		int selIndex = this->EditingControl->IndexFromPoint(e->Location);
		this->EditingControl->SelectedIndex = selIndex;
	}

	void ColumnComboBox::listBox_OnMouseClick(object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e)
	{
		if(e->Button == System::Windows::Forms::MouseButtons::Left)
			this->EditingResult = _EditingResult::Ok;
		else
			this->EditingResult = _EditingResult::Cancel;
	}

	bool ColumnComboBox::ShouldSerializeMaxDropDownItems()
	{
		return m_nMaxDropDownItems != DefaultMaxDropDownItems;
	}

	object^ ColumnComboBox::GetEditingValue(System::Windows::Forms::ListBox^ control)
	{
		return control->SelectedItem;
	}

	void ColumnComboBox::SetEditingValue(System::Windows::Forms::ListBox^ control, object^ value)
	{
		control->SelectedItem = value;
	}

	void ColumnComboBox::OnAttaching(AttachEventArgs^ e)
	{
		if(this->DataSource == nullptr && this->DataType->IsEnum == true)
		{
			this->DataSource = System::Enum::GetValues(this->DataType);
		}

		ColumnDropDown::OnAttaching(e);
	}

	ColumnPathSelector::ColumnPathSelector()
	{
		m_folderBrowerDialog = gcnew System::Windows::Forms::FolderBrowserDialog();
	}

	void ColumnPathSelector::OnEditValue(EditValueEventArgs^ e)
	{
		using namespace System::Windows::Forms;

		ColumnModal::OnEditValue(e);
		if(e->Value != nullptr)
			m_folderBrowerDialog->SelectedPath = e->Value->ToString();
		DialogResult dr = m_folderBrowerDialog->ShowDialog();
		if(dr == DialogResult::OK)
			e->Value = m_folderBrowerDialog->SelectedPath;
		e->SuppressEditing = true;
	}

	ColumnFlagControl::ColumnFlagControl()
	{
		this->EditingControl->EditOK += gcnew _EventHandler(this, &ColumnFlagControl::EditingControl_EditOK);
		this->EditingControl->EditCanceled += gcnew _EventHandler(this, &ColumnFlagControl::EditingControl_EditCanceled);
	}

	_Size ColumnFlagControl::GetPreferredSize(_Size proposedSize)
	{
		return _Size(proposedSize.Width, this->EditingControl->Height);
	}

	object^ ColumnFlagControl::GetEditingValue(FlagControl^ control)
	{
		return System::Enum::ToObject(this->DataType, control->Value);
	}

	void ColumnFlagControl::SetEditingValue(FlagControl^ control, object^ value)
	{
		if (value == nullptr)
			control->Value = 0;
		else
			control->Value = (int)value;
	}

	void ColumnFlagControl::OnAttaching(AttachEventArgs^ e)
	{
		this->EditingControl->FlagType = this->DataType;
		ColumnDropDown<FlagControl^>::OnAttaching(e);
	}

	void ColumnFlagControl::EditingControl_EditOK(object^ /*sender*/, _EventArgs^ /*e*/)
	{
		this->EditingResult = _EditingResult::Ok;
	}

	void ColumnFlagControl::EditingControl_EditCanceled(object^ /*sender*/, _EventArgs^ /*e*/)
	{
		this->EditingResult = _EditingResult::Cancel;
	}
} /*namespace Extensiton*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/