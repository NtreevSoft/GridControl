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
#include "TypeEditorForm.h"
#include "Column.h"
#include "GridControl.h"
#include "NativeUtilities.h"
#include "Cell.h"
#include "Row.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
	void TypeEditorForm::MessageFilter::Start(System::IntPtr mainHandle, Ntreev::Windows::Forms::Grid::Design::TypeEditorForm^ dropDownForm)
	{
		System::Windows::Forms::Application::AddMessageFilter(this);
		AssignHandle(mainHandle);

		m_dropDownForm = dropDownForm;
	}

	void TypeEditorForm::MessageFilter::Stop()
	{
		System::Windows::Forms::Application::RemoveMessageFilter(this);
		ReleaseHandle();
	}

	bool TypeEditorForm::MessageFilter::PreFilterMessage(System::Windows::Forms::Message% m)
	{
		using namespace System::Windows::Forms;
		Native::WM wm = (Native::WM)m.Msg;

		switch(wm)
		{
		case Native::WM::WM_NCLBUTTONDBLCLK:
		case Native::WM::WM_NCLBUTTONDOWN:
		case Native::WM::WM_NCRBUTTONDOWN:
		case Native::WM::WM_NCMBUTTONDOWN:
		case Native::WM::WM_LBUTTONDBLCLK:
		case Native::WM::WM_LBUTTONDOWN:
		case Native::WM::WM_RBUTTONDOWN:
		case Native::WM::WM_MBUTTONDOWN:
			{
				System::Drawing::Point cursorPos = System::Windows::Forms::Cursor::Position;

				if (!m_dropDownForm->Bounds.Contains(cursorPos))
				{
					if(m.HWnd != m_dropDownForm->GridControl->Handle)
					{
						System::IntPtr mainHandle = Native::Methods::GetRootWindow(m.HWnd);
						System::IntPtr mainHandle2 = Native::Methods::GetRootWindow(Native::Methods::WindowFromPoint(cursorPos));

						if(mainHandle2 != m_dropDownForm->MainHandle)
						{
							uint idcont = Native::Methods::GetWindowThreadProcessId(m.HWnd);
							uint idform = Native::Methods::GetWindowThreadProcessId(m_dropDownForm->Handle);

							if(idcont == idform)
								break;
						}
					}

					m_dropDownForm->Close_Ntreev_Windows_Forms_Grid_Design_IEditorService();

					Ntreev::Windows::Forms::Grid::GridControl^ gridControl = m_dropDownForm->GridControl;
					System::Drawing::Point localPos = gridControl->PointToClient(cursorPos);

					Ntreev::Windows::Forms::Grid::Cell^ cell = gridControl->GetCellAt(localPos);
					if(cell != nullptr)
					{
						if(cell->IsFocused == false)
						{
							m.HWnd = gridControl->Handle;
							m.LParam = Native::Methods::MakeLParam(localPos.X, localPos.Y);
							Native::Methods::SendMessage(m);
						}
					}
					else
					{
						Native::Methods::PostMessage(m);
					}
					return true;
				}
			} 
			break;
		}

		return false;
	}

	void TypeEditorForm::MessageFilter::WndProc(System::Windows::Forms::Message% m)
	{
		switch((Native::WM)m.Msg)
		{
		case Native::WM::WM_NCACTIVATE:
			{
				if(((int)m.WParam) == 0)
				{
					m.WParam = System::IntPtr(1);
					m.LParam = System::IntPtr::Zero;
				}
			}
			break;
		case Native::WM::WM_ACTIVATEAPP:
			{
				if ((int)m.WParam == 0)
				{
					m_dropDownForm->Close_Ntreev_Windows_Forms_Grid_Design_IEditorService();
					Native::Methods::PostMessage(Handle, Native::WM::WM_NCACTIVATE, System::IntPtr::Zero, System::IntPtr::Zero);
				}
			}
		case Native::WM::WM_ACTIVATE:
			{
				if (m.WParam == System::IntPtr(1))
				{
					
				}
			}
			break;
		}

		NativeWindow::WndProc(m);
	}

	TypeEditorForm::TypeEditorForm(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, Ntreev::Windows::Forms::Grid::ICell^ cell, EditingReason reason)
		: m_gridControl(gridControl), m_cell(cell), m_reason(reason)
	{
		using namespace System::Windows::Forms::VisualStyles;

		InitializeComponent();

		m_gridControl->EditClosed += gcnew System::EventHandler(this, &TypeEditorForm::gridControl_EditClosed);

		m_bounds = m_gridControl->RectangleToScreen(cell->Bounds);
		m_clientBounds = cell->ClientRectangle;
		m_clientBounds.Offset(cell->Bounds.Location);
		m_clientBounds = m_gridControl->RectangleToScreen(m_clientBounds);

		System::Drawing::Rectangle gs = GetClipScreenRect(m_gridControl);

		m_clientBounds = System::Drawing::Rectangle::Intersect(m_clientBounds, gs);
		m_mainHandle = Native::Methods::GetRootWindow(m_gridControl->Handle);

		try
		{
			m_gripperRenderer = gcnew VisualStyleRenderer(VisualStyleElement::Status::Gripper::Normal);
		}
		catch(System::Exception^)
		{

		}
	}

	TypeEditorForm::~TypeEditorForm()
	{
		m_filter.Stop();
		m_gridControl->EditClosed -= gcnew System::EventHandler(this, &TypeEditorForm::gridControl_EditClosed);
		this->Controls->Clear();

		if(m_control != nullptr)
		{
			m_control->Resize -= gcnew System::EventHandler(this, &TypeEditorForm::control_Resize);
			m_control->Size = m_controlSize;
		}
		m_control = nullptr;
		if (m_gripperRenderer != nullptr)
		{
			delete m_gripperRenderer;
		}

		m_gridControl->Select();
	}

	System::Drawing::Rectangle TypeEditorForm::GetClipScreenRect(System::Windows::Forms::Control^ control)
	{
		System::Windows::Forms::Control^ parent = control->Parent;

		if (parent == nullptr)
		{
			return control->RectangleToScreen(control->DisplayRectangle);
		}

		System::Drawing::Rectangle s = control->RectangleToScreen(control->DisplayRectangle);
		System::Drawing::Rectangle e = GetClipScreenRect(parent);

		return System::Drawing::Rectangle::Intersect(e, s);
	}

	void TypeEditorForm::OnSizeChanged(System::EventArgs^ e)
	{
		Form::OnSizeChanged(e);
		const int size = 50;
		m_resizableRect = System::Drawing::Rectangle(ClientRectangle.Right - size, ClientRectangle.Bottom - size, size, size);
	}

	void TypeEditorForm::OnPaint(System::Windows::Forms::PaintEventArgs^ e) 
	{
		Form::OnPaint(e);
		if(m_resizable == true)
		{
			System::Drawing::Rectangle rect((this->Width) - 18, (this->Height) - 20, 20, 20);
			m_gripperRenderer->DrawBackground(e->Graphics, rect);
		}
	}

	void TypeEditorForm::OnMouseUp(System::Windows::Forms::MouseEventArgs^ e) 
	{
		Form::OnMouseUp(e);
		this->Capture = false;
		m_dragging = false;
	}

	void TypeEditorForm::OnMouseDown(System::Windows::Forms::MouseEventArgs^ e) 
	{
		Form::OnMouseDown(e);
		if(m_resizable == false)
			return;

		if(m_resizableRect.Contains(e->Location) && e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			m_mouseDownPoint = e->Location;
			m_mouseMovePoint = e->Location;
			m_firstSize = this->Size;
			m_dragging = true;
			this->Capture = true;
		}
	}

	void TypeEditorForm::OnMouseMove(System::Windows::Forms::MouseEventArgs^ e) 
	{
		Form::OnMouseMove(e);

		if(m_resizable == false)
			return;

		if(m_resizableRect.Contains(e->Location))
			this->Cursor = System::Windows::Forms::Cursors::SizeNWSE;
		else
			this->Cursor = System::Windows::Forms::Cursors::Arrow;

		if(m_dragging == false || e->Button != System::Windows::Forms::MouseButtons::Left)
			return;

		if(e->Location == m_mouseMovePoint)
			return;

		System::Drawing::Point offset = e->Location - (System::Drawing::Size)m_mouseDownPoint;
		this->Size = m_firstSize + (System::Drawing::Size)offset;

		m_mouseMovePoint = e->Location;
		this->Invalidate();
	}

	void TypeEditorForm::OnMouseLeave(System::EventArgs^ e)
	{
		Form::OnMouseLeave(e);
		Cursor = System::Windows::Forms::Cursors::Default;
	}

	void TypeEditorForm::OnLayout(System::Windows::Forms::LayoutEventArgs^ levent)
	{
		try
		{
			this->m_resizing = true;
			Form::OnLayout(levent);
		}
		finally
		{
			this->m_resizing = false;
		}
	}

	void TypeEditorForm::OnVisibleChanged(System::EventArgs^ e)
	{
		if(this->Visible == false)
		{
			System::Drawing::Point cursorPos = System::Windows::Forms::Cursor::Position;

			cursorPos = m_gridControl->PointToClient(cursorPos);
			m_isin = m_gridControl->DisplayRectangle.Contains(cursorPos);
		}

		Form::OnVisibleChanged(e);
	}

	bool TypeEditorForm::ProcessCmdKey(System::Windows::Forms::Message% msg, System::Windows::Forms::Keys keyData)
	{
		bool result = Form::ProcessCmdKey(msg, keyData);

		return result;
	}

	bool TypeEditorForm::ProcessDialogKey(System::Windows::Forms::Keys keyData)
	{
		bool result = Form::ProcessDialogKey(keyData);
		if(keyData == System::Windows::Forms::Keys::Enter && result == false)
			this->Close_Ntreev_Windows_Forms_Grid_Design_IEditorService();
		if(keyData == System::Windows::Forms::Keys::Escape && result == false)
			this->CancelEdit();
		return result;
	}

	void TypeEditorForm::IsDropDownResizable::set(bool value)
	{
		if(value == true)
			this->Padding = System::Windows::Forms::Padding(0, 0, 0, m_paddingSize);
		else
			this->Padding = System::Windows::Forms::Padding(0, 0, 0, 0);
		m_resizable = value;
	}

	void TypeEditorForm::DefaultResult::set(bool value)
	{
		m_defaultResult = value;
	}

	EditingReasonType TypeEditorForm::ReasonType::get()
	{
		return m_reason.ReasonType;
	}

	void TypeEditorForm::Rectangle::set(System::Drawing::Rectangle value)
	{
		value.Height += ((Size.Height - ClientSize.Height) + Padding.Vertical);

		System::Drawing::Rectangle screenRect = System::Windows::Forms::Screen::FromRectangle(value)->WorkingArea;

		if(value.Height > screenRect.Height)
			value.Height = screenRect.Height;

		if(value.Bottom > screenRect.Bottom)
			value.Y = screenRect.Bottom - value.Height;

		if(value.Left < screenRect.Left)
			value.X = screenRect.Left;
		else if(value.Right > screenRect.Right)
			value.X = screenRect.Right - value.Width;

		this->Location = value.Location;
		this->Size = value.Size;
	}

	Ntreev::Windows::Forms::Grid::GridControl^ TypeEditorForm::GridControl::get()
	{
		return m_gridControl; 
	}

	System::Windows::Forms::DialogResult TypeEditorForm::Result::get()
	{
		return this->DialogResult;
	}

	System::IntPtr TypeEditorForm::MainHandle::get()
	{
		return m_mainHandle;
	}

	void TypeEditorForm::CloseDropDown()
	{
		Close_Ntreev_Windows_Forms_Grid_Design_IEditorService();
	}

	void TypeEditorForm::DropDownControl(System::Windows::Forms::Control^ control)
	{
		using namespace System::Windows::Forms;
		m_filter.Start(m_mainHandle, this);

		System::IntPtr handle = this->Handle;

		this->SuspendLayout();
		control->ImeMode = m_gridControl->ImeMode;
		m_controlSize = control->Size;
		this->Controls->Add(control);
		System::Drawing::Size size(control->Width + 2, control->Height + 2);
		if(m_resizable == true)
			size.Height += m_paddingSize;
		m_control = control;

		control->Dock = DockStyle::Fill;
		control->Visible = true;
		this->ResumeLayout(true);

		control->Resize += gcnew System::EventHandler(this, &TypeEditorForm::control_Resize);

		size.Width = System::Math::Max(m_bounds.Width, size.Width);
		size.Height = System::Math::Max(m_bounds.Height, size.Height);
		this->SetBounds(m_bounds.X, m_bounds.Bottom, size.Width, size.Height);
		this->Visible = true;

		Native::Methods::SendMessage(this->Handle, Native::WM::WM_LBUTTONDOWN, System::IntPtr::Zero, System::IntPtr::Zero);
		Native::Methods::SendMessage(this->Handle, Native::WM::WM_LBUTTONUP, System::IntPtr::Zero, System::IntPtr::Zero);

		Native::Methods::DoEventsModal(this);
		m_filter.Stop();
	}

	void TypeEditorForm::ShowControl(System::Windows::Forms::Control^ control)
	{
		using namespace System::Windows::Forms;

		if(m_reason.ReasonType == EditingReasonType::Mouse)
		{
			System::Windows::Forms::Message msg;
			System::Drawing::Point location = m_reason.Location;
			msg.HWnd = m_gridControl->Handle;
			msg.Msg = (int)Native::WM::WM_LBUTTONUP;
			msg.WParam = System::IntPtr(1);
			msg.LParam = Native::Methods::MakeLParam(location.X,location.Y);
			Native::Methods::SendMessage(msg);
		}

		m_filter.Start(m_mainHandle, this);
		System::IntPtr handle = this->Handle;

		this->SuspendLayout();
		control->ImeMode = m_gridControl->ImeMode;

		this->ImeMode = control->ImeMode;
		this->BackColor = control->BackColor;
		this->ForeColor = control->ForeColor;
		this->Font = control->Font;
		this->Controls->Add(control);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		this->Bounds = m_clientBounds;
		this->ResumeLayout(false);

		m_gridControl->Update();
		this->Visible = true;
		this->Activate();

		this->PreProcessEvent(control);
		System::Collections::ArrayList events;
		//Native::Methods::DoEvents(events);
		this->PostProcessEvent(control);

		if(events.Count > 0)
		{
			System::Windows::Forms::Message m = (System::Windows::Forms::Message)events[0];
			Native::Methods::SendMessage(m);
		}

		Native::Methods::DoEventsModal(this);
		m_filter.Stop();

		m_gridControl->ImeMode = control->ImeMode;
	}

	void TypeEditorForm::PreProcessEvent(System::Windows::Forms::Control^ control)
	{
		using namespace System::Windows::Forms;
		switch(m_reason.ReasonType)
		{
		case EditingReasonType::Mouse:
			{
				System::Windows::Forms::Message msg;
				System::Drawing::Point location = m_gridControl->PointToScreen(m_reason.Location);
				System::Windows::Forms::Control^ hitTest = m_gridControl->GetChildAt(control, location);
				if(hitTest == nullptr)
					hitTest = control;
				location = hitTest->PointToClient(location);

				msg.HWnd = hitTest->Handle;
				msg.Msg = (int)Native::WM::WM_MOUSEMOVE;
				msg.WParam = System::IntPtr(0);
				msg.LParam = Native::Methods::MakeLParam(location.X, location.Y);
				//Native::Methods::SendMessage(msg);

				msg.HWnd = hitTest->Handle;
				msg.Msg = (int)Native::WM::WM_LBUTTONDOWN;
				msg.WParam = System::IntPtr(1);
				msg.LParam = Native::Methods::MakeLParam(location.X, location.Y);
				//Native::Methods::SendMessage(msg);
			}
			break;
		case EditingReasonType::Key:
			{
				Message msg;
				msg.HWnd = ActiveControl->Handle;
				msg.Msg = (int)Native::WM::WM_KEYDOWN;
				msg.WParam = System::IntPtr((int)m_reason.Key);
				if(m_reason.Key != System::Windows::Forms::Keys::Enter)
					Native::Methods::SendMessage(msg);
			}
			break;
		case EditingReasonType::Char:
			{
				int vk = Native::Methods::VkKeyScan(m_reason.Char);

				Message msg;
				msg.HWnd = ActiveControl->Handle;
				msg.Msg = (int)Native::WM::WM_KEYDOWN;
				msg.WParam = System::IntPtr(vk);
				Native::Methods::TranslateAndDispatchMessage(msg);
			}
			break;
		case EditingReasonType::Ime:
			{
				System::Windows::Forms::Message msg;
				msg.HWnd = ActiveControl->Handle;
				msg.Msg = (int)Native::WM::WM_KEYDOWN;
				msg.WParam = System::IntPtr((int)Keys::ProcessKey);
				msg.Result = System::IntPtr((int)0x1e0001);
				Native::Methods::TranslateAndDispatchMessage(msg);
			}
			break;
		}
	}

	void TypeEditorForm::PostProcessEvent(System::Windows::Forms::Control^ control)
	{
		using namespace System::Windows::Forms;

		switch(m_reason.ReasonType)
		{
		case EditingReasonType::Mouse:
			{
				System::Windows::Forms::Message msg;
				System::Drawing::Point location = m_gridControl->PointToScreen(m_reason.Location);
				System::Windows::Forms::Control^ hitTest = m_gridControl->GetChildAt(control, location);
				if(hitTest == nullptr)
					hitTest = control;
				location = hitTest->PointToClient(location);

				msg.HWnd = hitTest->Handle;
				msg.Msg = (int)Native::WM::WM_MOUSEMOVE;
				msg.WParam = System::IntPtr(0);
				msg.LParam = Native::Methods::MakeLParam(location.X, location.Y);
				Native::Methods::SendMessage(msg);

				msg.HWnd = hitTest->Handle;
				msg.Msg = (int)Native::WM::WM_LBUTTONDOWN;
				msg.WParam = System::IntPtr(1);
				msg.LParam = Native::Methods::MakeLParam(location.X,location.Y);
				Native::Methods::SendMessage(msg);
			}
			break;
		}
	}

	System::Windows::Forms::DialogResult TypeEditorForm::ShowDialog_System_Windows_Forms_Design_IWindowsFormsEditorService(System::Windows::Forms::Form^ dialog)
	{
		this->DialogResult = dialog->ShowDialog();
		return this->DialogResult;
	}

	System::Object^ TypeEditorForm::GetService_System_IServiceProvider(System::Type^ serviceType)
	{
		if(serviceType == System::Windows::Forms::Design::IWindowsFormsEditorService::typeid)
			return this;
		if(serviceType == Ntreev::Windows::Forms::Grid::Design::IEditorService::typeid)
			return this;
		if(serviceType == System::ComponentModel::ITypeDescriptorContext::typeid)
			return this;
		if(serviceType == System::IServiceProvider::typeid)
			return this;
		if(serviceType == Ntreev::Windows::Forms::Grid::GridControl::typeid)
			return this->GridControl;
		if(serviceType == Ntreev::Windows::Forms::Grid::ITextCacheProvider::typeid)
			return dynamic_cast<Ntreev::Windows::Forms::Grid::ITextCacheProvider^>(m_cell->Column);

		return GetService(serviceType);
	}

	void TypeEditorForm::OnComponentChanged()
	{

	}

	bool TypeEditorForm::OnComponentChanging()
	{
		return false;
	}

	System::ComponentModel::IContainer^ TypeEditorForm::Container_System_ComponentModel_ITypeDescriptorContext::get()
	{
		return m_gridControl->Container;
	}

	System::Object^ TypeEditorForm::Instance_System_ComponentModel_ITypeDescriptorContext::get()
	{
		Ntreev::Windows::Forms::Grid::Row^ row = dynamic_cast<Ntreev::Windows::Forms::Grid::Row^>(m_gridControl->FocusedRow);
		if(row == nullptr)
			return nullptr;
		return row->Component;
	}

	System::ComponentModel::PropertyDescriptor^ TypeEditorForm::PropertyDescriptor_System_ComponentModel_ITypeDescriptorContext::get()
	{
		return nullptr;
	}

	void TypeEditorForm::Close_Ntreev_Windows_Forms_Grid_Design_IEditorService()
	{
		this->DialogResult = System::Windows::Forms::DialogResult::OK;
	}

	void TypeEditorForm::CancelEdit()
	{
		this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	}

	void TypeEditorForm::ShowDialog(System::Windows::Forms::Control^ control)
	{
		this->ControlBox = true;
		this->MaximizeBox = true;
		this->MinimizeBox = true;
		this->ShowInTaskbar = true;
		this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Sizable;
		this->Controls->Add(control);

		ShowDialog();
	}

	void TypeEditorForm::control_Resize(System::Object^ /*sender*/, System::EventArgs^ /*e*/)
	{
		if ((m_control != nullptr) && m_resizing == false)
		{
			int width = this->Width;
			System::Drawing::Size size(m_control->Width + 2, m_control->Height + 2);

			try
			{
				this->m_resizing = true;
				this->SuspendLayout();
				this->Size = size;
			}
			finally
			{
				this->m_resizing = false;
				this->ResumeLayout(false);
			}
			this->Left -= this->Width - width;
		}
	}

	void TypeEditorForm::gridControl_EditClosed(System::Object^ /*sender*/, System::EventArgs^ /*e*/)
	{
		this->CancelEdit();
	}
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/
