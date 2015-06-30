using System.ComponentModel;
using System.Windows.Forms;
using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using Ntreev.Windows.Forms.Grid.Properties;
using System.ComponentModel.Design;
using Ntreev.Windows.Forms.Grid.Natives;
using System.Drawing.Design;

namespace Ntreev.Windows.Forms.Grid
{
    [ToolboxItem(true)]
    [DefaultEvent("")]
    [ToolboxBitmap(typeof(GridControl))]
    [Designer("Design.GridControlDesigner, Design, Version=2.0.4510.20986, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca")]
    [Docking(DockingBehavior.Ask)]
    public class GridControl : UserControl
    {
        object m_dataSource;
        string m_dataMember;
        ColumnBindingCreation m_columnBindingCreation;

        readonly ColumnCollection m_columnList;
        //readonly VisibleColumnCollection m_visibleColumnList;
        //readonly DisplayableColumnCollection m_displayableColumnList;
        //readonly FrozenColumnCollection m_frozenColumnList;
        //readonly UnfrozenColumnCollection m_unfrozenColumnList;
        readonly RowCollection m_rowList;
        //readonly VisibleRowCollection m_visibleRowList;
        //readonly DisplayableRowCollection m_displayableRowList;
        //readonly SelectedRowCollection m_selectedRows;
        //readonly SelectedColumnCollection m_selectedColumns;
        readonly GroupRowCollection m_groupRows;

        Style m_style;

        Cell m_focusedCell;
        Cell m_editingCell;
        CaptionRow m_captionRow;
        GroupPanel m_groupPanel;

        WinFormGridCore m_pGridCore;
        WinFormWindow m_pGridWindow;
        GrGridRow m_pGridRow;
        GrColumnList m_pColumnList;
        GrDataRowList m_pDataRowList;
        GrGridPainter m_pGridPainter;

        ErrorDescriptor m_errorDescriptor;
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

        MessageBoxCallback m_messageBoxCallback;
        MessageBoxCallback m_defaultMessageBoxCallback;

        public GridControl()
        {
#if _TIME_TEST
		TimeTester timeTest("GridControl 생성자");
#endif
            this.SetStyle(ControlStyles.ContainerControl, true);
            m_pGridWindow = new WinFormWindow(this);
            m_pGridCore = new WinFormGridCore(this, m_pGridWindow);
            m_pGridCore.SetFont(Utility.FromManaged(this.Font));
            ResetBackColor();
            m_pGridCore.SetForeColor(this.ForeColor);
            m_pGridCore.SetBackColor(this.BackColor);
            m_pGridPainter = m_pGridWindow.GetGridPainter();

            m_pColumnList = m_pGridCore.ColumnList;
            m_pDataRowList = m_pGridCore.DataRowList;

            m_pGridCore.DisplayRectangle = this.DisplayRectangle;

#if _TIME_TEST
		Rendering = true;
#endif

            m_dataSource = null;
            m_dataMember = string.Empty;
            m_columnBindingCreation = ColumnBindingCreation.Create;

            m_tooltips = new ToolTip(this, 3);
            m_errorDescriptor = new ErrorDescriptor(this);

            m_columnList = new ColumnCollection(this);
            //m_visibleColumnList = new VisibleColumnCollection(this);
            //m_displayableColumnList = new DisplayableColumnCollection(this);
            m_backgroundColor = SystemColors.Control;
            m_paddingColor = SystemColors.ActiveCaption;
            m_lineColor = GrStyle.Default.LineColor;

            //m_unfrozenColumnList = new UnfrozenColumnCollection(this);
            m_rowList = new RowCollection(this);
            //m_visibleRowList = new VisibleRowCollection(this);
            //m_displayableRowList = new DisplayableRowCollection(this);

            //m_selectedColumns = new SelectedColumnCollection(this, this.Selector.GetSelectedColumns());
            //m_selectedRows = new SelectedRowCollection(this, this.Selector.GetSelectedRows());

            m_groupRows = new GroupRowCollection(this);

            m_captionRow = new CaptionRow(this, m_pGridCore.CaptionRow);
            m_groupPanel = new GroupPanel(this, m_pGridCore.GroupPanel);

            m_rowBuilder = new RowBuilder();

            m_defaultMessageBoxCallback = new MessageBoxCallback(DefaultShowMessage);

            //#if _MSC_VER >= 1600
            //#pragma warning(disable:4564)
            m_defaultDataSource = new System.Data.DataTable();
            //#pragma warning(default:4564)
            //#endif

            m_defaultManager = this.BindingContext[m_defaultDataSource] as CurrencyManager;
            m_listChangedEventHandler = new ListChangedEventHandler(currencyManager_ListChanged);
            m_bindingCompleteEventHandler = new BindingCompleteEventHandler(currencyManager_BindingComplete);
            m_styleDisposedEventHandler = new System.EventHandler(style_Disposed);

            this.DoubleBuffered = true;
            this.Name = "GridControl";

            OnCurrencyManagerChanging(new CurrencyManagerChangingEventArgs(m_defaultManager));
            OnCurrencyManagerChanged(new CurrencyManagerChangedEventArgs(m_defaultManager));
        }

        public void EditCell(Cell cell, EditingReason editBy)
        {
            if (cell.IsReadOnly == true)
                return;

            if (cell.IsFocused == false)
                cell.Focus();

            try
            {
                m_isEditing = true;
                bool focused = this.Focused;
                m_pGridCore.EditItem(cell.NativeRef, editBy.ToNative());
                if (focused == true)
                    this.Focus();
            }
            finally
            {
                m_isEditing = false;
            }
        }

        public void CloseEdit()
        {
            OnEditClosed(EventArgs.Empty);
        }

        public void BringIntoView(Cell cell)
        {
            m_pGridCore.BringIntoView(cell.NativeRef);
        }

        public void BringIntoView(Row row)
        {
            m_pDataRowList.BringIntoView(row.NativeRef);
        }

        public void BringIntoView(Column column)
        {
            m_pColumnList.BringIntoView(column.NativeRef);
        }

        public void DisplayFirst(RowBase rowBase)
        {
            m_pDataRowList.DisplayFirst(rowBase.NativeRef);
        }

        public void DisplayFirst(Column column)
        {
            m_pColumnList.DisplayFirst(column.NativeRef);
        }

        public void Clear()
        {
            m_disposing = true;
            try
            {
                OnCurrencyManagerChanging(new CurrencyManagerChangingEventArgs(m_defaultManager));
                ClearCore(false);

                m_dataSource = null;
                m_dataMember = string.Empty;
                m_manager = null;

                OnCurrencyManagerChanged(new CurrencyManagerChangedEventArgs(m_defaultManager));
            }
            finally
            {
                m_disposing = false;
            }
        }

        public void SelectAll()
        {
            if (m_pGridCore.GetMultiSelect() == false)
                throw new System.InvalidOperationException();

            this.Selector.SelectAll();
        }

        public void Select(IEnumerable<Row> rows)
        {
            if (m_pGridCore.GetMultiSelect() == false)
                throw new System.InvalidOperationException();

            HashSet<GrDataRow> dataRows = new HashSet<GrDataRow>();
            foreach (Row item in rows)
            {
                dataRows.Add(item.NativeRef);
            }

            this.Selector.SelectDataRows(dataRows, GrSelectionType.Normal);
        }

        public void Select(IEnumerable<Cell> cells)
        {
            if (m_pGridCore.GetMultiSelect() == false)
                throw new System.InvalidOperationException();

            HashSet<GrItem> items = new HashSet<GrItem>();
            foreach (Cell item in cells)
            {
                items.Add(item.NativeRef);
            }

            this.Selector.SelectItems(items, GrSelectionType.Normal);
        }

        public void ClearSelection()
        {
            this.ClearSelection(false);
        }

        public void ClearSelection(bool keepFocus)
        {
            this.Selector.ClearSelection();
            if (keepFocus == false)
                this.Focuser.Set(null as IFocusable);
        }

        public void Update(bool forceUpdate)
        {
            m_pGridCore.Update();
            if (forceUpdate == true)
                base.Update();
        }

        public CellBase GetAt(Point pt)
        {
            GrHitTest hitTest;
            if (m_pGridCore.HitTest(pt, out hitTest) == false)
                return null;

            object e = hitTest.pHitted.ManagedRef;
            return e as CellBase;
        }

        public Cell GetCellAt(Point pt)
        {
            GrHitTest hitTest;
            if (m_pGridCore.HitTest(pt, out hitTest) == false)
                return null;
            GrItem pItem = hitTest.pHitted as GrItem;
            if (pItem == null)
                return null;

            return pItem.ManagedRef as Cell;
        }

        public void ShowAbout()
        {
            AboutForm form = new AboutForm();
            form.ShowDialog();
        }

        public Cell GetFirstVisibleCell()
        {
            Row firstRow = null;
            if (this.InsertionRow.IsVisible == true)
            {
                firstRow = this.InsertionRow;
            }
            else if (this.Rows.Count > 0)
            {
                firstRow = this.Rows[0];
            }

            if (firstRow != null && firstRow.Cells.Count > 0)
            {
                return firstRow.Cells[0];
            }

            return null;
        }

        public override void ResetBackColor()
        {
            this.BackColor = GrStyle.Default.BackColor;
        }

        public override void ResetForeColor()
        {
            this.ForeColor = GrStyle.Default.ForeColor;
        }

