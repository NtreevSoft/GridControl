//=====================================================================================================================
// Ntreev Grid for .Net 1.0
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
#include "GridUtil.h"
#include "GridEvent.h"
#include "GridStyle.h"
#include "GridScrollProperty.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// 그리드 컨트롤 개체입니다.
	/// </summary>
	[System::ComponentModel::ToolboxItem(true)]
	[System::ComponentModel::DefaultEvent("")]
	[System::Drawing::ToolboxBitmap(GridControl::typeid)]
	[System::ComponentModel::DesignerAttribute("Ntreev.Windows.Forms.Grid.Design.GridControlDesigner, Ntreev.Windows.Forms.Grid.Design, Version=1.0.0.2, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca")]
	[System::Windows::Forms::Docking(System::Windows::Forms::DockingBehavior::Ask)]
	public ref class GridControl : System::Windows::Forms::UserControl
	{
		typedef System::Windows::Forms::ImeMode					_ImeMode;
		typedef System::Collections::Generic::List<Row^>		_RowList;
		typedef System::Collections::Generic::List<Column^>		_ColumnList;
		typedef System::Windows::Forms::Keys					_Keys;
		typedef System::Char									_Char;

	public: // methods
		/// <summary>
		/// <see cref="GridControl"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		GridControl();

		/// <summary>
		/// 삽입열에 속한 셀의 값들을 이용하여 새로운 행의 인스턴스를 생성하고 <see cref="GridControl::Rows"/>에 추가합니다.
		/// </summary>
		/// <returns>
		/// 삽입열에 속한 셀들의 값중 옳바르지 않은 값이 존재하거나 이벤트 등의 의해서 작업이 실패할 경우 null을 반환합니다.
		/// 성공시 새로 생성된 행의 인스턴스가 반환됩니다.
		/// </returns>
		Row^ AddNewRowFromInsertion();

		/// <summary>
		/// 기본값을 이용하여 새로운 열을 만들고 <see cref="GridControl::Rows"/>에 추가합니다.
		/// </summary>
		Row^ AddNewRow();

		/// <summary>
		/// 행을 <see cref="GridControl::Rows"/>에서 제거 합니다.
		/// </summary>
		/// <param name="row"><see cref="Row"/>의 인스턴스입니다.</param>
		/// <exception cref="System::Exception">제거할 행의 타입이 <see cref="InsertionRow"/>일때 발생합니다.</exception>
		/// <exception cref="System::ArgumentNullException">제거할 행의 인스턴스가 null일경우 발생합니다.</exception>
		/// <exception cref="System::ArgumentException">제거할 행이 이미 제거되었거나 사용한적이 없는 행일때 발생합니다.</exception>
		void RemoveRow(Row^ row);

		/// <summary>
		/// 셀의 편집을 시작합니다.
		/// </summary>
		void EditCell(Cell^ cell, EditingReason^ editBy);

		void EndEditCell();

		/// <summary>
		/// 대상이 되는 셀이 화면내에 표시 될 수 있도록 스크롤을 조정합니다.
		/// </summary>
		void EnsureVisible(Cell^ cell);

		/// <summary>
		/// 대상이 되는 행이 화면내에 표시 될 수 있도록 수직 스크롤을 조정합니다.
		/// </summary>
		/// <param name="row">화면내에 표시하려 하는 <see cref="Row"/>의 인스턴스입니다.</param>
		void EnsureVisible(_Row^ row);

		/// <summary>
		/// 대상이 되는 열이 화면내에 표시 될 수 있도록 수평 스크롤을 조정합니다.
		/// </summary>
		/// <param name="column">화면내에 표시하려 하는 <see cref="Column"/>의 인스턴스입니다.</param>
		void EnsureVisible(_Column^ column);

		/// <summary>
		/// 모든 데이터를 삭제하고 그리드 컨트롤을 초기화 상태로 설정합니다.
		/// </summary>
		void Clear();

		/// <summary>
		/// 모든 셀을 선택합니다.
		/// </summary>
		/// <exception cref="System::InvalidOperationException">MultiSelect속성이 false인 경우</exception>
		void SelectAll();

		/// <summary>
		/// 선택된 모든 셀들을 해제합니다.
		/// </summary>
		void ClearSelection();

		/// <summary>
		/// 즉시 렌더링에 필요한 데이터를 강제로 업데이트합니다.
		/// </summary>
		void UpdateGridRect();

		/// <summary>
		/// 지정된 위치를 포함하는 그리드 컨트롤의 셀을 검색합니다.
		/// </summary>
		/// <param name="pt">
		/// 검색할 클라이언트의 위치입니다.
		/// </param>
		/// <returns>
		/// 지정된 위치를 포함하는 <see cref="Cell"/>입니다. 위치를 포함하는 <see cref="Cell"/>이 없다면 null을 반환합니다.
		/// </returns>
		_Cell^ GetCellAt(_Point pt);

	public: // public property
		/// <summary>
		/// 그리드 컨트롤을 채울 데이터 소스를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 데이터 소스를 나타내는 개체입니다.
		/// </returns>
		/// <exception cref="System::NotSupportedException">
		/// 데이터 초기화에 실패 했거나 데이터 소스가 IList, IListSource 또는 IBindingList 인터페이스를 구현하는 개체가 아닐때.
		/// </exception>
		[System::ComponentModel::Category("Data")]
		[System::ComponentModel::RefreshProperties(System::ComponentModel::RefreshProperties::Repaint)]
		//[System::ComponentModel::TypeConverter("System.Windows.Forms.Design.DataSourceConverter, System.Design, Version=1.0.3300.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a")]
		[System::ComponentModel::AttributeProvider(System::ComponentModel::IListSource::typeid)]
		[_DefaultValue((string^)nullptr)]
		property object^ DataSource
		{
			object^ get(); 
			void set(object^); 
		}

		/// <summary>
		/// 데이터 소스가 바인딩 될때 사용할 데이터 멤버의 이름을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 데이터 소스가 바인딩 될때 사용할 데이터 멤버의 이름입니다.
		/// </returns>
		/// <exception cref="System::NotSupportedException">
		/// 데이터 초기화에 실패 했거나 데이터 소스가 IList, IListSource 또는 IBindingList 인터페이스를 구현하는 개체가 아닐때.
		/// </exception>
		[System::ComponentModel::DefaultValue("")]
		[System::ComponentModel::Editor("System.Windows.Forms.Design.DataMemberListEditor, System.Design, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a", System::Drawing::Design::UITypeEditor::typeid)]
		property string^ DataMember
		{
			string^ get();
			void set(string^);
		}

		/// <summary>
		/// 열의 너비조절을 수행하기 위한 열과 열 사이의 감지영역입니다.
		/// </summary>
		/// <remarks>
		/// 마우스 드래깅을 이용하여 열의 너비조절을 하기 위하여 커서가 해당 영역 안에 있는지 확인하게 됩니다.
		/// 열과 열 사이를 기준으로 -/+ 속성값 만큼의 너비안에 커서가 들어가게 되면 마우스 클릭시 열의 너비 조절을 할 수 있습니다.
		/// </remarks>
		[_Description("행의 너비조절을 수행하기 위한 열과 열 사이의 감지영역을 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(10)]
		property int ColumnSplitter
		{
			int get();
			void set(int);	
		}

		/// <summary>
		/// 행의 높이조절을 수행하기 위한 행과 행 사이의 감지영역입니다.
		/// </summary>
		/// <remarks>
		/// 마우스 드래깅을 이용하여 행의 높이조절을 하기 위하여  커서가 해당 영역 안에 있는지 확인하게 됩니다.
		/// 행과 행 사이를 기준으로 -/+ 속성값 만큼의 높이안에 커서가 들어가게 되면 마우스 클릭시 행의 높이 조절을 할 수 있습니다.
		/// </remarks>
		[_Description("열의 높이조절을 수행하기 위한 행과 행 사이의 감지영역을 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(3)]
		property int RowSplitter
		{
			int get();
			void set(int);
		}

		/// <summary>
		/// 열의 너비를 자동적으로 조절되는지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 셀의 내용이 변하게 되면 최대 너비를 계산하여 자동으로 설정됩니다.
		/// CellWordwrap 값이 true인 열에 대해서는 이 기능이 적용되지 않습니다.
		/// </remarks>
		[_Description("행의 너비를 자동적으로 조절할지에 대한 여부를 가져오거나 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(false)]
		property bool AutoFitColumn	
		{
			bool get();
			void set(bool);
		}

		/// <summary>
		/// 행의 높이를 자동적으로 조절되는지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 셀의 내용이 변경되어 문자열의 라인수가 변경되면 최대 높이를 계산하여 자동으로 설정됩니다.
		/// </remarks>
		[_Description("열의 높이를 자동적으로 조절할지에 대한 여부를 가져오거나 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(false)]
		property bool AutoFitRow
		{
			bool get();
			void set(bool);
		}

		/// <summary>
		/// 제목 표시열을 가져옵니다.
		/// </summary>
		/// <remarks>
		/// 제목을 설정할 수 있는 인터페이스를 제공합니다.
		/// </remarks>
		[_Description("제목을 설정할 수 있는 인터페이스를 제공합니다.")]
		[_Category("Behavior")]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Content)]
		property _CaptionRow^ CaptionRow
		{
			_CaptionRow^ get(); 
		}

		/// <summary>
		/// 그룹핑 표시열을 가져옵니다.
		/// </summary>
		/// <remarks>
		/// 그룹핑 표시에 대한 인터페이스를 제공합니다.
		/// </remarks>
		[_Description("그룹핑 표시에 대한 인터페이스를 제공합니다.")]
		[_Category("Behavior")]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Content)]
		property _GroupingRow^ GroupingRow
		{
			_GroupingRow^ get(); 
		}

		/// <summary>
		/// 열의 목록을 가져옵니다.
		/// </summary>
		[_Description("행의 목록을 가져옵니다.")]
		[_Category("Behavior")]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Content)]
		property ColumnCollection^ Columns
		{
			ColumnCollection^ get();
		}

		/// <summary>
		/// 표시되는 열의 목록을 가져옵니다.
		/// </summary>
#ifdef _DEBUG
		[_Category("Behavior")]
		[_Description("표시되는 열의 목록을 가져옵니다.")]
#else
		[_Browsable(false)]
#endif
		property VisibleColumnCollection^ VisibleColumns
		{
			VisibleColumnCollection^ get();
		}

		/// <summary>
		/// Displayable 열의 목록을 가져옵니다.
		/// </summary>
#ifdef _DEBUG
		[_Category("Behavior")]
#else
		[_Browsable(false)]
#endif
		property DisplayableColumnCollection^ DisplayableColumns
		{
			DisplayableColumnCollection^ get();
		}

		/// <summary>
		/// Frozen 열의 목록을 가져옵니다.
		/// </summary>
#ifdef _DEBUG
		[_Category("Behavior")]
#else
		[_Browsable(false)]
#endif
		property FrozenColumnCollection^ FrozenColumns
		{
			FrozenColumnCollection^ get();
		}

		/// <summary>
		/// Unfrozen 열의 목록을 가져옵니다.
		/// </summary>
#ifdef _DEBUG
		[_Category("Behavior")]
#else
		[_Browsable(false)]
#endif
		property UnfrozenColumnCollection^ UnfrozenColumns
		{
			UnfrozenColumnCollection^ get();
		}

		/// <summary>
		/// 포커스로 설정된 열을 가져옵니다.
		/// </summary>
#ifdef _DEBUG
		[_Category("Focus")]
		[_Description("포커스로 설정된 열을 가져옵니다.")]
#else
		[_Browsable(false)]
#endif
		property Column^ FocusedColumn
		{
			Column^ get();
		}

		/// <summary>
		/// 선택된 열들의 목록을 가져옵니다.
		/// </summary>
#ifdef _DEBUG
		[_Category("Behavior")]
#else
		[_Browsable(false)]
#endif
		property SelectedColumnCollection^ SelectedColumns
		{
			SelectedColumnCollection^ get();
		}

		/// <summary>
		/// 행의 목록을 가져옵니다.
		/// </summary>
		[_Category("Behavior")]
		[_Description("열의 목록을 가져옵니다.")]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Content)]
		property RowCollection^ Rows
		{
			RowCollection^ get();
		}

		/// <summary>
		/// Visible 행의 목록을 가져옵니다.
		/// </summary>
#ifdef _DEBUG
		[_Category("Behavior")]
#else
		[_Browsable(false)]
#endif
		property VisibleRowCollection^ VisibleRows
		{
			VisibleRowCollection^ get();
		}

		/// <summary>
		/// Displayable 행의 목록을 가져옵니다.
		/// </summary>
#ifdef _DEBUG
		[_Category("Behavior")]
#else
		[_Browsable(false)]
#endif
		property DisplayableRowCollection^ DisplayableRows
		{
			DisplayableRowCollection^ get();
		}

		/// <summary>
		/// 삽입열을 가져옵니다.
		/// </summary>
		/// <remarks>
		/// 새로운 행을 추가 하기 위하여 그리드컨트롤에서는 삽입열이라는 특수한 행을 제공합니다.
		/// 삽입열은 새로 만들거나 삭제할 수 없습니다.
		/// </remarks>
		[_Category("Behavior")]
		property _InsertionRow^ InsertionRow
		{
			_InsertionRow^ get();
		}

		/// <summary>
		/// 선택된 행들의 목록을 가져옵니다.
		/// </summary>
		/// <remarks>
		/// 각 요소는 현재 화면상에 보여지는 순서로 이루어져있습니다.
		/// </remarks>
#ifdef _DEBUG
		[_Category("Behavior")]
#else
		[_Browsable(false)]
#endif
		property SelectedRowCollection^ SelectedRows
		{
			SelectedRowCollection^	get();
		}	

		/// <summary>
		/// 포커스로 설정된 행을 가져옵니다.
		/// </summary>
#ifdef _DEBUG
		[_Category("Focus")]
		[_Description("포커스로 설정된 행을 가져옵니다.")]
#else
		[_Browsable(false)]
#endif
		property Row^ FocusedRow
		{
			Row^ get(); 
		}	

		/// <summary>
		/// 포커스를 지정할 셀을 가져오거나 설정합니다.
		/// </summary>
#ifdef _DEBUG
		[_Category("Focus")]
		[_Description("포커스로 설정된 행을 가져옵니다.")]
#else
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
		[_Browsable(false)]
#endif
		property Cell^ FocusedCell
		{
			Cell^ get(); 
			void set(Cell^);
		}

		/// <summary>
		/// 편집중인 셀을 가져옵니다.
		/// </summary>
#ifdef _DEBUG
		[_Description("편집중인 셀을 가져옵니다.")]
		[_Category("Behavior")]
#else
		[_Browsable(false)]
#endif
		property Cell^ EditingCell
		{
			Cell^ get(); 
		}

		/// <summary>
		/// 포커스를 이동할 수 있는 인터페이스를 제공합니다.
		/// </summary>
		[_Browsable(false)]
		property _CellIterator^ CellIterator
		{
			_CellIterator^ get(); 
		}

		/// <summary>
		/// 툴팁을 사용할 수 있는 인터페이스를 가져옵니다.
		/// </summary>
		[_Browsable(false)]
		property IToolTip^ ToolTip
		{
			IToolTip^ get();
		}

		/// <summary>
		/// 열의 위치 이동에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 이 속성의 값이 false일 경우 마우스 드래깅을 이용한 열의 위치 이동 기능이 제한됩니다.
		/// </remarks>
		[_Description("마우스를 이용하여 열의 위치 이동을 할 수 있는지에 대한 여부를 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(true)]
		property bool EnableColumnMoving
		{
			bool get();
			void set(bool); 
		}

		/// <summary>
		/// 열의 너비조절에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 이 속성의 값이 false일 경우 마우스 드래깅을 이용한 열의 너비조절 기능이 제한됩니다.
		/// </remarks>
		[_Description("마우스를 이용하여 열의 너비 조절을 할 수 있는지에 대한 여부를 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(true)]
		property bool EnableColumnResizing
		{
			bool get();	
			void set(bool);
		}

		/// <summary>
		/// 열의 수평 스크롤 적용에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// Column Frozing은 지정된 열이 스크롤에 영향을 받지 않도록 하는 기능입니다.
		/// 이 속성의 값이 false일 경우 마우스를 이용하여 Frozing을 설정하는 기능이 제한됩니다.
		/// </remarks>
		[_Description("행의 스크롤 적용여부를 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(true)]
		property bool EnableColumnFrozing
		{
			bool get();	
			void set(bool); 
		}

		/// <summary>
		/// 열의 정렬여부를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 이 속성의 값이 false일 경우 마우스 클릭으로 열의 정렬기능이 제한됩니다.
		/// </remarks>
		[_Description("마우스 클릭을 사용하여 열의 내용을 정렬할 수 있는지에 대한 여부를 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(true)]
		property bool EnableSorting
		{
			bool get();	
			void set(bool); 
		}

		/// <summary>
		/// 읽기 전용에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 이 속성의 값이 false일 경우 마우스 또는 키보드를 이용한 셀의 편집 기능이 제한됩니다.
		/// </remarks>
		[_Description("읽기 전용인지에 대한 여부를 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(false)]
		property bool ReadOnly
		{
			bool get();	
			void set(bool);
		}

		/// <summary>
		/// 행의 높이조절에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 이 속성의 값이 false일 경우 마우스 드래깅을 이용한 행 높이조절 기능이 제한됩니다.
		/// </remarks>
		[_Description("마우스를 이용하여 행의 높이를 조절할 수 있는지에 대한 여부를 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(true)]
		property bool EnableRowResizing
		{
			bool get();	
			void set(bool); 
		}

		/// <summary>
		/// 그룹화 기능에 대한 사용여부를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 이 속성의 값이 false일 경우 마우스 또는 키보드를 이용한 그룹화 기능이 제한됩니다.
		/// </remarks>
		[_Description("그룹화 기능을 사용할지에 대한 여부를 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(true)]
		property bool EnableGrouping
		{
			bool get();
			void set(bool);
		}

		/// <summary>
		/// 그룹화 되었는지에 대한 여부를 가져옵니다.
		/// </summary>
#ifdef _DEBUG
		[_Category("Debug")]
		[_DefaultValue(false)]
#else
		[_Browsable(false)]
#endif
		property bool IsGrouped
		{
			bool get();
		}

		/// <summary>
		/// 행의 숫자 표시에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 이 속성의 값이 false일 경우에는 DataGridView 형식으로 표시합니다.
		/// </remarks>
		[_Description("열의 숫자 표시에 대한 여부를 설정합니다.")]
		[_Category("Appearance"), _DefaultValue(true)]
		property bool IsRowNumberVisible
		{
			bool get();
			void set(bool);	
		}

		/// <summary>
		/// 우측에 남은 여백을 표시할지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		[_Description("우측에 남은 여백을 표시할지에 대한 여부를 설정합니다.")]
		[_Category("Appearance"), _DefaultValue(false)]
		property bool						IsMarginVisible			{ bool get();	void set(bool); }

		/// <summary>
		/// 새로운 열을 추가하기위한 Insertion열에 표시 여부를 가져오거나 설정합니다.
		/// </summary>
		[_Description("새로운 열을 추가하기위한 삽입열의 표시 여부를 설정합니다.")]
		[_Category("Appearance"), _DefaultValue(true)]
		property bool						IsInsertionRowVisible	{ bool get();	void set(bool); }

		/// <summary>
		/// 상단의 제목 표시 여부를 가져오거나 설정합니다.
		/// </summary>
		[_Description("상단의 제목 표시 여부를 설정합니다.")]
		[_Category("Appearance"), _DefaultValue(true)]
		property bool IsCaptionRowVisible
		{
			bool get();	
			void set(bool);
		}

		/// <summary>
		/// 상단의 그룹핑 표시 여부를 가져오거나 설정합니다.
		/// </summary>
		[_Description("상단의 그룹핑 표시 여부를 설정합니다.")]
		[_Category("Appearance"), _DefaultValue(true)]
		property bool IsGroupingRowVisible
		{
			bool get();	
			void set(bool);
		}

		/// <summary>
		/// 행의 전체선택에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 이 속성의 값이 true일 경우 마우스로 인한 셀 선택시 같은 범위내에 있는 모든 셀들이 선택됩니다.
		/// </remarks>
		[_Description("셀 선택시 같은 범위내에 있는 모든 셀들의 선택 여부를 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(false)]
		property bool FullRowSelect
		{
			bool get();	
			void set(bool); 
		}

		/// <summary>
		/// 포커스된 행의 강조 표시 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 이 속성의 값이 true일 경우 포커스 된 열은 강조 표시가 되며, 그 방법은 <see cref="RowHighlightType"/>의 값으로 변경시킬 수 있습니다.
		/// </remarks>
		[_Description("포커스된 행의 강조 표시 여부를 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(false)]
		property bool RowHighlight
		{
			bool get();	void set(bool); 
		}

		/// <summary>
		/// 포커스된 행의 강조 표시의 방법을 가져오거나 설정합니다.
		/// </summary>
		[_Description("포커스된 행의 강조 표시 방법을 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(_RowHighlightType::Fill)]
		property _RowHighlightType RowHighlightType
		{
			_RowHighlightType get();
			void set(_RowHighlightType);
		}

		/// <summary>
		/// 컨트롤이 포커스를 잃었을 때 해당 컨트롤에서 선택된 항목이 강조 표시된 상태로 남아 있는지 여부를 나타내는 값을 가져오거나 설정합니다.
		/// </summary>
		[_Description("컨트롤이 포커스를 잃었을대 선택 영역을 표시하는지에 대한 여부를 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(false)]
		property bool HideSelection 
		{
			bool get();
			void set(bool);
		}

		/// <summary>
		/// 여러 개의 항목을 선택할 수 있는지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		[_Description("여러 개의 항목을 선택할 수 있는지에 대한 여부를 설정합니다.")]
		[_Category("Behavior"), _DefaultValue(true)]
		property bool MultiSelect
		{
			bool get();
			void set(bool);
		}

		/// <summary>
		/// 스타일을 가져옵니다.
		/// </summary>
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
		[_Category("Appearance")]
		property Design::Style^ Style
		{
			Design::Style^ get();
			void set(Design::Style^);
		}

#ifdef _DEBUG
		[_Category("Debug")]
#else
		[_Browsable(false)]
#endif
		/// <summary>
		/// 셀들이 표시되는 영역을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 셀들의 표시영역을 나타내는 <see cref="System::Drawing::Rectangle"/>입니다.
		/// </returns>
		property _Rectangle DataRectangle
		{
			_Rectangle get();
		}

		/// <summary>
		/// 그리드 컨트롤에서 사용되지 않습니다.
		/// </summary>
		[_Browsable(false)]
		[System::ObsoleteAttribute]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
		property bool AutoScroll
		{
			virtual bool get() new { return false; }
			virtual void set(bool) new { throw gcnew System::NotImplementedException(); }
		}

		/// <summary>
		/// 그리드 컨트롤에서 사용되지 않습니다.
		/// </summary>
		[_Browsable(false)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
		[System::ObsoleteAttribute]
		property _Size AutoScrollMargin
		{
			_Size get() new { return _Size::Empty; }
			void set(_Size) new { throw gcnew System::NotImplementedException(); }
		}

		/// <summary>
		/// 그리드 컨트롤에서 사용되지 않습니다.
		/// </summary>
		[_Browsable(false)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
		[System::ObsoleteAttribute]
		property _Size AutoScrollMinSize
		{
			_Size get() new { return _Size::Empty; }
			void set(_Size) new { throw gcnew System::NotImplementedException(); }
		}

		/// <summary>
		/// 그리드 컨트롤에서 사용되지 않습니다.
		/// </summary>
		[_Browsable(false)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
		[System::ObsoleteAttribute]
		property _Point AutoScrollPosition
		{
			_Point get() new { return _Point::Empty; }
			void set(_Point) new { throw gcnew System::NotImplementedException(); }
		}

		/// <summary>
		/// 수평 스크롤 막대와 관련된 특징을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 수평 스크롤을 나타내는 <see cref="Ntreev::Windows::Forms::Grid::HScrollProperty"/>입니다.
		/// </returns>
		[_Browsable(false)]
		property HScrollProperty^ HorizontalScroll
		{
			HScrollProperty^ get() new;
		}

		/// <summary>
		/// 수직 스크롤 막대와 관련된 특징을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 수직 스크롤을 나타내는 <see cref="Ntreev::Windows::Forms::Grid::VScrollProperty"/>입니다.
		/// </returns>
		[_Browsable(false)]
		property VScrollProperty^ VerticalScroll
		{
			VScrollProperty^ get() new; 
		}

		/// <summary>
		/// 컨트롤에서 지원하는 기본 IME(입력기) 모드를 가져옵니다.
		/// </summary>
		/// <returns>
		/// <see cref="System::Windows::Forms::ImeMode"/> 값 중 하나입니다.
		/// </returns>
		[_Browsable(false)]
		property _ImeMode DefaultImeMode
		{
			virtual _ImeMode get() override { return _ImeMode::Alpha; }
		}

		/// <summary>
		/// IME 지원을 사용하도록 <see cref="System::Windows::Forms::Control::ImeMode"/> 속성을 활성 값으로 설정할 수 있는지 여부를 나타내는 값을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 모든 경우에 false를 반환합니다.
		/// </returns>
		[_Browsable(false)]
		property bool CanEnableIme
		{
			virtual bool get() override { return true; }
		}

#ifdef _TIME_TEST
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
		property bool Rendering;
#endif

	public:
		/// <summary>
		/// 셀의 값이 변경되기 전에 발생합니다.
		/// </summary>
		/// <remarks>
		/// 셀의 값이 변경되는 것을 원하지 않는 경우 이벤트 데이터를 통하여 취소시킬 수 있습니다.
		/// </remarks>
		[_Description("셀의 값이 변경되기 전에 발생합니다.")]
		[_Category("Behavior")]
		event ValueChangingEventHandler^ ValueChanging
		{
			void add(ValueChangingEventHandler^ p) { m_eventValueChanging += p; }
			void remove(ValueChangingEventHandler^ p) { m_eventValueChanging -= p; }
		private:
			void raise(object^ sender, ValueChangingEventArgs^ e) { if(m_eventValueChanging != nullptr) m_eventValueChanging->Invoke(sender, e); }
		}

		/// <summary>
		/// 셀의 값이 변경된 후에 발생합니다.
		/// </summary>
		[_Description("셀의 값이 변경된 후에 발생합니다.")]
		[_Category("Behavior")]
		event CellEventHandler^ ValueChanged
		{
			void add(CellEventHandler^ p) { m_eventValueChanged += p; }
			void remove(CellEventHandler^ p) { m_eventValueChanged -= p; }
		private:
			void raise(object^ sender, CellEventArgs^ e) { if(m_eventValueChanged != nullptr) m_eventValueChanged->Invoke(sender, e); }
		}

		/// <summary>
		/// 행이 삽입되기 전에 발생합니다.
		/// </summary>
		/// <remarks>
		/// 행이 삽입되는 것을 원하지 않는 경우 이벤트 데이터를 통하여 취소시킬 수 있습니다.
		/// </remarks>
		[_Description("열이 삽입되기 전에 발생합니다.")]
		[_Category("Row")]
		event RowInsertingEventHandler^	RowInserting
		{
			void add(RowInsertingEventHandler^ p) { m_eventRowInserting += p; }
			void remove(RowInsertingEventHandler^ p) { m_eventRowInserting -= p; }
		private:
			void raise(object^ sender, RowInsertingEventArgs^ e) { if(m_eventRowInserting != nullptr) m_eventRowInserting->Invoke(sender, e); }
		}

		/// <summary>
		/// 행이 삽입된 후에 발생합니다.
		/// </summary>
		[_Description("열이 삽입된 후에 발생합니다.")]
		[_Category("Row")]
		event RowEventHandler^ RowInserted
		{
			void add(RowEventHandler^ p) { m_eventRowInserted += p; }
			void remove(RowEventHandler^ p) { m_eventRowInserted -= p; }
		private:
			void raise(object^ sender, RowEventArgs^ e) { if(m_eventRowInserted != nullptr) m_eventRowInserted->Invoke(sender, e); }
		}

		/// <summary>
		/// 행이 목록에서 제거되기 전에 발생합니다.
		/// </summary>
		/// <remarks>
		/// 행이 목록에서 제거되는 것을 원하지 않는다면 이벤트 데이터를 통하여 취소시킬 수 있습니다.
		/// </remarks>
		[_Description("열이 목록에서 제거되기 전에 발생합니다.")]
		[_Category("Row")]
		event RowRemovingEventHandler^ RowRemoving
		{
			void add(RowRemovingEventHandler^ p) { m_eventRowRemoving += p; }
			void remove(RowRemovingEventHandler^ p) { m_eventRowRemoving -= p; }
		private:
			void raise(object^ sender, RowRemovingEventArgs^ e) { if(m_eventRowRemoving != nullptr) m_eventRowRemoving->Invoke(sender, e); }
		}

		/// <summary>
		/// 행이 목록에서 제거된 후에 발생합니다.
		/// </summary>
		[_Description("열이 목록에서 제거된 후에 발생합니다.")]
		[_Category("Row")]
		event RowRemovedEventHandler^ RowRemoved
		{
			void add(RowRemovedEventHandler^ p) { m_eventRowRemoved += p; }
			void remove(RowRemovedEventHandler^ p) { m_eventRowRemoved -= p; }
		private:
			void raise(object^ sender, RowRemovedEventArgs^ e) { if(m_eventRowRemoved != nullptr) m_eventRowRemoved->Invoke(sender, e); }
		}

		/// <summary>
		/// 삽입열에 의하여 새로운 행이 삽입되기 전에 발생합니다.
		/// </summary>
		/// <remarks>
		/// 삽입열에 의하여 새로운 행이 삽입되는 것을 원하지 않는다면 이벤트 데이터를 통하여 취소시킬 수 있습니다.
		/// </remarks>
		[_Description("삽입열에 의하여 새로운 행이 삽입되기 전에 발생합니다.")]
		[_Category("Row")]
		event InsertionRowInsertingEventHandler^ InsertionRowInserting
		{
			void add(InsertionRowInsertingEventHandler^ p) { m_eventInsertionRowInserting += p; }
			void remove(InsertionRowInsertingEventHandler^ p) { m_eventInsertionRowInserting -= p; }
		private:
			void raise(object^ sender, InsertionRowInsertingEventArgs^ e) { if(m_eventInsertionRowInserting != nullptr) m_eventInsertionRowInserting->Invoke(sender, e); }
		}

		/// <summary>
		/// 삽입열에 의하여 새로운 행이 삽입된 후에 발생합니다.
		/// </summary>
		[_Description("삽입열에 의하여 새로운 행이 삽입된 후에 발생합니다.")]
		[_Category("Row")]
		event RowEventHandler^ InsertionRowInserted
		{
			void add(RowEventHandler^ p) { m_eventInsertionRowInserted += p; }
			void remove(RowEventHandler^ p) { m_eventInsertionRowInserted -= p; }
		private:
			void raise(object^ sender, RowEventArgs^ e) { if(m_eventInsertionRowInserted != nullptr) m_eventInsertionRowInserted->Invoke(sender, e); }
		}

		/// <summary>
		/// 열들의 선택 범위가 변경되었을때 발생합니다.
		/// </summary>
		[_Description("행들의 선택 범위가 변경되었을때 발생합니다.")]
		[_Category("Selection")]
		event _EventHandler^ SelectedColumnsChanged
		{
			void add(_EventHandler^ p) { m_eventSelectedColumnsChanged += p; }
			void remove(_EventHandler^ p) { m_eventSelectedColumnsChanged -= p; }
		private:
			void raise(object^ sender, _EventArgs^ e) { if(m_eventSelectedColumnsChanged != nullptr) m_eventSelectedColumnsChanged->Invoke(sender, e); }
		}

		/// <summary>
		/// 행들의 선택 범위가 변경되었을때 발생합니다.
		/// </summary>
		[_Description("열들의 선택 범위가 변경되었을때 발생합니다.")]
		[_Category("Selection")]
		event _EventHandler^ SelectedRowsChanged
		{
			void add(_EventHandler^ p) { m_eventSelectedRowsChanged += p; }
			void remove(_EventHandler^ p) { m_eventSelectedRowsChanged -= p; }
		private:
			void raise(object^ sender, _EventArgs^ e) { if(m_eventSelectedRowsChanged != nullptr) m_eventSelectedRowsChanged->Invoke(sender, e); }
		}

		/// <summary>
		/// 셀들의 선택 범위가 변경되었을때 발생합니다.
		/// </summary>
		[_Description("셀들의 선택 범위가 변경되었을때 발생합니다.")]
		[_Category("Selection")]
		event _EventHandler^ SelectionChanged
		{
			void add(_EventHandler^ p) { m_eventSelectionChanged += p; }
			void remove(_EventHandler^ p) { m_eventSelectionChanged -= p; }
		private:
			void raise(object^ sender, _EventArgs^ e) { if(m_eventSelectionChanged != nullptr) m_eventSelectionChanged->Invoke(sender, e); }
		}

		/// <summary>
		/// 셀의 포커스가 변경되었을대 발생합니다.
		/// </summary>
		[_Description("셀의 포커스가 변경되었을대 발생합니다.")]
		[_Category("Focus")]
		event _EventHandler^ FocusedColumnChanged
		{
			void add(_EventHandler^ p) { m_eventFocusedColumnChanged += p; }
			void remove(_EventHandler^ p) { m_eventFocusedColumnChanged -= p; }
		private:
			void raise(object^ sender, _EventArgs^ e) { if(m_eventFocusedColumnChanged != nullptr) m_eventFocusedColumnChanged->Invoke(sender, e); }
		}

		/// <summary>
		/// 셀의 포커스가 변경되었을대 발생합니다.
		/// </summary>
		[_Description("셀의 포커스가 변경되었을대 발생합니다.")]
		[_Category("Focus")]
		event _EventHandler^ FocusedRowChanged
		{
			void add(_EventHandler^ p) { m_eventFocusedRowChanged += p; }
			void remove(_EventHandler^ p) { m_eventFocusedRowChanged -= p; }
		private:
			void raise(object^ sender, _EventArgs^ e) { if(m_eventFocusedRowChanged != nullptr) m_eventFocusedRowChanged->Invoke(sender, e); }
		}

		/// <summary>
		/// 셀의 포커스가 변경되었을대 발생합니다.
		/// </summary>
		[_Description("셀의 포커스가 변경되었을대 발생합니다.")]
		[_Category("Focus")]
		event CellEventHandler^ FocusedCellChanged
		{
			void add(CellEventHandler^ p) { m_eventFocusedCellChanged += p; }
			void remove(CellEventHandler^ p) { m_eventFocusedCellChanged -= p; }
		private:
			void raise(object^ sender, CellEventArgs^ e) { if(m_eventFocusedCellChanged != nullptr) m_eventFocusedCellChanged->Invoke(sender, e); }
		}

		/// <summary>
		/// 열이 삽입되기 전에 발생합니다.
		/// </summary>
		/// <remarks>
		/// 열이 삽입되는것을 원하지 않는다면 이벤트 데이터를 통하여 취소시킬 수 있습니다.
		/// </remarks>
		[_Description("행이 삽입되기 전에 발생합니다.")]
		[_Category("Column")]
		event ColumnInsertingEventHandler^ ColumnInserting
		{
			void add(ColumnInsertingEventHandler^ p) { m_eventColumnInserting += p; }
			void remove(ColumnInsertingEventHandler^ p) { m_eventColumnInserting -= p; }
		private:
			void raise(object^ sender, ColumnInsertingEventArgs^ e) { if(m_eventColumnInserting != nullptr) m_eventColumnInserting->Invoke(sender, e); }
		}

		/// <summary>
		/// 열이 삽입된 후에 발생합니다.
		/// </summary>
		[_Description("행이 삽입된 후에 발생합니다.")]
		[_Category("Column")]
		event ColumnEventHandler^ ColumnInserted
		{
			void add(ColumnEventHandler^ p) { m_eventColumnInserted += p; }
			void remove(ColumnEventHandler^ p) { m_eventColumnInserted -= p; }
		private:
			void raise(object^ sender, ColumnEventArgs^ e) { if(m_eventColumnInserted != nullptr) m_eventColumnInserted->Invoke(sender, e); }
		}

		/// <summary>
		/// 열이 데이터 소스와 연결되기 전에 발생합니다.
		/// </summary>
		[_Description("행이 데이터 소스와 연결되기 전에 발생합니다.")]
		[_Category("Column")]
		event ColumnBindingEventHandler^ ColumnBinding
		{
			void add(ColumnBindingEventHandler^ p) { m_eventColumnBinding += p; }
			void remove(ColumnBindingEventHandler^ p) { m_eventColumnBinding -= p; }
		private:
			void raise(object^ sender, ColumnBindingEventArgs^ e) { if(m_eventColumnBinding != nullptr) m_eventColumnBinding->Invoke(sender, e); }
		}

		/// <summary>
		/// 열이 데이터 소스와 연결된 후에 발생합니다.
		/// </summary>
		[_Description("행이 데이터 소스와 연결된 후에 발생합니다.")]
		[_Category("Column")]
		event ColumnEventHandler^ ColumnBinded
		{
			void add(ColumnEventHandler^ p) { m_eventColumnBinded += p; }
			void remove(ColumnEventHandler^ p) { m_eventColumnBinded -= p; }
		private:
			void raise(object^ sender, ColumnEventArgs^ e) { if(m_eventColumnBinded != nullptr) m_eventColumnBinded->Invoke(sender, e); }
		}
		
		/// <summary>
		/// 열의 너비가 변경되었을때 발생합니다.
		/// </summary>
		[_Description("행의 너비가 변경되었을때 발생합니다.")]
		[_Category("Column")]
		event ColumnEventHandler^ ColumnWidthChanged
		{
			void add(ColumnEventHandler^ p) { m_eventColumnWidthChanged += p; }
			void remove(ColumnEventHandler^ p) { m_eventColumnWidthChanged -= p; }
		private:
			void raise(object^ sender, ColumnEventArgs^ e) { if(m_eventColumnWidthChanged != nullptr) m_eventColumnWidthChanged->Invoke(sender, e); }
		}

		/// <summary>
		/// 마우스의 커서 위치가 열의 영역안으로 들어갔을때 발생합니다.
		/// </summary>
		[_Description("마우스의 커서 위치가 열의 영역안으로 들어갔을때 발생합니다.")]
		[_Category("Column")]
		event ColumnMouseEventHandler^ ColumnMouseEnter
		{
			void add(ColumnMouseEventHandler^ p) { m_eventColumnMouseEnter += p; }
			void remove(ColumnMouseEventHandler^ p) { m_eventColumnMouseEnter -= p; }
		private:
			void raise(object^ sender, ColumnMouseEventArgs^ e) { if(m_eventColumnMouseEnter != nullptr) m_eventColumnMouseEnter->Invoke(sender, e); }
		}

		/// <summary>
		/// 마우스의 커서 위치가 열의 영역밖으로 나갔을때 발생합니다.
		/// </summary>
		[_Description("마우스의 커서 위치가 열의 영역밖으로 나갔을때 발생합니다.")]
		[_Category("Column")]
		event ColumnMouseEventHandler^ ColumnMouseLeave
		{
			void add(ColumnMouseEventHandler^ p) { m_eventColumnMouseLeave += p; }
			void remove(ColumnMouseEventHandler^ p) { m_eventColumnMouseLeave -= p; }
		private:
			void raise(object^ sender, ColumnMouseEventArgs^ e) { if(m_eventColumnMouseLeave != nullptr) m_eventColumnMouseLeave->Invoke(sender, e); }
		}

		/// <summary>
		/// 열의 영역안에서 마우스의 좌측버튼이 눌러졌을때 발생합니다.
		/// </summary>
		[_Description("행의 영역안에서 마우스의 좌측버튼이 눌러졌을때 발생합니다.")]
		[_Category("Column")]
		event ColumnMouseEventHandler^ ColumnMouseDown
		{
			void add(ColumnMouseEventHandler^ p) { m_eventColumnMouseDown += p; }
			void remove(ColumnMouseEventHandler^ p) { m_eventColumnMouseDown -= p; }
		private:
			void raise(object^ sender, ColumnMouseEventArgs^ e) { if(m_eventColumnMouseDown != nullptr) m_eventColumnMouseDown->Invoke(sender, e); }
		}

		/// <summary>
		/// 열의 영역안에서 마우스의 좌측버튼이 띄워졌을때 발생합니다.
		/// </summary>
		[_Description("행의 영역안에서 마우스의 좌측버튼이 띄워졌을때 발생합니다.")]
		[_Category("Column")]
		event ColumnMouseEventHandler^ ColumnMouseUp
		{
			void add(ColumnMouseEventHandler^ p) { m_eventColumnMouseUp += p; }
			void remove(ColumnMouseEventHandler^ p) { m_eventColumnMouseUp -= p; }
		private:
			void raise(object^ sender, ColumnMouseEventArgs^ e) { if(m_eventColumnMouseUp != nullptr) m_eventColumnMouseUp->Invoke(sender, e); }
		}

		/// <summary>
		/// 마우스의 커서 위치가 열의 영역안에서 움직였을때 발생합니다.
		/// </summary>
		[_Description("마우스의 커서 위치가 열의 영역안에서 움직였을때 발생합니다.")]
		[_Category("Column")]
		event ColumnMouseEventHandler^ ColumnMouseMove
		{
			void add(ColumnMouseEventHandler^ p) { m_eventColumnMouseMove += p; }
			void remove(ColumnMouseEventHandler^ p) { m_eventColumnMouseMove -= p; }
		private:
			void raise(object^ sender, ColumnMouseEventArgs^ e) { if(m_eventColumnMouseMove != nullptr) m_eventColumnMouseMove->Invoke(sender, e); }
		}

		/// <summary>
		/// 셀의 영역안에서 마우스가 두번 눌러졌을때 발생합니다.
		/// </summary>
		/// <remakrs>
		/// 셀이 읽기 전용일때만 동작합니다.
		/// </remakrs>
		[_Description("셀의 영역안에서 마우스가 두번 눌러졌을때 발생합니다. 셀이 읽기 전용일때만 동작합니다.")]
		[_Category("Behavior")]
		event CellEventHandler^ CellDoubleClicked
		{
			void add(CellEventHandler^ p) { m_eventCellDoubleClicked += p; }
			void remove(CellEventHandler^ p) { m_eventCellDoubleClicked -= p; }
		private:
			void raise(object^ sender, CellEventArgs^ e) { if(m_eventCellDoubleClicked != nullptr) m_eventCellDoubleClicked->Invoke(sender, e); }
		}

		/// <summary>
		/// 셀의 편집이 시작됬을때 발생합니다.
		/// </summary>
		/// <remarks>
		/// 셀의 편집을 원하지 않는다면 이벤트 데이터를 통하여 취소시킬 수 있습니다.
		/// </remarks>
		[_Description("셀의 편집이 시작됬을때 발생합니다.")]
		[_Category("Behavior")]
		event BeginEditEventHandler^ BeginEdit
		{
			void add(BeginEditEventHandler^ p) { m_eventBeginEdit += p; }
			void remove(BeginEditEventHandler^ p) { m_eventBeginEdit -= p; }
		private:
			void raise(object^ sender, BeginEditEventArgs^ e) { if(m_eventBeginEdit != nullptr) m_eventBeginEdit->Invoke(sender, e); }
		}

		/// <summary>
		/// 셀의 편집이 종료됬을때 발생합니다.
		/// </summary>
		[_Description("셀의 편집이 종료됬을때 발생합니다.")]
		[_Category("Behavior")]
		event CellEventHandler^ EndEdit
		{
			void add(CellEventHandler^ p) { m_eventEndEdit += p; }
			void remove(CellEventHandler^ p) { m_eventEndEdit -= p; }
		private:
			void raise(object^ sender, CellEventArgs^ e) { if(m_eventEndEdit != nullptr) m_eventEndEdit->Invoke(sender, e); }
		}

		/// <summary>
		/// 모든 데이터가 삭제되기 전에 발생합니다.
		/// </summary>
		[_Description("모든 데이터가 삭제되기 전에 발생합니다.")]
		[_Category("Behavior")]
		event _EventHandler^ Clearing
		{
			void add(_EventHandler^ p) { m_eventClearing += p; }
			void remove(_EventHandler^ p) { m_eventClearing -= p; }
		private:
			void raise(object^ sender, _EventArgs^ e) { if(m_eventClearing != nullptr) m_eventClearing->Invoke(sender, e); }
		}

		/// <summary>
		/// 모든 데이터가 삭제된 후에 발생합니다.
		/// </summary>
		[_Description("모든 데이터가 삭제된 후에 발생합니다.")]
		[_Category("Behavior")]
		event _EventHandler^ Cleared
		{
			void add(_EventHandler^ p) { m_eventCleared += p; }
			void remove(_EventHandler^ p) { m_eventCleared -= p; }
		private:
			void raise(object^ sender, _EventArgs^ e) { if(m_eventCleared != nullptr) m_eventCleared->Invoke(sender, e); }
		}

		/// <summary>
		/// 데이터 소스가 변경되었을때 발생합니다.
		/// </summary>
		[_Description("데이터 소스가 변경되었을때 발생합니다.")]
		[_Category("Behavior")]
		event _EventHandler^ DataSourceChanged
		{
			void add(_EventHandler^ p) { m_eventDataSourceChanged += p; }
			void remove(_EventHandler^ p) { m_eventDataSourceChanged -= p; }
		private:
			void raise(object^ sender, _EventArgs^ e) { if(m_eventDataSourceChanged != nullptr) m_eventDataSourceChanged->Invoke(sender, e); }
		}

		/// <summary>
		/// 데이터 멤버가 변경되었을때 발생합니다.
		/// </summary>
		[_Description("데이터 멤버가 변경되었을때 발생합니다.")]
		event _EventHandler^ DataMemberChanged
		{
			void add(_EventHandler^ p) { m_eventDataMemberChanged += p; }
			void remove(_EventHandler^ p) { m_eventDataMemberChanged -= p; }
		private:
			void raise(object^ sender, _EventArgs^ e) { if(m_eventDataMemberChanged != nullptr) m_eventDataMemberChanged->Invoke(sender, e); }
		}

		/// <summary>
		/// 데이터 바인딩이 종료되었을때 발생합니다.
		/// </summary>
		[_Description("데이터 바인딩이 종료되었을때 발생합니다.")]
		event _EventHandler^ DataBindingComplete
		{
			void add(_EventHandler^ p) { m_eventDataBindingComplete += p; }
			void remove(_EventHandler^ p) { m_eventDataBindingComplete -= p; }
		private:
			void raise(object^ sender, _EventArgs^ e) { if(m_eventDataBindingComplete != nullptr) m_eventDataBindingComplete->Invoke(sender, e); }
		}

	internal:
		event _EventHandler^ StateBegin
		{
			void add(_EventHandler^ p) { m_eventStateBegin += p; }
			void remove(_EventHandler^ p) { m_eventStateBegin -= p; }
		private:
			void raise(object^ sender, _EventArgs^ e) { if(m_eventStateBegin != nullptr) m_eventStateBegin->Invoke(sender, e); }
		}

		event _EventHandler^ StateEnd
		{
			void add(_EventHandler^ p) { m_eventStateEnd += p; }
			void remove(_EventHandler^ p) { m_eventStateEnd -= p; }
		private:
			void raise(object^ sender, _EventArgs^ e) { if(m_eventStateEnd != nullptr) m_eventStateEnd->Invoke(sender, e); }
		}
	
	internal: // methods
		bool					InvokeValueChanging(Cell^ cell, object^ value, object^ oldValue);
		void					InvokeValueChanged(Cell^ cell);
		bool					InvokeInsertionRowInserting(_Row^ row);
		void					InvokeInsertionRowInserted(_Row^ row);
		bool					InvokeRowInserting(object^ component);
		void					InvokeRowInserted(Row^ row);
		bool					InvokeRowRemoving(Row^ row);
		void					InvokeRowRemoved(RowRemovedEventArgs^ e);
		bool					InvokeColumnInserting(_Column^ column);
		void					InvokeColumnInserted(_Column^ column);
		void					InvokeColumnBinding(ColumnBindingEventArgs^ e);
		void					InvokeColumnBinded(ColumnEventArgs^ e);
		bool					InvokeColumnMouseDown(Column^ column, _Point clientLocation);
		bool					InvokeColumnMouseUp(Column^ column, _Point clientLocation);
		void					InvokeColumnMouseEnter(Column^ column, _Point clientLocation);
		void					InvokeColumnMouseMove(Column^ column, _Point clientLocation);
		void					InvokeColumnMouseLeave(Column^ column);
		bool					InvokeBeginEdit(Cell^ cell);
		void					InvokeEndEdit(CellEventArgs^ e);
		void					InvokeScroll(System::Windows::Forms::ScrollEventArgs^ e);
		void					InvokeCellDoubleClicked(Cell^ cell);

		bool					DoHitTest(_Point pt, HitTest% hitTest);
		//void					InvalidateCell(const GrCell* pCell);
		object^					GetInternalService(_Type^ serviceType);

		void					gridCore_FocusChanging();
		void					gridCore_FocusChanged();
		void					gridCore_SelectedRowsChanged();
		void					gridCore_SelectedColumnsChanged();
		void					gridCore_SelectionChanged();
		void					dataColumnList_ColumnWidthChanged(GrColumn* pColumn);
		void					dataRowList_RowVisibleChanged();

		void					OnStateBegin(_EventArgs^ e);
		void					OnStateEnd(_EventArgs^ e);

	internal: // properties
		//property _State State
		//{
		//	_State get();
		//internal: 
		//	void set(_State);
		//}

		//property HitState HitState
		//{
		//	_HitState get();
		//private: 
		//	void set(_HitState); 
		//}

		property GrGridCore* GridCore
		{
			GrGridCore* get() { return m_pGridCore; }
		}

		property GrGridRenderer* GridRenderer
		{
			GrGridRenderer* get() { return m_pGridRenderer; }
		}

		property GrItemSelector* Selector
		{
			GrItemSelector* get();
		}

		property GrFocuser* Focuser
		{
			GrFocuser* get();
		}

		property GrHitTester* HitTester
		{
			GrHitTester* get();
		}

		property GrSelectionType NativeSelectionType
		{
			GrSelectionType get(); 
		}

		property Private::ErrorDescriptor^	ErrorDescriptor
		{
			Private::ErrorDescriptor^ get() { return m_errorDescriptor; }
		}

		property GridState::StateManager^ StateManager
		{
			GridState::StateManager^ get() { return m_states; }
		}

		property _SelectionType SelectionType
		{
			_SelectionType get();
		}

		property _SelectionRange SelectionRange
		{
			_SelectionRange get();  
		}

		property _CurrencyManager^ CurrencyManager
		{
			_CurrencyManager^ get() { if(m_manager == nullptr) return m_defaultManager; return m_manager; }
		}

	internal: // events
		event CurrencyManagerChangedEventHandler^ CurrencyManagerChanged
		{
			void add(CurrencyManagerChangedEventHandler^ p) { m_eventCurrencyManagerChanged += p; }
			void remove(CurrencyManagerChangedEventHandler^ p) { m_eventCurrencyManagerChanged -= p; }
		private:
			void raise(object^ sender, CurrencyManagerChangedEventArgs^ e) { if(m_eventCurrencyManagerChanged != nullptr) m_eventCurrencyManagerChanged->Invoke(sender, e); }
		}


	protected: // methods
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		~GridControl();

		// events

		/// <summary>
		/// <see cref="ValueChanging"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="ValueChangingEventArgs"/>입니다.
		/// </param>
		virtual void OnValueChanging(ValueChangingEventArgs^ e);

		/// <summary>
		/// <see cref="ValueChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="CellEventArgs"/>입니다.
		/// </param>
		virtual void OnValueChanged(CellEventArgs^ e);
		
		/// <summary>
		/// <see cref="RowInserting"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="RowInsertingEventArgs"/>입니다.
		/// </param>
		virtual void OnRowInserting(RowInsertingEventArgs^ e);

		/// <summary>
		/// <see cref="RowInserted"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="RowEventArgs"/>입니다.
		/// </param>
		virtual void OnRowInserted(RowEventArgs^ e);

		/// <summary>
		/// <see cref="RowRemoving"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="RowRemovingEventArgs"/>입니다.
		/// </param>
		virtual void OnRowRemoving(RowRemovingEventArgs^ e);

		/// <summary>
		/// <see cref="RowRemoved"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="RowRemovedEventArgs"/>입니다.
		/// </param>
		virtual void OnRowRemoved(RowRemovedEventArgs^ e);

		/// <summary>
		/// <see cref="InsertionRowInserting"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="InsertionRowInsertingEventArgs"/>입니다.
		/// </param>
		virtual void OnInsertionRowInserting(InsertionRowInsertingEventArgs^ e);

		/// <summary>
		/// <see cref="InsertionRowInserted"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="RowEventArgs"/>입니다.
		/// </param>
		virtual void OnInsertionRowInserted(RowEventArgs^ e);

		/// <summary>
		/// <see cref="SelectedColumnsChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnSelectedColumnsChanged(_EventArgs^ e);

		/// <summary>
		/// <see cref="SelectedRowsChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnSelectedRowsChanged(_EventArgs^ e);

		/// <summary>
		/// <see cref="SelectionChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnSelectionChanged(_EventArgs^ e);

		/// <summary>
		/// <see cref="FocusedColumnChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnFocusedColumnChanged(_EventArgs^ e);

		/// <summary>
		/// <see cref="FocusedRowChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnFocusedRowChanged(_EventArgs^ e);

		/// <summary>
		/// <see cref="FocusedCellChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="CellEventArgs"/>입니다.
		/// </param>
		virtual void OnFocusedCellChanged(CellEventArgs^ e);
				
		/// <summary>
		/// <see cref="ColumnInserting"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="ColumnInsertingEventArgs"/>입니다.
		/// </param>
		virtual void OnColumnInserting(ColumnInsertingEventArgs^ e);

		/// <summary>
		/// <see cref="ColumnInserted"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="ColumnEventArgs"/>입니다.
		/// </param>
		virtual void OnColumnInserted(ColumnEventArgs^ e);

		/// <summary>
		/// <see cref="ColumnBinding"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="ColumnBindingEventArgs"/>입니다.
		/// </param>
		virtual void OnColumnBinding(ColumnBindingEventArgs^ e);

		/// <summary>
		/// <see cref="ColumnBinded"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="ColumnEventArgs"/>입니다.
		/// </param>
		virtual void OnColumnBinded(ColumnEventArgs^ e);

		/// <summary>
		/// <see cref="ColumnWidthChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="ColumnEventArgs"/>입니다.
		/// </param>
		virtual void OnColumnWidthChanged(ColumnEventArgs^ e);

		/// <summary>
		/// <see cref="ColumnMouseEnter"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="ColumnMouseEventArgs"/>입니다.
		/// </param>
		virtual void OnColumnMouseEnter(ColumnMouseEventArgs^ e);

		/// <summary>
		/// <see cref="ColumnMouseLeave"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="ColumnMouseEventArgs"/>입니다.
		/// </param>
		virtual void OnColumnMouseLeave(ColumnMouseEventArgs^ e);

		/// <summary>
		/// <see cref="ColumnMouseDown"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="ColumnMouseEventArgs"/>입니다.
		/// </param>
		virtual void OnColumnMouseDown(ColumnMouseEventArgs^ e);

		/// <summary>
		/// <see cref="ColumnMouseUp"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="ColumnMouseEventArgs"/>입니다.
		/// </param>
		virtual void OnColumnMouseUp(ColumnMouseEventArgs^ e);

		/// <summary>
		/// <see cref="ColumnMouseMove"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="ColumnMouseEventArgs"/>입니다.
		/// </param>
		virtual void OnColumnMouseMove(ColumnMouseEventArgs^ e);

		/// <summary>
		/// <see cref="CellDoubleClicked"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="CellEventArgs"/>입니다.
		/// </param>
		virtual void OnCellDoubleClicked(CellEventArgs^ e);

		/// <summary>
		/// <see cref="BeginEdit"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="BeginEditEventArgs"/>입니다.
		/// </param>
		virtual void OnBeginEdit(BeginEditEventArgs^ e);

		/// <summary>
		/// <see cref="EndEdit"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="CellEventArgs"/>입니다.
		/// </param>
		virtual void OnEndEdit(CellEventArgs^ e);

		/// <summary>
		/// <see cref="Clearing"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnClearing(_EventArgs^ e);

		/// <summary>
		/// <see cref="Cleared"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnCleared(_EventArgs^ e);

		/// <summary>
		/// <see cref="DataSourceChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnDataSourceChanged(_EventArgs^ e);

		/// <summary>
		/// <see cref="DataMemberChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnDataMemberChanged(_EventArgs^ e);

		/// <summary>
		/// <see cref="DataBindingComplete"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnDataBindingComplete(_EventArgs^ e);

		// overrides
		/// <summary>
		/// <see cref="Paint"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::Windows::Forms::PaintEventArgs"/>입니다.
		/// </param>
		virtual void OnPaint(System::Windows::Forms::PaintEventArgs^ e) override;

		/// <summary>
		/// <see cref="VisibleChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnVisibleChanged(_EventArgs^ e) override;

		/// <summary>
		/// <see cref="SizeChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnSizeChanged(_EventArgs^ e) override;

		/// <summary>
		/// <see cref="ClientSizeChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnClientSizeChanged(_EventArgs^ e) override;

		/// <summary>
		/// <see cref="MouseLeave"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnMouseLeave(_EventArgs^ e) override;

		/// <summary>
		/// <see cref="PreviewKeyDown"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::Windows::Forms::PreviewKeyDownEventArgs"/>입니다.
		/// </param>
		virtual void OnPreviewKeyDown(System::Windows::Forms::PreviewKeyDownEventArgs^ e) override;

		/// <summary>
		/// <see cref="KeyDown"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::Windows::Forms::KeyEventArgs"/>입니다.
		/// </param>
		virtual void OnKeyDown(System::Windows::Forms::KeyEventArgs^ e) override;

		/// <summary>
		/// <see cref="KeyPress"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::Windows::Forms::KeyPressEventArgs"/>입니다.
		/// </param>
		virtual void OnKeyPress(System::Windows::Forms::KeyPressEventArgs^ e) override;

		/// <summary>
		/// <see cref="FontChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnFontChanged(_EventArgs^ e) override;

		/// <summary>
		/// <see cref="Leave"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnLeave(_EventArgs^ e) override;

		/// <summary>
		/// <see cref="PaddingChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnPaddingChanged(_EventArgs^ e) override;

		/// <summary>
		/// <see cref="Paint"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::Windows::Forms::PaintEventArgs"/>입니다.
		/// </param>
		virtual void OnPrint(System::Windows::Forms::PaintEventArgs^ e) override;

		/// <summary>
		/// <see cref="LostFocus"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnLostFocus(_EventArgs^ e) override;

		/// <summary>
		/// <see cref="GotFocus"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnGotFocus(_EventArgs^ e) override;

		/// <summary>
		/// <see cref="Layout"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::Windows::Forms::LayoutEventArgs"/>입니다.
		/// </param>
		virtual void OnLayout(System::Windows::Forms::LayoutEventArgs^ e) override;

		///// <summary>
		///// <see cref="Invalidated"/> 이벤트를 발생시킵니다.
		///// </summary>
		///// <param name="e">
		///// 이벤트 데이터가 들어 있는 <see cref="System::Windows::Forms::InvalidateEventArgs"/>입니다.
		///// </param>
		//virtual void OnInvalidated(System::Windows::Forms::InvalidateEventArgs^ e) override;

		/// <summary>
		/// Windows 메시지를 처리합니다.
		/// </summary>
		/// <param name="m">
		/// 처리할 Windows <see cref="System::Windows::Forms::Message"/>입니다.
		/// </param>
		virtual void WndProc(System::Windows::Forms::Message% m) override;

		/// <summary>
		/// 지정된 키가 일반 입력 키인지 또는 전처리를 필요로 하는 특수 키인지 여부를 확인합니다.
		/// </summary>
		/// <param name="keyData">
		/// <see cref="System::Windows::Forms::Keys"/> 값 중 하나입니다.
		/// </param>
		/// <returns>
		/// 지정된 키가 일반 입력 키이면 true이고, 그렇지 않으면 false입니다.
		/// </returns>
		virtual bool IsInputKey(System::Windows::Forms::Keys keyData) override;

		/// <summary>
		/// 사용 가능한 다음 컨트롤을 선택하여 활성 컨트롤로 만듭니다.
		/// </summary>
		/// <param name="forward">
		/// <see cref="System::Windows::Forms::ContainerControl"/>의 컨트롤 전체를 순환하려면 true로 설정하고, 그렇지 않으면 false로 설정합니다.
		/// </param>
		/// <returns>
		/// 컨트롤이 선택되면 true이고, 그렇지 않으면 false입니다.
		/// </returns>
		virtual bool ProcessTabKey(bool forward) override;
		
	private: // methods
		void SetNativeEvent(bool attached);

		void OnCurrencyManagerChanged(CurrencyManagerChangedEventArgs^ e);

		void UpdateDataRectangle();
		void PaintRowState(_Graphics^ g);
		void PaintColumnControls(_Graphics^ graphics, _Rectangle clipRectangle);
		void SetDataConnection(object^ dataSource, string^ dataMember);

		bool ShouldSerializeColumns();
		bool ShouldSerializeRows();
		
		void ResetColumns();
		void ResetRows();

	private: // properties

	private: // variables
		//static	int						s_ref = 0;

		object^							m_dataSource;
		string^							m_dataMember;

		initonly ColumnCollection^				m_columnList;
		initonly VisibleColumnCollection^		m_visibleColumnList;
		initonly DisplayableColumnCollection^	m_displayableColumnList;
		initonly FrozenColumnCollection^		m_frozenColumnList;
		initonly UnfrozenColumnCollection^		m_unfrozenColumnList;
		initonly RowCollection^					m_rowList;
		initonly VisibleRowCollection^			m_visibleRowList;
		initonly DisplayableRowCollection^		m_displayableRowList;

		Design::Style^					m_style;

		_CaptionRow^					m_captionRow;
		_GroupingRow^					m_groupingRow;
		_InsertionRow^					m_insertionRow;
		_Cell^							m_focusedCell;

		initonly HScrollProperty^		m_hScrollProperty;
		initonly VScrollProperty^		m_vScrollProperty;

		initonly SelectedRowCollection^		m_selectedRows;
		initonly SelectedColumnCollection^	m_selectedColumns;

		GrGridCore*						m_pGridCore;
		GrColumnList*					m_pColumnList;
		GrDataRowList*					m_pDataRowList;
		GrDataRow*						m_pInsertionRow;
		GrGridRenderer*					m_pGridRenderer;
		class NativeEvent*				m_pEvent;

		Private::ErrorDescriptor^		m_errorDescriptor;
		IToolTip^						m_tooltips;
		_CellIterator^					m_cellIterator;

		GridState::StateManager^		m_states;

		_SelectionType					m_selectionType;
		_SelectionRange					m_selectionRange;
		_Rectangle						m_dataRectangle;

		bool							m_enableColumnMoving;
		bool							m_enableColumnResizing;
		bool							m_enableColumnFrozing;
		bool							m_enableSorting;
		bool							m_readOnly;
		bool							m_enableRowResizing;
		bool							m_fullRowSelect;
		bool							m_hideSelection;
		bool							m_multiSelect;

		// events
		ValueChangingEventHandler^		m_eventValueChanging;
		CellEventHandler^				m_eventValueChanged;
		RowInsertingEventHandler^		m_eventRowInserting;
		RowEventHandler^				m_eventRowInserted;
		RowRemovingEventHandler^		m_eventRowRemoving;
		RowRemovedEventHandler^			m_eventRowRemoved;
		InsertionRowInsertingEventHandler^			m_eventInsertionRowInserting;
		RowEventHandler^				m_eventInsertionRowInserted;
		_EventHandler^					m_eventSelectedColumnsChanged;
		_EventHandler^					m_eventSelectedRowsChanged;
		_EventHandler^					m_eventSelectionChanged;
		_EventHandler^					m_eventFocusedColumnChanged;
		_EventHandler^					m_eventFocusedRowChanged;
		CellEventHandler^				m_eventFocusedCellChanged;
		ColumnInsertingEventHandler^	m_eventColumnInserting;
		ColumnEventHandler^				m_eventColumnInserted;
		ColumnBindingEventHandler^		m_eventColumnBinding;
		ColumnEventHandler^				m_eventColumnBinded;
		ColumnEventHandler^				m_eventColumnWidthChanged;
		ColumnMouseEventHandler^		m_eventColumnMouseEnter;
		ColumnMouseEventHandler^		m_eventColumnMouseLeave;
		ColumnMouseEventHandler^		m_eventColumnMouseDown;
		ColumnMouseEventHandler^		m_eventColumnMouseUp;
		ColumnMouseEventHandler^		m_eventColumnMouseMove;
		CellEventHandler^				m_eventCellDoubleClicked;
		BeginEditEventHandler^			m_eventBeginEdit;
		CellEventHandler^				m_eventEndEdit;
		_EventHandler^					m_eventClearing;
		_EventHandler^					m_eventCleared;
		_EventHandler^					m_eventDataSourceChanged;
		_EventHandler^					m_eventDataMemberChanged;
		_EventHandler^					m_eventDataBindingComplete;

		CurrencyManagerChangedEventHandler^	m_eventCurrencyManagerChanged;

		_EventHandler^					m_eventStateBegin;
		_EventHandler^					m_eventStateEnd;
		
		_CurrencyManager^				m_manager;
		_CurrencyManager^				m_defaultManager;
		System::Data::DataTable^		m_defaultDataSource;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/