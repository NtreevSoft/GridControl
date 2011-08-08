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

	void ColumnTextBox::OnAttaching(AttachEventArgs<System::Windows::Forms::TextBox^>^ e)
	{
		ColumnControl::OnAttaching(e);
		System::Windows::Forms::TextBox^ textBox = e->Control;
		textBox->ReadOnly	= IsReadOnly;
		textBox->Multiline	= CellMultiline;
		textBox->WordWrap	= CellWordWrap;
	} 

	void ColumnTextBox::OnAttached(AttachEventArgs<System::Windows::Forms::TextBox^>^ e)
	{
		ColumnControl::OnAttached(e);
		System::Windows::Forms::TextBox^ textBox = e->Control;
		switch(e->By->EditingReasonType)
		{
		case EditingReasonType::Char:
			{
				textBox->Text = e->By->Char.ToString();
				textBox->Select(textBox->TextLength, 0);
			}
			break;
		case EditingReasonType::Mouse:
			{
				//m_textBox->Select(0, 0);
				//m_textBox->SelectionStart = 0;
			}
			break;
		case EditingReasonType::Ime:
			{
				textBox->Text = "";
			}
			break;
		}
	}

	void ColumnTextBox::OnDetaching(DetachEventArgs<System::Windows::Forms::TextBox^>^ e)
	{
		ColumnControl::OnDetaching(e);
		e->Control->Select(0, 0);
	}

	void ColumnTextBox::textBox_OnKeyPress(object^ /*sender*/, System::Windows::Forms::KeyPressEventArgs^ e)
	{
		System::Windows::Forms::TextBox^ textBox = EditingControl;
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

	void ColumnTextBox::textBox_OnKeyDown(object^ /*sender*/, System::Windows::Forms::KeyEventArgs^ /*e*/)
	{
		//switch(e->KeyData)
		//{
		//case ::Keys::Return:
		//	{
		//		if(m_textBox->Multiline && e->Control == true)
		//		{
		//			GridControl->EditingCell->Row->Height = m_textBox->Lines->Length * (m_textBox->Font->Height * 1.75f);
		//		}
		//	}
		//	break;
		//}
	}

	void ColumnTextBox::SetControlLayout(System::Windows::Forms::TextBox^ control, _Point location, _Size size)
	{
		System::Windows::Forms::TextBox^ textBox = EditingControl;
		_Point pt = textBox->GetPositionFromCharIndex(0);
		_Point newPoint = location - (_Size)pt;

		_Size newSize(size.Width + pt.X, size.Height);
		//ColumnControl(SetBoundsCore

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
		control->KeyDown  += gcnew KeyEventHandler(this, &ColumnTextBox::textBox_OnKeyDown);
	}

	object^ ColumnTextBox::GetEditingValue(System::Windows::Forms::TextBox^ control)
	{
		if(DataType != string::typeid && control->Text == "")
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
		listBox->FontChanged += gcnew _EventHandler(this, &ColumnComboBox::listBox_OnFontChanged);
	}

	_Size ColumnComboBox::GetPreferredSize(_Size proposedSize)
	{
		System::Windows::Forms::ListBox^ listBox = EditingControl;

		//// 렌더링이 되기 전에는 데이터 갱신이 되지 않으므로 강제로 갱신할 수 있도록 그래픽 객체를 생성후 지워준다.
		//delete listBox->CreateGraphics();

		int itemHeight = listBox->Font->Height - 1;
		_Size size;
	
		size.Width = proposedSize.Width;

		int itemCount = listBox->Items->Count;

		if(listBox->DataSource != nullptr)
		{
			System::Windows::Forms::BindingContext^ bindingContext = gcnew System::Windows::Forms::BindingContext();
			System::Windows::Forms::BindingManagerBase^ managerBase = bindingContext[listBox->DataSource];
			if(managerBase != nullptr)
			{
				itemCount = managerBase->Count;
			}
		}
				
		if(itemCount > MaxDropDownItems)
		{
			size.Height = itemHeight * (MaxDropDownItems);
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
		System::Windows::Forms::ListBox^ listBox = EditingControl;
		listBox->DataSource = value;
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
		int selIndex = EditingControl->IndexFromPoint(e->Location);
		EditingControl->SelectedIndex = selIndex;
	}

	void ColumnComboBox::listBox_OnMouseClick(object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e)
	{
		if(e->Button == System::Windows::Forms::MouseButtons::Left)
			this->EditingResult = _EditingResult::Ok;
		else
			this->EditingResult = _EditingResult::Cancel;
	}

	void ColumnComboBox::listBox_OnFontChanged(object^ /*sender*/, _EventArgs^ /*e*/)
	{

	}

	object^ ColumnComboBox::GetEditingValue(System::Windows::Forms::ListBox^ control)
	{
		return control->SelectedItem;
	}

	void ColumnComboBox::SetEditingValue(System::Windows::Forms::ListBox^ control, object^ value)
	{
		control->SelectedItem = value;
	}

	ColumnPathSelector::ColumnPathSelector()
	{
		m_folderBrowerDialog = gcnew System::Windows::Forms::FolderBrowserDialog();
	}

	void ColumnPathSelector::OnEditValue(EditValueEventArgs^ e)
	{
		ColumnModal::OnEditValue(e);
		if(e->Value != nullptr)
			m_folderBrowerDialog->SelectedPath = e->Value->ToString();
		System::Windows::Forms::DialogResult dr = m_folderBrowerDialog->ShowDialog();
		if(dr == System::Windows::Forms::DialogResult::OK)
			e->Value = m_folderBrowerDialog->SelectedPath;
		e->SuppressEditing = true;
	}
} /*namespace Extensiton*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/