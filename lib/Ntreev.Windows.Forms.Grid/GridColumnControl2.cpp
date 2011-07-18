#include "StdAfx.h"
#include "GridColumnControl2.h"
#include "GridCell.h"
#include "GridWin32.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	ColumnControl<TEditing, TView>::ColumnControl()
	{
		m_controlPainter = gcnew Win32::BitBlt();

		m_control = safe_cast<TEditing>(System::Activator::CreateInstance(TEditing::typeid));
		OnControlCreated(m_control);
		m_viewControl = safe_cast<TView>(System::Activator::CreateInstance(TView::typeid));
		OnViewControlCreated(m_viewControl);
		NativeRef->m_bCustomItemRender = true;
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	ColumnControl<TEditing, TView>::ColumnControl(... cli::array<object^>^ controlArgs)
	{
		m_controlPainter = gcnew Win32::BitBlt();

		m_control = safe_cast<TEditing>(System::Activator::CreateInstance(TEditing::typeid, controlArgs));
		OnControlCreated(m_control);
		m_viewControl = safe_cast<TView>(System::Activator::CreateInstance(TView::typeid, controlArgs));
		OnViewControlCreated(m_viewControl);
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
		m_controlPainter->Do(hdc, m_viewControl, renderRect, value);
		graphics->ReleaseHdc();
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::OnControlCreated(TEditing /*control*/)
	{

	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::OnViewControlCreated(TView /*control*/)
	{

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
	void ColumnControl<TEditing, TView>::OnAttaching(AttachEventArgs<TEditing>^ e)
	{
		Attaching(this, e);
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::InvokeAttaching(EditingReason^ by, object^ value)
	{
		AttachEventArgs<TEditing> ae(by, value, m_control);
		OnAttaching(%ae);
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::OnAttached(AttachEventArgs<TEditing>^ e)
	{
		Attached(this, e);
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::InvokeAttached(EditingReason^ by, object^ value)
	{
		AttachEventArgs<TEditing> ae(by, value, m_control);
		OnAttached(%ae);
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::OnDetaching(DetachEventArgs<TEditing>^ e)
	{
		Detaching(this, e);
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::InvokeDetaching(bool modified, object^ value)
	{
		DetachEventArgs<TEditing> de(modified, value, m_control);
		OnDetaching(%de);
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::OnDetached(DetachEventArgs<TEditing>^ e)
	{
		Detached(this, e);
	}

	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	void ColumnControl<TEditing, TView>::InvokeDetached(bool modified, object^ value)
	{
		DetachEventArgs<TEditing> de(modified, value, m_control);
		OnDetached(%de);
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/