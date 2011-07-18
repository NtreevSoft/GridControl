#include "StdAfx.h"
#include "GridColumnExtension2.h"
#include "GridDebug.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Columns
{
	ColumnCheckBox::ColumnCheckBox()
	{
		using namespace System::Windows::Forms::VisualStyles;

		EditingControl->CheckAlign = System::Drawing::ContentAlignment::MiddleCenter;
		EditingControl->CheckedChanged += gcnew _EventHandler(this, &ColumnCheckBox::checkBox_CheckedChanged);

		try
		{
			m_rendererChecked = gcnew VisualStyleRenderer(System::Windows::Forms::VisualStyles::VisualStyleElement::Button::CheckBox::CheckedNormal);
			m_rendererUnchecked = gcnew VisualStyleRenderer(System::Windows::Forms::VisualStyles::VisualStyleElement::Button::CheckBox::UncheckedNormal);
		}
		catch(System::Exception^)
		{

		}
	}

	void ColumnCheckBox::checkBox_CheckedChanged(object^ /*sender*/, _EventArgs^ /*e*/)
	{

	}

	void ColumnCheckBox::SetEditingValue(CheckBox^ control, object^ value)
	{
		if(value == nullptr)
		{
			control->Checked = false;
			return;
		}

		if(value->GetType() == bool::typeid)
			control->Checked = (bool)value;
		else
			control->Checked = value->ToString() == System::Boolean::TrueString;
	}

	object^ ColumnCheckBox::GetEditingValue(CheckBox^ control)
	{
		if(DataType == bool::typeid)
		{
			return control->Checked;
		}
		return control->Checked.ToString();
	}
	
	void ColumnCheckBox::OnEditValue(EditValueEventArgs^ e)
	{
		ColumnControl::OnEditValue(e);
		EditingReason^ by = e->By;
		if(by->EditingReasonType == EditingReasonType::Key)
		{
			switch(by->Key)
			{
			case Keys::D1:
				{
					if(DataType == bool::typeid)
						e->Value = true;
					else
						e->Value = System::Boolean::TrueString;
					
					e->SuppressEditing = true;
				}
				break;
			case Keys::D0:
				{
					if(DataType == bool::typeid)
						e->Value = false;
					else
						e->Value = System::Boolean::FalseString;
					e->SuppressEditing = true;
				}
				break;
			case Keys::F2:
			case Keys::Space:
			case Keys::Enter:
				{
					if(DataType == bool::typeid)
					{
						e->Value = !(bool)e->Value;
					}
					else
					{
						if(e->Value->ToString() == System::Boolean::TrueString)
							e->Value = System::Boolean::FalseString;
						else
							e->Value = System::Boolean::TrueString;
					}
				
					e->SuppressEditing = true;
				}
				break;
			}
		}
	}

	bool ColumnCheckBox::KeyTest(Keys Key)
	{
		switch(Key)
		{
		case Keys::D1:
		case Keys::D0:
		case Keys::Space:
			return true;
		}
		return false;
	}

	void ColumnCheckBox::PaintValue(_Graphics^ graphics, _Rectangle renderRect, _Rectangle /*clipRect*/, IStyle^ /*style*/, object^ value)
	{
		bool checked = false;
		if(value == nullptr)
		{
			checked = false;
		}
		else if(value->GetType() == bool::typeid)
		{
			checked = (bool)value;
		}
		else
		{
			if(value->ToString() == System::Boolean::TrueString)
				checked = true;
			else
				checked = false;
		}

		// 길이가 홀수일때 렌더링 위치가 어긋남.
		if(renderRect.Width % 2 == 1)
			renderRect.X--;
		if(renderRect.Height % 2 == 1)
			renderRect.Y--;

		if(checked == true)
		{
			if(m_rendererChecked != nullptr)
				m_rendererChecked->DrawBackground(graphics, renderRect);
			else
			{
				int x1 = renderRect.Width/2 - 6 + renderRect.Left;
				int y1 = renderRect.Height/2 - 6 + renderRect.Top;
				System::Windows::Forms::ControlPaint::DrawCheckBox(graphics, x1, y1, 13, 13, System::Windows::Forms::ButtonState::Checked);
			}
		}
		else
		{
			if(m_rendererUnchecked != nullptr)
				m_rendererUnchecked->DrawBackground(graphics, renderRect);
			else
			{
				int x1 = renderRect.Width/2 - 6 + renderRect.Left;
				int y1 = renderRect.Height/2 - 6 + renderRect.Top;
				System::Windows::Forms::ControlPaint::DrawCheckBox(graphics, x1, y1, 13, 13, System::Windows::Forms::ButtonState::Normal);
			}
		}
	}

	Column^ CreateCheckBox()
	{
		return gcnew ColumnCheckBox();
	}
} /*namespace Extensiton*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/