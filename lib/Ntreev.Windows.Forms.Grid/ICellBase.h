#pragma once
#include "GridType.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// ��ü�� ��Ÿ�� �Ӽ��� �����մϴ�.
    /// </summary>
    public interface class ICellBase
    {
        /// <summary>
        /// ��ü�� ������� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ������� ��Ÿ���� <see cref="System::Drawing::Color"/>�Դϴ�.
        /// </returns>
        property System::Drawing::Color ForeColor
        {
            System::Drawing::Color get();
        }

        /// <summary>
        /// ��ü�� ������ �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ������ ��Ÿ���� <see cref="System::Drawing::Color"/>�Դϴ�.
        /// </returns>
        property System::Drawing::Color BackColor
        {
            System::Drawing::Color get();
        }

        /// <summary>
        /// ��ü�� �۲��� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// �۲û��� ��Ÿ���� <see cref="System::Drawing::Font"/>�Դϴ�.
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
        /// ���� ���� ���Ĺ���� �������ų� �����մϴ�.
        /// </summary>
        /// <returns>
        /// ���� ���Ĺ���� ��Ÿ���� <see cref="System::Drawing::StringAlignment"/>�Դϴ�.
        /// </returns>
        property System::Drawing::StringAlignment Alignment
        {
            System::Drawing::StringAlignment get(); 
        }

        /// <summary>
        /// ���� ���� ���Ĺ���� �������ų� �����մϴ�.
        /// </summary>
        /// <returns>
        /// ���� ���Ĺ���� ��Ÿ���� <see cref="System::Drawing::StringAlignment"/>�Դϴ�.
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