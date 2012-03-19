//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4461.30274
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
#include "GrGridType.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// 선택 방법을 지정합니다.
    /// </summary>
    public enum class SelectionType : int
    {
        /// <summary>
        /// 모든 선택된 개체를 제거후 대상을 선택합니다.
        /// </summary>
        Normal,

        /// <summary>
        /// 선택된 개체 리스트에 대상을 추가합니다.
        /// </summary>
        Add,

        /// <summary>
        /// 선택된 개체 리스트에 대상을 제거합니다.
        /// </summary>
        Remove,
    };

    /// <summary>
    /// 선택 범위를 지정합니다.
    /// </summary>
    public enum class SelectionRange : int
    {
        /// <summary>
        /// 선택 범위중 최종적인 하나만 선택합니다.
        /// </summary>
        One,

        /// <summary>
        /// 선택 범위중 모든것을 선택합니다.
        /// </summary>
        Multi,
    };

    /// <summary>
    /// 행의 강조표시 방법을 지정합니다.
    /// </summary>
    public enum class RowHighlightType : int
    {
        /// <summary>
        /// 행의 강조표시를 영역을 색상으로 채워 표시합니다.
        /// </summary>
        Fill,

        /// <summary>
        /// 행의 강조표시를 영역의 외곽선을 그려 표시합니다.
        /// </summary>
        Line,

        /// <summary>
        /// 행의 강조표시를 외곽선과 색상 채우기 모두를 사용합니다.
        /// </summary>
        Both,
    };

    public enum class ViewType : int
    {
        Text,
        Icon,
        Custom,
    };

    public enum class ClickEditType : int
    {
        Default,
        SingClick,
        DoubleClick,
        FocusedClick,
    };

    [System::FlagsAttribute]
    public enum class CellState : int
    {
        Normal = 0,
        Focused = 1,
        Hot = 2,
        Selected = 4,
        Pressed = 8,

        All = 0x0000000f,
    };

    /// <summary>
    /// 정렬 방식을 지정합니다.
    /// </summary>
    public enum class SortType : int
    {
        /// <summary>
        /// 정렬을 하지 않습니다.
        /// </summary>
        None,

        /// <summary>
        /// 오름차순으로 정렬을 합니다.
        /// </summary>
        Up,

        /// <summary>
        /// 내림차순으로 정렬을 합니다.
        /// </summary>
        Down,
    };


    /// <summary>
    /// 마우스 커서에 따른 열의 상태를 나타냅니다.
    /// </summary>
    public enum class ColumnState : int
    {
        /// <summary>
        /// 마우스 커서가 열의 영역에 있지 않은 일반적인 상태입니다.
        /// </summary>
        Normal,

        /// <summary>
        /// 마우스 커서가 열의 영역에 있고 좌측 버튼이 눌러진 상태입니다.
        /// </summary>
        Pressed,

        /// <summary>
        /// 마우스 커서가 열의 영역에 있는 상태입니다.
        /// </summary>
        Hot,
    };

    /// <summary>
    /// 편집 시작 방식을 지정합니다.
    /// </summary>
    public enum class EditingReasonType : int
    {
        /// <summary>
        /// 추가적인 정보 없이 편집합니다.
        /// </summary>
        None,

        /// <summary>
        /// 마우스 위치를 이용해 편집을 시작합니다.
        /// </summary>
        Mouse,

        /// <summary>
        /// 키를 이용해 편집을 시작합니다.
        /// </summary>
        /// <remarks>
        /// 알파벳과 한글키등을 제외한 숫자키, 특수문자, f1 ~ f12키 등이 해당됩니다.
        /// </remarks>
        Key,

        /// <summary>
        /// 문자열키 정보를 이용하여 편집을 시작합니다.
        /// </summary>
        /// <remarks>
        /// 일반적으로 알파벳에 해당합니다.
        /// </remarks>
        Char,

        /// <summary>
        /// ime키 정보를 이용하여 편집을 시작합니다.
        /// </summary>
        Ime,
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/