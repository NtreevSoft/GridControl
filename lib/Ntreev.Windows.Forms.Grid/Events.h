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
#include "Cell.h"
#include "Row.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// <see cref="Ntreev::Windows::Forms::Grid::Cell"/>과 관련된 이벤트의 데이터를 제공합니다.
    /// </summary>
    public ref class CellEventArgs
        : System::EventArgs
    {
    public: // methods

        /// <summary>
        /// <see cref="CellEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="cell">
        /// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Cell"/>의 인스턴스입니다.
        /// </param>
        CellEventArgs(Ntreev::Windows::Forms::Grid::Cell^ cell);

    public: // proeprties

        /// <summary>
        /// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Cell"/>을 가져옵니다.
        /// </summary>
        property Ntreev::Windows::Forms::Grid::Cell^ Cell
        {
            Ntreev::Windows::Forms::Grid::Cell^ get();
        }

    private: // variables

        Ntreev::Windows::Forms::Grid::Cell^ m_cell;
    };

    /// <summary>
    /// <see cref="Ntreev::Windows::Forms::Grid::Cell"/>과 관련된 이벤트를 처리하는 메서드를 나타냅니다.
    /// </summary>
    public delegate void CellEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::CellEventArgs^ e);

    /// <summary>
    /// 값 변경 이벤트의 관련된 데이터를 제공합니다.
    /// </summary>
    public ref class ValueChangingEventArgs
        : Ntreev::Windows::Forms::Grid::CellEventArgs
    {
    public: // methods

        /// <summary>
        /// <see cref="ValueChangingEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="cell">
        /// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Cell"/>의 인스턴스입니다.
        /// </param>
        /// <param name="newValue">
        /// 새로 변경될 값입니다.
        /// </param>
        /// <param name="oldValue">
        /// 변경되기 이전의 값입니다.
        /// </param>
        ValueChangingEventArgs(Ntreev::Windows::Forms::Grid::Cell^ cell, System::Object^ newValue, System::Object^ oldValue);

    public: // properties

        /// <summary>
        /// 값을 변경할지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 값의 변경이 취소 될 예정이면 true를, 값의 변경이 정상적으로 이루어진다면 false를 반환합니다.
        /// </returns>
        property bool Cancel
        {
            bool get();
            void set(bool cancel);
        }

        /// <summary>
        /// 변경될 값을 가져옵니다.
        /// </summary>
        property System::Object^ NewValue
        {
            System::Object^ get();
        }

        /// <summary>
        /// 변경되기 이전의 값을 가져옵니다.
        /// </summary>
        property System::Object^ OldValue
        {
            System::Object^ get();
        }

    private: // variables

        bool m_cancel;
        System::Object^ m_newValue;
        System::Object^ m_oldValue;
    };

    /// <summary>
    /// 값 변경 이벤트를 처리하는 메서드를 나타냅니다.
    /// </summary>
    public delegate void ValueChangingEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::ValueChangingEventArgs^ e);

    /// <summary>
    /// <see cref="Ntreev::Windows::Forms::Grid::Row"/>와 관련된 이벤트의 데이터를 제공합니다.
    /// </summary>
    public ref class RowEventArgs
        : System::EventArgs
    {
    public: // methods

        /// <summary>
        /// <see cref="RowEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="row">
        /// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Row"/>의 인스턴스입니다.
        /// </param>
        RowEventArgs(Ntreev::Windows::Forms::Grid::Row^ row);

    public: // properties

        /// <summary>
        /// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Row"/>을 가져옵니다.
        /// </summary>
        property Ntreev::Windows::Forms::Grid::Row^ Row
        {
            Ntreev::Windows::Forms::Grid::Row^ get();
        internal:
            void set(Ntreev::Windows::Forms::Grid::Row^ value);
        }

    internal: // methods

        RowEventArgs();

    private: // variables

        Ntreev::Windows::Forms::Grid::Row^ m_row;
    };

    /// <summary>
    /// <see cref="Ntreev::Windows::Forms::Grid::Row"/>와 관련된 이벤트를 처리하는 메서드를 나타냅니다.
    /// </summary>
    public delegate void RowEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::RowEventArgs^ e);

    /// <summary>
    /// 새로운 행이 추가되는 이벤트의 데이터를 제공합니다.
    /// </summary>
    public ref class RowInsertingEventArgs
        : System::EventArgs
    {
    public: // methods

        /// <summary>
        /// <see cref="Ntreev::Windows::Forms::Grid::RowInsertingEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="component">
        /// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Row"/>와 연결될 데이터를 나타냅니다.
        /// </param>
        RowInsertingEventArgs(System::Object^ component);

    public: // properties

        /// <summary>
        /// 새로운 행을 추가 할지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 새로운 행을 추가하는 작업이 취소되면 true, 그렇지 않으면 false를 반환합니다.
        /// </returns>
        property bool Cancel
        {
            void set(bool value);
            bool get();
        }

        /// <summary>
        /// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Row"/>와 연결될 데이터를 가져옵니다.
        /// </summary>
        /// <remarks>
        /// DataSource가 <see cref="System::Data::DataTable"/>일때는 이 속성의 타입은 <see cref="System::Data::DataRowView"/>가 되고 컬렉션일때는 컬렉션 항목의 타입이 됩니다.
        /// </remarks>
        property System::Object^ Component
        {
            System::Object^ get();
        internal:
            void set(System::Object^ value);
        }

    internal: // methods

        RowInsertingEventArgs();

    private: // variables

        System::Object^ m_component;
        bool m_cancel;
    };

    /// <summary>
    /// 새로운 행이 추가되는 이벤트를 처리하는 메서드를 나타냅니다.
    /// </summary>
    public delegate void RowInsertingEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::RowInsertingEventArgs^ e);

    /// <summary>
    /// 새로운 행이 추가되는 이벤트의 데이터를 제공합니다.
    /// </summary>
    public ref class RowInsertedEventArgs
        : System::EventArgs
    {
    public: // methods

        /// <summary>
        /// <see cref="Ntreev::Windows::Forms::Grid::RowInsertingEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        RowInsertedEventArgs(Ntreev::Windows::Forms::Grid::Row^ row);

    public: // properties

        /// <summary>
        /// 새로 추가된 행을 가져옵니다.
        /// </summary>
        property Ntreev::Windows::Forms::Grid::Row^ Row
        {
            Ntreev::Windows::Forms::Grid::Row^ get();
        }

        /// <summary>
        /// 새로 추가된 행과 연결된 데이터를 가져옵니다.
        /// </summary>
        /// <remarks>
        /// DataSource가 <see cref="System::Data::DataTable"/>일때는 이 속성의 타입은 <see cref="System::Data::DataRowView"/>가 되고 컬렉션일때는 컬렉션 항목의 타입이 됩니다.
        /// </remarks>
        property System::Object^ Component
        {
            System::Object^ get();
        }

    private: // variables

        Ntreev::Windows::Forms::Grid::Row^ m_row;
    };

    /// <summary>
    /// 새로운 행이 추가되는 이벤트를 처리하는 메서드를 나타냅니다.
    /// </summary>
    public delegate void RowInsertedEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::RowInsertedEventArgs^ e);

    /// <summary>
    /// 행이 제거되는 이벤트의 데이터를 제공합니다.
    /// </summary>
    public ref class RowRemovingEventArgs
        : Ntreev::Windows::Forms::Grid::RowEventArgs
    {
    public: // methods

        /// <summary>
        /// <see cref="RowRemovingEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="row">
        /// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Row"/>의 인스턴스입니다.
        /// </param>
        RowRemovingEventArgs(Ntreev::Windows::Forms::Grid::Row^ row);

    public: // properties

        /// <summary>
        /// 행을 제거 할지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 행을 제거하는 작업이 취소되면 true, 그렇지 않으면 false를 반환합니다.
        /// </returns>
        property bool Cancel
        {
            void set(bool value);
            bool get();
        }

    private: // variables

        bool m_cancel;
    };

    /// <summary>
    /// 행이 제거되는 이벤트를 처리하는 메서드를 나타냅니다.
    /// </summary>
    public delegate void RowRemovingEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::RowRemovingEventArgs^ e);

    /// <summary>
    /// 행이 제거된후 발생하는 이벤트의 데이터를 제공합니다.
    /// </summary>
    public ref class RowRemovedEventArgs
        : System::EventArgs
    {
    public: // methods

        /// <summary>
        /// <see cref="RowRemovedEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="nNewIndex">
        /// 이벤트 완료후 새로 선택되어질 인덱스입니다.
        /// </param>
        RowRemovedEventArgs(unsigned int nNewIndex);

    public: // properties

        /// <summary>
        /// 제거 작업이 완료된후 새로운 <see cref="Ntreev::Windows::Forms::Grid::Row"/>를 선택할지에 대한 여부를 설정합니다.
        /// </summary>
        property bool NewSelect
        {
            void set(bool value);
        }

        /// <summary>
        /// 제거 작업이 완료된후 새로 선택되어질 <see cref="Ntreev::Windows::Forms::Grid::Row"/>를 설정합니다.
        /// </summary>
        property unsigned int NewSelectionIndex
        {
            void set(unsigned int value);
        }

    private: // variables

        bool m_newSelect;
        unsigned int m_newIndex;
    };

    /// <summary>
    /// 행이 제거된후 발생하는 이벤트를 처리하는 메서드를 나타냅니다.
    /// </summary>
    public delegate void RowRemovedEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::RowRemovedEventArgs^ e);

    /// <summary>
    /// ColumnBinding 이벤트의 데이터를 제공합니다.
    /// </summary>
    public ref class ColumnBindingEventArgs
        : System::EventArgs
    {
    public: // methods

        /// <summary>
        /// <see cref="ColumnBindingEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="propertyDescriptor">
        /// 새로운 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스와 연결될 데이터의 정보입니다.
        /// </param>
        /// <param name="column">
        /// 연견될 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스입니다.
        /// </param>
        ColumnBindingEventArgs(System::ComponentModel::PropertyDescriptor^ propertyDescriptor, Ntreev::Windows::Forms::Grid::Column^ column);

    public: // properties

        /// <summary>
        /// 새로운 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스와 연결될 데이터의 정보를 가져옵니다.
        /// </summary>
        property System::ComponentModel::PropertyDescriptor^ PropertyDescriptor
        {
            System::ComponentModel::PropertyDescriptor^ get();
        }

        /// <summary>
        /// 연결될 새로운 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스를 가져오거나 설정합니다.
        /// </summary>
        /// <remarks>
        /// 이 속성의 값이 null일 경우 기본형태의 <see cref="Ntreev::Windows::Forms::Grid::Column"/>이 생성됩니다.
        /// </remarks>
        property Ntreev::Windows::Forms::Grid::Column^ BindingColumn
        {
            Ntreev::Windows::Forms::Grid::Column^ get();
            void set(Ntreev::Windows::Forms::Grid::Column^ value);
        }

    private: // variables

        System::ComponentModel::PropertyDescriptor^ m_propertyDescriptor;
        Ntreev::Windows::Forms::Grid::Column^ m_column;
    };

    /// <summary>
    /// ColumnBinding 이벤트를 처리하는 메서드를 나타냅니다.
    /// </summary>
    public delegate void ColumnBindingEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::ColumnBindingEventArgs^ e);

    /// <summary>
    /// <see cref="Ntreev::Windows::Forms::Grid::Column"/>과 관련된 이벤트의 데이터를 제공합니다.
    /// </summary>
    public ref class ColumnEventArgs
        : System::EventArgs
    {
    public: // methods

        /// <summary>
        /// <see cref="ColumnEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="column">
        /// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스입니다.
        /// </param>
        ColumnEventArgs(Ntreev::Windows::Forms::Grid::Column^ column);

    public: // properties

        /// <summary>
        /// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Column"/>을 가져옵니다.
        /// </summary>
        property Ntreev::Windows::Forms::Grid::Column^ Column
        {
            Ntreev::Windows::Forms::Grid::Column^ get();
        }

    private: // variables

        Ntreev::Windows::Forms::Grid::Column^ m_column;
    };

    /// <summary>
    /// <see cref="Ntreev::Windows::Forms::Grid::Column"/>과 관련된 이벤트를 처리하는 메서드를 나타냅니다.
    /// </summary>
    public delegate void ColumnEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::ColumnEventArgs^ e);

    /// <summary>
    /// 새로운 열이 추가되는 이벤트의 데이터를 제공합니다.
    /// </summary>
    public ref class ColumnInsertingEventArgs
        : Ntreev::Windows::Forms::Grid::ColumnEventArgs
    {
    public: // methods

        /// <summary>
        /// <see cref="ColumnInsertingEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="column">
        /// 새로 추가될 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스입니다.
        /// </param>
        ColumnInsertingEventArgs(Ntreev::Windows::Forms::Grid::Column^ column);

    public: // properties

        /// <summary>
        /// 새로운 열을 추가 할지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 새로운 열을 추가하는 작업이 취소되면 true, 그렇지 않으면 false를 반환합니다.
        /// </returns>
        property bool Cancel
        {
            void set(bool value);
            bool get();
        }

    private: // variables

        bool m_cancel;
    };

    /// <summary>
    /// 새로운 열이 추가되는 이벤트를 처리하는 메서드를 나타냅니다.
    /// </summary>
    public delegate void ColumnInsertingEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::ColumnInsertingEventArgs^ e);

    /// <summary>
    /// 마우스 이벤트중 <see cref="Ntreev::Windows::Forms::Grid::Column"/>과 관련된 이벤트의 데이터를 제공합니다.
    /// </summary>
    public ref class ColumnMouseEventArgs
        : Ntreev::Windows::Forms::Grid::ColumnEventArgs
    {
    public: // methods

        /// <summary>
        /// <see cref="ColumnMouseEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="column">
        /// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스입니다.
        /// </param>
        /// <param name="location">
        /// 이벤트가 발생한 시점의 마우스의 위치입니다. 마우스의 위치는 Column기준의 지역좌표입니다.
        /// </param>
        ColumnMouseEventArgs(Ntreev::Windows::Forms::Grid::Column^ column, System::Drawing::Point location);

    public: // properties

        /// <summary>
        /// 이벤트가 발생한 시점의 마우스의 x좌표 입니다. 마우스의 위치는 Column기준의 지역좌표입니다.
        /// </summary>
        property int X
        { 
            int get();
        }

        /// <summary>
        /// 이벤트가 발생한 시점의 마우스의 y좌표 입니다. 마우스의 위치는 Column기준의 지역좌표입니다.
        /// </summary>
        property int Y
        {
            int get();
        }

        /// <summary>
        /// 이벤트가 발생한 시점의 마우스의 좌표 입니다. 마우스의 위치는 Column기준의 지역좌표입니다.
        /// </summary>
        property System::Drawing::Point Location
        {
            System::Drawing::Point get();
        }

        /// <summary>
        /// 이벤트가 처리 되었는지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 이벤트가 처리 되었다면 true를 그렇지 않다면 false를 반환합니다.
        /// </returns>
        /// <remarks>
        /// 이 속성의 값이 true로 설정되었다면 각 이벤트에 해당하는 기본 작업이 취소됩니다.
        /// ColumnMouseDown일 경우 열의 셀 선택 기능이 취소 되며, ColumnMouseUp일 경우 Column정렬 기능이 취소됩니다.
        /// </remarks>
        property bool Handled 
        {
            bool get();
            void set(bool value);
        }

    private: // variables

        System::Drawing::Point m_location;
        bool m_handled;
    };

    /// <summary>
    /// 마우스 이벤트중 <see cref="Ntreev::Windows::Forms::Grid::Column"/>과 관련된 이벤트를 처리하는 메서드를 나타냅니다.
    /// </summary>
    public delegate void ColumnMouseEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::ColumnMouseEventArgs^ e);

    /// <summary>
    /// 마우스 이벤트중 <see cref="Ntreev::Windows::Forms::Grid::Cell"/>과 관련된 이벤트의 데이터를 제공합니다.
    /// </summary>
    public ref class CellMouseEventArgs
        : Ntreev::Windows::Forms::Grid::CellEventArgs
    {
    public: // methods

        /// <summary>
        /// <see cref="CellMouseEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="cell">
        /// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Cell"/>의 인스턴스입니다.
        /// </param>
        /// <param name="location">
        /// 이벤트가 발생한 시점의 마우스의 위치입니다. 마우스의 위치는 Cell기준의 지역좌표입니다.
        /// </param>
        CellMouseEventArgs(Ntreev::Windows::Forms::Grid::Cell^ cell, System::Drawing::Point location);

    public: // properties

        /// <summary>
        /// 이벤트가 발생한 시점의 마우스의 x좌표 입니다. 마우스의 위치는 Cell기준의 지역좌표입니다.
        /// </summary>
        property int X
        { 
            int get();
        }

        /// <summary>
        /// 이벤트가 발생한 시점의 마우스의 y좌표 입니다. 마우스의 위치는 Cell기준의 지역좌표입니다.
        /// </summary>
        property int Y
        {
            int get();
        }

        /// <summary>
        /// 이벤트가 발생한 시점의 마우스의 좌표 입니다. 마우스의 위치는 Cell기준의 지역좌표입니다.
        /// </summary>
        property System::Drawing::Point Location
        {
            System::Drawing::Point get();
        }

        /// <summary>
        /// 이벤트가 처리 되었는지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 이벤트가 처리 되었다면 true를 그렇지 않다면 false를 반환합니다.
        /// </returns>
        /// <remarks>
        /// 이 속성의 값이 true로 설정되었다면 각 이벤트에 해당하는 기본 작업이 취소됩니다.
        /// CellMouseDown일 경우 열의 셀 선택 기능이 취소 되며, CellMouseUp일 경우 Cell정렬 기능이 취소됩니다.
        /// </remarks>
        property bool Handled 
        {
            bool get();
            void set(bool value);
        }

    private: // variables

        System::Drawing::Point m_location;
        bool m_handled;
    };

    /// <summary>
    /// 마우스 이벤트중 <see cref="Ntreev::Windows::Forms::Grid::Cell"/>과 관련된 이벤트를 처리하는 메서드를 나타냅니다.
    /// </summary>
    public delegate void CellMouseEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::CellMouseEventArgs^ e);

    /// <summary>
    /// 편집을 취소할 수 있는 이벤트의 데이터를 제공합니다.
    /// </summary>
    public ref class BeginEditEventArgs
        : Ntreev::Windows::Forms::Grid::CellEventArgs
    {
    public: // methods

        /// <summary>
        /// <see cref="BeginEditEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="cell">
        /// 편집이 되는 <see cref="Ntreev::Windows::Forms::Grid::Cell"/>의 인스턴스입니다.
        /// </param>
        BeginEditEventArgs(Ntreev::Windows::Forms::Grid::Cell^ cell);

    public: // properties

        /// <summary>
        /// 편집을 진행 할지에 대한 여부를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 편집을 진행이 된다면 true, 그렇지 않으면 false를 반환합니다.
        /// </returns>
        property bool Cancel 
        {
            bool get();
            void set(bool value);
        }

    private: // variables

        bool m_cancel;
    };

    /// <summary>
    /// 편집을 취소할 수 있는 이벤트를 처리하는 메서드를 나타냅니다.
    /// </summary>
    public delegate void BeginEditEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::BeginEditEventArgs^ e);

    /// <summary>
    /// 데이터가 초기화 되는 이벤트의 데이터를 제공합니다.
    /// </summary>
    public ref class ClearEventArgs
        : System::EventArgs
    {
    public: // methods

        /// <summary>
        /// <see cref="ClearEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <param name="dataSourceOnly">
        /// 바인딩 할 수 있는 열까지 삭제해야 되는지를 나타냅니다.
        /// </param>
        ClearEventArgs(bool dataSourceOnly);

    public: // properties

        /// <summary>
        /// 바인딩 할 수 있는 열까지 삭제해야 되는지에 대한 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 바인딩 할 수 있는 열까지 삭제해야 한다면 true, 그렇지 않으면 false를 반환합니다.
        /// </returns>
        property bool DataSourceOnly 
        {
            bool get();
        }

    private: // variables

        bool m_dataSourceOnly;
    };

    /// <summary>
    /// 데이터가 초기화 되는 이벤트를 처리하는 메서드를 나타냅니다.
    /// </summary>
    public delegate void ClearEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::ClearEventArgs^ e);

    ref class CurrencyManagerChangedEventArgs
        : System::EventArgs
    {
    public: // methods

        CurrencyManagerChangedEventArgs(System::Windows::Forms::CurrencyManager^ currencyManager);

    public: // properties

        property System::Windows::Forms::CurrencyManager^ CurrecnyManager
        {
            System::Windows::Forms::CurrencyManager^ get();
        }

    private: // variables

        System::Windows::Forms::CurrencyManager^ m_manager;
    };

    delegate void CurrencyManagerChangedEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::CurrencyManagerChangedEventArgs^ e);

    ref class CurrencyManagerChangingEventArgs
        : Ntreev::Windows::Forms::Grid::CurrencyManagerChangedEventArgs
    {
    public: // maethods

        CurrencyManagerChangingEventArgs(System::Windows::Forms::CurrencyManager^ currencyManager);

    public: // properties

        property bool Cancel
        {
            bool get();
            void set(bool value);
        }

        property System::String^ CancelReason;

    private: // variables

        bool m_cancel;
    };

    delegate void CurrencyManagerChangingEventHandler(System::Object^ sender, Ntreev::Windows::Forms::Grid::CurrencyManagerChangingEventArgs^ e);
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/