#include "StdAfx.h"
#include "GridSelectionTimer.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Private
{
	SelectionTimer::SelectionTimer(_GridControl^ gridControl) : m_gridControl(gridControl)
	{
		BeginInit();
		Enabled = false;
		SynchronizingObject = gridControl;
		AutoReset = true;
		EndInit();
		

		m_horzEnable = true;
		m_vertEnable = true;

		m_horzScroll = 0;
		m_vertScroll = 0;

		m_scrollAcceleration	= true;
		m_intervalAcceleration	= true;
	}

	bool SelectionTimer::DoScroll()
	{
		bool result = false;
		if(HScrollEnabled == true)
		{
			int nNewHorz = HorizontalScroll->Value + m_horzScroll;
			if(HorizontalScroll->DoScroll(nNewHorz) == true)
				result = true;
		}

		if(VScrollEnabled == true)
		{
			int nNewVert = VerticalScroll->Value + m_vertScroll;
			if(VerticalScroll->DoScroll(nNewVert) == true)
				result = true;
		}
		return result;
	}

	int SelectionTimer::ComputeSpeed(int pos, int length)
	{
		if(length == 0)
			throw gcnew System::DivideByZeroException;
		else if(length == 1)
			return 3;
		return (int)System::Math::Ceiling( (pos / (float)length)  * 3.0f);
	}

	void SelectionTimer::MouseLocation::set(_Point point)
	{
		_Rectangle inside	= InsideRectangle;
		_Rectangle outside	= OutsideRectangle;

		m_vertScroll = 0;
		m_horzScroll = 0;

		if(outside.Contains(point) == false)
			return;

		if(HScrollEnabled == true)
		{
			if(point.X < inside.Left && point.X >= outside.Left)
			{
				if(HorizontalScroll->Value > HorizontalScroll->Minimum)
				{
					m_horzScroll = -ComputeSpeed(inside.Left - point.X, inside.Left - outside.Left);
				}
			}
			else if(point.X >= inside.Right && point.X < outside.Right)
			{
				if(HorizontalScroll->Value < HorizontalScroll->Maximum - HorizontalScroll->LargeChange + 1)
				{
					m_horzScroll = ComputeSpeed(point.X - inside.Right, outside.Right - inside.Right);
				}
			}	
		}

		if(VScrollEnabled == true)
		{
			if(point.Y < inside.Top && point.Y >= outside.Top)
			{
				if(VerticalScroll->Value > VerticalScroll->Minimum)
				{
					m_vertScroll = -ComputeSpeed(inside.Top - point.Y, inside.Top - outside.Top);
				}
			}
			else if(point.Y >= inside.Bottom && point.Y < outside.Bottom)
			{
				if(VerticalScroll->Value < VerticalScroll->Maximum - VerticalScroll->LargeChange + 1)
				{
					m_vertScroll = ComputeSpeed(point.Y - inside.Bottom, outside.Bottom - inside.Bottom);
				}
			}	
		}

		//Debug::WriteLine(m_vertScroll);

		int speed = System::Math::Max(System::Math::Abs(m_horzScroll), System::Math::Abs(m_vertScroll));
		if(m_scrollAcceleration == false)
			speed = 1;
		double interval;
		switch(speed)
		{
		case 1:
			interval = 200;
			break;
		case 2:
			interval = 125;
			break;
		default:
			interval = 50;
			break;
		}

		if(m_intervalAcceleration == false)
			interval = 200;

		if(Interval != interval)
			Interval = interval;
	}

	bool SelectionTimer::HScrollEnabled::get()
	{
		return 	m_horzEnable;
	}

	void SelectionTimer::HScrollEnabled::set(bool value)
	{
		m_horzEnable = value;
	}

	bool SelectionTimer::VScrollEnabled::get()
	{
		return m_vertEnable;	
	}

	void SelectionTimer::VScrollEnabled::set(bool value)
	{
		m_vertEnable = value;
	}


	bool SelectionTimer::CanHScroll::get()
	{
		return m_horzScroll != 0;
	}

	bool SelectionTimer::CanVScroll::get()
	{
		return m_vertScroll != 0;
	}

	
	bool SelectionTimer::ScrollAcceleration::get()
	{
		return m_scrollAcceleration;
	}

	void SelectionTimer::ScrollAcceleration::set(bool value)
	{
		m_scrollAcceleration = value;
	}
	
	bool SelectionTimer::IntervalAcceleration::get()
	{
		return m_intervalAcceleration;
	}

	void SelectionTimer::IntervalAcceleration::set(bool value)
	{
		m_intervalAcceleration = value;
	}

	_Rectangle	SelectionTimer::InsideRectangle::get()
	{
		return m_gridControl->RectangleToScreen(m_gridControl->DataRectangle);
	}

	_Rectangle	SelectionTimer::OutsideRectangle::get()
	{
		return System::Windows::Forms::Screen::PrimaryScreen->Bounds;
	}

	HScrollProperty^ SelectionTimer::HorizontalScroll::get()
	{
		return m_gridControl->HorizontalScroll;
	}

	VScrollProperty^ SelectionTimer::VerticalScroll::get()
	{
		return m_gridControl->VerticalScroll;
	}
} /*namespace Private*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/