#include "StdAfx.h"
#include "GridErrorDescriptor.h"
#include "GridControl.h"
#include "GridCell.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Private
{
	ErrorDescriptor::ErrorDescriptor(_GridControl^ gridControl) : GridObject(gridControl)
	{
		m_cells				= gcnew _CellsList;
		m_errorTimer		= gcnew _Timer();
		m_errorTimer->Elapsed += gcnew _ElapsedEventHandler(this, &ErrorDescriptor::errorTimer_Elapsed);
		m_errorCount		= 0;

		m_changed			= false;

		GridControl->Cleared += gcnew _EventHandler(this, &ErrorDescriptor::gridControl_Cleared);
	}

	void ErrorDescriptor::Add(Cell^ cell)
	{
		m_cells->Add(cell);
		m_changed = true;

		m_changed		= false;
		m_errorTimer->Start();
		m_errorTimer->Interval = 300;
		m_errorCount	= 0;
	}
	
	void ErrorDescriptor::Remove(Cell^ cell)
	{
		m_cells->Remove(cell);
		if(m_cells->Count == 0)
			m_errorCount = 0;
	}

	void ErrorDescriptor::errorTimer_Elapsed(object^ /*sender*/, _ElapsedEventArgs^  e)
	{
		m_errorCount++;
		if(m_errorCount > 8)
		{
			m_errorTimer->Stop();
			m_changed = false;
			m_errorCount = 0;
			//m_errorCell			= nullptr;
		}
		System::Diagnostics::Debug::WriteLine(string::Format("The Elapsed event was raised at {0}", e->SignalTime));
		Invalidate();
	}

	void ErrorDescriptor::gridControl_Cleared(object^ /*sender*/, _EventArgs^  /*e*/)
	{
		m_cells->Clear();
	}

	void ErrorDescriptor::Paint(_Graphics^ g)
	{
		if(m_errorCount % 2 != 0)
			return;

		for each(Cell^ cell in m_cells)
		{
			if(cell->IsDisplayed)
			{
				System::Drawing::Pen^ pen = gcnew System::Drawing::Pen(_Color::Firebrick);
				pen->Width = 2;
				pen->Alignment = System::Drawing::Drawing2D::PenAlignment::Inset;
				g->DrawRectangle(pen, cell->DisplayRectangle);
			}
		}
	}
} /*namespace Private*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/


