//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4478.19833
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
    using namespace System::Timers;
    using namespace System::Drawing;
    using namespace System::Drawing::Drawing2D;
    using namespace System::Collections::Generic;

    ErrorDescriptor::ErrorDescriptor(_GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_cells = gcnew List<Cell^>();
        m_rows = gcnew List<Row^>();
        m_timer = gcnew Timer();
        m_timer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &ErrorDescriptor::errorTimer_Elapsed);
        m_timer->Interval = 300;
        m_errorCount = 0;

        this->GridControl->Cleared += gcnew ClearEventHandler(this, &ErrorDescriptor::gridControl_Cleared);
        this->GridControl->VisibleChanged += gcnew System::EventHandler(this, &ErrorDescriptor::gridControl_VisibleChanged);
    }

    void ErrorDescriptor::Add(Cell^ cell)
    {
        m_cells->Add(cell);
        m_errorCount = 0;

        if(this->GridControl->Visible == true)
        {
            m_timer->Start();
        }
    }

    void ErrorDescriptor::Remove(Cell^ cell)
    {
        m_cells->Remove(cell);
        if(m_cells->Count == 0)
            m_errorCount = 0;
    }

    void ErrorDescriptor::Add(Row^ row)
    {
        m_rows->Add(row);
        m_errorCount = 0;

        if(this->GridControl->Visible == true)
        {
            m_timer->Start();
        }
    }

    void ErrorDescriptor::Remove(Row^ row)
    {
        m_rows->Remove(row);
        if(m_rows->Count == 0)
            m_errorCount = 0;
    }

    void ErrorDescriptor::errorTimer_Elapsed(System::Object^ /*sender*/, ElapsedEventArgs^ /*e*/)
    {
        m_errorCount++;
        if(m_errorCount > 8)
        {
            m_timer->Stop();
            m_errorCount = 0;
        }
        this->GridControl->Invalidate();
    }

    void ErrorDescriptor::gridControl_Cleared(System::Object^ /*sender*/, ClearEventArgs^ /*e*/)
    {
        m_cells->Clear();
        m_rows->Clear();
        m_timer->Stop();
        m_errorCount = 0;
    }

    void ErrorDescriptor::gridControl_VisibleChanged(System::Object^ /*sender*/, System::EventArgs^ /*e*/)
    {
        if(this->GridControl->Visible == false)
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

    void ErrorDescriptor::Paint(Graphics^ g)
    {
        if(m_errorCount % 2 != 0)
            return;

        Bitmap^ errorBitmap = _Resources::Error;
        Pen^ pen = gcnew Pen(Color::Firebrick, 2);
        pen->Alignment = PenAlignment::Inset;
        for each(Cell^ cell in m_cells)
        {
            if(cell->IsDisplayable == false)
                continue;

            Rectangle bounds = cell->Bounds;
            bounds.Width--;
            bounds.Height--;
            g->DrawRectangle(pen, bounds);

            g->DrawImage(errorBitmap, bounds.Left + 3, bounds.Top + 3, errorBitmap->Width, errorBitmap->Height);
        }

        for each(Row^ row in m_rows)
        {
            if(row->IsDisplayable == false)
                continue;

            Rectangle bounds = row->Bounds;
            bounds.Width = this->GridCore->GetColumnList()->GetBounds().GetWidth();
            bounds.Height--;
            g->DrawRectangle(pen, bounds);

            bounds = row->Bounds;
            g->DrawImage(errorBitmap, bounds.Left + 3, bounds.Top + 3, errorBitmap->Width, errorBitmap->Height);
        }
        delete pen;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/