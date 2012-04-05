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
#include "CellBase.h"
#include "ICell.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    ref class Column;
    ref class Row;

    /// <summary>
    /// 셀을 나타내는 개체입니다.
    /// </summary>
    [System::ComponentModel::TypeConverterAttribute(System::ComponentModel::ExpandableObjectConverter::typeid)]
    public ref class Cell
        : Ntreev::Windows::Forms::Grid::CellBase
        , Ntreev::Windows::Forms::Grid::ICell
    {
    public: // methods

        /// <summary>
        /// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>을 반환합니다.
        /// </summary>
        /// <returns>
        /// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>입니다.
        /// </returns>
        virtual System::String^ ToString() override;

        /// <summary>
        /// 변경된 셀의 값을 취소하고 이전값으로 되돌립니다.
        /// </summary>
        /// <returns>
        /// 셀의 값이 변경되지 않았다면 false를, 변경 되었다면 true를 반환합니다.
        /// </returns>
        /// <remarks>
        /// 셀의 값이 변경되었을때 사용자는 변경된 내용을 취소하고 이전값으로 되돌릴수 있습니다.
        /// 추가적으로 그리드 컨트롤에서 esc키와 같은 수단으로 취소 명령으로 인해 자동적으로 이 메서드가 호출됩니다.
        /// </remarks>
        bool CancelEdit();

        /// <summary>
        /// 셀의 변경된 값을 적용합니다.
        /// </summary>
        /// <returns>
        /// 셀의 변경된 값이 적용되었다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        /// <remarks>
        /// 셀의 편집으로 인해 값이 변경되었다면 셀은 이전값을 보관하고 있게 됩니다. 
        /// 그래서 <see cref="CancelEdit"/>메서드를 통해서 이전값으로 되돌릴수가 있게 됩니다. 
        /// 이 메서드는 이전값을 삭제하여 <see cref="CancelEdit"/>호출로 인해 이전 값으로 되돌릴 수 없도록 하는 기능입니다.
        /// 추가적으로 그리드 컨트롤에서 포커스된 행이 변경될때는 자동적으로 이 메서드가 호출됩니다.
        /// </remarks>
        bool ApplyEdit();

        /// <summary>
        /// 셀을 선택합니다.
        /// </summary>
        /// <param name="selectionType">셀을 선택하는 방법을 지정하는 <see cref="Ntreev::Windows::Forms::Grid::SelectionType"/>입니다.</param>
        void Select(Ntreev::Windows::Forms::Grid::SelectionType selectionType);

        /// <summary>
        /// 그리드 컨트롤에 대한 포커스 설정을 합니다.
        /// </summary>
        void Focus();

        /// <summary>
        /// 셀이 화면내에 표시될 수 있도록 스크롤을 조정합니다.
        /// </summary>
        /// <remarks>
        /// 이미 화면내에 표시가 되었다면 아무 동작도 일어나지 않습니다.
        /// </remarks>
        void BringIntoView();

    public: // properties

        /// <summary>
        /// 셀의 값을 가져오거나 설정합니다.
        /// </summary>
        /// <remarks>
        /// <para>
        /// 일반적으로 값의 데이터 타입은 이 셀을 소유한 열의 데이터 타입과 같은 형태 이어야만 합니다.
        /// 만약 데이터 타입이 다를 경우 열에서 제공하는 <see cref="System::ComponentModel::TypeConverter"/>를 사용하여 값의 변환이 가능한지 여부를 조사합니다.
        /// 값의 변환이 가능하다면 변환을 하여 값을 설정하게 되고 반대의 경우는 예외가 발생하게 됩니다.
        /// </para>
        /// <para>
        /// 추가적으로 설정 값이 이전값과 같으면 위 설명한 모든 과정이 일어나지 않습니다.
        /// </para>
        /// </remarks>
        /// <returns>
        /// 값을 나타내는 <see cref="System::Object"/>입니다.
        /// </returns>
        /// <exception cref="System::NotSupportedException">
        /// 지원되지 않는 값이 설정 되었을때 발생합니다.
        /// </exception>
        [System::ComponentModel::CategoryAttribute("Data")]
        [System::ComponentModel::EditorAttribute("Ntreev.Windows.Forms.Grid.Design.ValueEditor", System::Drawing::Design::UITypeEditor::typeid)]
        [System::ComponentModel::TypeConverterAttribute(System::ComponentModel::StringConverter::typeid)]
        property System::Object^ Value
        {
            System::Object^ get();
            void set(System::Object^);
        }

        /// <summary>
        /// 이 셀을 소유한 열의 인스턴스를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열을 나타내는 <see cref="Column"/>의 인스턴스입니다.
        /// </returns>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Behavior")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property Ntreev::Windows::Forms::Grid::Column^ Column
        {
            Ntreev::Windows::Forms::Grid::Column^ get();
        }

        /// <summary>
        /// 이 셀을 소유한 열의 고유 아이디를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 고유 아이디를 나타내는 <see cref="System::UInt32"/>형태의 정수값입니다.
        /// </returns>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Behavior")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property unsigned int ColumnID
        {
            unsigned int get(); 
        }

        /// <summary>
        /// 이 셀을 소유한 행의 인스턴스를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 행을 나타내는 <see cref="Row"/>의 인스턴스입니다.
        /// </returns>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Behavior")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property Ntreev::Windows::Forms::Grid::Row^ Row
        {
            Ntreev::Windows::Forms::Grid::Row^ get();
        }

        /// <summary>
        /// 이 셀을 소유한 행의 고유 아이디를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 고유 아이디를 나타내는 <see cref="System::UInt32"/>형태의 정수값입니다.
        /// </returns>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Behavior")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property unsigned int RowID
        {
            unsigned int get(); 
        }

        /// <summary>
        /// 셀의 선택 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <remarks>
        /// 속성의 값을 true로 설정했다면 선택목록에 이 셀이 추가됩니다. 반대로 false로 설정했다면 선택목록에서 제거됩니다.
        /// </remarks>
        /// <returns>
        /// 선택되어져 있다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        /// <exception cref="System::InvalidOperationException">Cell.Row가 사용되지 않거나, 숨겨져 있을때 발생합니다.</exception>
        [System::ComponentModel::CategoryAttribute("Behavior")]
        [System::ComponentModel::DefaultValueAttribute(false)]
        [System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
        property bool IsSelected
        {
            bool get();
            void set(bool);
        }

        /// <summary>
        /// 셀의 포커스 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <remarks>
        /// 포커스로 설정할 수 있는 셀은 그리드 컨트롤내에 한개만 설정될 수 있습니다. 속성의 값을 true를 설정했다면 이전의 포커스로 설정된 셀이 현재의 셀로 대체됩니다. 
        /// 반대로 속성의 값을 false로 설정했다면 그리드 컨트롤내에서는 포커스로 설정된 셀이 없는 상태가 되어버립니다.
        /// </remarks>
        /// <returns>
        /// 포커스로 설정되었다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        /// <exception cref="System::InvalidOperationException">Cell.Row가 사용되지 않거나, 숨겨져 있을때 발생합니다.</exception>
        [System::ComponentModel::CategoryAttribute("Behavior")]
        [System::ComponentModel::DefaultValueAttribute(false)]
        [System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
        property bool IsFocused
        {
            bool get();
            void set(bool);
        }

        /// <summary>
        /// 마우스 커서의 위치가 셀 영역 안에 있는지의 여부를 확인합니다.
        /// </summary>
        /// <returns>
        /// 마우스 커서의 위치가 셀 영역 안에 있다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Debug")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif 
        property bool IsMouseOvered
        { 
            bool get();
        }

        /// <summary>
        /// 읽기 전용인지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <remarks>
        /// 이 셀을 소유하는 열 또는 행의 읽기 전용 속성값이 ture라면 셀 속성값의 상관없이 true를 반환합니다.
        /// </remarks>
        /// <returns>
        /// 읽기 전용이라면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Behavior")]
        [System::ComponentModel::DefaultValueAttribute(false)]
        property bool IsReadOnly
        {
            bool get();
            void set(bool);
        }

        /// <summary>
        /// 셀이 현재 편집중인지의 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 셀이 편집중이면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Debug")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property bool IsBeingEdited
        {
            bool get();
        }

        /// <summary>
        /// 셀의 값이 변경 되었는지의 여부를 가져옵니다.
        /// </summary>
        /// <remarks>
        /// 이 속성은 <see cref="CancelEdit"/> 또는 <see cref="ApplyEdit"/>의 호출로 인해서 값이 초기화 됩니다.
        /// </remarks>
        /// <returns>
        /// 셀의 값이 변경되었다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Debug")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property bool IsEdited
        {
            bool get();
        }

        /// <summary>
        /// 셀의 내용이 잘못 되었다는 표시를 가져오거나 설정합니다.
        /// </summary>
        /// <remarks>
        /// 이 속성의 문자열이 설정되면 그리드 컨트롤은 경고의 의미로 셀 주변을 빨간 테두리로 표시해줍니다.
        /// 또한 마우스의 커서가 이 셀 영역 안으로 들어가면 툴팁의 형태로 이 속성의 문자열을 추가적으로 표시해줍니다.
        /// 이 기능을 해제하고 싶을때는 빈 문자열을 설정하면 됩니다.
        /// </remarks>
        /// <returns>
        /// 셀의 잘못된 내용을 나타내는 문자열입니다.
        /// </returns>
        [System::ComponentModel::CategoryAttribute("Behavior")]
        [System::ComponentModel::DefaultValueAttribute("")]
        property System::String^ ErrorDescription
        {
            System::String^ get();
            void set(System::String^);
        }

        /// <summary>
        /// 문자열이 출력되는 영역을 가져옵니다.
        /// </summary>
        /// <remarks>
        /// 모든 문자열이 다 들어갈 수 있는 사각형의 영역을 나타냅니다. 이 값은 <see cref="Bounds"/>위치의 상대값입니다.
        /// </remarks>
        /// <example>
        /// 이 예제는 마우스 커서를 기준으로 셀을 검색한뒤 셀의 문자열 출력 영역을 검은색 테두리로 표시하는 코드입니다.
        /// <code>
        /// private void gridControl1_Paint(object sender, PaintEventArgs e)
        /// {
        /// Point location = this.gridControl1.PointToClient(Cursor.Position);
        /// Cell cell = this.gridControl1.GetCellAt(location);
        ///
        /// if (cell == null)
        /// return;
        ///
        /// Rectangle textBound = cell.TextBound;
        /// textBound.Offset(cell.DisplayRectangle.Location);
        ///
        /// e.Graphics.DrawRectangle(Pens.Black, textBound);
        /// }
        /// </code>
        /// </example>
        /// <returns>
        /// 문자열이 출력되는 영역을 나타내늗 <see cref="System::Drawing::Rectangle"/>입니다.
        /// </returns>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Debug")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property System::Drawing::Rectangle TextBound
        {
            System::Drawing::Rectangle get();
        }

        /// <summary>
        /// 정의되지 않은 셀입니다.
        /// </summary>
        static Ntreev::Windows::Forms::Grid::Cell^ Null = nullptr;

    internal: // methods

        Cell(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, GrItem* pItem);

        void UpdateNativeText();

        void UpdateNativeText(System::Object^ value);

        System::Object^ ValidateValue(System::Object^ value);

        void SetDefaultValue();

    internal: // properties

        property GrItem* NativeRef
        {
            GrItem* get() { return m_pItem; } 
        }

        property bool IsSelecting
        {
            bool get(); 
        }

        property System::Object^ ValueCore
        {
            System::Object^ get();
            void set(System::Object^);
        }

    private: // methods

        bool ShouldSerializeValue();

    private: // properties

        property System::Object^ Value_ICell
        {
            virtual System::Object^ get() sealed = ICell::Value::get;
        }

        property System::Object^ Tag_ICell
        {
            virtual System::Object^ get() sealed = ICell::Tag::get;
        }

        property Ntreev::Windows::Forms::Grid::IColumn^ Column_ICell
        {
            virtual Ntreev::Windows::Forms::Grid::IColumn^ get() sealed = ICell::Column::get;
        }

    private: // variables

        Ntreev::Windows::Forms::Grid::Column^ m_column;
        Ntreev::Windows::Forms::Grid::Row^ m_row;

        GrItem* m_pItem;
        System::Object^ m_oldValue;

        System::String^ m_errorDescription;
        System::Object^ m_value;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/