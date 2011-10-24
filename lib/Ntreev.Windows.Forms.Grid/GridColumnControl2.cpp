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
#include "GridColumnControl2.h"
#include "GridCell.h"
#include "GridWin32.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	ColumnControl<TEditing, TView>::ColumnControl()
	{
		m_controlPainter = gcnew Win32::ControlPainter();

		m_control = safe_cast<TEditing>(System::Activator::CreateInstance(TEditing::typeid));
		m_viewControl = safe_cast<TView>(System::Activator::CreateInstance(TView::typeid));
		NativeRef->m_bCustomItemRender = true;
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	ColumnControl<TEditing, TView>::ColumnControl(... cli::array<object^>^ controlArgs)
	{
		m_controlPainter = gcnew Win32::ControlPainter();

		m_control = safe_cast<TEditing>(System::Activator::CreateInstance(TEditing::typeid, controlArgs));
		m_viewControl = safe_cast<TView>(System::Activator::CreateInstance(TView::typeid, controlArgs));
		NativeRef->m_bCustomItemRender = true;
	}
	
	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::PaintValue(_Graphics^ graphics, _Rectangle renderRect, _Rectangle /*clipRect*/, IStyle^ style, object^ value)
	{
		SetViewControlLayout(m_viewControl, renderRect.Location, renderRect.Size);
		if(m_viewControl->BackColor != style->BackColor)
			m_viewControl->BackColor = style->BackColor;
		if(m_viewControl->ForeColor != style->ForeColor)
			m_viewControl->ForeColor = style->ForeColor;
		if(m_viewControl->Font != style->Font)
			m_viewControl->Font = style->Font;
		SetViewValue(m_viewControl, value);
		System::IntPtr hdc = graphics->GetHdc();
		try
		{
			if(this->IsViewControlTransparented == true)
				m_controlPainter->TransparentBlt(hdc, m_viewControl, renderRect, value);
			else
				m_controlPainter->BitBlt(hdc, m_viewControl, renderRect, value);
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

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::SetControlLayout(TEditing control, _Point location, _Size size)
	{
		if(control->Location != location)
			control->Location = location;
		if(control->Width != size.Width)
			control->Width = size.Width;
		if(control->Height != size.Height)
			control->Height = size.Height;
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::SetViewControlLayout(TView viewControl, _Point location, _Size size)
	{
		if(viewControl->Location != location)
			viewControl->Location = location;
		if(viewControl->Width != size.Width)
			viewControl->Width = size.Width;
		if(viewControl->Height != size.Height)
			viewControl->Height = size.Height;
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::OnAttaching(AttachEventArgs^ e)
	{
		Attaching(this, e);
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::InvokeAttaching(EditingReason^ by, object^ value)
	{
		AttachEventArgs ae(by, value);
		OnAttaching(%ae);
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::OnAttached(AttachEventArgs^ e)
	{
		Attached(this, e);
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::InvokeAttached(EditingReason^ by, object^ value)
	{
		AttachEventArgs ae(by, value);
		OnAttached(%ae);
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::OnDetaching(DetachEventArgs^ e)
	{
		Detaching(this, e);
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::InvokeDetaching(bool modified, object^ value)
	{
		DetachEventArgs de(modified, value);
		OnDetaching(%de);
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::OnDetached(DetachEventArgs^ e)
	{
		Detached(this, e);
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::InvokeDetached(bool modified, object^ value)
	{
		DetachEventArgs de(modified, value);
		OnDetached(%de);
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/