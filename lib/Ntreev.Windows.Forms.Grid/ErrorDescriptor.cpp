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


#include "StdAfx.h"
#include "ErrorDescriptor.h"
#include "GridControl.h"
#include "Cell.h"
#include "Resources.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    ErrorDescriptor::ErrorDescriptor(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_cells = gcnew System::Collections::Generic::List<Ntreev::Windows::Forms::Grid::Cell^>();
        m_rows = gcnew System::Collections::Generic::List<Ntreev::Windows::Forms::Grid::Row^>();
        m_timer = gcnew System::Timers::Timer();
        m_timer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &ErrorDescriptor::errorTimer_Elapsed);
        m_timer->Interval = 300;
        m_errorCount = 0;

        GridControl->Cleared += gcnew ClearEventHandler(this, &ErrorDescriptor::gridControl_Cleared);
        GridControl->VisibleChanged += gcnew System::EventHandler(this, &ErrorDescriptor::gridControl_VisibleChanged);
    }

    void ErrorDescriptor::Add(Ntreev::Windows::Forms::Grid::Cell^ cell)
    {
        m_cells->Add(cell);
        m_errorCount = 0;

        if(GridControl->Visible == true)
        {
            m_timer->Start();
        }
    }

    void ErrorDescriptor::Remove(Ntreev::Windows::Forms::Grid::Cell^ cell)
    {
        m_cells->Remove(cell);
        if(m_cells->Count == 0)
            m_errorCount = 0;
    }

    void ErrorDescriptor::Add(Ntreev::Windows::Forms::Grid::Row^ row)
    {
        m_rows->Add(row);
        m_errorCount = 0;

        if(GridControl->Visible == true)
        {
            m_timer->Start();
        }
    }

    void ErrorDescriptor::Remove(Ntreev::Windows::Forms::Grid::Row^ row)
    {
        m_rows->Remove(row);
        if(m_rows->Count == 0)
            m_errorCount = 0;
    }

    void ErrorDescriptor::errorTimer_Elapsed(System::Object^ /*sender*/, System::Timers::ElapsedEventArgs^ /*e*/)
    {
        m_errorCount++;
        if(m_errorCount > 8)
        {
            m_timer->Stop();
            m_errorCount = 0;
        }
        GridControl->Invalidate();
    }

    void ErrorDescriptor::gridControl_Cleared(System::Object^ /*sender*/, Ntreev::Windows::Forms::Grid::ClearEventArgs^ /*e*/)
    {
        m_cells->Clear();
        m_rows->Clear();
        m_timer->Stop();
        m_errorCount = 0;
    }

    void ErrorDescriptor::gridControl_VisibleChanged(System::Object^ /*sender*/, System::EventArgs^ /*e*/)
    {
        if(GridControl->Visible == false)
        {
            m_timer->Stop();
            m_errorCount = 0;
        }
        else
        {
            if(m_cells->Count > 0 || m_rows->Count > 0)
            {
                m_timer->Start();
            }
        }
    }

    void ErrorDescriptor::Paint(System::Drawing::Graphics^ g)
    {
        if(m_errorCount % 2 != 0)
            return;

        System::Drawing::Bitmap^ errorBitmap = Properties::Resources::Error;
        System::Drawing::Pen^ pen = gcnew System::Drawing::Pen(System::Drawing::Color::Firebrick, 2);
        pen->Alignment = System::Drawing::Drawing2D::PenAlignment::Inset;
        for each(Ntreev::Windows::Forms::Grid::Cell^ cell in m_cells)
        {
            if(cell->IsDisplayable == false)
                continue;

            System::Drawing::Rectangle bounds = cell->Bounds;
            bounds.Width--;
            bounds.Height--;
            g->DrawRectangle(pen, bounds);

            g->DrawImage(errorBitmap, bounds.Left + 3, bounds.Top + 3, errorBitmap->Width, errorBitmap->Height);
        }

        for each(Ntreev::Windows::Forms::Grid::Row^ row in m_rows)
        {
            if(row->IsDisplayable == false)
                continue;

            System::Drawing::Rectangle bounds = row->Bounds;
            bounds.Width = GridCore->GetColumnList()->GetBounds().GetWidth();
            bounds.Height--;
            g->DrawRectangle(pen, bounds);

            bounds = row->Bounds;

            g->DrawImage(errorBitmap, bounds.Left + 3, bounds.Top + 3, errorBitmap->Width, errorBitmap->Height);

        }
        delete pen;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/