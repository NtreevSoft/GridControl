//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5792.31442
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
#include "Cell.h"

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
			//m_viewControl->Visible = true;
			m_eventCellMouseMove = gcnew CellMouseEventHandler(this, &ColumnControl<TControl>::gridControl_CellMouseMove);
			m_viewControlInvalidate = gcnew System::Windows::Forms::InvalidateEventHandler(this, &ColumnControl<TControl>::viewControl_Invalidated);
			this->NativeRef->SetItemMinSize(m_viewControl->PreferredSize);
        }
        
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
            m_eventCellMouseMove = gcnew CellMouseEventHandler(this, &ColumnControl<TControl>::gridControl_CellMouseMove);
			m_viewControlInvalidate = gcnew System::Windows::Forms::InvalidateEventHandler(this, &ColumnControl<TControl>::viewControl_Invalidated);
			this->NativeRef->SetItemMinSize(m_viewControl->PreferredSize);
        }

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

		System::Drawing::Point location = this->GridControl->PointToClient(System::Windows::Forms::Cursor::Position);
		location -= (System::Drawing::Size)cell->Bounds.Location;
		location.X -= cell->Padding.Left;
		location.Y -= cell->Padding.Left;

        this->SetControlValue(m_viewControl, value);
		this->SetControlLayout(m_viewControl, cell);

        System::IntPtr hdc = graphics->GetHdc();

		System::Windows::Forms::Cursor^ cursor = nullptr;

        if(this->Site == nullptr)
        {
			if(m_viewControl->Bounds.Contains(location) == true)
            {
				location = m_viewControl->PointToScreen(location);
				System::Windows::Forms::Control^ hitTest = this->GridControl->GetChildAt(m_viewControl, location);
				if(hitTest == nullptr)
					hitTest = m_viewControl;
				location = hitTest->PointToClient(location);

                System::Windows::Forms::Message msg;
                msg.HWnd = hitTest->Handle;
                msg.Msg = (int)Native::WM::WM_MOUSEMOVE;
                msg.WParam = System::IntPtr(0);
                msg.LParam = Native::Methods::MakeLParam(location.X, location.Y);
                Native::Methods::SendMessage(msg);

				msg.HWnd = hitTest->Handle;
				msg.Msg = (int)Native::WM::WM_SETCURSOR;
				msg.WParam = hitTest->Handle;
				msg.LParam = Native::Methods::MakeLParam(1, (int)Native::WM::WM_MOUSEMOVE);
				Native::Methods::SendMessage(msg);

				cursor = System::Windows::Forms::Cursor::Current;
            }
            else
            {
                System::Windows::Forms::Message msg;
                msg.HWnd = m_viewControl->Handle;
                msg.Msg = (int)Native::WM::WM_MOUSELEAVE;
				msg.WParam = System::IntPtr::Zero;
                msg.LParam = System::IntPtr(1);
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

		if(cursor != nullptr)
			this->GridControl->Cursor = cursor;
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
            return;

        m_editorService->Close();
    }

     generic<class TControl> where TControl : System::Windows::Forms::Control
        Ntreev::Windows::Forms::Grid::Design::EditStyle ColumnControl<TControl>::GetEditStyle()
    {
        return Ntreev::Windows::Forms::Grid::Design::EditStyle::Control;
    }

	 generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnControl<TControl>::OnCellBoundUpdate(Cell^ cell, System::Object^ value)
    {
		if(m_viewControl == nullptr)
			return;
		
		this->SetControlLayout(m_viewControl, cell);
		this->SetControlValue(m_viewControl, value);
		cell->NativeRef->SetTextBounds(m_viewControl->PreferredSize);
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnControl<TControl>::OnGridControlAttached()
    {
        Column::OnGridControlAttached();
        
		if(m_viewControl != nullptr)
		{
			this->GridControl->CellMouseMove += m_eventCellMouseMove;
			this->messageFilter = gcnew MessageFilter(m_viewControl);
			System::Windows::Forms::Application::AddMessageFilter(this->messageFilter);
		}
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnControl<TControl>::OnGridControlDetached()
    {
		if(this->messageFilter != nullptr)
		{
			System::Windows::Forms::Application::RemoveMessageFilter(this->messageFilter);
			this->messageFilter = nullptr;
			this->GridControl->CellMouseMove -= m_eventCellMouseMove;
		}
        
        Column::OnGridControlDetached();
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnControl<TControl>::gridControl_CellMouseMove(System::Object^ /*sender*/, Ntreev::Windows::Forms::Grid::CellMouseEventArgs^ e)
    {
        if(this->IsVisible == false || e->Cell->Column != this || e->Cell->IsReadOnly == true || this->Site != nullptr)
            return;

		Cell^ cell = e->Cell;
		//System::Drawing::Point location = e->Location;
		//location.X -= cell->Padding.Left;
		//location.Y -= cell->Padding.Left;

		//this->SetControlLayout(m_viewControl, cell);

		//if(m_viewControl->Bounds.Contains(location) == false)
		//	return;

		//this->SetControlValue(m_viewControl, cell->Value);

		//System::Windows::Forms::Message msg;
		//System::Windows::Forms::Control^ hitTest = this->GetChildAt(m_viewControl, location);
		//location = hitTest->PointToClient(location);
		//msg.HWnd = hitTest->Handle;
		//msg.Msg = (int)Native::WM::WM_MOUSEMOVE;
		//msg.WParam = System::IntPtr(0);
		//msg.LParam = Native::Methods::MakeLParam(location.X, location.Y);
		//Native::Methods::SendMessage(msg);

		//msg.HWnd = hitTest->Handle;
		//msg.Msg = (int)Native::WM::WM_SETCURSOR;
		//msg.WParam = hitTest->Handle;
		//msg.LParam = Native::Methods::MakeLParam(1, (int)Native::WM::WM_MOUSEMOVE);
		//Native::Methods::SendMessage(msg);

		//this->GridControl->Cursor = System::Windows::Forms::Cursor::Current;
		cell->Invalidate();
		//System::Console::WriteLine(cell->Location);

        e->Handled = true;
    }

	generic<class TControl> where TControl : System::Windows::Forms::Control
		void ColumnControl<TControl>::viewControl_Invalidated(System::Object^ /*sender*/, System::Windows::Forms::InvalidateEventArgs^ /*e*/)
	{

	}

	generic<class TControl> where TControl : System::Windows::Forms::Control
		bool ColumnControl<TControl>::MessageFilter::PreFilterMessage(System::Windows::Forms::Message% m)
	{
		if(m.Msg == (int)Native::WM::WM_MOUSELEAVE)
		{
			if(m.HWnd == m_control->Handle && m.LParam == System::IntPtr::Zero)
			{
				return true;
			}
		}

		return false;
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/