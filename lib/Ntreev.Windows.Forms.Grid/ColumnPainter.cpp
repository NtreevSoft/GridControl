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
