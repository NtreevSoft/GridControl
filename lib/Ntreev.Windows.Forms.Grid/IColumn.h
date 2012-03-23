//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4464.32161
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
#include "ICellBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// 열을 나타내기위한 속성을 정의합니다.
    /// </summary>
    public interface class IColumn
        : Ntreev::Windows::Forms::Grid::ICellBase
    {
        /// <summary>
        /// 표시할 열의 제목을 가져옵니다.
        /// </summary>
        /// <returns>
        /// 표시할 열의 제목을 나타내는 <see cref="System::String"/>입니다.
        /// </returns>
        property System::String^ Title
        {
            System::String^ get();
        }

        /// <summary>
        /// 열의 이름을 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열의 이름을 나타내는 <see cref="System::String"/>입니다.
        /// </returns>
        property System::String^ ColumnName
        {
            System::String^ get();
        }

        /// <summary>
        /// 열의 툴팁을 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열의 툴팁을 나타내는 <see cref="System::String"/>입니다.
        /// </returns>
        property System::String^ Tooltip
        {
            System::String^ get();
        }

        /// <summary>
        /// 열의 고유 아이디를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 고유 아이디를 나타내는 <see cref="System::UInt32"/>형태의 정수값입니다.
        /// </returns>
        property unsigned int ColumnID
        {
            unsigned int get(); 
        }

        /// <summary>
        /// 열의 최소 너비를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열의 최소 너비를 나타내는 <see cref="System::Int32"/>형태의 정수값입니다.
        /// </returns>
        property int MinWidth
        {
            int get();
        }

        /// <summary>
        /// 열의 최대 너비를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열의 최대 너비를 나타내는 <see cref="System::Int32"/>형태의 정수값입니다.
        /// </returns>
        property int MaxWidth
        {
            int get();
        }

        /// <summary>
        /// 열의 표시 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열이 표시되면 true를, 그렇지 않다면 false를 가져옵니다.
        /// </returns>
        property bool IsVisible
        {
            bool get();
        }

        /// <summary>
        /// 열의 위치 이동 여부를 가져옵니다.
        /// </summary>
        /// <remarks>
        /// 이 속성은 마우스를 이용하여 표시되는 열의 위치를 바꿀수 있는지에 대한 여부를 확인하는 것입니다. 
        /// 이 속성의 값이 false이면 마우스를 이용하여 열의 위치 이동을 할 수 없습니다.
        /// </remarks>
        /// <returns>
        /// 열의 위치 이동이 가능하다면 true를, 그렇지 않다면 false를 가져옵니다.
        /// </returns>
        property bool IsMovable
        {
            bool get();
        }

        /// <summary>
        /// 열의 너비 조절 여부를 가져옵니다.
        /// </summary>
        /// <remarks>
        /// 이 속성은 마우스를 이용하여 열의 너비를 조절할 수 있는지에 대한 여부를 확인합니다. 
        /// 이 속성의 값이 false이면 마우스를 이용하여 열의 너비조절을 할 수 없습니다.
        /// </remarks>
        /// <returns>
        /// 열의 너비 조절이 가능하면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        property bool IsResizable
        {
            bool get();
        }

        /// <summary>
        /// 수평 스크롤시 스크롤에 적용 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 수평 스크롤시 스크롤에 적용이 되지 않으면 true, 적용이 되면 false를 반환합니다.
        /// </returns>
        /// <remarks>
        /// 열의 갯수가 많아 화면에 다 표시 되지 못할때는 수평 스크롤을 통해서 보이지 않는 열을 볼 수 있습니다. 
        /// 이 속성의 값이 true이면 분할면을 기준으로 좌측에 배치되어 수평 스크롤을 해도 움직이지 않고 고정되어 항상 화면에 표시할 수 있습니다.
        /// </remarks>
        property bool IsFrozen
        {
            bool get();
        }

        /// <summary>
        /// 열이 읽기 전용인지에 대한 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열이 읽기 전용이라면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        property bool IsReadOnly
        {
            bool get();
        }

        /// <summary>
        /// 정렬 가능 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 정렬이 가능하다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        property bool IsSortable
        {
            bool get();
        }

        ///// <summary>
        ///// 열이 데이터 소스와 바인딩 될 수 있는지에 대한 여부를 가져오거나 설정합니다.
        ///// </summary>
        ///// <returns>
        ///// 열이 데이터 소스와 바인딩 될 수 있다면 true를, 그렇지 않다면 false를 반환합니다.
        ///// </returns>
        //property bool IsBindable
        //{
        // bool get();
        //}

        /// <summary>
        /// 소유한 셀들의 선택 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 소유한 셀들중 한개 이상이라도 선택이 되었다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        property bool IsSelected
        {
            bool get();
        }

        /// <summary>
        /// 소유한 셀들이 모두 선택되어 있는지에 대한 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 소유한 셀들이 모두 선택되었다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        property bool IsFullSelected
        {
            bool get(); 
        }

        /// <summary>
        /// 열의 그룹핑 기능이 적용되었는지에 대한 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열에 그룹핑 기능이 적용되었다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        property bool IsGrouped
        {
            bool get();
        }

        /// <summary>
        /// 열의 그룹핑 기능을 적용할 수 있는지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 열에 그룹핑 기능을 적용할 수 있다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        property bool IsGroupable
        {
            bool get();
        }

        /// <summary>
        /// 데이터 소스에 의해 바인딩 될 경우에 데이터 소스에서 지정한 데이터 타입을 가져옵니다.
        /// </summary>
        /// <returns>
        /// 데이터 타입을 나타내는 <see cref="System::Type"/>입니다.
        /// </returns>
        property System::Type^ SourceType
        {
            System::Type^ get();
        }

        /// <summary>
        /// 소유한 셀들이 가질 값의 데이터 타입을 가져옵니다.
        /// </summary>
        /// <returns>
        /// 데이터 타입을 나타내는 <see cref="System::Type"/>입니다.
        /// </returns>
        property System::Type^ DataType
        {
            System::Type^ get();
        }

        property System::ComponentModel::TypeConverter^ TypeConverter
        {
            System::ComponentModel::TypeConverter^ get();
        }

        /// <summary>
        /// 열의 정렬방식을 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열의 정렬방식을 나타내는 <see cref="SortType"/>입니다.
        /// </returns>
        property Ntreev::Windows::Forms::Grid::SortType SortType
        {
            Ntreev::Windows::Forms::Grid::SortType get();
        }

        /// <summary>
        /// 열이 표시되는 순서를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열이 보여지지 않거나 그리드 컨트롤에 종속되어 있지 안다면 -1을 반환합니다.
        /// </returns>
        property int VisibleIndex
        {
            int get(); 
        }

        /// <summary>
        /// 열이 화면에 표시되는 순서를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열이 화면에 보여지지 않다면 -1을 반환합니다.
        /// </returns>
        property int DisplayIndex
        {
            int get(); 
        }

        /// <summary>
        /// 그리드 컨트롤에 종속된 순서를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 그리드 컨트롤에 종속되었다면 종속된 순서(0부터 시작)를 반환하고, 그렇지 않다면 -1를 반환합니다.
        /// </returns>
        property int Index
        {
            int get();
        }

        /// <summary>
        /// 열의 대한 사용자 정의 데이터를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열의 대한 사용자 정의 데이터를 나타내는 <see cref="System::Object"/>입니다.
        /// </returns>
        property System::Object^ Tag
        {
            System::Object^ get();
        }

        /// <summary>
        /// 마우스 커서에 대한 열의 상태를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 마우스 커서에 대한 열의 상태를 나타내는 <see cref="ColumnState"/>입니다.
        /// </returns>
        property Ntreev::Windows::Forms::Grid::ColumnState ColumnState
        {
            Ntreev::Windows::Forms::Grid::ColumnState get();
        }
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/