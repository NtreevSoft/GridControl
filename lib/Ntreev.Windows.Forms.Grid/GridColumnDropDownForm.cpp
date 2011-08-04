#include "StdAfx.h"
#include "GridColumnDropDownForm.h"
#include "GridColumn.h"
#include "GridControl.h"
#include "GridDebug.h"
#include "GridWin32.h"
#include "GridCell.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Private
{
	ColumnDropDownForm::MessageFilter::MessageFilter()
	{

	}

	bool ColumnDropDownForm::MessageFilter::PreFilterMessage(System::Windows::Forms::Message% m)
	{
		if (m_dropDownForm != nullptr)
		{
			switch (m.Msg)
			{				
			case Win32::API::WM_LBUTTONDBLCLK:
			case Win32::API::WM_LBUTTONDOWN:
			case Win32::API::WM_RBUTTONDOWN:
			case Win32::API::WM_MBUTTONDOWN:
			case Win32::API::WM_NCLBUTTONDBLCLK:
			case Win32::API::WM_NCLBUTTONDOWN:
			case Win32::API::WM_NCRBUTTONDOWN:
			case Win32::API::WM_NCMBUTTONDOWN:
				{
					_Point cursorPos = System::Windows::Forms::Cursor::Position;
					if (!m_dropDownForm->Bounds.Contains(cursorPos))
					{
						_GridControl^ gridControl = m_dropDownForm->GridControl;
						m_dropDownForm->Visible = false;
						m_dropDownForm = nullptr;

						_Rectangle rect = gridControl->RectangleToScreen(gridControl->DisplayRectangle);
						if(rect.Contains(cursorPos) == true)
						{
							_Point clientPos = gridControl->PointToClient(cursorPos);
							_Cell^ cell = gridControl->GetCellAt(clientPos);
							if(cell != nullptr)
							{
								return cell->IsFocused;
							}
						}
					}
				}				
				break;
			}
		}
		return false;
	}

	void ColumnDropDownForm::MessageFilter::WndProc(System::Windows::Forms::Message% m)
	{
		NativeWindow::WndProc(m);
		if (m_dropDownForm != nullptr)
		{
			if(m.Msg == Win32::API::WM_NCACTIVATE)
			{
				if(((int)m.WParam) == 0)
				{
					Win32::API::SendMessage(Handle, Win32::API::WM_NCACTIVATE, System::IntPtr(1), System::IntPtr::Zero);
				}

			}
			else if(m.Msg == Win32::API::WM_ACTIVATEAPP)
			{
				if ((int)m.WParam == 0)
				{
					m_dropDownForm->Visible = false;
					m_dropDownForm = nullptr;
					Win32::API::PostMessage(Handle, Win32::API::WM_NCACTIVATE, System::IntPtr::Zero, System::IntPtr::Zero);
				}
			}
		}
	}

	ColumnDropDownForm::ColumnDropDownForm(_GridControl^ gridControl)
		: m_gridControl(gridControl)
	{
		this->SuspendLayout();
		// 
		// ColumnDropDownForm
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->BackColor = System::Drawing::SystemColors::Control;
		this->ClientSize = System::Drawing::Size(212, 265);
		this->ControlBox = false;
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->Name = L"ColumnDropDownForm";
		this->Padding = System::Windows::Forms::Padding(0, 0, 0, 12);
		this->ShowInTaskbar = false;
		this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		this->TopMost = true;
		this->DoubleBuffered	= true;
		this->ResumeLayout(false);

		m_filter = gcnew MessageFilter();
		m_sizable = false;

		try
		{
			visualRenderer	= gcnew _VisualStyleRenderer(_VisualStyleElement::Status::Gripper::Normal);
		}
		catch(System::Exception^)
		{
			
		}
	}

	ColumnDropDownForm::~ColumnDropDownForm()
	{
		if (visualRenderer != nullptr)
		{
			delete visualRenderer;
		}
	}

	void ColumnDropDownForm::OnSizeChanged(_EventArgs^ e)
	{
		Form::OnSizeChanged(e);
		const int size = Padding.Bottom;
		m_sizableRect = _Rectangle(ClientRectangle.Right - size, ClientRectangle.Bottom - size, size, size);
		//Invalidate(_Rectangle(ClientRectangle.Left, ClientRectangle.Bottom - nSize, ClientRectangle.Width, nSize));
		//PerformLayout();
		Refresh();
	}

	void ColumnDropDownForm::OnPaint(System::Windows::Forms::PaintEventArgs^ e) 
	{
		Form::OnPaint(e);
		if(m_sizable == true)
		{
			if(visualRenderer)
				visualRenderer->DrawBackground(e->Graphics, m_sizableRect);
			else
				System::Windows::Forms::ControlPaint::DrawSizeGrip(e->Graphics, this->BackColor, m_sizableRect);
		}
	}

	void ColumnDropDownForm::OnMouseUp(System::Windows::Forms::MouseEventArgs^ e) 
	{
		Form::OnMouseUp(e);
		this->Capture	= false;
		m_sizing		= false;
	}

	void ColumnDropDownForm::OnMouseDown(System::Windows::Forms::MouseEventArgs^ e) 
	{
		Form::OnMouseDown(e);
		if(m_sizable == false)
			return;

		if(m_sizableRect.Contains(e->Location) && e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			m_lButtonDownPoint	= e->Location;
			m_mouseMovePoint	= e->Location;
			m_firstSize		= this->Size;
			m_sizing		= true;
			this->Capture	= true;
		}
	}

	void ColumnDropDownForm::OnMouseMove(System::Windows::Forms::MouseEventArgs^ e) 
	{
		Form::OnMouseMove(e);
		if(m_sizable == false || e->Location == m_mouseMovePoint)
			return;

		if(m_sizableRect.Contains(e->Location))
			Cursor = System::Windows::Forms::Cursors::SizeNWSE;
		else
			Cursor = System::Windows::Forms::Cursors::Arrow;

		if(m_sizing == true)
		{
			int xOffset = e->Location.X - m_lButtonDownPoint.X;
			int yOffset = e->Location.Y - m_lButtonDownPoint.Y;

			this->Size = _Size(m_firstSize.Width + xOffset, m_firstSize.Height	+ yOffset);
			//PerformLayout();
		}

		m_mouseMovePoint = e->Location;
	}

	void ColumnDropDownForm::OnMouseLeave(_EventArgs^ e)
	{
		Form::OnMouseLeave(e);
		Cursor = System::Windows::Forms::Cursors::Arrow;
	}

	void ColumnDropDownForm::OnVisibleChanged(System::EventArgs^ e)
	{
		if(this->Visible == false)
		{
			this->Owner = nullptr;
			this->Controls->Clear();

			System::Windows::Forms::Application::RemoveMessageFilter(m_filter);
			m_filter->ReleaseHandle();

			System::Drawing::Point cursorPos = System::Windows::Forms::Cursor::Position;

			cursorPos = m_gridControl->PointToClient(cursorPos);
			m_isin = m_gridControl->DisplayRectangle.Contains(cursorPos);
		}

		Form::OnVisibleChanged(e);
	}

	void ColumnDropDownForm::Sizable::set(bool sizable)
	{
		if(sizable)
			this->Padding = System::Windows::Forms::Padding(0, 0, 0, 12);
		else
			this->Padding = System::Windows::Forms::Padding(0, 0, 0, 0);
		m_sizable = sizable;
	}

	void ColumnDropDownForm::Rectangle::set(_Rectangle value)
	{
		value.Height += ((Size.Height - ClientSize.Height) + Padding.Vertical);

		_Rectangle screenRect = System::Windows::Forms::Screen::FromRectangle(value)->WorkingArea;

		if(value.Height > screenRect.Height)
			value.Height = screenRect.Height;

		if(value.Bottom > screenRect.Bottom)
			value.Y = screenRect.Bottom - value.Height;

		if(value.Left < screenRect.Left)
			value.X = screenRect.Left;
		else if(value.Right > screenRect.Right)
			value.X = screenRect.Right - value.Width;

		this->Location = value.Location;
		this->Size	   = value.Size;
	}

	void ColumnDropDownForm::Show(IColumnDropDown^ column, _Cell^ editingCell, _Rectangle rect)
	{
		System::Windows::Forms::Application::AddMessageFilter(m_filter);
		System::Windows::Forms::Form^ owner = m_gridControl->FindForm();
		while(owner->ParentForm != nullptr)
			owner = owner->ParentForm;
		
		Debug::WriteLine("ShowPopup : {0}", owner->Name);

		m_filter->DropDownForm = this;
		m_filter->AssignHandle(owner->Handle);
		_Control^ control = column->EditingControl;
		delete control->CreateGraphics();

		this->SuspendLayout();
		
		_Point location(rect.Left, rect.Bottom);
		_Size size = column->GetPreferredSize(rect.Size);

		this->Font	= editingCell->Font;
		this->Sizable	= column->Sizable;

		this->Controls->Add(control);
		this->Rectangle	= _Rectangle(location, size);
		
		this->Owner = owner;
		//owner->AddOwnedForm(this);
		//this->Tag = owner;

		this->ResumeLayout(false);
		this->Show();
		this->Activate();
	}
} /*namespace Private*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/