        public void ResetBackgroundColor()
        {
            this.BackgroundColor = SystemColors.Control;
        }

        public void ResetPaddingColor()
        {
            this.PaddingColor = SystemColors.ActiveCaption;
        }

        public void ResetLineColor()
        {
            this.LineColor = GrStyle.Default.LineColor;
        }

        public void ClearInsertion()
        {
            this.InsertionRow.CancelEdit();
            foreach (Cell cell in this.InsertionRow.Cells)
            {
                cell.ClearLocalValue();
                cell.UpdateNativeText();
            }
        }

        public DialogResult ShowMessage(string text)
        {
            return this.ShowMessage(text, MessageBoxButtons.OK);
        }

        public DialogResult ShowMessage(string text, MessageBoxButtons buttons)
        {
            return this.ShowMessage(text, string.Empty, buttons);
        }

        public DialogResult ShowMessage(string text, MessageBoxButtons buttons, MessageBoxIcon icon)
        {
            return this.ShowMessage(text, string.Empty, buttons, icon);
        }

        public DialogResult ShowMessage(string text, string caption, MessageBoxButtons buttons)
        {
            return this.ShowMessage(text, caption, buttons, MessageBoxIcon.None);
        }

        public DialogResult ShowMessage(string text, string caption, MessageBoxButtons buttons, MessageBoxIcon icon)
        {
            return this.MessageBoxCallback(text, caption, buttons, icon);
        }

