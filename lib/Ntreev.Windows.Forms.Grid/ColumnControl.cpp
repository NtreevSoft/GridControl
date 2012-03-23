//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4464.32161
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
#include "ColumnControl.h"
#include "ICell.h"
#include "NativeUtilities.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    generic<class TControl> where TControl : System::Windows::Forms::Control
        ColumnControl<TControl>::ColumnControl()
    {
        m_controlPainter = gcnew Native::ControlPainter();
        m_control = CreateControlInstance(nullptr);

        if(this->ViewType != Ntreev::Windows::Forms::Grid::ViewType::Text)
        {
            m_viewControl = CreateControlInstance(nullptr);
            m_form = gcnew System::Windows::Forms::Form();
            m_form->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            m_form->Controls->Add(m_viewControl);
        }

        m_eventCellMouseMove = gcnew CellMouseEventHandler(this, &ColumnControl<TControl>::gridControl_CellMouseMove);
        NativeRef->m_customItemPaint = this->ViewType != Ntreev::Windows::Forms::Grid::ViewType::Text;
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        ColumnControl<TControl>::ColumnControl(... cli::array<System::Object^>^ controlArgs)
    {
        m_controlPainter = gcnew Native::ControlPainter();
        m_control = CreateControlInstance(controlArgs);

        if(this->ViewType != Ntreev::Windows::Forms::Grid::ViewType::Text)
        {
            m_viewControl = CreateControlInstance(controlArgs);
            m_form = gcnew System::Windows::Forms::Form();
            m_form->Controls->Add(m_viewControl);
        }

        m_eventCellMouseMove = gcnew CellMouseEventHandler(this, &ColumnControl<TControl>::gridControl_CellMouseMove);
        NativeRef->m_customItemPaint = this->ViewType != Ntreev::Windows::Forms::Grid::ViewType::Text;
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        ColumnControl<TControl>::~ColumnControl()
    {

    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        TControl ColumnControl<TControl>::CreateControlInstance(... cli::array<System::Object^>^ controlArgs)
    {
        if(controlArgs == nullptr)
            return safe_cast<TControl>(System::Activator::CreateInstance(TControl::typeid));
        return safe_cast<TControl>(System::Activator::CreateInstance(TControl::typeid, controlArgs));
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        System::Object^ ColumnControl<TControl>::GetControlValue(TControl /*control*/)
    {
        return nullptr; 
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnControl<TControl>::SetControlValue(TControl /*control*/, System::Object^ /*value*/)
    {

    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnControl<TControl>::PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
    {
        if(m_viewControl == nullptr)
            return;
        SetControlLayout(m_viewControl, cell);
        SetControlValue(m_viewControl, value);
        System::IntPtr hdc = graphics->GetHdc();

        if((cell->State & CellState::Focused) == CellState::Focused)
            m_form->ActiveControl = m_viewControl;
        else
            m_form->ActiveControl = nullptr;

        if(this->Site == nullptr)
        {
            if((cell->State & CellState::Hot) == CellState::Hot)
            {
                System::Windows::Forms::Message msg;
                msg.HWnd = m_viewControl->Handle;
                msg.Msg = (int)Native::WM::WM_MOUSEMOVE;
                msg.WParam = System::IntPtr(0);
                System::Drawing::Point location = m_viewControl->PointToClient(System::Windows::Forms::Cursor::Position);
                msg.LParam = Native::Methods::MakeLParam(location.X, location.Y);
                Native::Methods::SendMessage(msg);
            }
            else
            {
                System::Windows::Forms::Message msg;
                msg.HWnd = m_viewControl->Handle;
                msg.Msg = (int)Native::WM::WM_MOUSELEAVE;
                msg.WParam = System::IntPtr(0);
                msg.LParam = System::IntPtr(0);
                Native::Methods::SendMessage(msg);
            }
        }

        try
        {
            m_controlPainter->TransparentBlt(hdc, m_viewControl, paintRect, value);
        }
        catch(System::Exception^ exception)
        {
            throw exception;
        }
        finally
        {
            graphics->ReleaseHdc();
        }
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        TControl ColumnControl<TControl>::Control::get()
    {
        return m_control;
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        Ntreev::Windows::Forms::Grid::ViewType ColumnControl<TControl>::ViewType::get()
    {
        return Ntreev::Windows::Forms::Grid::ViewType::Custom;
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnControl<TControl>::SetControlLayout(TControl control, Ntreev::Windows::Forms::Grid::ICell^ cell)
    {
        control->SetBounds(0, 0, cell->ClientRectangle.Width, cell->ClientRectangle.Height);
        if(control->BackColor != cell->PaintingBackColor)
            control->BackColor = cell->PaintingBackColor;
        if(control->ForeColor != cell->PaintingForeColor)
            control->ForeColor = cell->PaintingForeColor;
        if(control->Font != cell->Font)
            control->Font = cell->Font;
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        System::Object^ ColumnControl<TControl>::EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
    {
        SetControlLayout(m_control, cell);
        SetControlValue(m_control, value);
        m_editorService = editorService;
        editorService->ShowControl(m_control);
        m_editorService = nullptr;
        return GetControlValue(m_control);
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        bool ColumnControl<TControl>::CanEdit(Ntreev::Windows::Forms::Grid::ICell^ cell, Ntreev::Windows::Forms::Grid::EditingReason reason)
    {
        if(reason.ReasonType == Ntreev::Windows::Forms::Grid::EditingReasonType::Mouse)
            return cell->ClientRectangle.Contains(reason.Location);
        return false;
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnControl<TControl>::CloseControl()
    {
        if(m_editorService == nullptr)
            throw gcnew System::NotImplementedException();

        m_editorService->Close();
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnControl<TControl>::OnGridControlAttached()
    {
        Column::OnGridControlAttached();
        this->GridControl->CellMouseMove += m_eventCellMouseMove;
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnControl<TControl>::OnGridControlDetached()
    {
        this->GridControl->CellMouseMove -= m_eventCellMouseMove;
        Column::OnGridControlDetached();
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnControl<TControl>::gridControl_CellMouseMove(System::Object^ /*sender*/, Ntreev::Windows::Forms::Grid::CellMouseEventArgs^ e)
    {
        if(this->IsVisible == false)
            return;

        //System::Diagnostics::Trace::WriteLine(System::Windows::Forms::Cursor::Position);
        if(e->Cell->Column != this || e->Cell->IsReadOnly == true || m_viewControl == nullptr)
            return;

        System::Drawing::Point cursorLocation = System::Windows::Forms::Cursor::Position;
        System::Drawing::Point location = e->Location + (System::Drawing::Size)e->Cell->Location;

        System::Drawing::Rectangle bounds(e->Cell->Bounds.Left + e->Cell->Padding.Left, 
            e->Cell->Bounds.Top + e->Cell->Padding.Top, 
            e->Cell->ClientRectangle.Width, e->Cell->ClientRectangle.Height);

        m_form->Bounds = this->GridControl->RectangleToScreen(bounds);

        location = this->GridControl->PointToScreen(location);
        location = m_form->PointToClient(location);


        //SetControlLayout(m_viewControl, e->Cell);
        //SetControlValue(m_viewControl, e->Cell->Value);

        if(this->Site == nullptr)
        {
            System::Windows::Forms::Message msg;

            msg.HWnd = m_viewControl->Handle;
            msg.Msg = (int)Native::WM::WM_MOUSEMOVE;
            msg.WParam = System::IntPtr(0);
            msg.LParam = Native::Methods::MakeLParam(location.X, location.Y);
            Native::Methods::SendMessage(msg);

            msg.HWnd = m_viewControl->Handle;
            msg.Msg = (int)Native::WM::WM_SETCURSOR;
            msg.WParam = m_viewControl->Handle;
            msg.LParam = Native::Methods::MakeLParam(1, (int)Native::WM::WM_MOUSEMOVE);
            Native::Methods::SendMessage(msg);

            this->GridControl->Cursor = System::Windows::Forms::Cursor::Current;
        }



        //System::Drawing::Rectangle rect = Native::Methods::GetWindowRect(m_viewControl->Handle);
        //System::Drawing::Point cur = System::Windows::Forms::Cursor::Position;

        //System::Diagnostics::Trace::WriteLine(System::Windows::Forms::Cursor::Current);

        //if(rect.Contains(cur) == true)
        //{
        // int qwer=0;
        //}

        e->Handled = true;


    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/