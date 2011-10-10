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
	void ColumnDropDown<TControl>::OnAttaching(AttachEventArgs^ e)
	{
		EditingControl->Dock    = System::Windows::Forms::DockStyle::Fill;
		EditingControl->Visible	= true;
		Attaching(this, e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::InvokeAttaching(EditingReason^ by, object^ value)
	{
		AttachEventArgs e(by, value);
		OnAttaching(%e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::OnAttached(AttachEventArgs^ e)
	{
		Attached(this, e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::InvokeAttached(EditingReason^ by, object^ value)
	{
		AttachEventArgs e(by, value);
		OnAttached(%e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::OnDetaching(DetachEventArgs^ e)
	{
		Detaching(this, e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::InvokeDetaching(bool modified, object^ value)
	{
		DetachEventArgs de(modified, value);
		OnDetaching(%de);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::OnDetached(DetachEventArgs^ e)
	{
		Detached(this, e);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::InvokeDetached(bool modified, object^ value)
	{
		DetachEventArgs de(modified, value);
		OnDetached(%de);
	}

	generic<class TControl> where TControl : _Control
	void ColumnDropDown<TControl>::OnControlCreated(TControl /*control*/)
	{

	}
}/*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/