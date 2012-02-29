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
#include "GridTypeEditorForm.h"
#include "GridColumn.h"
#include "GridControl.h"
#include "GridWin32.h"
#include "GridCell.h"

bool sadfsfas = false;

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    void TypeEditorForm::MessageFilter::Start(System::Windows::Forms::Form^ mainForm, TypeEditorForm^ dropDownForm)
    {
        System::Windows::Forms::Application::AddMessageFilter(this);
        AssignHandle(mainForm->Handle);

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
        Win32::WM wm = (Win32::WM)m.Msg;
        //System::Windows::Forms::Control^ control = System::Windows::Forms::Control::FromHandle(m.HWnd);
        //if(control != nullptr)
        //    System::Diagnostics::Trace::WriteLine(control->GetType());

        //System::Diagnostics::Trace::Write("PreFilterMessage : ");
        //System::Diagnostics::Trace::WriteLine(wm);
        switch(wm)
        {
        case Win32::WM::WM_PAINT:
            {

                return sadfsfas;

            }
        case Win32::WM::WM_NCLBUTTONDBLCLK:
        case Win32::WM::WM_NCLBUTTONDOWN:
        case Win32::WM::WM_NCRBUTTONDOWN:
        case Win32::WM::WM_NCMBUTTONDOWN:
        case Win32::WM::WM_LBUTTONDBLCLK:
        case Win32::WM::WM_LBUTTONDOWN:
        case Win32::WM::WM_RBUTTONDOWN:
        case Win32::WM::WM_MBUTTONDOWN:
            {
                System::Drawing::Point cursorPos = System::Windows::Forms::Cursor::Position;

                if (!m_dropDownForm->Bounds.Contains(cursorPos))
                {
                    m_dropDownForm->CloseEdit();

                    Ntreev::Windows::Forms::Grid::GridControl^ gridControl = m_dropDownForm->GridControl;
                    System::Drawing::Point localPos = gridControl->PointToClient(cursorPos);

                    Cell^ cell = gridControl->GetCellAt(localPos);
                    if(cell != nullptr && cell->IsFocused == true)
                        return true;
                    Win32::API::PostMessage(m);
                    return true;
                }
            }    
            break;
        }

        return false;
    }

    void TypeEditorForm::MessageFilter::WndProc(System::Windows::Forms::Message% m)
    {
        NativeWindow::WndProc(m);

        switch((Win32::WM)m.Msg)
        {
        case Win32::WM::WM_NCACTIVATE:
            {
                if(((int)m.WParam) == 0)
                {
                    Win32::API::SendMessage(Handle, Win32::WM::WM_NCACTIVATE, System::IntPtr(1), System::IntPtr::Zero);
                }
            }
            break;
        case Win32::WM::WM_ACTIVATEAPP:
            {
                if ((int)m.WParam == 0)
                {
                    m_dropDownForm->CloseEdit();
                    Win32::API::PostMessage(Handle, Win32::WM::WM_NCACTIVATE, System::IntPtr::Zero, System::IntPtr::Zero);
                }
            }
        case Win32::WM::WM_ACTIVATE:
            {
                if (m.WParam == System::IntPtr(1))
                {
                    int qwer=0;
                    //m_dropDownForm->CancelEdit(false);
                }
            }
            break;
        }

        // Win32::WM wm = (Win32::WM)m.Msg;
        //System::Windows::Forms::Control^ control = System::Windows::Forms::Control::FromHandle(m.HWnd);
        //if(control != nullptr)
        //    System::Diagnostics::Trace::WriteLine(control->GetType());

        //System::Diagnostics::Trace::Write("WndProc : ");
        //System::Diagnostics::Trace::WriteLine(wm);
    }

    TypeEditorForm::TypeEditorForm(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, ICell^ cell, EditingReason reason)
        : m_gridControl(gridControl), m_cell(cell), m_reason(reason)
    {
        using namespace System::Windows::Forms::VisualStyles;

        InitializeComponent();

        m_bounds = m_gridControl->RectangleToScreen(cell->Bounds);
        m_clientBounds = cell->ClientRectangle;
        m_clientBounds.Offset(cell->Bounds.Location);
        m_clientBounds = m_gridControl->RectangleToScreen(m_clientBounds);

        System::Drawing::Rectangle gs = GetClipScreenRect(m_gridControl);


        //if(m_clientBounds.Right >= gs.Right)
        //{
        //    m_clientBounds.Width = gs.Right - m_clientBounds.Left - 2;
        //}


        m_clientBounds = System::Drawing::Rectangle::Intersect(m_clientBounds, gs);

        Form^ owner = m_gridControl->FindForm();
        while(owner->ParentForm != nullptr)
            owner = owner->ParentForm;

        m_parentForm = owner;

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
    }

    System::Drawing::Rectangle TypeEditorForm::GetClipScreenRect(System::Windows::Forms::Control^ control)
    {
        //using namespace System::Windows::Forms;
        //using namespace System::Drawing;
        System::Windows::Forms::Control^ parent = control->Parent;

        if (parent == nullptr)
        {
            return control->RectangleToScreen(control->DisplayRectangle);
        }

        System::Drawing::Rectangle s = control->RectangleToScreen(control->DisplayRectangle);
        //            s.Location = s.Location + (Size)control.Location;



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
            //if(m_gripperRenderer)
            //    m_gripperRenderer->DrawBackground(e->Graphics, m_resizableRect);
            //else
            //    System::Windows::Forms::ControlPaint::DrawSizeGrip(e->Graphics, this->BackColor, m_resizableRect);
            //e->Graphics->DrawRectangle(System::Drawing::Pens::Black, m_resizableRect);

            //System::Drawing::Rectangle r = this->DisplayRectangle;
            //r.Y = m_control->Bottom;
            //r.Height = this->DisplayRectangle.Bottom - r.Top;

            //e->Graphics->FillRectangle(System::Drawing::Brushes::Red, r);

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
            m_dragging  = true;
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
            //m_filter.Stop();

            System::Drawing::Point cursorPos = System::Windows::Forms::Cursor::Position;

            cursorPos = m_gridControl->PointToClient(cursorPos);
            m_isin = m_gridControl->DisplayRectangle.Contains(cursorPos);
        }

        Form::OnVisibleChanged(e);
    }

    bool TypeEditorForm::ProcessCmdKey(System::Windows::Forms::Message% msg, System::Windows::Forms::Keys keyData)
    {
        //switch(keyData)
        //{
        //case System::Windows::Forms::Keys::Escape:
        //    CancelEdit(true);
        //    return true;
        //}
        return Form::ProcessCmdKey(msg, keyData);
    }

    bool TypeEditorForm::ProcessDialogKey(System::Windows::Forms::Keys keyData)
    {
        bool result = Form::ProcessDialogKey(keyData);
        if(keyData == System::Windows::Forms::Keys::Enter && result == false)
            this->CloseEdit();
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
        this->Size    = value.Size;
    }

    //void TypeEditorForm::Show(IColumnDropDown^ column, Cell^ editingCell, System::Drawing::Rectangle rect)
    //{
    //    m_filter.Start(m_parentForm, this);

    //    System::Windows::Forms::Control^ control = column->EditingControl;
    //    delete control->CreateGraphics();

    //    this->SuspendLayout();

    //    System::Drawing::Point location(rect.Left, rect.Bottom);
    //    System::Drawing::Size size = column->GetPreferredSize(rect.Size);

    //    this->Font    = editingCell->Font;
    //    this->Sizable = column->Sizable;

    //    this->Controls->Add(control);
    //    this->Rectangle = System::Drawing::Rectangle(location, size);

    //    this->Owner = m_parentForm;
    //    this->ResumeLayout(false);
    //    this->Show();
    //    this->Activate();
    //}

    void TypeEditorForm::CloseDropDown()
    {
        CloseEdit();
    }

    void TypeEditorForm::DropDownControl(System::Windows::Forms::Control^ control)
    {
        using namespace System::Windows::Forms;
        m_filter.Start(m_parentForm, this);

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

        Win32::API::SendMessage(this->Handle, Win32::WM::WM_LBUTTONDOWN, System::IntPtr::Zero, System::IntPtr::Zero);
        Win32::API::SendMessage(this->Handle, Win32::WM::WM_LBUTTONUP, System::IntPtr::Zero, System::IntPtr::Zero);

        Win32::API::DoEventsModal(this);
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
            msg.Msg  = (int)Win32::WM::WM_LBUTTONUP;
            msg.WParam = System::IntPtr(1);
            msg.LParam = Win32::API::MakeLParam(location.X,location.Y);
            Win32::API::SendMessage(msg);
        }

        m_filter.Start(m_parentForm, this);

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

        this->Visible = true;
        this->Activate();


        PreProcessEvent(control);
        Win32::API::DoEvents();
        PostProcessEvent(control);

        Win32::API::DoEventsModal(this);
        m_filter.Stop();

        m_gridControl->ImeMode = control->ImeMode;
    }

    void TypeEditorForm::PreProcessEvent(System::Windows::Forms::Control^ control)
    {
        using namespace System::Windows::Forms;
        if(m_reason.ReasonType == EditingReasonType::Mouse)
        {
            System::Windows::Forms::Message msg;
            System::Drawing::Point location = m_gridControl->PointToScreen(m_reason.Location);;
            location = control->PointToClient(location);

            msg.HWnd = control->Handle;
            msg.Msg  = (int)Win32::WM::WM_LBUTTONDOWN;
            msg.WParam = System::IntPtr(1);
            msg.LParam = Win32::API::MakeLParam(location.X,location.Y);
            Win32::API::SendMessage(msg);
        }
    }

    void TypeEditorForm::PostProcessEvent(System::Windows::Forms::Control^ control)
    {
        using namespace System::Windows::Forms;
        switch(m_reason.ReasonType)
        {
        case EditingReasonType::Char:
            {
                int vk = Win32::API::VkKeyScan(m_reason.Char);

                Message msg;
                msg.HWnd = ActiveControl->Handle;
                msg.Msg  = (int)Win32::WM::WM_KEYDOWN;
                msg.WParam = System::IntPtr(vk);
                Win32::API::TranslateAndDispatchMessage(msg);
            }
            break;
        case EditingReasonType::Ime:
            {
                System::Char imeChar = Win32::API::ImmGetVirtualKey(m_gridControl->Handle);

                System::Windows::Forms::Message msg;
                msg.HWnd = ActiveControl->Handle;
                msg.Msg  = (int)Win32::WM::WM_KEYDOWN;
                msg.WParam = System::IntPtr((int)Keys::ProcessKey);
                msg.Result = System::IntPtr((int)0x1e0001);
                Win32::API::TranslateAndDispatchMessage(msg);
            }
            break;
        case EditingReasonType::Mouse:
            {
                System::Windows::Forms::Message msg;
                System::Drawing::Point location = m_gridControl->PointToScreen(m_reason.Location);;
                location = control->PointToClient(location);

                msg.HWnd = control->Handle;
                msg.Msg  = (int)Win32::WM::WM_LBUTTONDOWN;
                msg.WParam = System::IntPtr(1);
                msg.LParam = Win32::API::MakeLParam(location.X,location.Y);
                Win32::API::SendMessage(msg);

                //if(b == true)
                //{
                //    msg.Msg  = (int)Win32::WM::WM_LBUTTONUP;
                //    msg.WParam = System::IntPtr(1);
                //    msg.LParam = Win32::API::MakeLParam(location.X,location.Y);
                //    Win32::API::PostMessage(msg);
                //}
            }
            break;
        }

    }

    System::Windows::Forms::DialogResult TypeEditorForm::ShowDialog2(Form^ dialog)
    {
        this->DialogResult = dialog->ShowDialog();
        return this->DialogResult;
    }

    System::Object^ TypeEditorForm::GetService_IServiceProvider(System::Type^ serviceType)
    {
        if(serviceType == IWindowsFormsEditorService::typeid)
            return this;
        if(serviceType == Design::IEditorService::typeid)
            return this;
        if(serviceType == System::ComponentModel::ITypeDescriptorContext::typeid)
            return this;
        if(serviceType == System::IServiceProvider::typeid)
            return this;
        if(serviceType == Ntreev::Windows::Forms::Grid::GridControl::typeid)
            return this->GridControl;
        return GetService(serviceType);
    }

    void TypeEditorForm::OnComponentChanged()
    {

    }

    bool TypeEditorForm::OnComponentChanging()
    {
        return false;
    }

    System::ComponentModel::IContainer^ TypeEditorForm::Container2::get()
    {
        return m_gridControl->Container;
    }

    System::Object^ TypeEditorForm::Instance::get()
    {
        return m_gridControl->FocusedRow->Component;
    }

    System::ComponentModel::PropertyDescriptor^ TypeEditorForm::PropertyDescriptor::get()
    {
        return nullptr;
    }

    //void TypeEditorForm::WndProc(System::Windows::Forms::Message% m)
    //{
    //    switch((Win32::WM)m.Msg)
    //    {
    //    case Win32::WM::WM_ACTIVATE:
    //        {
    //            //if(m.WParam == System::IntPtr::Zero)
    //            //{
    //            //    System::Diagnostics::Trace::WriteLine("Inactivate");
    //            //}
    //            //else
    //            //{
    //            //    System::Diagnostics::Trace::WriteLine("Activate");
    //            //}
    //        }
    //        break;
    //    case Win32::WM::WM_KEYDOWN:
    //        {
    //            int wqer=0;
    //        }
    //        break;
    //    }

    //    Form::WndProc(m);
    //}

    //bool TypeEditorForm::ProcessDialogKey(System::Windows::Forms::Keys keyData)
    //{
    //    if(keyData == System::Windows::Forms::Keys::Escape)
    //    {
    //        return true;
    //    }
    //    return Form::ProcessDialogKey(keyData);
    //}

    void TypeEditorForm::CloseEdit()
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

    void TypeEditorForm::control_Resize(System::Object^ sender, System::EventArgs^ e)
    {
        if ((m_control != nullptr) && m_resizing == false)
        {

            int width = this->Width;
            System::Drawing::Size size(m_control->Width + 2, m_control->Height + 2);
            //if (this->resizable)
            //{
            //    size.Height += ResizeBarSize;
            //}

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
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/