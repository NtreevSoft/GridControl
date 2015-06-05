using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    [ToolboxItem(true)]
    [DefaultEvent("")]
    [ToolboxBitmap(typeof(GridControl))]
    [Designer("Ntreev.Windows.Forms.Grid.Design.GridControlDesigner, Ntreev.Windows.Forms.Grid.Design, Version=2.0.4510.20986, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca")]
    [Docking(DockingBehavior.Ask)]
    public class GridControl : UserControl
    {
		object m_dataSource;
        string m_dataMember;
        _ColumnBindingCreation m_columnBindingCreation;

        readonly ColumnCollection m_columnList;
        readonly VisibleColumnCollection m_visibleColumnList;
        readonly DisplayableColumnCollection m_displayableColumnList;
        readonly FrozenColumnCollection m_frozenColumnList;
        readonly UnfrozenColumnCollection m_unfrozenColumnList;
        readonly RowCollection m_rowList;
        readonly VisibleRowCollection m_visibleRowList;
        readonly DisplayableRowCollection m_displayableRowList;
        readonly SelectedRowCollection m_selectedRows;
        readonly SelectedColumnCollection m_selectedColumns;
        readonly GroupRowCollection m_groupRows;

        Style m_style;

        Cell m_focusedCell;
        Cell m_editingCell;
        _CaptionRow m_captionRow;
        GroupPanel m_groupPanel;

        Native.WinFormGridCore m_pGridCore;
        Native.WinFormWindow m_pGridWindow;
        Native.GrGridRow m_pGridRow;
        GrColumnList m_pColumnList;
        GrDataRowList m_pDataRowList;
        GrGridPainter m_pGridPainter;

        _ErrorDescriptor m_errorDescriptor;
        ToolTip m_tooltips;
        System.DateTime m_buttonDownTime;

        Column m_oldFocusedColumn;
        RowBase m_oldFocusedRow;

        Color m_backgroundColor;
        Color m_paddingColor;
		Color m_lineColor;
		bool m_paintBackground;
        bool m_disposing;
        bool m_isEditing;

        RowBuilder m_rowBuilder;

        

        CurrencyManager m_manager;
        CurrencyManager m_defaultManager;
        System.Data.DataTable m_defaultDataSource;
        int m_dataBindingRef;

        Ntreev.Windows.Forms.Grid.MessageBoxCallback m_messageBoxCallback;
        Ntreev.Windows.Forms.Grid.MessageBoxCallback m_defaultMessageBoxCallback;

    public GridControl();

        public void EditCell(Cell cell, EditingReason editBy);

        public void CloseEdit();

        public void BringIntoView(Cell cell);

        public void BringIntoView(Row row);

        public void BringIntoView(Column column);

        public void DisplayFirst(RowBase rowBase);

        public void DisplayFirst(Column column);

        public void Clear();

        public void SelectAll();

		public void Select(IEnumerable<Row> rows);

		public void Select(IEnumerable<Cell> cells);

        public void ClearSelection();

		public void ClearSelection(bool keepFocus);

		public void Update(bool forceUpdate);

        public CellBase GetAt(Point pt);

        public Cell GetCellAt(Point pt);

        public void ShowAbout();

        public Cell GetFirstVisibleCell();

		public override void ResetBackColor() ;

		public override void ResetForeColor();

		public void ResetBackgroundColor();

        public void ResetPaddingColor();

		public void ResetLineColor();

		public void ClearInsertion();

        public DialogResult ShowMessage(string text);

        public DialogResult ShowMessage(string text, MessageBoxButtons buttons);

		public DialogResult ShowMessage(string text, MessageBoxButtons buttons, MessageBoxIcon icon);

        public DialogResult ShowMessage(string text, string caption, MessageBoxButtons buttons);

        public DialogResult ShowMessage(string text, string caption, MessageBoxButtons buttons, MessageBoxIcon icon);

		public bool ShowQuestion(string message);

        public void ShowError(string message);

        [Category("Appearance")]
        public string Caption
        {
            get;
            set;
        }

        [Category("Data")]
        [DefaultValue(ColumnBindingCreation.Create)]
        public ColumnBindingCreation ColumnBindingCreation
        {
            get;
            set;
        }

        [Category("Data")]
        [RefreshProperties(RefreshProperties.Repaint)]
        //[TypeConverter("System.Windows.Forms.Design.DataSourceConverter, System.Design, Version=1.0.3300.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a")]
        [AttributeProvider(typeof(IListSource))]
        [DefaultValue((string)null)]
        public object DataSource
        {
            get;
            set;
        }

        [Category("Data")]
        [DefaultValue("")]
        [Editor("System.Windows.Forms.Design.DataMemberListEditor, System.Design, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a", Design.UITypeEditor.typeid)]
        public string DataMember
        {
            get;
            set;
        }

        [Description("열의 너비를 자동적으로 조절할지에 대한 여부를 가져오거나 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(false)]
        [SettingsBindable(true)]
        public bool AutoFitColumn 
        {
            get;
            set;
        }

        [Description("행의 높이를 자동적으로 조절할지에 대한 여부를 가져오거나 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(false)]
        public bool AutoFitRow
        {
            get;
            set;
        }

        [Description("제목을 설정할 수 있는 인터페이스를 제공합니다.")]
        [Category("Behavior")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
        public CaptionRow CaptionRow
        {
            get;
        }

        [Description("그룹핑 표시에 대한 인터페이스를 제공합니다.")]
        [Category("Behavior")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
        public GroupPanel GroupPanel
        {
            get;
        }

        [Description("열의 목록을 가져옵니다.")]
        [Category("Behavior")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
        public ColumnCollection Columns
        {
            get;
        }

#if DEBUG
        [Category("Debug")]
        [Description("표시되는 열의 목록을 가져옵니다.")]
#else
        [Browsable(false)]
#endif
        public VisibleColumnCollection VisibleColumns
        {
            get;
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public DisplayableColumnCollection DisplayableColumns
        {
            get;
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public FrozenColumnCollection FrozenColumns
        {
            get;
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public UnfrozenColumnCollection UnfrozenColumns
        {
            get;
        }

#if DEBUG
        [Category("Focus")]
        [Description("포커스로 설정된 열을 가져옵니다.")]
#else
        [Browsable(false)]
#endif
        public Column FocusedColumn
        {
            get;
        }

        /// <summary>
        /// 선택된 열들의 목록을 가져옵니다.
        /// </summary>
#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public SelectedColumnCollection SelectedColumns
        {
            get;
        }

        [Category("Behavior")]
        [Description("행의 목록을 가져옵니다.")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public RowCollection Rows
        {
            get;
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public GroupRowCollection GroupRows
        {
            get;
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public VisibleRowCollection VisibleRows
        {
            get;
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public DisplayableRowCollection DisplayableRows
        {
            get;
        }

        [Category("Behavior")]
        public Row InsertionRow
        {
            get;
        }

#if DEBUG
        [Category("Behavior")]
#else
        [Browsable(false)]
#endif
        public SelectedRowCollection SelectedRows
        {
            get;
        } 

#if DEBUG
        [Category("Focus")]
        [Description("포커스로 설정된 행을 가져옵니다.")]
#else
        [Browsable(false)]
#endif
        public RowBase FocusedRow
        {
            get;
        } 

#if DEBUG
        [Category("Focus")]
        [Description("포커스로 설정된 셀을 가져옵니다.")]
#else
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        [Browsable(false)]
#endif
        [DefaultValue((string)null)]
        public Cell FocusedCell
        {
            get;
            set;
        }

#if DEBUG
        [Description("편집중인 셀을 가져옵니다.")]
        [Category("Behavior")]
#else
        [Browsable(false)]
#endif
        public Cell EditingCell
        {
            get;
        }

        [Browsable(false)]
        public ToolTip ToolTip
        {
            get;
        }

        [Description("마우스를 이용하여 열의 위치 이동을 할 수 있는지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(true)]
        public bool IsColumnMovable
        {
            get;
            set;
        }

        [Description("마우스를 이용하여 열의 너비 조절을 할 수 있는지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(true)]
        public bool IsColumnResizable
        {
            get;
            set;
        }

        [Description("행의 스크롤 적용여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(true)]
        public bool IsColumnFreezable
        {
            get;
            set;
        }

        [Description("마우스 클릭을 사용하여 열의 내용을 정렬할 수 있는지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(true)]
        public bool IsColumnSortable
        {
            get;
            set;
        }

        [Description("읽기 전용인지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(false)]
        public bool ReadOnly
        {
            get;
            set;
        }

        [Description("마우스를 이용하여 행의 높이를 조절할 수 있는지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(true)]
        public bool IsRowResizable
        {
            get;
            set;
        }

        [Description("마우스를 이용하여 행의 위치를 이동할 수 있는지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(false)]
        public bool IsRowMovable
        {
            get;
            set;
        }

        [Description("그룹화 기능을 사용할지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(true)]
        public bool IsGroupable
        {
            get;
            set;
        }

#if DEBUG
        [Category("Debug")]
        [DefaultValue(false)]
#else
        [Browsable(false)]
#endif
        public bool IsGrouped
        {
            get;
        }

        [Description("행의 숫자 표시에 대한 여부를 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(true)]
        public bool IsRowNumberVisible
        {
            get;
            set;
        }

        [Description("행의 헤더 표시에 대한 여부를 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(true)]
        public bool IsRowHeaderVisible
        {
            get;
            set;
        }

        [Description("새로운 행을 추가하기위한 삽입행의 표시 여부를 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(true)]
        public bool IsInsertionRowVisible { get;

        /// <summary>
        /// 상단의 제목 표시 여부를 가져오거나 설정합니다.
        /// </summary>
        [Description("상단의 제목 표시 여부를 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(true)]
        public bool IsCaptionRowVisible
        {
            get;
            set;
        }

        [Description("상단의 그룹핑 표시 여부를 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(true)]
        public bool IsGroupPanelVisible
        {
            get;
            set;
        }

        [Description("컬럼 고정 분할면 표시 여부를 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(true)]
        public bool IsFrozingSplitterVisible
        {
            get;
            set;
        }

        [Description("우측 빈 여백을 채울지에 대한 여부를 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(false)]
        public bool FillBlank
        {
            get;
            set;
        }

        [Description("셀 선택시 같은 범위내에 있는 모든 셀들의 선택 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(false)]
        public bool FullRowSelect
        {
            get;
            set;
        }

        [Description("포커스된 행의 강조 표시 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(false)]
        public bool RowHighlight
        {
            get;
        }

        [Description("포커스된 행의 강조 표시 방법을 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(RowHighlightType.Fill)]
        public RowHighlightType RowHighlightType
        {
            get;
            set;
        }

        [Description("컨트롤이 포커스를 잃었을대 선택 영역을 표시하는지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(false)]
        public bool HideSelection 
        {
            get;
            set;
        }

        [Description("여러 개의 항목을 선택할 수 있는지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(true)]
        public bool MultiSelect
        {
            get;
            set;
        }

		[DefaultValue((string)null)]
        [Category("Appearance")]
		[TypeConverter(typeof(StyleConverter))]
        public Style Style
        {
            get;
            set;
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        /// <summary>
        /// 셀들이 표시되는 영역을 가져옵니다.
        /// </summary>
        /// <returns>
        /// 셀들의 표시영역을 나타내는 <see cref="Rectangle"/>입니다.
        /// </returns>
        public Rectangle DataRectangle
        {
            get;
        }

		[Browsable(false)]
		public BindingContext BindingContext
		{
			get;
            set;
		}

		public Rectangle DisplayRectangle
        {
			get;
        }

		public bool HScrollVisible
		{
			get;
		}

		public bool VScrollVisible
		{
			get;
		}

        [Browsable(false)]
        public bool CanEnableIme
        {
            virtual get;
        }

		[Description("구성요소의 배경색입니다.")]
        [Category("Appearance")]
        virtual public Color BackColor 
        {
            get;
            set;
        }

		[Description("그리드가 포함되지 않는 여분의 배경색입니다.")]
        [Category("Appearance")]
        public Color BackgroundColor 
        {
            get;
            set;
        }

        [Description("내부 간격의 색입니다.")]
        [Category("Appearance")]
        public Color PaddingColor 
        {
            get;
            set;
        }

		[Description("그리드의 선을 나타내는 색입니다.")]
        [Category("Appearance")]
        public Color LineColor
        {
            get;
            set;
        }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public MessageBoxCallback MessageBoxCallback
        {
            get;
            set;
        }

#if _TIME_TEST
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public bool Rendering;
#endif

        [Description("데이터가 리셋되었을때 발생합니다.")]
        [Category("Behavior")]
        public event EventHandler Reseted;

        [Description("셀의 값이 변경되기 전에 발생합니다.")]
        [Category("Behavior")]
        public event ValueChangingEventHandler ValueChanging;

        [Description("셀의 값이 변경된 후에 발생합니다.")]
        [Category("Behavior")]
        public event CellEventHandler ValueChanged;

        [Description("행이 삽입되기 전에 발생합니다.")]
        [Category("Row")]
        public event RowInsertingEventHandler RowInserting;

        [Description("행이 삽입된 후에 발생합니다.")]
        [Category("Row")]
        public event RowInsertedEventHandler RowInserted;

        [Description("열이 목록에서 제거되기 전에 발생합니다.")]
        [Category("Row")]
        public event RowRemovingEventHandler RowRemoving;

        [Description("행이 목록에서 제거된 후에 발생합니다.")]
        [Category("Row")]
        public event RowRemovedEventHandler RowRemoved;

        [Description("행의 내용이 변경된 경우에 발생합니다.")]
        [Category("Row")]
        public event RowEventHandler RowChanged;

        [Description("행이 데이터 소스와 연결 되기 전에 발생합니다.")]
        [Category("Row")]
        public event RowBindingEventHandler RowBinding;

        [Description("행이 데이터 소스와 연결 된 후에 발생합니다.")]
        [Category("Row")]
        public event RowEventHandler RowBinded;

        [Description("행의 위치가 변경되기 전에 발생합니다.")]
        [Category("Row")]
        public event RowMovingEventHandler RowMoving;

        [Description("행의 위치가 변경되었을때 발생합니다.")]
        [Category("Row")]
        public event RowEventHandler RowMoved;

        [Description("행이 데이터 소스와 연결이 해제 되기 전에 발생합니다.")]
        [Category("Row")]
        public event RowEventHandler RowUnbinding;

        [Description("행이 데이터 소스와 연결이 해제 된 후에 발생합니다.")]
        [Category("Row")]
        public event RowEventHandler RowUnbinded;

        [Description("열들의 선택 범위가 변경되었을때 발생합니다.")]
        [Category("Selection")]
        public event EventHandler SelectedColumnsChanged;

        [Description("행들의 선택 범위가 변경되었을때 발생합니다.")]
        [Category("Selection")]
        public event EventHandler SelectedRowsChanged;

        [Description("셀들의 선택 범위가 변경되었을때 발생합니다.")]
        [Category("Selection")]
        public event EventHandler SelectionChanged;

        [Description("셀의 포커스가 변경되었을대 발생합니다.")]
        [Category("Focus")]
        public event EventHandler FocusedColumnChanged;

        [Description("행의 포커스가 변경되었을대 발생합니다.")]
        [Category("Focus")]
        public event EventHandler FocusedRowChanged;

        [Description("셀의 포커스가 변경되었을대 발생합니다.")]
        [Category("Focus")]
        public event CellEventHandler FocusedCellChanged;

        [Description("열이 삽입되기 전에 발생합니다.")]
        [Category("Column")]
        public event ColumnInsertingEventHandler ColumnInserting;

        [Description("열이 삽입된 후에 발생합니다.")]
        [Category("Column")]
        public event ColumnEventHandler ColumnInserted;

        [Description("열이 데이터 소스와 연결되기 전에 발생합니다.")]
        [Category("Column")]
        public event ColumnBindingEventHandler ColumnBinding;

        [Description("열이 데이터 소스와 연결된 후에 발생합니다.")]
        [Category("Column")]
        public event ColumnEventHandler ColumnBinded;

        [Description("열의 너비가 변경되었을때 발생합니다.")]
        [Category("Column")]
        public event ColumnEventHandler ColumnWidthChanged;

        [Description("열의 Frozen속성이 변경되었을때 발생합니다.")]
        [Category("Column")]
        public event ColumnEventHandler ColumnFrozenChanged;

        [Description("마우스의 커서 위치가 열의 영역안으로 들어갔을때 발생합니다.")]
        [Category("Column")]
        public event ColumnMouseEventHandler ColumnMouseEnter;

        [Description("마우스의 커서 위치가 열의 영역밖으로 나갔을때 발생합니다.")]
        [Category("Column")]
        public event ColumnMouseEventHandler ColumnMouseLeave;

        [Description("열의 영역안에서 마우스의 좌측버튼이 눌러졌을때 발생합니다.")]
        [Category("Column")]
        public event ColumnMouseEventHandler ColumnMouseDown;

        [Description("열의 영역안에서 마우스의 좌측버튼이 띄워졌을때 발생합니다.")]
        [Category("Column")]
        public event ColumnMouseEventHandler ColumnMouseUp;

        [Description("마우스의 커서 위치가 열의 영역안에서 움직였을때 발생합니다.")]
        [Category("Column")]
        public event ColumnMouseEventHandler ColumnMouseMove;

        [Description("마우스의 커서 위치가 셀의 영역안으로 들어갔을때 발생합니다.")]
        [Category("Behavior")]
        public event CellEventHandler CellMouseEnter;

        [Description("셀의 영역안에서 마우스가 움직였을때 발생합니다.")]
        [Category("Behavior")]
        public event CellMouseEventHandler CellMouseMove;

        [Description("마우스의 커서 위치가 셀의 영역안에서 밖으로 나갔을때 발생합니다.")]
        [Category("Behavior")]
        public event CellEventHandler CellMouseLeave;

        [Description("셀의 영역안에서 마우스가 눌러졌을때 발생합니다.")]
        [Category("Behavior")]
        public event CellEventHandler CellClick;

        [Description("셀의 영역안에서 마우스가 두번 눌러졌을때 발생합니다. 셀이 읽기 전용일때만 동작합니다.")]
        [Category("Behavior")]
        public event CellEventHandler CellDoubleClick;

        [Description("셀의 편집이 시작됬을때 발생합니다.")]
        [Category("Behavior")]
        public event EditBegunEventHandler EditBegun;

        [Description("셀의 편집이 종료됬을때 발생합니다.")]
        [Category("Behavior")]
        public event CellEventHandler EditEnded;

        [Description("모든 데이터가 삭제되기 전에 발생합니다.")]
        [Category("Behavior")]
        public event ClearEventHandler Clearing;

        [Description("모든 데이터가 삭제된 후에 발생합니다.")]
        [Category("Behavior")]
        public event ClearEventHandler Cleared;

        [Description("데이터 소스가 변경되었을때 발생합니다.")]
        [Category("Behavior")]
        public event EventHandler DataSourceChanged;

        [Description("데이터 멤버가 변경되었을때 발생합니다.")]
        public event EventHandler DataMemberChanged;

        [Description("데이터 바인딩이 종료되었을때 발생합니다.")]
        public event EventHandler DataBindingComplete;

		[Category("PropertyChanged")]
        [Description("BackgroundColor 값이 변경되면 발생합니다.")]
        public event EventHandler BackgroundColorChanged;

		[Category("PropertyChanged")]
        [Description("PaddingColor 값이 변경되면 발생합니다.")]
        public event EventHandler PaddingColorChanged;

		[Category("PropertyChanged")]
        [Description("LineColor 값이 변경되면 발생합니다.")]
        public event EventHandler LineColorChanged;

        [Description("DisplayRectangle 값이 변경되면 발생합니다.")]
        public event EventHandler DisplayRectangleChanged;

        [Description("보여지는 행들의 높이값이 달라지면 발생합니다.")]
        public event EventHandler VisibleHeightChanged;

		public event ScrollEventHandler Scroll;


		internal void InvokeReset();
        internal bool InvokeValueChanging(Cell cell, object value, object oldValue);
        internal void InvokeValueChanged(Cell cell);
        internal bool InvokeRowInserting(Row row);
        internal void InvokeRowInserted(Row row);
        internal bool InvokeRowBinding(object component);
        internal void InvokeRowBinded(Row row);
        internal void InvokeRowUnbinding(Row row);
        internal void InvokeRowUnbinded(Row row);
		internal bool InvokeRowMoving(Row row, int index);
		internal void InvokeRowMoved(Row row);
        internal bool InvokeRowRemoving(Row row);
        internal void InvokeRowRemoved(RowRemovedEventArgs e);
        internal void InvokeRowChanged(Row row);
        internal bool InvokeColumnInserting(Column column);
        internal void InvokeColumnInserted(Column column);
        internal Column InvokeColumnBinding(PropertyDescriptor propertyDescriptor, Column existColumn);
        internal void InvokeColumnBinded(Column column);
        internal bool InvokeColumnMouseDown(Column column, Point clientLocation);
        internal bool InvokeColumnMouseUp(Column column, Point clientLocation);
        internal void InvokeColumnMouseEnter(Column column, Point clientLocation);
        internal bool InvokeColumnMouseMove(Column column, Point clientLocation);
        internal void InvokeColumnMouseLeave(Column column);
        internal void InvokeColumnWidthChanged(Column column);
        internal void InvokeColumnFrozenChanged(Column column);
		internal void InvokeColumnVisibleIndexChanged(Column column);
        internal bool InvokeEditBegun(Cell cell);
        internal void InvokeEditEnded(CellEventArgs e);
        internal void InvokeScroll(ScrollEventArgs e);
        internal void InvokeCellMouseEnter(Cell cell);
        internal bool InvokeCellMouseMove(Cell cell, Point clientLocation);
        internal void InvokeCellMouseLeave(Cell cell);
        internal void InvokeCellClick(Cell cell);
        internal void InvokeCellDoubleClick(Cell cell);
		internal void InvokeDisplayRectangleChanged();
		internal void InvokeVisibleHeightChanged();

        internal object GetInternalService(System.Type serviceType);

        internal void InvokeFocusChanging();
        internal void InvokeFocusChanged();
        internal void InvokeSelectedRowsChanged();
        internal void InvokeSelectedColumnsChanged();
        internal void InvokeSelectionChanged();

        internal GridControl InvokeNewChildGridControl(Native.GrGridRow pGridRow);

        internal bool DesignTimeHitTest(Point globalLocation);
        internal void PostPaint(Graphics graphics, Rectangle clipRectangle);
		internal Control GetChildAt(Control control, Point location);
        internal Row CreateRow(GrDataRow pDataRow);
		internal void EndCurrentEdit(Row row);
		internal void DeleteFocusedCell();

        internal WinFormGridCore GridCore
        {
            get;
        }

        internal GrGridPainter GridPainter
        {
            get;
        }

        internal GrItemSelector Selector
        {
            get;
        }

        internal GrFocuser Focuser
        {
            get;
        }

        internal ErrorDescriptor ErrorDescriptor
        {
            get;
        }

		internal bool HScrollInternal
		{
			virtual get;
		}

		internal bool VScrollInternal
		{
			virtual get;
		}


        internal event CurrencyManagerChangingEventHandler CurrencyManagerChanging;

        internal event CurrencyManagerChangedEventHandler CurrencyManagerChanged;

        internal event EventHandler EditClosed;


        protected virtual void OnReseted(System.EventArgs e);

        protected virtual void OnValueChanging(ValueChangingEventArgs e);

        protected virtual void OnValueChanged(CellEventArgs e);

        protected virtual void OnRowInserting(RowInsertingEventArgs e);

        protected virtual void OnRowInserted(RowInsertedEventArgs e);

        protected virtual void OnRowRemoving(RowRemovingEventArgs e);

        protected virtual void OnRowRemoved(RowRemovedEventArgs e);

        protected virtual void OnRowChanged(RowEventArgs e);

        protected virtual void OnRowBinding(RowBindingEventArgs e);

        protected virtual void OnRowBinded(RowEventArgs e);

        protected virtual void OnRowUnbinding(RowEventArgs e);

        protected virtual void OnRowUnbinded(RowEventArgs e);

		protected virtual void OnRowMoving(RowMovingEventArgs e);

		protected virtual void OnRowMoved(RowEventArgs e);

        protected virtual void OnSelectedColumnsChanged(System.EventArgs e);

        protected virtual void OnSelectedRowsChanged(System.EventArgs e);

        protected virtual void OnSelectionChanged(System.EventArgs e);

        protected virtual void OnFocusedColumnChanged(System.EventArgs e);

        protected virtual void OnFocusedRowChanged(System.EventArgs e);

        protected virtual void OnFocusedCellChanged(CellEventArgs e);

        protected virtual void OnColumnInserting(ColumnInsertingEventArgs e);

        protected virtual void OnColumnInserted(ColumnEventArgs e);

        protected virtual void OnColumnBinding(ColumnBindingEventArgs e);

        protected virtual void OnColumnBinded(ColumnEventArgs e);

        protected virtual void OnColumnWidthChanged(ColumnEventArgs e);

        protected virtual void OnColumnFrozenChanged(ColumnEventArgs e);

        protected virtual void OnColumnMouseEnter(ColumnMouseEventArgs e);

        protected virtual void OnColumnMouseLeave(ColumnMouseEventArgs e);

        protected virtual void OnColumnMouseDown(ColumnMouseEventArgs e);

        protected virtual void OnColumnMouseUp(ColumnMouseEventArgs e);

        protected virtual void OnColumnMouseMove(ColumnMouseEventArgs e);

        protected virtual void OnCellMouseEnter(CellEventArgs e);

        protected virtual void OnCellMouseMove(CellMouseEventArgs e);

        protected virtual void OnCellMouseLeave(CellEventArgs e);

        protected virtual void OnCellClick(CellEventArgs e);

        protected virtual void OnCellDoubleClick(CellEventArgs e);

		protected virtual void OnDisplayRectangleChanged(System.EventArgs e);

		protected virtual void OnVisibleHeightChanged(System.EventArgs e);

        protected virtual void OnEditBegun(EditBegunEventArgs e);

        protected virtual void OnEditEnded(CellEventArgs e);

        protected virtual void OnClearing(ClearEventArgs e);

        protected virtual void OnCleared(ClearEventArgs e);

        protected virtual void OnDataSourceChanged(System.EventArgs e);

        protected virtual void OnDataMemberChanged(System.EventArgs e);

        protected virtual void OnDataBindingComplete(System.EventArgs e);

        protected override void OnPaint(PaintEventArgs e) ;

        protected override void OnPaintBackground(PaintEventArgs e) ;

        protected override void OnMouseLeave(System.EventArgs e) ;

        protected override void OnMouseMove(MouseEventArgs e) ;

        protected override void OnMouseDown(MouseEventArgs e) ;

        protected override void OnMouseUp(MouseEventArgs e) ;

        protected override void OnMouseClick(MouseEventArgs e) ;

        protected override void OnMouseDoubleClick(MouseEventArgs e) ;

        protected override void OnMouseWheel(MouseEventArgs e) ;

        protected override void OnPreviewKeyDown(PreviewKeyDownEventArgs e) ;

        protected override void OnKeyDown(KeyEventArgs e) ;

        protected override void OnFontChanged(System.EventArgs e) ;

        protected override void OnPrint(PaintEventArgs e) ;

        protected override void OnLostFocus(System.EventArgs e) ;

        protected override void OnGotFocus(System.EventArgs e) ;

        protected override void OnEnter(System.EventArgs e) ;

        protected override void OnLeave(System.EventArgs e) ;

        protected override void OnLayout(LayoutEventArgs e) ;

		protected override void OnForeColorChanged(System.EventArgs e) ;

		protected override void OnBackColorChanged(System.EventArgs e) ;

		protected virtual void OnBackgroundColorChanged(System.EventArgs e);

        protected virtual void OnPaddingColorChanged(System.EventArgs e);

		protected virtual void OnLineColorChanged(System.EventArgs e);

		protected new void OnScroll(ScrollEventArgs e) ;

        protected override void WndProc(Message m) ;

        protected override bool IsInputKey(Keys keyData) ;

        protected override bool ProcessCmdKey(Message msg, Keys keyData) ;

        protected override bool ProcessDialogKey(Keys keyData) ;

        protected override bool ProcessDialogChar(char charCode) ;

        protected override bool ProcessTabKey(bool forward) ;

        protected virtual Row NewRowFromBuilder(RowBuilder rowBuilder);

        protected virtual GridControl NewChildGridControl(PropertyDescriptor descriptor, Row row);


#if DEBUG
        protected override void OnInvalidated(InvalidateEventArgs e) ;
        protected override void OnCursorChanged(System.EventArgs e) ;
#endif

		protected bool HScroll
		{
			virtual get;
		}

		protected bool VScroll
		{
			virtual get;
		}

        private void OnCurrencyManagerChanging(CurrencyManagerChangingEventArgs e);
        private void OnCurrencyManagerChanged(CurrencyManagerChangedEventArgs e);
        private void OnEditClosed(System.EventArgs e);

        private void PaintRowState(Graphics g);
        private void PaintColumnControls(Graphics graphics, Rectangle clipRectangle);
		private void PaintCell(Graphics graphics, Rectangle clipRectangle, GrItem pItem);
        private void SetDataConnection(object dataSource, string dataMember);

        private bool ShouldSerializeColumns();
        private bool ShouldSerializeRows();
        private bool ShouldSerializeCaption();
        private bool ShouldSerializeBackgroundColor();
        private bool ShouldSerializePaddingColor();
		private bool ShouldSerializeLineColor();
		private bool ShouldSerializeBackColor();
		private bool ShouldSerializeForeColor();

        private void ResetColumns();
        private void ResetRows();

        private void ClearCore(bool dataSourceOnly);

        private void dataSource_Initialized(object sender, System.EventArgs e);
        private void currencyManager_ListChanged(object sender, ListChangedEventArgs e);
        private void currencyManager_BindingComplete(object sender, BindingCompleteEventArgs e);
		private void style_Disposed(object sender, System.EventArgs e);
        private void childGridControl_FocusedCellChanged(object sender, CellEventArgs e);

        private DialogResult DefaultShowMessage(string text, string caption, MessageBoxButtons buttons, MessageBoxIcon icon);

		private EventHandler m_styleDisposedEventHandler;

        class DataBindingRef : IDisposable
        {
            GridControl m_gridControl;
        public DataBindingRef(GridControl gridControl)
        {

        }
            
           
        
public void Dispose()
{
 	throw new NotImplementedException();
}
}

		class StyleConverter : ReferenceConverter
		{
		public StyleConverter() : base(typeof(Style)) {}
		}

        ListChangedEventHandler m_listChangedEventHandler;
        BindingCompleteEventHandler m_bindingCompleteEventHandler;
    }
}
