﻿#include "StdAfx.h"
#include "GridColumnDropDown.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	generic<class TControl> where TControl : _Control
	ColumnDropDown<TControl>::ColumnDropDown()
	{
		m_sizable	= false;
		m_control	= safe_cast<TControl>(System::Activator::CreateInstance(TControl::typeid));
		OnControlCreated(m_control);
	}

	generic<class TControl> where TControl : _Control
	ColumnDropDown<TControl>::ColumnDropDown(... cli::array<object^>^ controlArgs)
	{
		Sizable		= false;
		m_control	= safe_cast<TControl>(System::Activator::CreateInstance(TControl::typeid, controlArgs));
		OnControlCreated(m_control);
	}

	generic<class TControl> where TControl : _Control
	_Size ColumnDropDown<TControl>::GetPreferredSize(_Size proposedSize)
	{
		return _Size(proposedSize.Width, EditingControl->Size.Height);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::OnAttaching(AttachEventArgs<TControl>^ e)
	{
		EditingControl->Dock    = System::Windows::Forms::DockStyle::Fill;
		EditingControl->Visible	= true;
		Attaching(this, e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::InvokeAttaching(EditingReason^ by, object^ value)
	{
		AttachEventArgs<TControl> e(by, value, m_control);
		OnAttaching(%e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::OnAttached(AttachEventArgs<TControl>^ e)
	{
		Attached(this, e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::InvokeAttached(EditingReason^ by, object^ value)
	{
		AttachEventArgs<TControl> e(by, value, m_control);
		OnAttached(%e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::OnDetaching(DetachEventArgs<TControl>^ e)
	{
		Detaching(this, e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::InvokeDetaching(bool modified, object^ value)
	{
		DetachEventArgs<TControl> de(modified, value, m_control);
		OnDetaching(%de);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::OnDetached(DetachEventArgs<TControl>^ e)
	{
		Detached(this, e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::InvokeDetached(bool modified, object^ value)
	{
		DetachEventArgs<TControl> de(modified, value, m_control);
		OnDetached(%de);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::OnControlCreated(TControl /*control*/)
	{

	}
}/*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/