        public bool ShowQuestion(string message)
        {
            return this.ShowMessage(message, "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes;
        }

        public void ShowError(string message)
        {
            this.ShowMessage(message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        [Category("Appearance")]
        public string Caption
        {
            get { return m_captionRow.Text; }
            set { m_captionRow.Text = value; }
        }

        [Category("Data")]
        [DefaultValue(ColumnBindingCreation.Create)]
        public ColumnBindingCreation ColumnBindingCreation
        {
            get { return m_columnBindingCreation; }
            set
            {
                if (m_columnBindingCreation == value)
                    return;
                m_columnBindingCreation = value;
            }
        }

        [Category("Data")]
        [RefreshProperties(RefreshProperties.Repaint)]
        //[TypeConverter("Design.DataSourceConverter, System.Design, Version=1.0.3300.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a")]
        [AttributeProvider(typeof(IListSource))]
        [DefaultValue((string)null)]
        public object DataSource
        {
            get { return m_dataSource; }
            set
            {
                if (m_dataSource != value)
                {
                    if (value == null || m_dataSource != null)
                        m_dataMember = string.Empty;

                    SetDataConnection(value, m_dataMember);
                    if (m_dataSource == value)
                        OnDataSourceChanged(EventArgs.Empty);
                }
            }
        }

        [Category("Data")]
        [DefaultValue("")]
        [Editor("Design.DataMemberListEditor, System.Design, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a", typeof(UITypeEditor))]
        public string DataMember
        {
            get
            {
                if (m_dataSource == null)
                    return string.Empty;
                return m_dataMember;
            }
            set
            {
                if (m_dataMember != value)
                {
                    SetDataConnection(DataSource, value);
                    if (m_dataMember == value)
                        OnDataMemberChanged(EventArgs.Empty);
                }
            }
        }

        [Description("열의 너비를 자동적으로 조절할지에 대한 여부를 가져오거나 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(false)]
        [SettingsBindable(true)]
        public bool AutoFitColumn
        {
            get { return m_pGridCore.AutoFitColumn; }
            set { m_pGridCore.AutoFitColumn = value; }
        }

        [Description("행의 높이를 자동적으로 조절할지에 대한 여부를 가져오거나 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(false)]
        public bool AutoFitRow
        {
            get { return m_pGridCore.AutoFitRow; }
            set { m_pGridCore.AutoFitRow = value; }
        }

        [Description("제목을 설정할 수 있는 인터페이스를 제공합니다.")]
        [Category("Behavior")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
        public CaptionRow CaptionRow
        {
            get { return m_captionRow; }
        }

        [Description("그룹핑 표시에 대한 인터페이스를 제공합니다.")]
        [Category("Behavior")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
        public GroupPanel GroupPanel
        {
            get { return m_groupPanel; }
        }

        [Description("열의 목록을 가져옵니다.")]
        [Category("Behavior")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
        public ColumnCollection Columns
        {
            get { return m_columnList; }
        }

        //#if DEBUG
        //        [Category("Debug")]
        //        [Description("표시되는 열의 목록을 가져옵니다.")]
        //#else
        //        [Browsable(false)]
        //#endif
        //        public VisibleColumnCollection VisibleColumns
        //        {
        //            get;
        //        }

        //#if DEBUG
        //        [Category("Debug")]
        //#else
        //        [Browsable(false)]
        //#endif
        //        public DisplayableColumnCollection DisplayableColumns
        //        {
        //            get;
        //        }

        //#if DEBUG
        //        [Category("Debug")]
        //#else
        //        [Browsable(false)]
        //#endif
        //        public FrozenColumnCollection FrozenColumns
        //        {
        //            get;
        //        }

        //#if DEBUG
        //        [Category("Debug")]
        //#else
        //        [Browsable(false)]
        //#endif
        //        public UnfrozenColumnCollection UnfrozenColumns
        //        {
        //            get;
        //        }

#if DEBUG
        [Category("Focus")]
        [Description("포커스로 설정된 열을 가져옵니다.")]
#else
        [Browsable(false)]
#endif
        public Column FocusedColumn
        {
            get
            {
                Cell focusedCell = FocusedCell;
                if (focusedCell == null)
                    return null;
                return focusedCell.Column;
            }
        }

        //        /// <summary>
        //        /// 선택된 열들의 목록을 가져옵니다.
        //        /// </summary>
        //#if DEBUG
        //        [Category("Debug")]
        //#else
        //        [Browsable(false)]
        //#endif
        //        public SelectedColumnCollection SelectedColumns
        //        {
        //            get;
        //        }

        [Category("Behavior")]
        [Description("행의 목록을 가져옵니다.")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public RowCollection Rows
        {
            get { return m_rowList; }
        }

#if DEBUG
        [Category("Debug")]
#else
        [Browsable(false)]
#endif
        public GroupRowCollection GroupRows
        {
            get { return m_groupRows; }
        }

        //#if DEBUG
        //        [Category("Debug")]
        //#else
        //        [Browsable(false)]
        //#endif
        //        public VisibleRowCollection VisibleRows
        //        {
        //            get;
        //        }

        //#if DEBUG
        //        [Category("Debug")]
        //#else
        //        [Browsable(false)]
        //#endif
        //        public DisplayableRowCollection DisplayableRows
        //        {
        //            get;
        //        }

        [Category("Behavior")]
        public Row InsertionRow
        {
            get { return this.Rows.InsertionRow; }
        }

        //#if DEBUG
        //        [Category("Behavior")]
        //#else
        //        [Browsable(false)]
        //#endif
        //        public SelectedRowCollection SelectedRows
        //        {
        //            get;
        //        }

#if DEBUG
        [Category("Focus")]
        [Description("포커스로 설정된 행을 가져옵니다.")]
#else
        [Browsable(false)]
#endif
        public RowBase FocusedRow
        {
            get
            {
                IDataRow pDataRow = this.Focuser.GetFocusedRow();
                return FromNative.Get(pDataRow);
            }
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
            get { return m_focusedCell; }
            set
            {
                if (value != null)
                    this.Focuser.Set(value.NativeRef);
                else
                    this.Focuser.Set(null as IFocusable);
            }
        }

#if DEBUG
        [Description("편집중인 셀을 가져옵니다.")]
        [Category("Behavior")]
#else
        [Browsable(false)]
#endif
        public Cell EditingCell
        {
            get { return m_editingCell; }
        }

        [Browsable(false)]
        public ToolTip ToolTip
        {
            get { return m_tooltips; }
        }

        [Description("마우스를 이용하여 열의 위치 이동을 할 수 있는지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(true)]
        public bool IsColumnMovable
        {
            get { return m_pGridCore.GetColumnMovable(); }
            set { m_pGridCore.SetColumnMovable(value); }
        }

        [Description("마우스를 이용하여 열의 너비 조절을 할 수 있는지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(true)]
        public bool IsColumnResizable
        {
            get { return m_pGridCore.GetColumnResizable(); }
            set { m_pGridCore.SetColumnResizable(value); }
        }

        [Description("행의 스크롤 적용여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(true)]
        public bool IsColumnFreezable
        {
            get { return m_pGridCore.GetColumnFreezable(); }
            set { m_pGridCore.SetColumnFreezable(value); }
        }

        [Description("마우스 클릭을 사용하여 열의 내용을 정렬할 수 있는지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(true)]
        public bool IsColumnSortable
        {
            get { return m_pGridCore.GetColumnSortable(); }
            set { m_pGridCore.SetColumnSortable(value); }
        }

        [Description("읽기 전용인지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(false)]
        public bool ReadOnly
        {
            get
            {
                GridControl parent = this.Parent as GridControl;
                if (parent != null && parent.ReadOnly == true)
                    return true;
                return m_pGridCore.GetReadOnly();
            }
            set { m_pGridCore.SetReadOnly(value); }
        }

        [Description("마우스를 이용하여 행의 높이를 조절할 수 있는지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(true)]
        public bool IsRowResizable
        {
            get { return m_pGridCore.GetRowResizable(); }
            set { m_pGridCore.SetRowResizable(value); }
        }

        [Description("마우스를 이용하여 행의 위치를 이동할 수 있는지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(false)]
        public bool IsRowMovable
        {
            get { return m_pGridCore.GetRowMovable(); }
            set { m_pGridCore.SetRowMovable(value); }
        }

        [Description("그룹화 기능을 사용할지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(true)]
        public bool IsGroupable
        {
            get { return m_pGridCore.IsGroupable; }
            set { m_pGridCore.IsGroupable = value; }
        }

#if DEBUG
        [Category("Debug")]
        [DefaultValue(false)]
#else
        [Browsable(false)]
#endif
        public bool IsGrouped
        {
            get { return m_pGridCore.IsGrouped(); }
        }

        [Description("행의 숫자 표시에 대한 여부를 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(true)]
        public bool IsRowNumberVisible
        {
            get { return m_pDataRowList.GetRowNumberVisible(); }
            set { m_pDataRowList.SetRowNumberVisible(value); }
        }

        [Description("행의 헤더 표시에 대한 여부를 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(true)]
        public bool IsRowHeaderVisible
        {
            get { return m_pGridCore.GetRowVisible(); }
            set { m_pGridCore.SetRowVisible(value); }
        }

        [Description("새로운 행을 추가하기위한 삽입행의 표시 여부를 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(true)]
        public bool IsInsertionRowVisible
        {
            get { return this.InsertionRow.IsVisible; }
        }

        /// <summary>
        /// 상단의 제목 표시 여부를 가져오거나 설정합니다.
        /// </summary>
        [Description("상단의 제목 표시 여부를 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(true)]
        public bool IsCaptionRowVisible
        {
            get { return m_captionRow.IsVisible; }
            set
            {
                if (value == m_captionRow.IsVisible)
                    return;

                m_captionRow.IsVisible = value;
            }
        }

        [Description("상단의 그룹핑 표시 여부를 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(true)]
        public bool IsGroupPanelVisible
        {
            get { return m_groupPanel.IsVisible; }
            set
            {
                if (value == m_groupPanel.IsVisible)
                    return;

                m_groupPanel.IsVisible = value;
            }
        }

        [Description("컬럼 고정 분할면 표시 여부를 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(true)]
        public bool IsFrozingSplitterVisible
        {
            get { return m_pColumnList.GetColumnSplitter().IsVisible; }
            set { m_pColumnList.GetColumnSplitter().IsVisible = value; }
        }

        [Description("우측 빈 여백을 채울지에 대한 여부를 설정합니다.")]
        [Category("Appearance")]
        [DefaultValue(false)]
        public bool FillBlank
        {
            get { return m_pGridCore.GetFillBlank(); }
            set { m_pGridCore.SetFillBlank(value); }
        }

        [Description("셀 선택시 같은 범위내에 있는 모든 셀들의 선택 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(false)]
        public bool FullRowSelect
        {
            get { return m_pGridCore.GetFullRowSelect(); }
            set { m_pGridCore.SetFullRowSelect(value); }
        }

        [Description("포커스된 행의 강조 표시 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(false)]
        public bool RowHighlight
        {
            get { return m_pGridCore.GetRowHighlight(); }
            set { m_pGridCore.SetRowHighlight(value); }
        }

        [Description("포커스된 행의 강조 표시 방법을 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(RowHighlightType.Fill)]
        public RowHighlightType RowHighlightType
        {
            get { return (RowHighlightType)m_pGridCore.RowHighlightType;}
            set { m_pGridCore.RowHighlightType = (GrRowHighlightType)value; }
        }

        [Description("컨트롤이 포커스를 잃었을대 선택 영역을 표시하는지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(false)]
        public bool HideSelection
        {
            get { return m_pGridCore.GetHideSelection(); }
            set { m_pGridCore.SetHideSelection(value); }
        }

        [Description("여러 개의 항목을 선택할 수 있는지에 대한 여부를 설정합니다.")]
        [Category("Behavior")]
        [DefaultValue(true)]
        public bool MultiSelect
        {
            get { return m_pGridCore.GetMultiSelect(); }
            set { m_pGridCore.SetMultiSelect(value); }
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
            get { return m_pGridCore.GetDataRect(); }
        }

        [Browsable(false)]
        public override BindingContext BindingContext
        {
            get
            {
                BindingContext bindingContext = base.BindingContext;
                if (bindingContext == null)
                {
                    bindingContext = new BindingContext();
                    this.BindingContext = bindingContext;
                }

                return bindingContext;
            }
            set
            {
                base.BindingContext = value;
            }
        }

        public override Rectangle DisplayRectangle
        {
            get
            {
                Rectangle clientRectangle = base.ClientRectangle;
                clientRectangle.X += this.Padding.Left;
                clientRectangle.Y += this.Padding.Top;
                clientRectangle.Width -= this.Padding.Horizontal;
                clientRectangle.Height -= this.Padding.Vertical;
                return clientRectangle;
            }
        }

        public bool HScrollVisible
        {
            get { return m_pGridCore.HorzScroll.IsVisible; }
        }

        public bool VScrollVisible
        {
            get { return m_pGridCore.VertScroll.IsVisible; }
        }

        [Browsable(false)]
        protected override bool CanEnableIme
        {
            get { return true; }
        }

        [Description("구성요소의 배경색입니다.")]
        [Category("Appearance")]
        public override Color BackColor
        {
            get
            {
                if (m_paintBackground == true)
                    return m_backgroundColor;
                return base.BackColor;
            }
            set { base.BackColor = value; }
        }

        [Description("그리드가 포함되지 않는 여분의 배경색입니다.")]
        [Category("Appearance")]
        public Color BackgroundColor
        {
            get { return m_backgroundColor; }
            set
            {
                if (m_backgroundColor == value)
                    return;
                m_backgroundColor = value;
                OnBackgroundColorChanged(EventArgs.Empty);
            }
        }

        [Description("내부 간격의 색입니다.")]
        [Category("Appearance")]
        public Color PaddingColor
        {
            get { return m_paddingColor; }
            set
            {
                if (m_paddingColor == value)
                    return;
                m_paddingColor = value;
                OnPaddingColorChanged(EventArgs.Empty);
            }
        }

        [Description("그리드의 선을 나타내는 색입니다.")]
        [Category("Appearance")]
        public Color LineColor
        {
            get { return m_lineColor; }
            set
            {
                if (m_lineColor == value)
                    return;
                m_lineColor = value;
                OnLineColorChanged(EventArgs.Empty);
            }
        }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public MessageBoxCallback MessageBoxCallback
        {
            get
            {
                if (m_messageBoxCallback == null)
                    return m_defaultMessageBoxCallback;
                return m_messageBoxCallback;
            }
            set
            {
                m_messageBoxCallback = value;
            }
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

        public new event ScrollEventHandler Scroll;

        internal void InvokeReset()
        {
            OnReseted(EventArgs.Empty);
        }

        internal bool InvokeValueChanging(Cell cell, object value, object oldValue)
        {
            ValueChangingEventArgs e = new ValueChangingEventArgs(cell, value, oldValue);

            OnValueChanging(e);
            return e.Cancel != true;
        }

        internal void InvokeValueChanged(Cell cell)
        {
            //m_manager.EndCurrentEdit();
            OnValueChanged(new CellEventArgs(cell));
        }

        internal bool InvokeRowInserting(Row row)
        {
            RowInsertingEventArgs e = new RowInsertingEventArgs(row);

            OnRowInserting(e);
            return e.Cancel != true;
        }

        internal void InvokeRowInserted(Row row)
        {
            OnRowInserted(new RowInsertedEventArgs(row));
        }

        internal bool InvokeRowBinding(object component)
        {
            RowBindingEventArgs e = new RowBindingEventArgs(component);
            OnRowBinding(e);
            return e.Cancel != true;
        }

        internal void InvokeRowBinded(Row row)
        {
            RowEventArgs e = new RowEventArgs(row);
            OnRowBinded(e);
        }

        internal void InvokeRowUnbinding(Row row)
        {
            RowEventArgs e = new RowEventArgs(row);
            OnRowUnbinding(e);
        }

        internal void InvokeRowUnbinded(Row row)
        {
            RowEventArgs e = new RowEventArgs(row);
            OnRowUnbinded(e);
        }

        internal bool InvokeRowMoving(Row row, int index)
        {
            RowMovingEventArgs e = new RowMovingEventArgs(row, index);
            OnRowMoving(e);
            return e.Cancel != true;
        }

        internal void InvokeRowMoved(Row row)
        {
            m_pGridCore.Update();
            RowEventArgs e = new RowEventArgs(row);

            OnRowMoved(e);
        }

        internal bool InvokeRowRemoving(Row row)
        {
            RowRemovingEventArgs e = new RowRemovingEventArgs(row);

            OnRowRemoving(e);
            return e.Cancel != true;
        }

        internal void InvokeRowRemoved(RowRemovedEventArgs e)
        {
            OnRowRemoved(e);
        }

        internal void InvokeRowChanged(Row row)
        {
            OnRowChanged(new RowEventArgs(row));
        }

        internal bool InvokeColumnInserting(Column column)
        {
            ColumnInsertingEventArgs e = new ColumnInsertingEventArgs(column);

            OnColumnInserting(e);
            return e.Cancel == false;
        }

        internal void InvokeColumnInserted(Column column)
        {
            ColumnEventArgs e = new ColumnEventArgs(column);

            OnColumnInserted(e);
        }

        internal Column InvokeColumnBinding(PropertyDescriptor propertyDescriptor, Column existColumn)
        {
            ColumnBindingEventArgs e = new ColumnBindingEventArgs(propertyDescriptor, existColumn);

            OnColumnBinding(e);

            if (e.Cancel == true)
            {
                if (existColumn != null)
                    existColumn.IsVisible = false;
                return null;
            }

            if (e.BindingColumn == null && this.ColumnBindingCreation != ColumnBindingCreation.None)
            {
                e.BindingColumn = this.Columns.CreateColumnInstance();
            }

            return e.BindingColumn;
        }

        internal void InvokeColumnBinded(Column column)
        {
            ColumnEventArgs e = new ColumnEventArgs(column);

            OnColumnBinded(e);
        }

        internal bool InvokeColumnMouseDown(Column column, Point clientLocation)
        {
            ColumnMouseEventArgs ce = new ColumnMouseEventArgs(column, clientLocation);
            OnColumnMouseDown(ce);
            return ce.Handled == true;
        }

        internal bool InvokeColumnMouseUp(Column column, Point clientLocation)
        {
            ColumnMouseEventArgs ce = new ColumnMouseEventArgs(column, clientLocation);
            OnColumnMouseUp(ce);
            return ce.Handled == true;
        }

        internal void InvokeColumnMouseEnter(Column column, Point clientLocation)
        {
            ColumnMouseEventArgs ce = new ColumnMouseEventArgs(column, clientLocation);
            OnColumnMouseEnter(ce);
        }

        internal bool InvokeColumnMouseMove(Column column, Point clientLocation)
        {
            ColumnMouseEventArgs ce = new ColumnMouseEventArgs(column, clientLocation);
            OnColumnMouseMove(ce);

            return ce.Handled;
        }

        internal void InvokeColumnMouseLeave(Column column)
        {
            ColumnMouseEventArgs ce = new ColumnMouseEventArgs(column, Point.Empty);
            OnColumnMouseLeave(ce);
        }

        internal void InvokeColumnWidthChanged(Column column)
        {
            ColumnEventArgs e = new ColumnEventArgs(column);

            OnColumnWidthChanged(e);

            if (this.Site != null)
            {
                IComponentChangeService service = GetService(typeof(IComponentChangeService)) as IComponentChangeService;
                PropertyDescriptor propertyDescriptor = TypeDescriptor.GetProperties(column)["Width"];
                service.OnComponentChanging(column, propertyDescriptor);
                service.OnComponentChanged(column, propertyDescriptor, column.Width, column.Width);
            }
        }

        internal void InvokeColumnFrozenChanged(Column column)
        {
            ColumnEventArgs e = new ColumnEventArgs(column);

            OnColumnFrozenChanged(e);

            if (this.Site != null)
            {
                IComponentChangeService service = GetService(typeof(IComponentChangeService)) as IComponentChangeService;
                PropertyDescriptor propertyDescriptor = TypeDescriptor.GetProperties(column)["IsFrozen"];
                service.OnComponentChanging(column, propertyDescriptor);
                service.OnComponentChanged(column, propertyDescriptor, !column.IsFrozen, column.IsFrozen);
            }
        }

        internal void InvokeColumnVisibleIndexChanged(Column column)
        {
            //ColumnEventArgs e = new //ColumnEventArgs(column);

            //OnColumnFrozenChanged(e);

            if (this.Site != null)
            {
                IComponentChangeService service = GetService(typeof(IComponentChangeService)) as IComponentChangeService;
                PropertyDescriptor propertyDescriptor = TypeDescriptor.GetProperties(column)["VisibleIndex"];
                service.OnComponentChanging(column, propertyDescriptor);
                service.OnComponentChanged(column, propertyDescriptor, null, column.VisibleIndex);
            }
        }

        internal bool InvokeEditBegun(Cell cell)
        {
            EditBegunEventArgs e = new EditBegunEventArgs(cell);

            OnEditBegun(e);

            if (e.Cancel == true)
                return false;

            Row row = cell.Row;


            if (row.IsBeingEdited == false)
                row.BeginEdit();

            m_editingCell = cell;
            return true;
        }

        internal void InvokeEditEnded(CellEventArgs e)
        {
            m_editingCell = null;
            OnEditEnded(e);
        }

        internal void InvokeScroll(ScrollEventArgs e)
        {
            m_pGridCore.Invalidate();
            m_pGridCore.Update();
            OnScroll(e);
        }

        internal void InvokeCellMouseEnter(Cell cell)
        {
            CellEventArgs e = new CellEventArgs(cell);

            OnCellMouseEnter(e);
        }

        internal bool InvokeCellMouseMove(Cell cell, Point clientLocation)
        {
            CellMouseEventArgs e = new CellMouseEventArgs(cell, clientLocation);

            OnCellMouseMove(e);
            return e.Handled;
        }

        internal void InvokeCellMouseLeave(Cell cell)
        {
            CellEventArgs e = new CellEventArgs(cell);

            OnCellMouseLeave(e);
        }

        internal void InvokeCellClick(Cell cell)
        {
            OnCellClick(new CellEventArgs(cell));
        }

        internal void InvokeCellDoubleClick(Cell cell)
        {
            OnCellDoubleClick(new CellEventArgs(cell));
        }

        internal void InvokeDisplayRectangleChanged()
        {
            OnDisplayRectangleChanged(EventArgs.Empty);
        }

        internal void InvokeVisibleHeightChanged()
        {
            OnVisibleHeightChanged(EventArgs.Empty);
        }

        internal object GetInternalService(System.Type serviceType)
        {
            return base.GetService(serviceType);
        }

        internal void InvokeFocusChanging()
        {
            m_oldFocusedColumn = FocusedColumn;
            m_oldFocusedRow = FocusedRow;
        }

        internal void InvokeFocusChanged()
        {
            if (m_disposing == true)
                return;

            GrItem pFocusedItem = this.Focuser.GetItem();

            if (pFocusedItem == null)
            {
                m_focusedCell = null;
            }
            else
            {
                m_focusedCell = FromNative.Get(pFocusedItem);
            }

            if (this.FocusedColumn != m_oldFocusedColumn)
            {
                OnFocusedColumnChanged(EventArgs.Empty);
            }

            if (this.FocusedRow != m_oldFocusedRow)
            {
                OnFocusedRowChanged(EventArgs.Empty);
            }

            OnFocusedCellChanged(new CellEventArgs(m_focusedCell));
        }

        internal void InvokeSelectedRowsChanged()
        {
            OnSelectedRowsChanged(EventArgs.Empty);
        }

        internal void InvokeSelectedColumnsChanged()
        {
            OnSelectedColumnsChanged(EventArgs.Empty);
        }

        internal void InvokeSelectionChanged()
        {
            OnSelectionChanged(EventArgs.Empty);
        }

        internal GridControl InvokeNewChildGridControl(GrGridRow pGridRow)
        {

            PropertyDescriptor descriptor = pGridRow.GetPropertyDescriptor();
            Row row = pGridRow.GetParentRow();

            GridControl control = this.NewChildGridControl(descriptor, row);
            control.Visible = false;
            control.Name = descriptor.Name;
            this.Controls.Add(control);
            control.FocusedCellChanged += childGridControl_FocusedCellChanged;
            control.m_pGridRow = pGridRow;

            return control;
        }

        internal bool DesignTimeHitTest(Point globalLocation)
        {
            Point point = this.PointToClient(globalLocation);

            try
            {
                GrStateManager pStateManager = m_pGridCore.GetStateManager();

                switch (pStateManager.GetHitTest(point))
                {
                    case GrGridState.ColumnPressing:
                    case GrGridState.ColumnResizing:
                    case GrGridState.ColumnSplitterMoving:
                    case GrGridState.RowResizing:
                    case GrGridState.GroupPressing:
                        return true;
                    default:
                        break;

                }
            }
            catch (Exception)
            {
                return false;
            }

            if (this.DisplayRectangle.Contains(point) == false)
            {
                return true;
            }

            return false;
        }

        internal void PostPaint(Graphics graphics, Rectangle clipRectangle)
        {
            PaintColumnControls(graphics, clipRectangle);

            if (IsRowNumberVisible == false)
                PaintRowState(graphics);

            m_errorDescriptor.Paint(graphics);
        }

        internal Control GetChildAt(Control control, Point location)
        {
            Point pt = control.PointToClient(location);
            Control found = control.GetChildAtPoint(pt);
            if (found != null && found.HasChildren == true)
                return this.GetChildAt(found, location);
            if (found == null && control.Bounds.Contains(pt) == true)
                return control;
            return found;
        }

        internal Row CreateRow(GrDataRow pDataRow)
        {
            m_rowBuilder.GridControl = this;
            m_rowBuilder.NativeRef = pDataRow;
            return NewRowFromBuilder(m_rowBuilder);
        }

        internal void EndCurrentEdit(Row row)
        {
            try
            {
                m_manager.EndCurrentEdit();
                row.EndEditInternal();
            }
            catch (Exception e)
            {
                m_manager.CancelCurrentEdit();
                row.CancelEdit();
                this.ShowMessage(e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        internal void DeleteFocusedCell()
        {
            this.Focuser.Delete();
        }


        internal WinFormGridCore GridCore
        {
            get { return m_pGridCore; }
        }

        internal GrGridPainter GridPainter
        {
            get { return m_pGridPainter; }
        }

        internal GrItemSelector Selector
        {
            get { return m_pGridCore.ItemSelector; }
        }

        internal GrFocuser Focuser
        {
            get { return m_pGridCore.Focuser; }
        }

        internal ErrorDescriptor ErrorDescriptor
        {
            get { return m_errorDescriptor; }
        }

        internal bool HScrollInternal
        {
            get { return this.HScroll; }
        }

        internal bool VScrollInternal
        {
            get { return this.VScroll; }
        }


        internal event CurrencyManagerChangingEventHandler CurrencyManagerChanging;

        internal event CurrencyManagerChangedEventHandler CurrencyManagerChanged;

        internal event EventHandler EditClosed;


        protected virtual void OnReseted(EventArgs e)
        {
            if (this.Reseted != null)
            {
                this.Reseted(this, e);
            }
        }

        protected virtual void OnValueChanging(ValueChangingEventArgs e)
        {
            ValueChanging(this, e);
        }

        protected virtual void OnValueChanged(CellEventArgs e)
        {
            ValueChanged(this, e);

            Cell cell = e.Cell;
            if (cell.IsDisplayable == true)
                Invalidate(cell.Bounds);
        }

        protected virtual void OnRowInserting(RowInsertingEventArgs e)
        {
            if (this.RowInserting != null)
            {
                this.RowInserting(this, e);
            }
        }

        protected virtual void OnRowInserted(RowInsertedEventArgs e)
        {
            if (this.RowInserted != null)
            {
                this.RowInserted(this, e);
            }
        }

        protected virtual void OnRowRemoving(RowRemovingEventArgs e)
        {
            if (this.RowRemoving != null)
            {
                this.RowRemoving(this, e);
            }
        }

        protected virtual void OnRowRemoved(RowRemovedEventArgs e)
        {
            if (this.RowRemoved != null)
            {
                this.RowRemoved(this, e);
            }
        }

        protected virtual void OnRowChanged(RowEventArgs e)
        {
            if (this.RowChanged != null)
            {
                this.RowChanged(this, e);
            }
        }

        protected virtual void OnRowBinding(RowBindingEventArgs e)
        {
            if (this.RowBinding != null)
            {
                this.RowBinding(this, e);
            }
        }

        protected virtual void OnRowBinded(RowEventArgs e)
        {
            if (this.RowBinded != null)
            {
                this.RowBinded(this, e);
            }
        }

        protected virtual void OnRowUnbinding(RowEventArgs e)
        {
            RowUnbinding(this, e);
        }

        protected virtual void OnRowUnbinded(RowEventArgs e)
        {
            RowUnbinded(this, e);
        }

        protected virtual void OnRowMoving(RowMovingEventArgs e)
        {
            RowMoving(this, e);
        }

        protected virtual void OnRowMoved(RowEventArgs e)
        {
            RowMoved(this, e);
        }

        protected virtual void OnSelectedColumnsChanged(EventArgs e)
        {
            SelectedColumnsChanged(this, e);
        }

        protected virtual void OnSelectedRowsChanged(EventArgs e)
        {
            SelectedRowsChanged(this, e);
        }

        protected virtual void OnSelectionChanged(EventArgs e)
        {
            SelectionChanged(this, e);
        }

        protected virtual void OnFocusedColumnChanged(EventArgs e)
        {
            if (this.FocusedColumnChanged != null)
            {
                this.FocusedColumnChanged(this, e);
            }
        }

        protected virtual void OnFocusedRowChanged(EventArgs e)
        {
            Row row = m_oldFocusedRow as Row;

            if (row != null)
            {
                if (row.IsBeingEdited == true && row != this.InsertionRow)
                {
                    this.EndCurrentEdit(row);
                }
            }

            if (m_manager != null)
            {
                if (this.FocusedRow == this.InsertionRow)
                {
                    this.Rows.BeginInsertion();
                }
                else if (m_oldFocusedRow == this.InsertionRow)
                {
                    this.Rows.EndInsertion();
                }
            }

            if (this.FocusedRowChanged != null)
            {
                this.FocusedRowChanged(this, e);
            }
        }

        protected virtual void OnFocusedCellChanged(CellEventArgs e)
        {
            if (this.FocusedCellChanged != null)
            {
                this.FocusedCellChanged(this, e);
            }
        }

        protected virtual void OnColumnInserting(ColumnInsertingEventArgs e)
        {
            if (this.ColumnInserting != null)
            {
                this.ColumnInserting(this, e);
            }
        }

        protected virtual void OnColumnInserted(ColumnEventArgs e)
        {
            Column column = e.Column;
            this.InsertionRow.NewCell(column);
            foreach (Row item in m_rowList)
            {
                item.NewCell(column);
            }

            if (this.ColumnInserted != null)
            {
                this.ColumnInserted(this, e);
            }
        }

        protected virtual void OnColumnBinding(ColumnBindingEventArgs e)
        {
            if (this.ColumnBinding != null)
            {
                this.ColumnBinding(this, e);
            }
        }

        protected virtual void OnColumnBinded(ColumnEventArgs e)
        {
            Column column = e.Column;
            this.InsertionRow.NewCell(column);
            foreach (Row item in m_rowList)
            {
                item.NewCell(column);
            }

            if (this.ColumnBinded != null)
            {
                ColumnBinded(this, e);
            }
        }

        protected virtual void OnColumnWidthChanged(ColumnEventArgs e)
        {
            if (this.ColumnWidthChanged != null)
            {
                this.ColumnWidthChanged(this, e);
            }
        }

        protected virtual void OnColumnFrozenChanged(ColumnEventArgs e)
        {
            if (this.ColumnFrozenChanged != null)
            {
                this.ColumnFrozenChanged(this, e);
            }
        }

        protected virtual void OnColumnMouseEnter(ColumnMouseEventArgs e)
        {
            if (this.Site == null)
            {
                Point location = e.Column.Bounds.Location;
                location.Y -= 50;

                m_tooltips.Show(e.Column.Tooltip);
            }

            if (this.ColumnMouseEnter != null)
            {
                this.ColumnMouseEnter(this, e);
            }
        }

        protected virtual void OnColumnMouseLeave(ColumnMouseEventArgs e)
        {
            if (this.Site == null)
            {
                m_tooltips.Hide();
            }

            if (this.ColumnMouseLeave != null)
            {
                this.ColumnMouseLeave(this, e);
            }
        }

        protected virtual void OnColumnMouseDown(ColumnMouseEventArgs e)
        {
            if (this.ColumnMouseDown != null)
            {
                this.ColumnMouseDown(this, e);
            }
        }

        protected virtual void OnColumnMouseUp(ColumnMouseEventArgs e)
        {
            if (this.ColumnMouseUp != null)
            {
                this.ColumnMouseUp(this, e);
            }
        }

        protected virtual void OnColumnMouseMove(ColumnMouseEventArgs e)
        {
            if (this.ColumnMouseMove != null)
            {
                this.ColumnMouseMove(this, e);
            }
        }

        protected virtual void OnCellMouseEnter(CellEventArgs e)
        {
            try
            {
                Cell cell = e.Cell;
                string invalidValue = cell.InvalidValue;
                string error = cell.Error;
                string sourceError = cell.SourceError;
                if (invalidValue != string.Empty)
                {
                    this.ToolTip.Show(invalidValue);
                }
                else if (error != string.Empty)
                {
                    this.ToolTip.Show(error);
                }
                else if (sourceError != string.Empty)
                {
                    this.ToolTip.Show(sourceError);
                }
            }
            catch (Exception)
            {
            }

            if (this.CellMouseEnter != null)
            {
                CellMouseEnter(this, e);
            }
        }

        protected virtual void OnCellMouseMove(CellMouseEventArgs e)
        {
            if (this.CellMouseMove != null)
            {
                this.CellMouseMove(this, e);
            }
        }

        protected virtual void OnCellMouseLeave(CellEventArgs e)
        {
            this.ToolTip.Hide();
            if (this.CellMouseLeave != null)
            {
                this.CellMouseLeave(this, e);
            }
        }

        protected virtual void OnCellClick(CellEventArgs e)
        {
            if (this.CellClick != null)
            {
                this.CellClick(this, e);
            }
        }

        protected virtual void OnCellDoubleClick(CellEventArgs e)
        {
            if (this.CellDoubleClick != null)
            {
                this.CellDoubleClick(this, e);
            }
        }

        protected virtual void OnDisplayRectangleChanged(EventArgs e)
        {
            if (this.DisplayRectangleChanged != null)
            {
                this.DisplayRectangleChanged(this, e);
            }
        }

        protected virtual void OnVisibleHeightChanged(EventArgs e)
        {
            if (this.VisibleHeightChanged != null)
            {
                this.VisibleHeightChanged(this, e);
            }
        }

        protected virtual void OnEditBegun(EditBegunEventArgs e)
        {
            if (this.EditBegun != null)
            {
                this.EditBegun(this, e);
            }
        }

        protected virtual void OnEditEnded(CellEventArgs e)
        {
            if (this.EditEnded != null)
            {
                this.EditEnded(this, e);
            }
        }

        protected virtual void OnClearing(ClearEventArgs e)
        {
            if (this.Clearing != null)
            {
                this.Clearing(this, e);
            }
        }

        protected virtual void OnCleared(ClearEventArgs e)
        {
            if (this.Cleared != null)
            {
                this.Cleared(this, e);
            }
            Invalidate(false);
        }

        protected virtual void OnDataSourceChanged(EventArgs e)
        {
            if (this.DataSourceChanged != null)
            {
                this.DataSourceChanged(this, e);
            }
        }

        protected virtual void OnDataMemberChanged(EventArgs e)
        {
            DataMemberChanged(this, e);
        }

        protected virtual void OnDataBindingComplete(EventArgs e)
        {
            try
            {

                Column column = m_columnList.Where(item => item.IsVisible).FirstOrDefault();
                Row row = m_rowList.Where(item => item.IsVisible).FirstOrDefault();
                if (column != null && row != null)
                {
                    GrItem pItem = row.Cells[column].NativeRef;
                    this.Selector.SetSelectionGroup(pItem);
                    this.Selector.SelectItem(pItem, GrSelectionType.Normal);
                    this.Selector.SetAnchor(pItem);
                    this.Focuser.Set(pItem);
                }
            }
            catch (Exception)
            {

            }
            if (this.DataBindingComplete != null)
            {
                this.DataBindingComplete(this, e);
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            try
            {
                Console.WriteLine(DateTime.Now);
                GrRect clipping = e.ClipRectangle;

                int left = clipping.Left;
                int top = clipping.Top;
                int right = Math.Min(clipping.Right, this.DisplayRectangle.Right);
                int bottom = Math.Min(clipping.Bottom, this.DisplayRectangle.Bottom);


                System.IntPtr hdc = e.Graphics.GetHdc();
                try
                {
                    m_pGridWindow.OnSizeChanged(this.DisplayRectangle);
                    m_pGridWindow.OnPaint(hdc, clipping);
                }
                catch (Exception exception)
                {
                    throw exception;
                }
                finally
                {
                    e.Graphics.ReleaseHdc(hdc);
                }

                base.OnPaint(e);
            }
            catch (Exception exception)
            {
                e.Graphics.Clear(Color.White);

                StringFormat format = new StringFormat(StringFormat.GenericDefault);

                PointF location = (PointF)this.DisplayRectangle.Location;
                SizeF size = e.Graphics.MeasureString(exception.Message, this.Font, this.DisplayRectangle.Width, format);

                e.Graphics.DrawString(exception.Message, this.Font, Brushes.Black, new RectangleF(location, size), format);

                size = e.Graphics.MeasureString(exception.StackTrace, this.Font, this.DisplayRectangle.Width, format);
                location.Y += size.Height;
                e.Graphics.DrawString(exception.StackTrace, this.Font, Brushes.Black, new RectangleF(location, size), format);
            }
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {

            if (this.BackgroundImage != null)
            {
                try
                {
                    m_paintBackground = true;
                    base.OnPaintBackground(e);
                }
                finally
                {
                    m_paintBackground = false;
                }
            }
            else
            {
                SolidBrush br = new SolidBrush(m_backgroundColor);
                SolidBrush br2 = new SolidBrush(m_paddingColor);
                e.Graphics.FillRectangle(br2, e.ClipRectangle);
                e.Graphics.FillRectangle(br, this.DisplayRectangle);
            }
        }

        protected override void OnMouseLeave(EventArgs e)
        {
            base.OnMouseLeave(e);
            m_pGridWindow.OnMouseLeave();
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);
            m_pGridWindow.OnMouseMove(e.Location, e.Button == MouseButtons.Left);
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            //System.Diagnostics.Trace.WriteLine(string.Format("mouse down : {0}", System.DateTime.Now));
            m_buttonDownTime = System.DateTime.Now;
            base.OnMouseDown(e);
            if (e.Button == MouseButtons.Left)
                m_pGridWindow.OnMouseDown(e.Location);

            //if(m_pGridRow == null)
            //{
            //    this.FocusInternal();
            //}
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            //System.Diagnostics.Trace.WriteLine(string.Format("mouse up : {0}", System.DateTime.Now));
            base.OnMouseUp(e);
            if (e.Button == MouseButtons.Left)
                m_pGridWindow.OnMouseUp(e.Location);
            //System.Diagnostics.Trace.WriteLine(string.Format("mouse up end: {0}", System.DateTime.Now));
        }

        protected override void OnMouseClick(MouseEventArgs e)
        {
            base.OnMouseClick(e);
            if (e.Button == MouseButtons.Left)
                m_pGridWindow.OnMouseClick(e.Location);
        }

        protected override void OnMouseDoubleClick(MouseEventArgs e)
        {
            //System.Diagnostics.Trace.WriteLine(string.Format("mouse double click : {0}", System.DateTime.Now));
            //System.TimeSpan timeSpan = System.DateTime.Now - m_buttonDownTime;
            //if(timeSpan.Milliseconds >= SystemInformation.DoubleClickTime)
            // return;

            base.OnMouseDoubleClick(e);
            if (e.Button == MouseButtons.Left)
                m_pGridWindow.OnMouseDoubleClick(e.Location);
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            base.OnMouseWheel(e);
            m_pGridWindow.OnMouseWheel(e.Location, e.Delta);
        }

        protected override void OnPreviewKeyDown(PreviewKeyDownEventArgs e)
        {

            base.OnPreviewKeyDown(e);

            switch (e.KeyCode)
            {
                case Keys.ProcessKey:
                    {
                        if (e.Control == true || e.Alt == true)
                            break;

                        Column column = this.FocusedColumn;

                        if (column == null)
                        {
                            GridControl parent = this.Parent as GridControl;
                            if (parent != null && parent.FocusedCell != null)
                            {
                                parent.OnPreviewKeyDown(e);
                            }
                        }
                        else
                        {
                            System.Char imeChar = Natives.NativeMethods.ImmGetVirtualKey(Handle);
                            EditingReason er = new EditingReason(e.KeyValue);
                            if (System.Char.IsLetter(imeChar) && column.CanEditInternal(m_focusedCell, er))
                            {
                                this.EditCell(m_focusedCell, er);
                            }
                        }
                    }
                    break;
            }
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);
            return;

            ////if(m_pGridWindow.OnKeyDown((GrKeys)e.KeyCode) == true)
            ////{
            ////    e.Handled = true;
            ////    e.SuppressKeyPress = true;
            ////    return;
            ////}

            //switch(e.KeyCode)
            //{
            //    //case Keys.Right:
            //    //    {
            //    //        GrGridRow pGridRow = dynamic_cast<GrGridRow*>(this.Focuser.GetFocusedRow());
            //    //        if(pGridRow != null)
            //    //        {
            //    //            pGridRow.FocusGridControl();
            //    //            e.Handled = true;
            //    //        }
            //    //    }
            //    //    break;
            //    //case Keys.Left:
            //    //    {
            //    //        int eqwr=0;
            //    //    }
            //    //    break;
            //    //case Keys.Enter:
            //    //    {
            //    //        if(this.InsertionRow.IsFocused == true)
            //    //        {
            //    //            Row row = this.Rows.AddFromInsertion();
            //    //            if(row != null)
            //    //            {
            //    //                //Cell cell = row.Cells[m_focusedCell.Column];
            //    //                //cell.Select(SelectionType.Normal);
            //    //                //cell.Focus();
            //    //                //cell.BringIntoView();
            //    //            }
            //    //            e.SuppressKeyPress = true;
            //    //        }
            //    //    }
            //    //    break;
            //case Keys.Tab:
            //case Keys.Escape:
            //    {
            //        e.SuppressKeyPress = true;
            //    }
            //default:
            //    {
            //        Column column = this.FocusedColumn;

            //        if(column != null)
            //        {
            //            EditingReason er(e.KeyCode);
            //            if(column.CanEditInternal(m_focusedCell, er) == true)
            //            {
            //                this.EditCell(m_focusedCell, er);
            //                e.SuppressKeyPress = true;
            //            }
            //        }
            //    }
            //    break;
            //}
            //if(e.Control == true)
            //    e.SuppressKeyPress = true;
        }

        protected override void OnFontChanged(EventArgs e)
        {
            m_pGridCore.SetFont(Utility.FromManaged(this.Font));
            base.OnFontChanged(e);
#if DEBUG
            //System.Diagnostics.Debug.WriteLine("Invalidate");
#endif
        }

        protected override void OnPrint(PaintEventArgs e)
        {
            OnPaint(e);
        }

        protected override void OnLostFocus(EventArgs e)
        {
            if (this.EditingCell == null && this.FocusedRow != this.InsertionRow)
            {
                Row row = this.FocusedRow as Row;
                if (row != null)
                {
                    this.EndCurrentEdit(row);
                }
            }

            m_pGridWindow.OnLostFocus();
            base.OnLostFocus(e);
        }

        protected override void OnGotFocus(EventArgs e)
        {
            m_pGridWindow.OnGotFocus();
            base.OnGotFocus(e);

            //System.Console.WriteLine(this.Handle);
        }

        protected override void OnEnter(EventArgs e)
        {
            base.OnEnter(e);

            if (this.FocusedRow == this.InsertionRow)
            {
                this.Rows.BeginInsertion();
            }
        }

        protected override void OnLeave(EventArgs e)
        {
            base.OnLeave(e);

            if (this.FocusedRow == this.InsertionRow)
            {
                this.Rows.EndInsertion();
            }
        }

        protected override void OnLayout(LayoutEventArgs e)
        {
            base.OnLayout(e);
            m_pGridWindow.OnSizeChanged(this.DisplayRectangle);
            m_pGridCore.Update();
        }

        protected override void OnForeColorChanged(EventArgs e)
        {
            m_pGridCore.SetForeColor(this.ForeColor);
            base.OnForeColorChanged(e);
        }

        protected override void OnBackColorChanged(EventArgs e)
        {
            m_pGridCore.SetBackColor(this.BackColor);
            base.OnBackColorChanged(e);
        }

        protected virtual void OnBackgroundColorChanged(EventArgs e)
        {
            this.BackgroundColorChanged(this, e);
            this.Invalidate();
        }

        protected virtual void OnPaddingColorChanged(EventArgs e)
        {
            this.BackgroundColorChanged(this, e);
            this.Invalidate();
        }

        protected virtual void OnLineColorChanged(EventArgs e)
        {
            m_pGridCore.SetLineColor(this.LineColor);
            this.LineColorChanged(this, e);
        }

        protected new void OnScroll(ScrollEventArgs e)
        {
            if (this.Scroll != null)
            {
                this.Scroll(this, e);
            }
        }

        protected override void WndProc(ref Message m)
        {
            switch ((WindowMessages)m.Msg)
            {
                //case Native.WM.WM_SETFOCUS:
                //    {
                //        this.DefWndProc(m);
                //        this.OnGotFocus(EventArgs.Empty);
                //    }
                //    return;
                //case Native.WM.WM_KILLFOCUS:
                //    {
                //        this.DefWndProc(m);
                //        this.OnLostFocus(EventArgs.Empty);
                //    }
                //    return;
                case WindowMessages.WM_HSCROLL:
                    {
                        WinFormScroll pScroll = m_pGridCore.HorzScroll as WinFormScroll;
                        pScroll.WndProc(m.HWnd, m.WParam);
                    }
                    return;
                case WindowMessages.WM_VSCROLL:
                    {
                        WinFormScroll pScroll = m_pGridCore.VertScroll as WinFormScroll;
                        pScroll.WndProc(m.HWnd, m.WParam);
                    }
                    return;
            }
            base.WndProc(ref m);
        }

        protected override bool IsInputKey(Keys keyData)
        {
            //Keys key = keyData & Keys.KeyCode;

            //switch (key)
            //{
            //case Keys.Tab:
            //case Keys.Up:
            //case Keys.Down:
            //case Keys.Right:
            //case Keys.Left:
            //    return true;
            //default:
            //    break;
            //}

            return base.IsInputKey(keyData);
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            switch (keyData)
            {
                case Keys.Enter:
                    {
                        if (m_focusedCell != null && m_focusedCell.Row != this.InsertionRow)
                        {
                            this.EditCell(m_focusedCell, new EditingReason(keyData));
                            return true;
                        }
                    }
                    break;
                case (Keys.Down | Keys.Alt):
                    {
                        if (m_focusedCell != null && m_focusedCell.Column.GetEditStyle() == Design.EditStyle.DropDown)
                        {
                            this.EditCell(m_focusedCell, new EditingReason(keyData));
                            return true;
                        }
                    }
                    break;
                case Keys.F2:
                    {
                        if (m_focusedCell != null)
                        {
                            this.EditCell(m_focusedCell, new EditingReason(keyData));
                            return true;
                        }
                    }
                    break;
                case Keys.F12:
                    {
                        if (this.InsertionRow.IsVisible == true)
                        {
                            this.InsertionRow.Select(SelectionType.Normal);
                            this.InsertionRow.Focus();
                            this.InsertionRow.BringIntoView();
                            return true;
                        }
                    }
                    break;
                case Keys.Escape:
                    {
                        if (m_focusedCell == null)
                            break;

                        if (m_focusedCell.IsEdited == true)
                        {
                            m_focusedCell.CancelEdit();
                            if (m_pGridCore.IsGrouped() == true)
                                m_pGridCore.DataRowList.Update(true);
                        }
                        else if (m_focusedCell.Row.IsEdited == true)
                        {
                            m_focusedCell.Row.CancelEdit();
                            if (m_pGridCore.IsGrouped() == true)
                                m_pGridCore.DataRowList.Update(true);
                        }
                        else
                        {
                            return false;
                        }

                        this.Invalidate(false);
                        return true;
                    }
                    break;
                default:
                    {
                        Column column = this.FocusedColumn;

                        if (column != null)
                        {
                            EditingReason er = new EditingReason(keyData);
                            if (column.CanEditInternal(m_focusedCell, er) == true)
                            {
                                this.EditCell(m_focusedCell, er);
                                return true;
                            }
                        }
                    }
                    break;
            }

            return base.ProcessCmdKey(ref msg, keyData);
        }

        protected override bool ProcessDialogKey(Keys keyData)
        {
            Keys keys = keyData & Keys.KeyCode;
            if (m_pGridWindow.OnKeyDown((GrKeys)keys) == true)
                return true;

            if ((keyData & (Keys.Alt | Keys.Control)) == Keys.None)
            {
                switch (keys)
                {
                    case Keys.Up:
                    case Keys.Down:
                    case Keys.Left:
                        {
                            if (this.Parent is GridControl == true)
                                break;
                        }
                        return true;
                    case Keys.Right:
                        {
                            GrGridRow pGridRow = this.Focuser.GetFocusedRow() as GrGridRow;
                            if (pGridRow != null)
                            {
                                pGridRow.SetFocus();
                            }

                            if (this.Parent is GridControl == true)
                                break;
                        }
                        return true;
                    case Keys.Enter:
                        {
                            if (this.ReadOnly == false && this.InsertionRow.IsFocused == true)
                            {
                                Row row = this.Rows.AddFromInsertion();
                                if (row != null)
                                {
                                    //Cell cell = row.Cells[m_focusedCell.Column];
                                    //cell.Select(SelectionType.Normal);
                                    //cell.Focus();
                                    //cell.BringIntoView();
                                }
                                return true;
                            }
                        }
                        break;
                    case Keys.Tab:
                        {
                            base.ProcessDialogKey(keyData);
                            return true;
                        }
                        break;
                }
            }

            return base.ProcessDialogKey(keyData);
        }

        protected override bool ProcessDialogChar(char charCode)
        {
            if (m_focusedCell != null && ((Control.ModifierKeys & (Keys.Control | Keys.Alt)) == Keys.None))
            {
                Column column = m_focusedCell.Column;
                EditingReason reason = new EditingReason(charCode);
                if (column.CanEditInternal(m_focusedCell, reason) == true)
                {
                    this.EditCell(m_focusedCell, reason);
                    return true;
                }
            }

            return base.ProcessDialogChar(charCode);
        }

        protected override bool ProcessTabKey(bool forward)
        {
            return true;
        }

        protected virtual Row NewRowFromBuilder(RowBuilder rowBuilder)
        {
            return new Row(rowBuilder, this);
        }

        protected virtual GridControl NewChildGridControl(PropertyDescriptor descriptor, Row row)
        {
            GridControl control = System.Activator.CreateInstance(this.GetType()) as GridControl;
            control.Name = descriptor.Name;
            control.Caption = descriptor.Name;
            control.IsCaptionRowVisible = this.IsCaptionRowVisible;
            control.IsGroupPanelVisible = false;
            control.IsFrozingSplitterVisible = this.IsFrozingSplitterVisible;
            //control.Padding = Padding((int)this.Font.Size);
            control.BorderStyle = BorderStyle.FixedSingle;
            //control.Padding = Padding(1);
            control.Visible = false;
            control.AutoFitColumn = this.AutoFitColumn;
            control.AutoFitRow = this.AutoFitRow;
            control.ReadOnly = this.ReadOnly;
            control.Font = this.Font;

            return control;
        }


#if DEBUG
        protected override void OnInvalidated(InvalidateEventArgs e)
        {
            base.OnInvalidated(e);
            //System.Diagnostics.Debug.WriteLine(e.InvalidRect);
        }

        protected override void OnCursorChanged(EventArgs e)
        {
            base.OnCursorChanged(e);
            //System.Diagnostics.Debug.WriteLine("OnCursorChanged");
        }
#endif

        protected new bool HScroll
        {
            get { return true; }
        }

        protected new bool VScroll
        {
            get { return true; }
        }

        private void OnCurrencyManagerChanging(CurrencyManagerChangingEventArgs e)
        {
            if (m_manager != null)
            {
                m_manager.ListChanged -= m_listChangedEventHandler;
                m_manager.BindingComplete -= m_bindingCompleteEventHandler;
            }

            this.CurrencyManagerChanging(this, e);
        }

        private void OnCurrencyManagerChanged(CurrencyManagerChangedEventArgs e)
        {
            m_manager = e.CurrecnyManager;

            if (m_manager != null)
            {
                m_pGridCore.Reserve(m_manager.GetItemProperties().Count, m_manager.List.Count);
                m_manager.ListChanged += m_listChangedEventHandler;
                m_manager.BindingComplete += m_bindingCompleteEventHandler;

                this.CurrencyManagerChanged(this, e);
            }
            m_pGridCore.Invalidate();
        }

        private void OnEditClosed(EventArgs e)
        {
            this.EditClosed(this, e);
        }

        private void PaintRowState(Graphics g)
        {

            IDataRow pFocusedRow = this.Focuser.GetFocusedRow();
            if (pFocusedRow == null || pFocusedRow.IsDisplayable == false)
                return;

            Row row = this.FocusedRow as Row;

            if (EditingCell != null)
            {
                GrRect bound = EditingCell.NativeRef.GetDataRow().Bounds;
                GrPoint center = bound.GetCenter();

                Bitmap image = Resources.RowEditing;
                center.X -= (image.Width / 2);
                center.Y -= (image.Height / 2);
                g.DrawImageUnscaled(image, center.X, center.Y);
            }
            else if (row != null && row.IsEdited == true)
            {
                GrRect bound = pFocusedRow.Bounds;
                GrPoint center = bound.GetCenter();

                Bitmap image = Resources.RowEditing;
                center.X -= (image.Width / 2);
                center.Y -= (image.Height / 2);
                g.DrawImageUnscaled(image, center.X, center.Y);
            }
            else if (pFocusedRow.IsDisplayable == true)
            {
                GrRect bound = pFocusedRow.Bounds;
                GrPoint center = bound.GetCenter();

                Bitmap image = Resources.RowFocused;
                if (pFocusedRow.GetRowType() == GrRowType.InsertionRow)
                    image = Resources.InsertionRowFocused;
                center.X -= (image.Width / 2);
                center.Y -= (image.Height / 2);
                g.DrawImageUnscaled(image, center.X, center.Y);
            }

            GrDataRow pInsertionRow = m_pDataRowList.InsertionRow;
            if (pInsertionRow.IsVisible == true && pInsertionRow.HasFocused() == false)
            {
                GrRect bound = pInsertionRow.Bounds;
                GrPoint center = bound.GetCenter();

                Bitmap image = Resources.InsertionRow;
                center.X -= (image.Width / 2);
                center.Y -= (image.Height / 2);
                g.DrawImageUnscaled(image, center.X, center.Y);
            }
        }

        private void PaintColumnControls(Graphics graphics, Rectangle clipRectangle)
        {
            GrDataRow pInsertionRow = m_pDataRowList.InsertionRow;

            GrItem pMouseOvered = null;

            for (int i = 0; i < m_pColumnList.GetDisplayableColumnCount(); i++)
            {
                GrColumn pColumn = m_pColumnList.GetDisplayableColumn(i);
                if (pColumn.IntersectsHorzWith(clipRectangle) == false)
                    continue;

                Column column = m_columnList[pColumn];
                ViewType viewType = column.ViewType;
                if (viewType == ViewType.Text)
                    continue;

                //unsigned int drows = m_pDataRowList.GetDisplayableRowCount();
                for (int j = 0; j < m_pDataRowList.GetDisplayableRowCount() + 1; j++)
                {
                    GrDataRow pDataRow = null;
                    if (j == 0)
                        pDataRow = pInsertionRow.IsVisible == true ? pInsertionRow : null;
                    else
                        pDataRow = m_pDataRowList.GetDisplayableRow(j - 1) as GrDataRow;

                    if (pDataRow == null)
                        continue;

                    if (pDataRow.IntersectsVertWith(clipRectangle) == false)
                        continue;

                    GrItem pItem = pDataRow.GetItem(pColumn);
                    if (pItem.GetMouseOvered() == true)
                        pMouseOvered = pItem;
                    else
                        this.PaintCell(graphics, clipRectangle, pItem);
                }
            }

            if (pMouseOvered != null)
                this.PaintCell(graphics, clipRectangle, pMouseOvered);
        }

        private void PaintCell(Graphics graphics, Rectangle clipRectangle, GrItem pItem)
        {
            Cell cell = FromNative.Get(pItem);

            Column column = cell.Column;
            ViewType viewType = column.ViewType;
            Rectangle paintRect = pItem.ClientRectangle;
            paintRect.Offset(pItem.Location);

            if (viewType == ViewType.Icon)
            {
                paintRect.Width = GrDefineUtility.DEF_ICON_SIZE;
                paintRect.X -= (GrDefineUtility.DEF_ICON_SIZE + column.CellPadding.Left);
            }

            if (pItem.GetClipped() == true)
                paintRect.Intersect(clipRectangle);
            column.PaintValue(graphics, paintRect, cell, cell.DisplayValue);
        }

        private void SetDataConnection(object dataSource, string dataMember)
        {
            using (new DataBindingRef(this))
            {

                if (m_dataSource == dataSource && m_dataMember == dataMember)
                    return;

                CurrencyManager manager;

                if (dataSource == null)
                {
                    manager = m_defaultManager;
                }
                else
                {
                    ISupportInitializeNotification support = dataSource as ISupportInitializeNotification;

                    if (support != null && support.IsInitialized == false)
                    {
                        m_dataMember = dataMember;
                        support.Initialized += dataSource_Initialized;
                        return;
                    }

                    manager = this.BindingContext[dataSource, dataMember] as CurrencyManager;

                    if (manager == null)
                    {
                        throw new NotSupportedException(Resources.DataSourceInitializingFailed);
                    }
                    else if (support != null)
                    {
                        support.Initialized -= dataSource_Initialized;
                    }
                }

                CurrencyManagerChangingEventArgs e1 = new CurrencyManagerChangingEventArgs(manager);
                OnCurrencyManagerChanging(e1);

                if (e1.Cancel == true)
                {
                    throw new NotSupportedException(e1.CancelReason);
                }

                bool managerChanged = m_manager != manager;

                if (managerChanged == true)
                    ClearCore(true);

                m_dataSource = dataSource;
                m_dataMember = dataMember;

                CurrencyManagerChangedEventArgs e2 = new CurrencyManagerChangedEventArgs(manager);
                OnCurrencyManagerChanged(e2);

                if (managerChanged == true)
                {
                    OnDataBindingComplete(EventArgs.Empty);
                    m_pGridCore.Invalidate();
                }

                //if(dataSource != null && dataMember != string.Empty && manager == null)
                //{
                // throw new System.NotSupportedException("데이터 소스 초기화에 실패했습니다. 데이터 소스가 IList, IListSource 또는 IBindingList 인터페이스를 구현하는 개체인지 확인하십시오");
                //}
            }
        }

        private bool ShouldSerializeColumns()
        {
            return this.Columns.Count != 0;

        }

        private bool ShouldSerializeRows()
        {
            return this.Rows.Count != 0;

        }

        private bool ShouldSerializeCaption()
        {
            return m_captionRow.Text != Resources.CaptionRow_Text;
        }

        private bool ShouldSerializeBackgroundColor()
        {
            return m_backgroundColor != SystemColors.Control;
        }

        private bool ShouldSerializePaddingColor()
        {
            return m_paddingColor != SystemColors.ActiveCaption;
        }

        private bool ShouldSerializeLineColor()
        {
            return this.LineColor != (Color)GrStyle.Default.LineColor;

        }

        private bool ShouldSerializeBackColor()
        {
            return this.BackColor != (Color)GrStyle.Default.BackColor;
        }

        private bool ShouldSerializeForeColor()
        {
            return this.ForeColor != (Color)GrStyle.Default.ForeColor;

        }

        private void ResetColumns()
        {
            //this.Columns.Clear();
        }

        private void ResetRows()
        {
            this.Rows.Clear();
        }


        private void ClearCore(bool dataSourceOnly)
        {
            m_pGridCore.Update(true);

            ClearEventArgs e = new ClearEventArgs(dataSourceOnly);

            OnClearing(e);

            m_pGridCore.Clear();
            m_focusedCell = null;
            m_oldFocusedColumn = null;
            m_oldFocusedRow = null;

            m_defaultDataSource.Clear();

            OnCleared(e);
        }


        private void dataSource_Initialized(object sender, EventArgs e)
        {

            ISupportInitializeNotification support = sender as ISupportInitializeNotification;

            if (support.IsInitialized == true)
            {
                support.Initialized -= dataSource_Initialized;
                SetDataConnection(sender, m_dataMember);
            }
        }

        private void currencyManager_ListChanged(object sender, ListChangedEventArgs e)
        {
            switch (e.ListChangedType)
            {
                case ListChangedType.Reset:
                    {
                        m_pGridCore.Reserve(m_manager.GetItemProperties().Count, m_manager.List.Count);

                        if (this.BindingContext.Contains(m_dataSource, m_dataMember) == false)
                        {
                            m_dataMember = string.Empty;
                        }
                    }
                    break;
            }
        }

        private void currencyManager_BindingComplete(object sender, BindingCompleteEventArgs e)
        {

        }

        private void style_Disposed(object sender, EventArgs e)
        {
            if (sender == m_style)
            {
                this.Style = null;
            }

        }

        private void childGridControl_FocusedCellChanged(object sender, CellEventArgs e)
        {
            GridControl control = sender as GridControl;

            if (e.Cell != null)
            {
                this.Selector.ClearSelection();
                this.Focuser.Set(control.m_pGridRow);
            }
        }

        private DialogResult DefaultShowMessage(string text, string caption, MessageBoxButtons buttons, MessageBoxIcon icon)
        {
            return MessageBox.Show(text, caption, buttons, icon);
        }

        private EventHandler m_styleDisposedEventHandler;

        class DataBindingRef : IDisposable
        {
            GridControl m_gridControl;
            public DataBindingRef(GridControl gridControl)
            {
                m_gridControl = gridControl;
                m_gridControl.m_dataBindingRef++;
                m_gridControl.SuspendLayout();
            }



            public void Dispose()
            {
                m_gridControl.m_dataBindingRef--;
                m_gridControl.ResumeLayout();
            }
        }

        class StyleConverter : ReferenceConverter
        {
            public StyleConverter() : base(typeof(Style)) { }
        }

        ListChangedEventHandler m_listChangedEventHandler;
        BindingCompleteEventHandler m_bindingCompleteEventHandler;
    }
}
