#pragma once
#include "Types.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// 개체의 스타일 속성을 정의합니다.
    /// </summary>
    public interface class ICellBase
    {
        /// <summary>
        /// 개체의 전경색을 가져옵니다.
        /// </summary>
        /// <returns>
        /// 전경색을 나타내는 <see cref="System::Drawing::Color"/>입니다.
        /// </returns>
        property System::Drawing::Color ForeColor
        {
            System::Drawing::Color get();
        }

        /// <summary>
        /// 개체의 배경색을 가져옵니다.
        /// </summary>
        /// <returns>
        /// 배경색을 나타내는 <see cref="System::Drawing::Color"/>입니다.
        /// </returns>
        property System::Drawing::Color BackColor
        {
            System::Drawing::Color get();
        }

        /// <summary>
        /// 개체의 글꼴을 가져옵니다.
        /// </summary>
        /// <returns>
        /// 글꼴색을 나타내는 <see cref="System::Drawing::Font"/>입니다.
        /// </returns>
        property System::Drawing::Font^ Font
        {
            System::Drawing::Font^ get();
        }

        property System::Drawing::Color PaintingForeColor
        {
            System::Drawing::Color get();
        }

        property System::Drawing::Color PaintingBackColor
        {
            System::Drawing::Color get();
        }

        property System::Drawing::Rectangle Bounds
        {
            System::Drawing::Rectangle get();
        }

        property int X
        {
            int get();
        }

        property int Y
        {
            int get();
        }

        property int Width
        {
            int get();
        }

        property int Height
        {
            int get();
        }

        property System::Drawing::Point Location
        {
            System::Drawing::Point get();
        }

        property System::Drawing::Size Size
        {
            System::Drawing::Size get();
        }

        property int Left
        {
            int get();
        }

        property int Top
        {
            int get();
        }

        property int Right
        {
            int get();
        }

        property int Bottom
        {
            int get();
        }

        property System::Drawing::Rectangle ClientRectangle
        {
            System::Drawing::Rectangle get();
        }

        property System::Windows::Forms::Padding Padding
        {
            System::Windows::Forms::Padding get();
        }

        property bool Multiline
        {
            bool get();
        }

        property bool WordWrap
        {
            bool get();
        }

        /// <summary>
        /// 셀의 수평 정렬방식을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 수평 정렬방식을 나타내는 <see cref="System::Drawing::StringAlignment"/>입니다.
        /// </returns>
        property System::Drawing::StringAlignment Alignment
        {
            System::Drawing::StringAlignment get(); 
        }

        /// <summary>
        /// 셀의 수직 정렬방식을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 수직 정렬방식을 나타내는 <see cref="System::Drawing::StringAlignment"/>입니다.
        /// </returns>
        property System::Drawing::StringAlignment LineAlignment
        {
            System::Drawing::StringAlignment get();
        }

        property CellState State
        {
            CellState get();
        }
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/