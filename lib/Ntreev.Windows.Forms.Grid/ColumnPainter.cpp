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


#include "stdafx.h"
#include "ColumnPainter.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    ColumnPainter::ColumnPainter()
    {
        using namespace System::Windows::Forms::VisualStyles;

        try
        {
            m_sortedDownRenderer = gcnew VisualStyleRenderer(VisualStyleElement::Header::SortArrow::SortedDown);
        }
        catch(System::Exception^)
        {

        }

        try
        {
            m_sortedUpPainter = gcnew VisualStyleRenderer(VisualStyleElement::Header::SortArrow::SortedUp);
        }
        catch(System::Exception^)
        {

        }
    }

    void ColumnPainter::DrawSortArrow(System::Drawing::Graphics^ g, System::Drawing::Rectangle paintRect, SortType sortType)
    {
        using namespace System::Windows::Forms;

        switch(sortType)
        {
        case SortType::Up:
            if(m_sortedUpPainter)
                m_sortedUpPainter->DrawBackground(g, paintRect);
            else
                ControlPaint::DrawScrollButton(g, paintRect, ScrollButton::Up, ButtonState::Flat);
            break;
        case SortType::Down:
            if(m_sortedDownRenderer)
                m_sortedDownRenderer->DrawBackground(g, paintRect);
            else
                ControlPaint::DrawScrollButton(g, paintRect, ScrollButton::Down, ButtonState::Flat);
            break;
        }
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/