//=====================================================================================================================
// Ntreev Grid for .Net 2.0.0.0
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


#pragma once
#include "GridBase.h"
#include "GridScrollProperty.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Private
{
	private ref class SelectionTimer : System::Timers::Timer, IGridControlProvider
	{
		enum class ScrollSpeed : int
		{
			Normal,
			Fast,
			Fastest,
		};
	public:
		SelectionTimer(GridControl^ gridControl);

		bool				DoScroll();

	public:
		property _Point		MouseLocation		{ void set(_Point); }
		property bool		HScrollEnabled		{ bool get(); void set(bool); }
		property bool		VScrollEnabled		{ bool get(); void set(bool); }

		property bool		ScrollAcceleration	{ bool get(); void set(bool); }
		property bool		IntervalAcceleration{ bool get(); void set(bool); }

		property bool		CanHScroll			{ bool get(); }
		property bool		CanVScroll			{ bool get(); }

		property _GridControl^		GridControl
		{
			virtual _GridControl^ get() { return m_gridControl; }
		}

		property HScrollProperty^ HorizontalScroll { HScrollProperty^ get(); }
		property VScrollProperty^ VerticalScroll { VScrollProperty^ get(); }

	protected:
		property _Rectangle	InsideRectangle		{ virtual _Rectangle get(); }
		property _Rectangle	OutsideRectangle	{ virtual _Rectangle get(); }
	private:
		int					ComputeSpeed(int pos, int length);

	private:
		bool				m_horzEnable;
		bool				m_vertEnable;

		bool				m_scrollAcceleration;
		bool				m_intervalAcceleration;

		int					m_horzScroll;
		int					m_vertScroll;
		_GridControl^		m_gridControl;
	};

} /*namespace Private*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/