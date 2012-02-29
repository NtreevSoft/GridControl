﻿//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
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
#include "GridEvent.h"
#include "GridCellBase.h"
#include "GridUtil.h"
#include "GridTypeEditor.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
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
    /// 열을 나타내기위한 속성을 정의합니다.
    /// </summary>
    public interface class IColumn : ICellBase
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
        property uint ColumnID
        {
            uint get(); 
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
        property bool CanBeSorted
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
        //	bool get();
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
        property bool CanBeGrouping
        {
            bool get();
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

    /// <summary>
    /// 열을 그릴수 있는 개체입니다.
    /// </summary>
    public ref class ColumnPainter abstract : System::ComponentModel::Component
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
        System::Windows::Forms::VisualStyles::VisualStyleRenderer^	m_sortedDownRenderer;
        System::Windows::Forms::VisualStyles::VisualStyleRenderer^	m_sortedUpPainter;
    };

    /// <summary>
    /// 열을 나타냅니다.
    /// </summary>
    [System::ComponentModel::TypeConverter(System::ComponentModel::ExpandableObjectConverter::typeid)]
    [System::ComponentModel::ToolboxItem(false)]
    [System::ComponentModel::DesignTimeVisible(false)]
    [System::ComponentModel::Designer("Ntreev.Windows.Forms.Grid.Design.ColumnDesigner, Ntreev.Windows.Forms.Grid.Design, Version=1.1.4324.22060, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca")]
    public ref class Column : CellBase, IColumn, System::ComponentModel::IComponent, System::IServiceProvider
    {
    public: // methods
        /// <summary>
        /// <see cref="Column"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        Column();

        /// <summary>
        /// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>을 반환합니다.
        /// </summary>
        /// <returns>
        /// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>입니다.
        /// </returns>
        virtual	System::String^ ToString() override;

        /// <summary>
        /// 사용되지 않습니다.
        /// </summary>
        [System::ObsoleteAttribute]
        void RefreshAll();

        /// <summary>
        /// 셀의 전경색을 기본값으로 되돌립니다.
        /// </summary>
        void ResetCellForeColor();

        /// <summary>
        /// 셀의 배경색을 기본값으로 되돌립니다.
        /// </summary>
        void ResetCellBackColor();

        /// <summary>
        /// 셀의 내부간격을 기본값으로 되돌립니다.
        /// </summary>
        void ResetCellPadding();

        /// <summary>
        /// 셀의 글꼴을 기본값으로 되돌립니다.
        /// </summary>
        void ResetCellFont();

        /// <summary>
        /// 셀의 값을 그릴때 호출됩니다.
        /// </summary>
        /// <param name="graphics">그리기에 사용되는 그래픽 개체입니다.</param>
        /// <param name="paintRect">그리기에 대상이 되는 셀의 안쪽 영역입니다.</param>
        /// <param name="cell">그리기에 대상이 되는 셀의 스타일 정보를 가져올 수 있는 개체입니다.</param>
        /// <param name="value">그리기에 대상이 되는 셀의 값입니다.</param>
        virtual void PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, ICell^ cell, System::Object^ value);

    public: // properties
        /// <summary>
        /// 표시할 열의 제목을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 표시할 열의 제목을 나타내는 <see cref="System::String"/>입니다.
        /// </returns>
        property System::String^ Title
        {
            virtual System::String^ get() sealed;
            void set(System::String^); 
        }

        /// <summary>
        /// 열의 이름을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 열의 이름을 나타내는 <see cref="System::String"/>입니다.
        /// </returns>
        /// <remarks>
        /// 열의 이름은 일종의 키 값으로 사용됩니다. 따라서 다른 열과 이름이 같을경우 예외가 발생합니다.
        /// </remarks>
        /// <exception cref="System::ArgumentException">이름을 빈 문자열로 설정하거나. <see cref="GridControl"/>에 종속되어 있는 경우 같은 이름을 가진 열이 이미 있을때.</exception>
        [System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::Repaint)]
        property System::String^ ColumnName
        {
            virtual System::String^ get() sealed;
            virtual void set(System::String^); 
        }

        /// <summary>
        /// 열의 툴팁을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 열의 툴팁을 나타내는 <see cref="System::String"/>입니다.
        /// </returns>
        [System::ComponentModel::DefaultValueAttribute("")]
        property System::String^ Tooltip
        {
            virtual System::String^ get() sealed;
            void set(System::String^);
        }

        /// <summary>
        /// 열의 고유 아이디를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 고유 아이디를 나타내는 <see cref="System::UInt32"/>형태의 정수값입니다.
        /// </returns>
        [System::ComponentModel::BrowsableAttribute(false)]
        property uint ColumnID
        {
            virtual uint get() sealed;
        }

        /// <summary>
        /// 열의 너비를 가져오거나 설정합니다.
        /// </summary>
        /// <remarks>
        /// 최소 너비보가 작거나 최대 너비보다 클 경우 자동으로 값이 수정됩니다.
        /// </remarks>
        /// <returns>
        /// 열의 너비를 나타내는 <see cref="System::Int32"/>형태의 정수값입니다.
        /// </returns>
        /// <exception cref="System::ArgumentException">0보다 작은 수를 설정할때.</exception>
        [System::ComponentModel::CategoryAttribute("Layout"), System::ComponentModel::DefaultValueAttribute(100)]
        property int Width
        {
            int get();
            void set(int);
        }

        /// <summary>
        /// 열의 최소 너비를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 열의 최소 너비를 나타내는 <see cref="System::Int32"/>형태의 정수값입니다.
        /// </returns>
        /// <exception cref="System::ArgumentException">0보다 작거나 최대 너비보다 큰 수를 설정할때.</exception>
        [System::ComponentModel::CategoryAttribute("Layout"), System::ComponentModel::DefaultValueAttribute(30)]
        property int MinWidth
        {
            virtual int get() sealed;
            void set(int); 
        }

        /// <summary>
        /// 열의 최대 너비를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 열의 최대 너비를 나타내는 <see cref="System::Int32"/>형태의 정수값입니다.
        /// </returns>
        /// <exception cref="System::ArgumentException">0보다 작거나 최소 너비보다 작은 수를 설정할때.</exception>
        [System::ComponentModel::CategoryAttribute("Layout"), System::ComponentModel::DefaultValueAttribute(10000)]
        property int MaxWidth
        {
            virtual int get() sealed;
            void set(int);
        }

        /// <summary>
        /// 열의 표시 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 열이 표시되면 true를, 그렇지 않다면 false를 가져옵니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Behavior"), System::ComponentModel::DefaultValueAttribute(true)]
        property bool IsVisible
        {
            virtual bool get() sealed;
            void set(bool); 
        }

        /// <summary>
        /// 열의 위치 이동 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <remarks>
        /// 이 속성은 마우스를 이용하여 표시되는 열의 위치를 바꿀수 있는지에 대한 여부를 확인하는 것입니다. 
        /// 이 속성의 값이 false이면 마우스를 이용하여 열의 위치 이동을 할 수 없습니다.
        /// </remarks>
        /// <returns>
        /// 열의 위치 이동이 가능하다면 true를, 그렇지 않다면 false를 가져옵니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Behavior"), System::ComponentModel::DefaultValueAttribute(true)]
        property bool IsMovable
        {
            virtual bool get() sealed;
            void set(bool);
        }

        /// <summary>
        /// 열의 너비 조절 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <remarks>
        /// 이 속성은 마우스를 이용하여 열의 너비를 조절할 수 있는지에 대한 여부를 확인합니다. 
        /// 이 속성의 값이 false이면 마우스를 이용하여 열의 너비조절을 할 수 없습니다.
        /// </remarks>
        /// <returns>
        /// 열의 너비 조절이 가능하면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Behavior"), System::ComponentModel::DefaultValueAttribute(true)]
        property bool IsResizable
        {
            virtual bool get() sealed;
            void set(bool);
        }

        /// <summary>
        /// 수평 스크롤시 스크롤에 적용 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 수평 스크롤시 스크롤에 적용이 되지 않으면 true, 적용이 되면 false를 반환합니다.
        /// </returns>
        /// <remarks>
        /// 열의 갯수가 많아 화면에 다 표시 되지 못할때는 수평 스크롤을 통해서 보이지 않는 열을 볼 수 있습니다. 
        /// 이 속성의 값이 true이면 분할면을 기준으로 좌측에 배치되어 수평 스크롤을 해도 움직이지 않고 고정되어 항상 화면에 표시할 수 있습니다.
        /// </remarks>
        [System::ComponentModel::CategoryAttribute("Behavior"), System::ComponentModel::DefaultValueAttribute(false)]
        property bool IsFrozen
        {
            virtual bool get() sealed;
            void set(bool);
        }

        /// <summary>
        /// 열이 읽기 전용인지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 열이 읽기 전용이라면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        /// <remarks>
        /// 이 속성의 값이 true이면 열이 소유한 모든 셀이 읽기 전용으로 적용됩니다.
        /// </remarks>
        [System::ComponentModel::CategoryAttribute("Behavior"), System::ComponentModel::DefaultValueAttribute(false)]
        property bool IsReadOnly
        {
            virtual bool get() sealed;
            void set(bool);
        }

        /// <summary>
        /// 정렬 가능 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 정렬이 가능하다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Behavior"), System::ComponentModel::DefaultValueAttribute(true)]
        property bool CanBeSorted
        {
            virtual bool get() sealed;
            void set(bool);
        }

        /// <summary>
        /// 소유한 셀들의 선택 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 소유한 셀들중 한개 이상이라도 선택이 되었다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        /// <param name="value">
        /// 소유한 모든 셀을 선택하려면 true를, 해제하려면 false를 설정합니다.
        /// </param>
        [System::ComponentModel::BrowsableAttribute(false)]
        [System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
        property bool IsSelected
        {
            virtual bool get() sealed;
            void set(bool);
        }

        /// <summary>
        /// 소유한 셀들이 모두 선택되어 있는지에 대한 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 소유한 셀들이 모두 선택되었다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        [System::ComponentModel::BrowsableAttribute(false)]
        property bool IsFullSelected
        {
            virtual bool get() sealed; 
        }

        /// <summary>
        /// 열의 그룹핑 기능이 적용되었는지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 열에 그룹핑 기능이 적용되었다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        [System::ComponentModel::DefaultValueAttribute(false)]
        property bool IsGrouped
        {
            virtual bool get() sealed;
            void set(bool);
        }

        /// <summary>
        /// 열의 그룹핑 기능을 적용 할 수 있는지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 열에 그룹핑 기능을 적용 할 수 있다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        /// <remarks>
        /// 이 속성은 사용자가 그룹핑 기능을 사용할지에 대한 판단근거로 사용하기 위해 제공됩니다. 
        /// 때문에 이 속성이 false라고 해도 그룹핑 기능을 사용할 수 없는것은 아닙니다.
        /// 하지만 마우스를 이용한 그룹핑 기능을 사용할 수 없습니다.
        /// </remarks>
        [System::ComponentModel::DefaultValueAttribute(true)]
        property bool CanBeGrouping
        {
            virtual bool get() sealed;
            void set(bool);
        }

        /// <summary>
        /// 소유한 셀들이 가질 값의 데이터 타입을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 데이터 타입을 나타내는 <see cref="System::Type"/>입니다.
        /// </returns>
        /// <exception cref="System::InvalidOperationException">
        /// 그리드 컨트롤에 바인딩 된 후에 값을 설정하려고 할때.
        /// </exception>
        [System::ComponentModel::EditorAttribute("Ntreev.Windows.Forms.Grid.Design.TypeSelector, Ntreev.Windows.Forms.Grid.Design, Version=1.1.4324.22060, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca", System::Drawing::Design::UITypeEditor::typeid)]
        property System::Type^ DataType
        {
            virtual System::Type^ get() sealed;
            void set(System::Type^);
        }

        /// <summary>
        /// 소유한 셀들이 가질 값의 데이터 타입 변환기를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 데이터 타입 변환기를 나타내는 <see cref="System::ComponentModel::TypeConverter"/>입니다.
        /// </returns>
        /// <remarks>
        /// 이 속성의 기본값은 <see cref="DataType"/>을 기준으로 설정됩니다. 추가적인 타입에 대한 변환를 설정하려면 새로운 타입 변환기를 정의하여 설정하면 됩니다.
        /// </remarks>
        [System::ComponentModel::BrowsableAttribute(false)]
        [System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
        property System::ComponentModel::TypeConverter^ TypeConverter
        {
            System::ComponentModel::TypeConverter^ get();
            void set(System::ComponentModel::TypeConverter^);
        }

        /// <summary>
        /// 열의 정렬방식을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 열의 정렬방식을 나타내는 <see cref="SortType"/>입니다.
        /// </returns>
        [System::ComponentModel::DefaultValueAttribute(Ntreev::Windows::Forms::Grid::SortType::None)]
        property Ntreev::Windows::Forms::Grid::SortType SortType
        {
            virtual Ntreev::Windows::Forms::Grid::SortType get() sealed;
            void set(Ntreev::Windows::Forms::Grid::SortType);
        }

        /// <summary>
        /// 열의 정렬을 하기 위해 비교하는 개체를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 값의 비교를 위한 개체를 나타내는 <see cref="System::Collections::IComparer"/>입니다.
        /// </returns>
        [System::ComponentModel::BrowsableAttribute(false), System::ComponentModel::DefaultValueAttribute((System::String^)nullptr)]
        property System::Collections::IComparer^ SortComparer
        {
            System::Collections::IComparer^ get();
            void set(System::Collections::IComparer^);
        }

        /// <summary>
        /// 열이 표시되는 순서를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열이 보여지지 않거나 그리드 컨트롤에 종속되어 있지 안다면 -1을 반환합니다.
        /// </returns>
        [System::ComponentModel::BrowsableAttribute(false)]
        property int VisibleIndex
        {
            virtual int get() sealed; 
        }

        /// <summary>
        /// 열이 화면에 표시되는 순서를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열이 화면에 보여지지 않다면 -1을 반환합니다.
        /// </returns>
        [System::ComponentModel::BrowsableAttribute(false)]
        property int DisplayIndex
        {
            virtual int get() sealed;
        }

        /// <summary>
        /// 그리드 컨트롤에 종속된 순서를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 그리드 컨트롤에 종속되었다면 종속된 순서(0부터 시작)를 반환하고, 그렇지 않다면 -1를 반환합니다.
        /// </returns>
        [System::ComponentModel::DescriptionAttribute("컨트롤에 내에 Column 인덱스를 가져옵니다.")]
        [System::ComponentModel::CategoryAttribute("Debug")]
        property int Index
        {
            virtual int get() sealed; 
        }

        /// <summary>
        /// 셀의 기본값을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 셀의 기본값을 나타내는 <see cref="System::Object"/>입니다.
        /// </returns>
        /// <remarks>
        /// 이 속성의 사용 목적은 새로운 행을 추가 하기 위하여 제공되는 <see cref="InsertionRow"/>에 기본값을 제공하기 위해서입니다. 
        /// 데이터 타입에 대한 검사가 없으므로, 가급적 <see cref="DataType"/>과 같은 타입의 개체를 사용하시기 바랍니다.
        /// </remarks>
        [System::ComponentModel::BrowsableAttribute(false)]
        [System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
        property System::Object^ DefaultValue
        {
            virtual System::Object^ get();
            virtual void set(System::Object^);
        }

        /// <summary>
        /// 열을 그릴수 있는 개체를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 열을 그릴수 있는 개체를 나타내는 <see cref="ColumnPainter"/>입니다.
        /// </returns>
        [System::ComponentModel::BrowsableAttribute(false)]
        [System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
        property Ntreev::Windows::Forms::Grid::ColumnPainter^ ColumnPainter
        {
            Ntreev::Windows::Forms::Grid::ColumnPainter^ get();
            void set(Ntreev::Windows::Forms::Grid::ColumnPainter^);
        }

        /// <summary>
        /// 셀의 값을 문자열로 출력시에 사용하는 수평 정렬방식을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 문자열 정렬 방식을 나타내는 <see cref="System::Drawing::StringAlignment"/>입니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Cell")]
        [System::ComponentModel::DefaultValueAttribute(System::Drawing::StringAlignment::Near)]
        property System::Drawing::StringAlignment CellAlignment
        {
            System::Drawing::StringAlignment get();
            void set(System::Drawing::StringAlignment);
        }

        /// <summary>
        /// 셀의 값을 문자열로 출력시에 사용하는 수직 정렬방식을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 문자열 정렬 방식을 나타내는 <see cref="System::Drawing::StringAlignment"/>입니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Cell")]
        [System::ComponentModel::DefaultValueAttribute(System::Drawing::StringAlignment::Near)]
        property System::Drawing::StringAlignment CellLineAlignment
        {
            System::Drawing::StringAlignment get();
            void set(System::Drawing::StringAlignment);
        }

        /// <summary>
        /// 셀의 값을 문자열로 출력시에 자동개행을 적용할 지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 자동개행을 적용하면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Cell"), System::ComponentModel::DefaultValueAttribute(false)]
        property bool CellWordWrap
        {
            bool get();
            void set(bool);
        }

        /// <summary>
        /// 셀의 값을 문자열로 출력시에 멀티라인을 적용할 지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 멀티라인 적용하면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Cell"), System::ComponentModel::DefaultValueAttribute(false)]
        property bool CellMultiline
        {
            bool get();
            void set(bool);
        }

        /// <summary>
        /// 셀의 전경색을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 셀의 전경색을 나타내는 <see cref="System::Drawing::Color"/>입니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Cell")]
        property System::Drawing::Color CellForeColor
        {
            System::Drawing::Color get();
            void set(System::Drawing::Color);
        }

        /// <summary>
        /// 셀의 배경색을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 셀의 배경색을 나타내는 <see cref="System::Drawing::Color"/>입니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Cell")]
        property System::Drawing::Color CellBackColor
        {
            System::Drawing::Color get();
            void set(System::Drawing::Color);
        }

        /// <summary>
        /// 셀의 내부간격을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 셀의 내부간격을 나타내는 <see cref="System::Windows::Forms::Padding"/>입니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Cell")]
        property System::Windows::Forms::Padding CellPadding
        {
            System::Windows::Forms::Padding get();
            void set(System::Windows::Forms::Padding);
        }

        /// <summary>
        /// 셀의 글꼴을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 셀의 글꼴을 나타내는 <see cref="System::Drawing::Font"/>입니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Cell")]
        property System::Drawing::Font^ CellFont
        {
            System::Drawing::Font^ get();
            void set(System::Drawing::Font^); 
        }

        /// <summary>
        /// 셀의 최소 높이를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 셀의 최소 높이를 나타내는 정수 값입니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Cell")]
        [System::ComponentModel::DefaultValueAttribute(0)]
        property int CellMinHeight
        {
            int get();
            void set(int);
        }

        [System::ComponentModel::DefaultValueAttribute(Ntreev::Windows::Forms::Grid::ClickEditType::Default)]
        property Ntreev::Windows::Forms::Grid::ClickEditType ClickEditType
        {
            Ntreev::Windows::Forms::Grid::ClickEditType get();
            void set(Ntreev::Windows::Forms::Grid::ClickEditType);
        }

        /// <summary>
        /// 셀의 값을 그릴 수 있는 기능을 제공하는지에 대한 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 셀의 값을 그리기 위해 <see cref="PaintValue"/>를 사용하려면 true를, 그렇지 않으면 false를 반환합니다.
        /// </returns>
        property ViewType View
        {
            virtual ViewType get();
        }

        /// <summary>
        /// 데이터 소스 삭제시 이 열의 인스턴스 삭제 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 데이터 소스 삭제시 이 열의 인스턴스가 삭제된다면 true를 그렇지 않다면 false를 반환합니다.
        /// </returns>
        [System::ComponentModel::DefaultValueAttribute(false)]
        property bool HasLifeline;

    protected: // methods
        /// <summary>
        /// 소멸자 입니다.
        /// </summary>
        ~Column();

        virtual System::Object^ EditValue(Design::IEditorService^ editorService, ICell^ cell, System::Object^ value);

        virtual bool CanEdit(ICell^ cell, Design::EditingReason reason);

        virtual Design::EditStyle GetEditStyle();

        virtual void OnGridControlAttached() {};

        virtual void OnGridControlDetached() {};

    public protected:
        virtual void OnGridControlAttachedInternal() override;

        virtual void OnGridControlDetachedInternal() override;

    internal: // methods
        System::Object^ EditValueInternal(Design::IEditorService^ editorService, ICell^ cell, System::Object^ value);

        bool CanEditInternal(ICell^ cell, Design::EditingReason reason);

        static Column^ FromNative(const GrColumn* pColumn);

        void AsyncDisplayText();

        bool CanConvertFrom(System::Type^ sourceType);

        System::Object^ ConvertFromSource(System::Object^ value);

        System::Object^ ConvertToSource(System::Object^ value);

    internal: // properties
        property System::ComponentModel::PropertyDescriptor^ PropertyDescriptor
        {
            System::ComponentModel::PropertyDescriptor^ get();
            void set(System::ComponentModel::PropertyDescriptor^);
        }

        property GrColumn* NativeRef
        {
            GrColumn* get() { return m_pColumn; }
        }

        property Design::TypeEditor^ TypeEditor
        {
            Design::TypeEditor^ get();
        }

    private: // methods
        virtual System::Object^ GetService(System::Type^ serviceType) sealed = System::IServiceProvider::GetService
        {
            if(m_site != nullptr)
                return m_site->GetService(serviceType);
            return nullptr;
        }

        void ValidateDefaultValue();

        bool ShouldSerializeCellForeColor();
        bool ShouldSerializeCellBackColor();
        bool ShouldSerializeCellPadding();
        bool ShouldSerializeCellFont();
        bool ShouldSerializeTitle();
        bool ShouldSerializeDataType();
        bool ShouldSerializeTypeConverter();

        void propertyDescriptor_ValueChanged(System::Object^ sender, System::EventArgs^ e);
        void SetEditStyleToNative();

    private: // properties
        property System::Object^ Tag_IColumnDescriptor
        {
            virtual System::Object^ get() sealed = IColumn::Tag::get { return this->Tag; }
        }

        property Ntreev::Windows::Forms::Grid::ColumnState ColumnState
        {
            virtual Ntreev::Windows::Forms::Grid::ColumnState get() sealed = IColumn::ColumnState::get;
        }

        property System::ComponentModel::ISite^ Site_IComponent
        {
            virtual System::ComponentModel::ISite^ get() sealed = System::ComponentModel::IComponent::Site::get { return m_site; }
            virtual void set(System::ComponentModel::ISite^ value) sealed  = System::ComponentModel::IComponent::Site::set;
        }

    private: // events
        event System::EventHandler^ Disposed_IComponent
        {
            virtual void add(System::EventHandler^ p) sealed = System::ComponentModel::IComponent::Disposed::add { m_eventDisposed += p; }
            virtual void remove(System::EventHandler^ p)  sealed = System::ComponentModel::IComponent::Disposed::remove { m_eventDisposed -= p; }
        private:

            virtual void raise(System::Object^ sender, System::EventArgs^ e) sealed { if(m_eventDisposed != nullptr) { m_eventDisposed->Invoke(sender, e); } }
        }

    private: // variables
        System::Type^						m_dataType;
        System::ComponentModel::TypeConverter^ m_typeConverter;
        System::String^						m_title;
        System::String^						m_name;
        System::String^						m_tooltip;
        System::ComponentModel::PropertyDescriptor^		m_propertyDescriptor;
        System::Collections::IComparer^ m_sortComparer;

        System::Object^						m_defaultValue;
        Ntreev::Windows::Forms::Grid::ColumnPainter^				m_columnPainter;

        class CustomPaint*			m_pCustomPaint;

        System::EventHandler^				m_eventDisposed;
        System::ComponentModel::ISite^	m_site;

        Design::TypeEditor^         m_typeEditor;
        GrColumn*					m_pColumn;
    };

    
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/