//=====================================================================================================================
// Ntreev Grid for .Net 1.0.4300.26762
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

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// <see cref="Ntreev::Windows::Forms::Grid::Cell"/>과 관련된 이벤트의 데이터를 제공합니다.
	/// </summary>
	public ref class CellEventArgs : _EventArgs
	{
	public: // methods
		/// <summary>
		/// <see cref="CellEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="cell">
		/// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Cell"/>의 인스턴스입니다.
		/// </param>
		CellEventArgs(_Cell^ cell) 
			: m_cell(cell) 
		{ 

		}

	public: // proeprties
		/// <summary>
		/// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Cell"/>을 가져옵니다.
		/// </summary>
		property _Cell^ Cell
		{
			_Cell^ get() { return m_cell; }
		}

	private: // variables
		_Cell^		m_cell;
	};

	/// <summary>
	/// <see cref="Ntreev::Windows::Forms::Grid::Cell"/>과 관련된 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	public delegate void CellEventHandler(object^ sender, CellEventArgs^ e);

	/// <summary>
	/// 값 변경 이벤트의 관련된 데이터를 제공합니다.
	/// </summary>
	public ref class ValueChangingEventArgs : CellEventArgs
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
		ValueChangingEventArgs(_Cell^ cell, object^ newValue, object^ oldValue) 
			: CellEventArgs(cell), m_cancel(false), m_newValue(newValue), m_oldValue(oldValue)
		{
		
		}

	public: // properties
		/// <summary>
		/// 값을 변경할지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 값의 변경이 취소 될 예정이면 true를, 값의 변경이 정상적으로 이루어진다면 false를 반환합니다.
		/// </returns>
		property bool Cancel
		{
			bool get() { return m_cancel; } 
			void set(bool cancel) { m_cancel = cancel; }
		}

		/// <summary>
		/// 변경될 값을 가져옵니다.
		/// </summary>
		property object^ NewValue
		{
			object^ get() { return m_newValue; }
		}

		/// <summary>
		/// 변경되기 이전의 값을 가져옵니다.
		/// </summary>
		property object^ OldValue
		{
			object^ get() { return m_oldValue; }
		}

	private: // variables
		bool		m_cancel;
		object^		m_newValue;
		object^		m_oldValue;
	};

	/// <summary>
	/// 값 변경 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	public delegate void ValueChangingEventHandler(object^ sender, ValueChangingEventArgs^ e);

	/// <summary>
	/// <see cref="Ntreev::Windows::Forms::Grid::Row"/>와 관련된 이벤트의 데이터를 제공합니다.
	/// </summary>
	public ref class RowEventArgs : _EventArgs
	{
	public: // methods
		/// <summary>
		/// <see cref="RowEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="row">
		/// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Row"/>의 인스턴스입니다.
		/// </param>
		RowEventArgs(_Row^ row)
			: m_row(row)
		{
		}

	public: // properties
		/// <summary>
		/// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Row"/>을 가져옵니다.
		/// </summary>
		property _Row^ Row
		{
			_Row^ get() { return m_row; } 
		internal:
			void set(_Row^ value) { m_row = value; }
		}

	internal: // methods
		RowEventArgs() {}

	private: // variables
		_Row^		m_row;
	};

	/// <summary>
	/// <see cref="Ntreev::Windows::Forms::Grid::Row"/>와 관련된 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	public delegate void RowEventHandler(object^ sender, RowEventArgs^ e);

	/// <summary>
	/// 새로운 행이 <see cref="Ntreev::Windows::Forms::Grid::InsertionRow"/>를 참조하여 추가되는 이벤트의 데이터를 제공합니다.
	/// </summary>
	public ref class InsertionRowInsertingEventArgs : RowEventArgs
	{
	public: // methods]
		/// <summary>
		/// <see cref="InsertionRowInsertingEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="row">
		/// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::InsertionRow"/>의 인스턴스입니다.
		/// </param>
		InsertionRowInsertingEventArgs(_Row^ row) : RowEventArgs(row), m_cancel(false) { }

	public: // properties
		/// <summary>
		/// InsertionRow를 참조하여 새로운 행을 추가 할지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 새로운 행을 추가하는 작업이 취소되면 true, 그렇지 않으면 false를 반환합니다.
		/// </returns>
		property bool Cancel 
		{
			bool get() { return m_cancel; }
			void set(bool value) { m_cancel = value; } 
		}

	private: // variables
		bool		m_cancel;
	};
	
	/// <summary>
	/// 새로운 행이 <see cref="Ntreev::Windows::Forms::Grid::InsertionRow"/>를 참조하여 추가되는 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	public delegate void InsertionRowInsertingEventHandler(object^ sender, InsertionRowInsertingEventArgs^ e);

	/// <summary>
	/// 새로운 행이 추가되는 이벤트의 데이터를 제공합니다.
	/// </summary>
	public ref class RowInsertingEventArgs : _EventArgs
	{
	public: // methods
		/// <summary>
		/// <see cref="RowInsertingEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="component">
		/// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Row"/>와 연결될 데이터를 나타냅니다.
		/// </param>
		RowInsertingEventArgs(object^ component)
			: m_component(component), m_cancel(false)
		{

		}

	public: // properties
		/// <summary>
		/// 새로운 행을 추가 할지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 새로운 행을 추가하는 작업이 취소되면 true, 그렇지 않으면 false를 반환합니다.
		/// </returns>
		property bool Cancel
		{
			void set(bool value) { m_cancel = value; } 
			bool get() { return m_cancel; }
		}

		/// <summary>
		/// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Row"/>와 연결될 데이터를 가져옵니다.
		/// </summary>
		/// <remarks>
		/// DataSource가 <see cref="System::Data::DataTable"/>일때는 이 속성의 타입은 <see cref="System::Data::DataRowView"/>가 되고 컬렉션일때는 컬렉션 항목의 타입이 됩니다.
		/// </remarks>
		property object^ Component
		{
			object^ get() { return m_component; } 
		internal:
			void set(object^ value) { m_component = value; m_cancel = false; }
		}

	internal:
		RowInsertingEventArgs() {};

	private: // variables
		object^		m_component;
		bool		m_cancel;
	};

	/// <summary>
	/// 새로운 행이 추가되는 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	public delegate void RowInsertingEventHandler(object^ sender, RowInsertingEventArgs^ e);

	/// <summary>
	/// 행이 제거되는 이벤트의 데이터를 제공합니다.
	/// </summary>
	public ref class RowRemovingEventArgs : RowEventArgs
	{
	public: // methods
		/// <summary>
		/// <see cref="RowRemovingEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="row">
		/// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Row"/>의 인스턴스입니다.
		/// </param>
		RowRemovingEventArgs(_Row^ row) 
			: RowEventArgs(row), m_cancel(false)
		{

		}

	public: // properties
		/// <summary>
		/// 행을 제거 할지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 행을 제거하는 작업이 취소되면 true, 그렇지 않으면 false를 반환합니다.
		/// </returns>
		property bool Cancel
		{
			void set(bool value) { m_cancel = value; } 
			bool get() { return m_cancel; }
		}

	private: // variables
		bool	m_cancel;
	};

	/// <summary>
	/// 행이 제거되는 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	public delegate void RowRemovingEventHandler(object^ sender, RowRemovingEventArgs^ e);

	/// <summary>
	/// 행이 제거된후 발생하는 이벤트의 데이터를 제공합니다.
	/// </summary>
	public ref class RowRemovedEventArgs : _EventArgs
	{
	public: // methods
		/// <summary>
		/// <see cref="RowRemovedEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="nNewIndex">
		/// 이벤트 완료후 새로 선택되어질 인덱스입니다.
		/// </param>
		RowRemovedEventArgs(uint nNewIndex)
			: m_newSelect(false), m_newIndex(nNewIndex) 
		{ 

		}

	public: // properties
		/// <summary>
		/// 제거 작업이 완료된후 새로운 <see cref="Ntreev::Windows::Forms::Grid::Row"/>를 선택할지에 대한 여부를 설정합니다.
		/// </summary>
		property  bool NewSelect
		{
			void set(bool value) { m_newSelect = value; }
		}

		/// <summary>
		/// 제거 작업이 완료된후 새로 선택되어질 <see cref="Ntreev::Windows::Forms::Grid::Row"/>를 설정합니다.
		/// </summary>
		property uint NewSelectionIndex
		{
			void set(uint value) { m_newIndex = value; }
		}

	private: // variables
		bool				m_newSelect;
		uint				m_newIndex;
	};

	/// <summary>
	/// 행이 제거된후 발생하는 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	public delegate void RowRemovedEventHandler(object^ sender, RowRemovedEventArgs^ e);

	/// <summary>
	/// ColumnBinding 이벤트의 데이터를 제공합니다.
	/// </summary>
	public ref class ColumnBindingEventArgs : _EventArgs
	{
	public:
		/// <summary>
		/// <see cref="ColumnBindingEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="propertyDescriptor">
		/// 새로운 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스와 연결될 데이터의 정보입니다.
		/// </param>
		ColumnBindingEventArgs(_PropertyDescriptor^ propertyDescriptor)
			: m_propertyDescriptor(propertyDescriptor)
		{

		}

		/// <summary>
		/// <see cref="ColumnBindingEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="propertyDescriptor">
		/// 새로운 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스와 연결될 데이터의 정보입니다.
		/// </param>
		/// <param name="column">
		/// 연견될 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스입니다.
		/// </param>
		ColumnBindingEventArgs(_PropertyDescriptor^ propertyDescriptor, _Column^ column)
			: m_propertyDescriptor(propertyDescriptor), m_column(column)
		{

		}
		
	public:
		/// <summary>
		/// 새로운 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스와 연결될 데이터의 정보를 가져옵니다.
		/// </summary>
		property _PropertyDescriptor^ PropertyDescriptor
		{
			_PropertyDescriptor^ get()
			{
				return m_propertyDescriptor;
			}
		}

		/// <summary>
		/// 연결될 새로운 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 이 속성의 값이 null일 경우 기본형태의 <see cref="Ntreev::Windows::Forms::Grid::Column"/>이 생성됩니다.
		/// </remarks>
		property _Column^ BindingColumn
		{
			_Column^ get()
			{
				return m_column;
			}

			void set(_Column^ value)
			{
				m_column = value;
			}
		}

	private:
		_PropertyDescriptor^	m_propertyDescriptor;
		_Column^				m_column;
	};

	/// <summary>
	/// ColumnBinding 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	public delegate void ColumnBindingEventHandler(object^ sender, ColumnBindingEventArgs^ e);
	
	/// <summary>
	/// <see cref="Ntreev::Windows::Forms::Grid::Column"/>과 관련된 이벤트의 데이터를 제공합니다.
	/// </summary>
	public ref class ColumnEventArgs : _EventArgs
	{
	public: // methods
		/// <summary>
		/// <see cref="ColumnEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="column">
		/// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스입니다.
		/// </param>
		ColumnEventArgs(_Column^ column)
			: m_column(column)
		{
		}

	public: // properties
		/// <summary>
		/// 이벤트의 대상이 되는 <see cref="Ntreev::Windows::Forms::Grid::Column"/>을 가져옵니다.
		/// </summary>
		property _Column^ Column
		{
			_Column^ get() { return m_column; } 
		}

	private: // variables
		_Column^		m_column;
	};

	/// <summary>
	/// <see cref="Ntreev::Windows::Forms::Grid::Column"/>과 관련된 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	public delegate void ColumnEventHandler(object^ sender, ColumnEventArgs^ e);

	/// <summary>
	/// 새로운 열이 추가되는 이벤트의 데이터를 제공합니다.
	/// </summary>
	public ref class ColumnInsertingEventArgs : ColumnEventArgs
	{
	public: // methods
		/// <summary>
		/// <see cref="ColumnInsertingEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="column">
		/// 새로 추가될 <see cref="Ntreev::Windows::Forms::Grid::Column"/>의 인스턴스입니다.
		/// </param>
		ColumnInsertingEventArgs(_Column^ column)
			: ColumnEventArgs(column), m_cancel(false)
		{
		}

	public: // properties
		/// <summary>
		/// 새로운 열을 추가 할지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 새로운 열을 추가하는 작업이 취소되면 true, 그렇지 않으면 false를 반환합니다.
		/// </returns>
		property bool Cancel
		{
			void set(bool value) { m_cancel = value; } 
			bool get() { return m_cancel; }
		}

	private: // variables
		bool		m_cancel;
	};
	
	/// <summary>
	/// 새로운 열이 추가되는 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	public delegate void ColumnInsertingEventHandler(object^ sender, ColumnInsertingEventArgs^ e);

	/// <summary>
	/// 마우스 이벤트중 <see cref="Ntreev::Windows::Forms::Grid::Column"/>과 관련된 이벤트의 데이터를 제공합니다.
	/// </summary>
	public ref class ColumnMouseEventArgs : ColumnEventArgs
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
		ColumnMouseEventArgs(_Column^ column, _Point location)
			: ColumnEventArgs(column), m_location(location), m_handled(false)
		{

		}

	public: // properties
		/// <summary>
		/// 이벤트가 발생한 시점의 마우스의 x좌표 입니다. 마우스의 위치는 Column기준의 지역좌표입니다.
		/// </summary>
		property int X
		{ 
			int	get() { return m_location.X; }		
		}

		/// <summary>
		/// 이벤트가 발생한 시점의 마우스의 y좌표 입니다. 마우스의 위치는 Column기준의 지역좌표입니다.
		/// </summary>
		property int Y
		{
			int get() { return m_location.Y; }
		}

		/// <summary>
		/// 이벤트가 발생한 시점의 마우스의 좌표 입니다. 마우스의 위치는 Column기준의 지역좌표입니다.
		/// </summary>
		property _Point Location
		{
			_Point get() { return m_location; }
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
			bool get() { return m_handled; }
			void set(bool value) { m_handled = value; }	
		}

	private: // variables
		_Point			m_location;
		bool			m_handled;
	};

	/// <summary>
	/// 마우스 이벤트중 <see cref="Ntreev::Windows::Forms::Grid::Column"/>과 관련된 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	public delegate void ColumnMouseEventHandler(object^ sender, ColumnMouseEventArgs^ e);

	/// <summary>
	/// 편집을 취소할 수 있는 이벤트의 데이터를 제공합니다.
	/// </summary>
	public ref class BeginEditEventArgs : CellEventArgs
	{
	public: // methods
		/// <summary>
		/// <see cref="BeginEditEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="cell">
		/// 편집이 되는 <see cref="Ntreev::Windows::Forms::Grid::Cell"/>의 인스턴스입니다.
		/// </param>
		BeginEditEventArgs(_Cell^ cell)
			: m_cancel(false), CellEventArgs(cell)
		{
		}

	public: // properties
		/// <summary>
		/// 편집을 진행 할지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 편집을 진행이 된다면 true, 그렇지 않으면 false를 반환합니다.
		/// </returns>
		property bool Cancel 
		{
			bool get() { return m_cancel; }
			void set(bool value) { m_cancel = value; } 
		}

	private: // variables
		bool		m_cancel;
	};

	/// <summary>
	/// 편집을 취소할 수 있는 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	public delegate void BeginEditEventHandler(object^ sender, BeginEditEventArgs^ e);

	/// <summary>
	/// 데이터가 초기화 되는 이벤트의 데이터를 제공합니다.
	/// </summary>
	public ref class ClearEventArgs : _EventArgs
	{
	public: // methods
		/// <summary>
		/// <see cref="ClearEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="dataSourceOnly">
		/// 바인딩 할 수 있는 열까지 삭제해야 되는지를 나타냅니다.
		/// </param>
		ClearEventArgs(bool dataSourceOnly)
			: m_dataSourceOnly(dataSourceOnly)
		{
		}

	public: // properties
		/// <summary>
		/// 바인딩 할 수 있는 열까지 삭제해야 되는지에 대한 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 바인딩 할 수 있는 열까지 삭제해야 한다면 true, 그렇지 않으면 false를 반환합니다.
		/// </returns>
		property bool DataSourceOnly 
		{
			bool get() { return m_dataSourceOnly; }
		}

	private: // variables
		bool		m_dataSourceOnly;
	};

	/// <summary>
	/// 데이터가 초기화 되는 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	public delegate void ClearEventHandler(object^ sender, ClearEventArgs^ e);

	ref class CurrencyManagerChangedEventArgs : _EventArgs
	{
	public: // methods
		CurrencyManagerChangedEventArgs(System::Windows::Forms::CurrencyManager^ currencyManager)
			: m_manager(currencyManager)
		{

		}

	public: // properties
		property System::Windows::Forms::CurrencyManager^ CurrecnyManager
		{
			System::Windows::Forms::CurrencyManager^ get() { return m_manager; }
		}

	private: // variables
		System::Windows::Forms::CurrencyManager^ m_manager;
	};

	delegate void CurrencyManagerChangedEventHandler(object^ sender, CurrencyManagerChangedEventArgs^ e);

	ref class CurrencyManagerChangingEventArgs : CurrencyManagerChangedEventArgs
	{
	public: // methods
		CurrencyManagerChangingEventArgs(System::Windows::Forms::CurrencyManager^ currencyManager)
			: m_cancel(false), CurrencyManagerChangedEventArgs(currencyManager)
		{
			this->CancelReason = string::Empty;
		}

	public: // properties
		property bool Cancel
		{
			bool get() { return m_cancel; }
			void set(bool value) { m_cancel = value; }
		}

		property string^ CancelReason;

	private: // variables
		bool m_cancel;
	};

	delegate void CurrencyManagerChangingEventHandler(object^ sender, CurrencyManagerChangingEventArgs^ e);

} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/