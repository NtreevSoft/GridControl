//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4646.22417
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
#include "RowBase.h"
#include "CellCollection.h"
#include "CellTagCollection.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    ref class RowBuilder;
    ref class CellBuilder;

    /// <summary>
    /// 행을 나타냅니다.
    /// </summary>
    [System::ComponentModel::TypeConverterAttribute(System::ComponentModel::ExpandableObjectConverter::typeid)]
    public ref class Row 
        : Ntreev::Windows::Forms::Grid::RowBase
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
        /// 셀의 값을 변경하기 시작합니다.
        /// </summary>
        void BeginEdit();

        /// <summary>
        /// 셀들의 변경된 내용을 취소합니다.
        /// </summary>
        /// <remarks>
        /// 셀들의 데이터가 변경되었을때 사용자는 변경된 내용을 취소하고 원래값으로 되돌릴수 있습니다.
        /// </remarks>
        void CancelEdit();

        /// <summary>
        /// 셀들의 변경된 내용을 적용합니다.
        /// </summary>
        /// <remarks>
        /// 셀들의 데이터가 변경되었을때 사용자는 변경된 내용을 취소하고 원래값으로 되돌릴수 있지만, 이 작업을 수행한 이후로는
        /// 원래값으로 되돌릴 수 없게 됩니다.
        /// </remarks>
        void EndEdit();

        /// <summary>
        /// 행이 화면에 표시되도록 스크롤을 조정합니다.
        /// </summary>
        void BringIntoView();

        /// <summary>
        /// 이전 선택 항목을 해재한후 행을 선택합니다.
        /// </summary>
        void Select();

        /// <summary>
        /// 행을 선택합니다.
        /// </summary>
        /// <param name="selectionType">
        /// 행을 선택하기 위한 방법을 지정합니다.
        /// </param>
        void Select(SelectionType selectionType);

        /// <summary>
        /// 셀들의 전경색을 기본값으로 되돌립니다.
        /// </summary>
        void ResetCellForeColor();

        /// <summary>
        /// 셀들의 배경색을 기본값으로 되돌립니다.
        /// </summary>
        void ResetCellBackColor();

        /// <summary>
        /// 지정한 인덱스에 있는 항목를 가져옵니다.
        /// </summary>
        /// <param name="index">
        /// 가져올 항목의 인덱스(0부터 시작)입니다.
        /// </param>
        /// <returns>
        /// 지정한 인덱스의 항목입니다.
        /// </returns>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index가 0보다 작거나, <see cref="CellCount"/>보다 클 경우
        /// </exception>
        Cell^ GetAt(int index);

    public: // properties

        /// <summary>
        /// 셀의 컬렉션을 가져옵니다.
        /// </summary>
        /// <returns>
        /// 셀이 들어있는 <see cref="CellCollection"/>입니다.
        /// </returns>
        [System::ComponentModel::DescriptionAttribute("셀들의 컬렉션입니다.")]
        [System::ComponentModel::CategoryAttribute("Appearance")]
        property CellCollection^ Cells
        {
            CellCollection^ get();
        }

        /// <summary>
        /// 셀태그의 컬렉션을 가져옵니다.
        /// </summary>
        /// <returns>
        /// 셀태그가 들어있는 <see cref="CellCollection"/>입니다.
        /// </returns>
        [System::ComponentModel::DescriptionAttribute("셀태그의 컬렉션입니다.")]
        [System::ComponentModel::CategoryAttribute("Appearance")]
        property CellTagCollection^ CellTags
        {
            CellTagCollection^ get();
        }

        /// <summary>
        /// <see cref="Cells"/>의 갯수를 가져옵니다.
        /// </summary>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Debug")]
        [System::ComponentModel::DescriptionAttribute("셀들의 갯수입니다.")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property int CellCount
        {
            int get();
        }

        /// <summary>
        /// 지정한 인덱스에 필드값을 가져오거나 설정합니다.
        /// </summary>
        /// <param name="index">
        /// 가져올 필드값의 해당하는 인덱스(0부터 시작)입니다.
        /// </param>
        /// <returns>
        /// 필드의 값입니다.
        /// </returns>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index가 0보다 작거나, <see cref="CellCount"/>보다 클 경우
        /// </exception>
        property System::Object^ default[int]
        {
            System::Object^ get(int index);
            void set(int index, System::Object^);
        }

        /// <summary>
        /// 열의 이름으로 필드값을 가져오거나 설정합니다.
        /// </summary>
        /// <param name="columnName">
        /// 찾을 셀을 소유한 열의 이름입니다.
        /// </param>
        /// <exception cref="System::ArgumentException">
        /// 이름과 같은 열을 찾지 못하는 경우
        /// </exception>
        /// <returns>
        /// 필드의 값입니다.
        /// </returns>
        property System::Object^ default[System::String^]
        {
            System::Object^ get(System::String^ columnName); 
            void set(System::String^ columnName, System::Object^);
        }

        /// <summary>
        /// 열의 인스턴스로 필드값을 가져오거나 설정합니다.
        /// </summary>
        /// <param name="column">
        /// 찾을 항목의 <see cref="Column"/>의 인스턴스 입니다.
        /// </param>
        /// <returns>
        /// 필드의 값입니다.
        /// </returns>
        /// <exception cref="System::NullReferenceException">
        /// 매개변수가 null일 경우
        /// </exception>
        property System::Object^ default[Column^]
        {
            System::Object^ get(Column^ column);
            void set(Column^ column, System::Object^);
        }

        /// <summary>
        /// 고유 아이디를 가져옵니다.
        /// </summary>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Debug")]
        [System::ComponentModel::DescriptionAttribute("열의 고유 아이디입니다.")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property unsigned int RowID
        {
            unsigned int get();
        }

        /// <summary>
        /// 표시되고 있는지의 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 표시되고 있다면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        [System::ComponentModel::DescriptionAttribute("표시되고 있는지의 여부를 가져오거나 설정합니다.")]
        [System::ComponentModel::CategoryAttribute("Appearance")]
        [System::ComponentModel::DefaultValueAttribute(true)]
        property bool IsVisible 
        {
            bool get() new;
            void set(bool);
        }

        /// <summary>
        /// 읽기 전용인지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 읽기 전용이면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        /// <remarks>
        /// 이 속성의 값이 true일 경우 Row에 포함되어있는 <see cref="Cells"/>들도 읽기 전용상태로 됩니다.
        /// </remarks>
        [System::ComponentModel::DescriptionAttribute("읽기 전용인지에 대한 여부를 가져오거나 설정합니다.")]
        [System::ComponentModel::CategoryAttribute("Appearance")]
        [System::ComponentModel::DefaultValueAttribute(false)]
        property bool IsReadOnly
        {
            bool get();
            void set(bool);
        }

        /// <summary>
        /// 선택되어 있는지의 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 소유하고 있는 <see cref="Cells"/>중 적어도 한개이상이 선택되었다면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        /// <param name="value">
        /// 매개변수의 값이 true라면 모든 셀이 선택되어집니다.
        /// 반대로 매개변수의 값을 false로 설정하면 모든 셀의 선택이 해제가 됩니다.
        /// </param>
        /// <remarks>
        /// 모든 셀이 선택되었는지를 확인하려면 <see cref="IsFullSelected"/>를 사용하십시오.
        /// </remarks>
        [System::ComponentModel::DescriptionAttribute("선택되어 있는지의 여부를 가져오거나 설정합니다.")]
        [System::ComponentModel::CategoryAttribute("Appearance")]
        [System::ComponentModel::DefaultValueAttribute(false)]
        [System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
        property bool IsSelected
        {
            bool get();
            void set(bool);
        }

        /// <summary>
        /// 현재의 행이 편집중인지에 대한 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 현재의 행이 편집중이면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
        /// </returns>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Debug")]
        [System::ComponentModel::DescriptionAttribute("소유하고 있는 셀의 대한 편집 여부를 가져옵니다.")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property bool IsBeingEdited
        {
            bool get(); 
        }

        /// <summary>
        /// 소유하고 있는 <see cref="Cells"/>의 대한 편집 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 소유하고 있는 <see cref="Cells"/>중 적어도 한개이상이 편집되었다면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
        /// </returns>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Debug")]
        [System::ComponentModel::DescriptionAttribute("소유하고 있는 셀의 대한 편집 여부를 가져옵니다.")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property bool IsEdited
        {
            bool get(); 
        }

        /// <summary>
        /// <see cref="Ntreev::Windows::Forms::Grid::GridControl"/>내에 행의 인덱스를 가져옵니다.
        /// </summary>
        /// <returns>
        /// <see cref="Ntreev::Windows::Forms::Grid::GridControl"/>내에 포함되 있다면 인덱스(0부터 시작)를 반환하고, 그렇지 않다면 0xffffffff를 반환합니다.
        /// </returns>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Debug")]
        [System::ComponentModel::DescriptionAttribute("컨트롤에 내에 Row 인덱스를 가져옵니다.")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property int Index
        {
            int get(); 
        }

        /// <summary>
        /// 소유하고 있는 모든 셀이 선택되었는지의 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 소유하고 있는 모든 셀이 선택되었다면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
        /// </returns>
#ifdef _DEBUG
        [System::ComponentModel::CategoryAttribute("Debug")]
        [System::ComponentModel::DescriptionAttribute("소유하고 있는 모든 셀이 선택되었는지의 여부를 가져옵니다.")]
#else
        [System::ComponentModel::BrowsableAttribute(false)]
#endif
        property bool IsFullSelected
        {
            bool get(); 
        }

        /// <summary>
        /// <see cref="Cells"/>의 전경색을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// <see cref="Cells"/>의 전경색을 나타내는 <see cref="System::Drawing::Color"/>입니다.
        /// </returns>
        [System::ComponentModel::DescriptionAttribute("셀들의 전경색입니다")]
        [System::ComponentModel::CategoryAttribute("Appearance")]
        property System::Drawing::Color CellForeColor
        {
            System::Drawing::Color get();
            void set(System::Drawing::Color);
        }

        /// <summary>
        /// <see cref="Cells"/>의 배경색을 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// <see cref="Cells"/>의 배경색을 나타내는 <see cref="System::Drawing::Color"/>입니다.
        /// </returns>
        [System::ComponentModel::DescriptionAttribute("셀들의 배경색입니다")]
        [System::ComponentModel::CategoryAttribute("Appearance")]
        property System::Drawing::Color CellBackColor
        {
            System::Drawing::Color get();
            void set(System::Drawing::Color); 
        }

        [System::ComponentModel::DefaultValueAttribute("")]
        property System::String^ ErrorDescription
        {
            System::String^ get();
            void set(System::String^);
        }

        property bool HasErrorCell
        {
            bool get();
        }

        property System::Object^ Component
        {
            System::Object^ get() { return m_component; }
        internal:
            void set(System::Object^);
        }

    public: // events

    public protected:
        Row(RowBuilder^ rowBuilder);

    protected: // methods
        int GetCellsTextCapacity(); 

        virtual Cell^ NewCellFromBuilder(CellBuilder^ builder);

    internal: // methodss

        //Row(Ntreev::Windows::Forms::Grid::GridControl^ gridControl);

        //Row(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, GrDataRow* pDataRow);

        void AddEditedCell();
        void RemoveEditedCell();
        void AddErrorCell();
        void RemoveErrorCell();

        void ProcessChildControl();
        void DetachChildControl();
        Cell^ NewCell(Column^ column);
        void ValueToSource(System::Object^ component);

    internal: // properties

        property int ComponentIndex
        {
            int get() { return m_componentIndex; }
            void set(int value) { m_componentIndex = value; }
        }

        property GrDataRow* NativeRef
        {
            GrDataRow* get();
        }

    internal: // variables
        int m_textCapacity;

    private: // methods

        bool ShouldSerializeCellForeColor();
        bool ShouldSerializeCellBackColor();
        bool ShouldSerializeCellFont();

    private: // variables

        CellCollection^ m_cells;
        CellTagCollection^ m_cellTags;
        GrDataRow* m_pDataRow;
        System::Object^ m_component;
        int m_componentIndex;
        int m_editedCount;
        int m_errorCell;
        bool m_editing;
        System::String^ m_errorDescription;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/