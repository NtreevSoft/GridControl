#include "StdAfx.h"
#include "GridColumnControl.h"
#include "GridCell.h"
#include "GridWin32.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	generic<class TControl> where TControl : _Control
	ColumnControl<TControl>::ColumnControl()
	{
		m_control = safe_cast<TControl>(System::Activator::CreateInstance(TControl::typeid));
		OnControlCreated(m_control);

		if(PaintValueSupported == true)
		{
			m_controlPainter = gcnew Win32::BitBlt();
			m_viewControl = safe_cast<TControl>(System::Activator::CreateInstance(TControl::typeid));
			OnViewControlCreated(m_control);
		}

		NativeRef->m_bCustomItemRender = PaintValueSupported;
	}

	generic<class TControl> where TControl : _Control
	ColumnControl<TControl>::ColumnControl(... cli::array<object^>^ controlArgs)
	{
		m_control = safe_cast<TControl>(System::Activator::CreateInstance(TControl::typeid, controlArgs));
		OnControlCreated(m_control);

		if(PaintValueSupported == true)
		{
			m_controlPainter = gcnew Win32::BitBlt();
			m_viewControl = safe_cast<TControl>(System::Activator::CreateInstance(TControl::typeid, controlArgs));
			OnViewControlCreated(m_control);
		}

		NativeRef->m_bCustomItemRender = PaintValueSupported;
	}

	generic<class TControl> where TControl : _Control
	void ColumnControl<TControl>::PaintValue(_Graphics^ graphics, _Rectangle renderRect, _Rectangle /*clipRect*/, IStyle^ style, object^ value)
	{
		SetControlLayout(m_viewControl, renderRect.Location, renderRect.Size);
		if(m_viewControl->BackColor != style->BackColor)
			m_viewControl->BackColor = style->BackColor;
		if(m_viewControl->ForeColor != style->ForeColor)
			m_viewControl->ForeColor = style->ForeColor;
		if(m_viewControl->Font != style->Font)
			m_viewControl->Font = style->Font;
		SetEditingValue(m_viewControl, value);
		System::IntPtr hdc = graphics->GetHdc();
		try
		{
			m_controlPainter->Do(hdc, m_viewControl, renderRect, value);
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

	generic<class TControl> where TControl : _Control
	void ColumnControl<TControl>::OnControlCreated(TControl /*control*/)
	{

	}

	generic<class TControl> where TControl : _Control
		void ColumnControl<TControl>::OnViewControlCreated(TControl /*control*/)
	{

	}

	generic<class TControl> where TControl : _Control
	void ColumnControl<TControl>::SetControlLayout(TControl control, _Point location, _Size size)
	{
		if(control->Location != location)
			control->Location = location;

		if(control->Width != size.Width)
			control->Width = size.Width;

		if(control->Height != size.Height)
			control->Height = size.Height;
	}

	generic<class TControl> where TControl : _Control
	void ColumnControl<TControl>::OnAttaching(AttachEventArgs<TControl>^ e)
	{
		Attaching(this, e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnControl<TControl>::InvokeAttaching(EditingReason^ by, object^ value)
	{
		AttachEventArgs<TControl> ae(by, value, m_control);
		OnAttaching(%ae);
	}
 
	generic<class TControl> where TControl : _Control
	void ColumnControl<TControl>::OnAttached(AttachEventArgs<TControl>^ e)
	{
		Attached(this, e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnControl<TControl>::InvokeAttached(EditingReason^ by, object^ value)
	{
		AttachEventArgs<TControl> ae(by, value, m_control);
		OnAttached(%ae);
	}

	generic<class TControl> where TControl : _Control
	void ColumnControl<TControl>::OnDetaching(DetachEventArgs<TControl>^ e)
	{
		Detaching(this, e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnControl<TControl>::InvokeDetaching(bool modified, object^ value)
	{
		DetachEventArgs<TControl> de(modified, value, m_control);
		OnDetaching(%de);
	}

	generic<class TControl> where TControl : _Control
	void ColumnControl<TControl>::OnDetached(DetachEventArgs<TControl>^ e)
	{
		Detached(this, e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnControl<TControl>::InvokeDetached(bool modified, object^ value)
	{
		DetachEventArgs<TControl> de(modified, value, m_control);
		OnDetached(%de);
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/