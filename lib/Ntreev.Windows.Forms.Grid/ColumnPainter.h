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


#pragma once
#include "IColumn.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// 열을 그릴수 있는 개체입니다.
    /// </summary>
    [System::ComponentModel::ToolboxItemAttribute(true)]
    [System::Drawing::ToolboxBitmapAttribute(ColumnPainter::typeid)]
    public ref class ColumnPainter abstract
        : System::ComponentModel::Component
    {
    public: // methods
        /// <summary>
        /// <see cref="ColumnPainter"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        ColumnPainter();

        /// <summary>
        /// 열의 배경을 그립니다.
        /// </summary>
        /// <returns>
        /// 그리기가 성공하여 기본 그리기 수행이 필요 없을때는 true, 그리기가 실패하여 기본 그리기를 수행할 필요가 있다면 false를 반환합니다.
        /// </returns>
        /// <param name="g">그리기에 사용되는 그래픽 개체입니다.</param>
        /// <param name="paintRect">그리기에 대상이 되는 열의 영역입니다.</param>
        /// <param name="column">그리기에 대상이 되는 열의 정보를 가져올 수 있는 개체입니다.</param>
        virtual bool PaintBackground(System::Drawing::Graphics^ g, System::Drawing::Rectangle paintRect, IColumn^ column) abstract;

        /// <summary>
        /// 열의 배경을 제외한 나머지를 그립니다.
        /// </summary>
        /// <remarks>
        /// 배경을 제외한 텍스트 출력 및 정렬 표시등을 그리고 싶다면 이 메서드를 구현하면 됩니다.
        /// </remarks>
        /// <returns>
        /// 그리기가 성공하여 기본 그리기 수행이 필요 없을때는 true, 그리기가 실패하여 기본 그리기를 수행할 필요가 있다면 false를 반환합니다.
        /// </returns>
        /// <param name="g">그리기에 사용되는 그래픽 개체입니다.</param>
        /// <param name="paintRect">그리기에 대상이 되는 열의 영역입니다.</param>
        /// <param name="column">그리기에 대상이 되는 열의 정보를 가져올 수 있는 개체입니다.</param>
        virtual bool PaintContents(System::Drawing::Graphics^ g, System::Drawing::Rectangle paintRect, IColumn^ column) abstract;

    protected: // methods
        /// <summary>
        /// 기본 그리기에서 정렬방식을 표현하는 화살표를 그립니다.
        /// </summary>
        /// <param name="g">그리기에 사용되는 그래픽 개체입니다.</param>
        /// <param name="paintRect">화살표를 그리기 위한 영역입니다.</param>
        /// <param name="sortType">정렬방식을 나타내는 <see cref="SortType"/>입니다.</param>
        void DrawSortArrow(System::Drawing::Graphics^ g, System::Drawing::Rectangle paintRect, SortType sortType);

    private: // variables
        System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_sortedDownRenderer;
        System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_sortedUpPainter;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/