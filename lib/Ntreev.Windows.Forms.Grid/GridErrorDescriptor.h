#pragma once
#include "GridBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Private
{
	ref class ErrorDescriptor : GridObject
	{
	private:
		typedef System::Timers::Timer						_Timer;
		typedef System::Timers::ElapsedEventArgs			_ElapsedEventArgs;
		typedef System::Timers::ElapsedEventHandler			_ElapsedEventHandler;
		typedef System::Collections::Generic::List<Cell^>	_CellsList;

	public:
		ErrorDescriptor(_GridControl^ gridControl);

		void							Paint(_Graphics^ g);
		void							Add(Cell^ cell);
		void							Remove(Cell^ cell);

	private:
		void							errorTimer_Elapsed(object^ sender, _ElapsedEventArgs^  e);
		void							gridControl_Cleared(object^ sender, _EventArgs^  e);

	private:
		_CellsList^						m_cells;

		_Timer^							m_errorTimer;
		int								m_errorCount;

		bool							m_changed;
	};
} /*namespace Private*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/