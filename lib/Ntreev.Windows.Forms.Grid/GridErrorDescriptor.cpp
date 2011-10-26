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

		GridControl->Cleared += gcnew ClearEventHandler(this, &ErrorDescriptor::gridControl_Cleared);
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
	}

	void ErrorDescriptor::gridControl_Cleared(object^ /*sender*/, ClearEventArgs^ /*e*/)
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