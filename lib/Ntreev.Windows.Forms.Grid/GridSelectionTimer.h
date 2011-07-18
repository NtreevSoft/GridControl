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