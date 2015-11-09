using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    public class GrGridCore : IDisposable
    {
        GrRootRow rootRow;
        GrCaption paption;
        GrGroupPanel groupPanel;
        GrColumnList columnList;
        GrRowSplitter splitterRow;
        GrDataRowList dataRowList;
        GrFocusMover focusMover;

        GrStateManager stateManager;
        GrGridWindow gridWindow;
        GrInvalidator invalidator;

        GrRect displayRect;

        GrItemSelector itemSelector;
        GrFocuser focuser;
        GrTextUpdater textUpdater;
        GrMouseOverer mouseOverer;
        GrMousePresser mousePresser;


        //ulong flag;

        int reservedColumn;
        int reservedRow;

        bool autoFitColumn;
        GrAutoFitColumnType autoFitColumnType;
        bool autoFitRow;
        //int columnSplitterWidth;
        //int rowSplitterHeight;

        bool updating;
        //bool painting;
        bool fullRowSelect;
        bool selectionVisible;
        bool rowHighlight;
        GrRowHighlightType rowHighlightType;
        bool columnMovable;
        bool columnSortable;
        bool columnResizable;
        bool columnFreezable;
        bool readOnly;
        bool rowResizable;
        bool rowMovable;
        bool rowVisible;
        bool hideSelection;
        bool multiSelect;
        bool fillBlank;
        GrClickEditing clickEditing;

        GrColor foreColor;
        GrColor backColor;
        GrColor lineColor;
        GrFont pFont;
        GrPadding padding;

        GrStyle pStyle;

        int attachedCount;
        //int this.createdCell;

        public GrGridCore(GrGridWindow gridWindow)
        {
            this.gridWindow = gridWindow;
            //this.createdCell = 0;
            this.attachedCount = 0;

            this.updating = false;

            this.autoFitColumn = false;
            this.autoFitColumnType = GrAutoFitColumnType.ColumnIncluded;
            this.autoFitRow = false;
            //this.columnSplitterWidth = 10;
            //this.rowSplitterHeight = 3;

            this.reservedColumn = 0;
            this.reservedRow = 0;

            this.fullRowSelect = false;
            this.selectionVisible = true;
            this.rowHighlight = false;
            this.rowHighlightType = GrRowHighlightType.Fill;
            this.columnMovable = true;
            this.columnSortable = true;
            this.columnResizable = true;
            this.columnFreezable = true;
            this.readOnly = false;
            this.rowResizable = true;
            this.rowMovable = false;
            this.hideSelection = false;
            this.multiSelect = true;
            this.rowVisible = true;
            this.fillBlank = false;
            this.clickEditing = DefaultClickEditing;

            this.foreColor = GrStyle.Default.ForeColor;
            this.backColor = GrStyle.Default.BackColor;
            this.lineColor = GrStyle.Default.LineColor;
            this.padding = GrStyle.Default.Padding;
            this.pFont = GrStyle.Default.Font;
            this.pStyle = null;

            this.itemSelector = new GrItemSelectorInternal();
            this.focuser = new GrFocuserInternal();
            this.textUpdater = new GrTextUpdater();
            this.mouseOverer = new GrMouseOverer();
            this.mousePresser = new GrMousePresser();
            this.rootRow = new GrRootRow();
            this.paption = new GrCaption();
            this.groupPanel = new GrGroupPanel();
            this.columnList = new GrColumnList();
            this.dataRowList = new GrDataRowList();
            this.splitterRow = new GrRowSplitter();
            this.focusMover = new GrFocusMover();
            this.stateManager = new GrStateManager();

            this.invalidator = this.gridWindow.GetInvalidator();

            AttachObject(this.itemSelector);
            AttachObject(this.focuser);
            AttachObject(this.textUpdater);
            AttachObject(this.mouseOverer);
            AttachObject(this.mousePresser);
            AttachObject(this.rootRow);
            AttachObject(this.paption);
            AttachObject(this.groupPanel);
            AttachObject(this.columnList);
            AttachObject(this.dataRowList);
            AttachObject(this.splitterRow);
            AttachObject(this.stateManager);
            AttachObject(this.gridWindow);
            AttachObject(this.focusMover);

            this.rootRow.AddChild(this.paption);
            this.rootRow.AddChild(this.groupPanel);
            this.rootRow.AddChild(this.columnList);
            this.rootRow.AddChild(this.dataRowList.InsertionRow);
            this.rootRow.AddChild(this.splitterRow);
            this.rootRow.AddChild(this.dataRowList);

            this.columnList.ColumnInserted += columnList_ColumnInserted;
            this.columnList.ColumnRemoved += columnList_ColumnRemoved;
            this.columnList.ColumnWidthChanged += columnList_ColumnWidthChanged;
            this.columnList.ColumnWordwrapChanged += columnList_ColumnWordwrapChanged;
            this.columnList.ColumnHorzAlignChanged += columnList_ColumnHorzAlignChanged;
            this.columnList.ColumnVertAlignChanged += columnList_ColumnVertAlignChanged;
            this.columnList.ColumnPaddingChanged += columnList_ColumnPaddingChanged;

            this.focuser.FocusChanging += focuser_FocusChanging;

            this.Created(this, EventArgs.Empty);
        }

        public void Reserve(int columnCount, int rowCount)
        {
            this.reservedColumn = columnCount + 2;
            this.reservedRow = rowCount + 2;

            this.dataRowList.Reserve(this.reservedRow);
            this.columnList.Reserve(this.reservedColumn);

            this.OnCapacityChanged(EventArgs.Empty);
        }

        public int GetReservedColumn()
        {
            return this.reservedColumn;
        }

        public int GetReservedRow()
        {
            return this.reservedRow;
        }
        
        public void Clear()
        {
            Update(true);
            OnCleared(EventArgs.Empty);
            Update(true);
        }

        public bool Update()
        {
            return this.Update(false);
        }

        public bool Update(bool force)
        {
            if (this.updating == true)
                return false;

            this.updating = true;

            this.textUpdater.UpdateTextBounds();
            this.rootRow.Update(force);
            this.textUpdater.UpdateTextBounds();
            this.textUpdater.UpdateTextAlign();

            int i = 0;
            while (this.rootRow.ShouldUpdate() == true)
            {
                this.textUpdater.UpdateTextBounds();
                this.rootRow.Update(force);
                this.textUpdater.UpdateTextBounds();
                this.textUpdater.UpdateTextAlign();
                i++;

                //#ifdef _MANAGED
                //        //System::Console::Write("횟수");
                //        //System::Console::WriteLine(i);
                //#endif
            }

            this.updating = false;
            return true;
        }

        public GrRootRow RootRow
        {
            get { return this.rootRow; }
        }

        public GrDataRowList DataRowList
        {
            get { return this.dataRowList; }
        }

        public GrColumnList ColumnList
        {
            get { return this.columnList; }
        }

        public GrDataRow InsertionRow
        {
            get { return this.dataRowList.InsertionRow; }
        }

        public GrCaption CaptionRow
        {
            get { return this.paption; }
        }

        public GrGroupPanel GroupPanel
        {
            get { return this.groupPanel; }
        }

        public GrItemSelector ItemSelector
        {
            get { return this.itemSelector; }
        }

        public GrFocuser Focuser
        {
            get { return this.focuser; }
        }

        public GrFocusMover GetFocusMover() { return this.focusMover; }

        public GrTextUpdater GetTextUpdater() { return this.textUpdater; }

        public GrScroll HorzScroll
        {
            get { return this.gridWindow.GetHorzScroll(); }
        }

        public GrScroll VertScroll
        {
            get { return this.gridWindow.GetVertScroll(); }
        }

        public GrGridWindow GetGridWindow() { return this.gridWindow; }

        public GrStateManager GetStateManager() { return this.stateManager; }

        public GrRect DisplayRectangle
        {
            get { return this.displayRect; }
            set
            {
                if (this.displayRect == value)
                    return;
                this.displayRect = value;
                this.Update();
                int horz = this.HorzScroll.IsVisible == true ? this.HorzScroll.Value : this.HorzScroll.Minimum;
                int vert = this.VertScroll.IsVisible == true ? this.VertScroll.Value : this.VertScroll.Minimum;
                this.rootRow.Clip(this.displayRect, horz, vert);
                this.OnDisplayRectangleChanged(EventArgs.Empty);
            }
        }

        public GrRect GetBounds()
        {
            return this.rootRow.Bounds;
        }

        public GrRect GetVisibleBounds()
        {
            return this.rootRow.GetVisibleBounds();
        }

        public GrRect GetDataRect()
        {

            int left = this.columnList.GetUnfrozenX();
            int top = this.dataRowList.Y;
            int right = Math.Min(GetBounds().Right, this.displayRect.Right);
            int bottom = Math.Min(GetBounds().Bottom, this.displayRect.Bottom);
            GrRect dataRect = GrRect.FromLTRB(left, top, right, bottom);
            return dataRect;
        }

        public bool AutoFitColumn
        {
            get { return this.autoFitColumn; }
            set { this.autoFitColumn = value; }
        }

        public GrAutoFitColumnType GetAutoFitColumnType()
        {
            return this.autoFitColumnType;
        }

        public void GetAutoFitColumnType(GrAutoFitColumnType value)
        {
            if (this.autoFitColumnType == value)
                return;
            this.autoFitColumnType = value;

            if (this.autoFitColumn == true)
            {
                this.columnList.SetWidthChanged();
            }
        }

        public bool AutoFitRow
        {
            get { return this.autoFitRow; }
            set { this.autoFitRow = value; }
        }

        public bool IsInsertionRowVisible
        {
            get { return this.InsertionRow.IsVisible; }
            set { this.InsertionRow.IsVisible = value; }
        }

        public bool GetRowHighlight()
        {
            return this.rowHighlight;
        }

        public void SetRowHighlight(bool b)
        {
            if (this.rowHighlight == b)
                return;
            this.rowHighlight = b;
            Invalidate();
        }

        public GrRowHighlightType RowHighlightType
        {
            get { return this.rowHighlightType; }
            set
            {
                if (this.rowHighlightType == value)
                    return;
                this.rowHighlightType = value;
                this.Invalidate();
            }
        }

        public bool GetFullRowSelect()
        {
            return this.fullRowSelect;
        }

        public void SetFullRowSelect(bool b)
        {
            this.fullRowSelect = b;
        }

        public bool GetSelectionVisible()
        {
            return this.selectionVisible;
        }

        public void SetSelectionVisible(bool b)
        {
            this.selectionVisible = b;
        }

        public bool GetRowVisible()
        {
            return this.rowVisible;
        }

        public void SetRowVisible(bool b)
        {
            if (this.rowVisible == b)
                return;
            this.rowVisible = b;
            this.columnList.SetVisibleChanged();
            Invalidate();
        }

        public bool GetFillBlank()
        {
            return this.fillBlank;
        }

        public void SetFillBlank(bool b)
        {
            if (this.fillBlank == b)
                return;
            this.fillBlank = b;
            Invalidate();
        }

        public bool IsGrouped()
        {
            return this.groupPanel.Groups.Count > 0 ? true : false;
        }

        public bool IsUpdating() { return this.updating; }

        public bool IsGroupable
        {
            get { return this.groupPanel.IsGroupable; }
            set
            {
                this.groupPanel.IsGroupable = value;
                this.rootRow.SetVisibleChanged();
            }
        }

        public bool GetColumnMovable()
        {
            return this.columnMovable;
        }

        public void SetColumnMovable(bool b)
        {
            this.columnMovable = b;
        }

        public bool GetColumnSortable()
        {
            return this.columnSortable;
        }

        public void SetColumnSortable(bool b)
        {
            this.columnSortable = b;
        }

        public bool GetColumnResizable()
        {
            return this.columnResizable;
        }

        public void SetColumnResizable(bool b)
        {
            this.columnResizable = b;
        }

        public bool GetColumnFreezable()
        {
            return this.columnFreezable;
        }

        public void SetColumnFreezable(bool b)
        {
            this.columnFreezable = b;
        }

        public bool GetReadOnly()
        {
            return this.readOnly;
        }

        public void SetReadOnly(bool value)
        {
            if (this.readOnly == value)
                return;
            this.readOnly = value;
            Invalidate();
        }

        public bool GetRowResizable()
        {
            return this.rowResizable;
        }

        public void SetRowResizable(bool b)
        {
            this.rowResizable = b;
        }

        public bool GetRowMovable()
        {
            return this.rowMovable;
        }

        public void SetRowMovable(bool b)
        {
            this.rowMovable = b;
        }

        public bool GetHideSelection()
        {
            return this.hideSelection;
        }

        public void SetHideSelection(bool b)
        {
            this.hideSelection = b;
        }

        public bool GetMultiSelect()
        {
            return this.multiSelect;
        }

        public void SetMultiSelect(bool b)
        {
            this.multiSelect = b;
        }

        public GrClickEditing GetClickEditing()
        {
            return this.clickEditing;
        }

        public void SetClickEditing(GrClickEditing clickEditing)
        {
            if (clickEditing == GrClickEditing.Default)
                clickEditing = DefaultClickEditing;
            this.clickEditing = clickEditing;
        }

        public GrColor GetForeColor()
        {
            return this.foreColor;
        }

        public GrColor GetBackColor()
        {
            return this.backColor;
        }

        public GrColor GetLineColor()
        {
            return this.lineColor;
        }

        public GrFont GetFont()
        {
            return this.pFont;
        }

        public GrPadding GetPadding()
        {
            return this.padding;
        }

        public void SetForeColor(GrColor foreColor)
        {
            if (this.foreColor == foreColor)
                return;
            this.foreColor = foreColor;
            Invalidate();
        }

        public void SetBackColor(GrColor backColor)
        {
            if (this.backColor == backColor)
                return;
            this.backColor = backColor;
            Invalidate();
        }

        public void SetLineColor(GrColor lineColor)
        {
            if (this.lineColor == lineColor)
                return;
            this.lineColor = lineColor;
            Invalidate();
        }

        public void SetFont(GrFont pFont)
        {
            if (this.pFont == pFont)
                return;
            this.pFont = pFont;
            OnFontChanged(EventArgs.Empty);
        }

        public void SetPadding(GrPadding padding)
        {
            if (this.padding == padding)
                return;
            this.padding = padding;
            Invalidate();
        }

        static GrEditingReason temp;

        public void EditItem(GrItem pItem, GrEditingReason reason)
        {

            temp = reason;

            if (pItem.IsDisplayable == false)
            {
                pItem.BringIntoView();
            }
            this.stateManager.ChangeState(GrGridState.ItemEditing, pItem, temp);
        }

        public void BringIntoView(GrItem pItem)
        {
            if (pItem == null)
                return;

            Update();
            this.columnList.BringIntoView(pItem.Column);
            this.dataRowList.BringIntoView(pItem.DataRow);
        }

        public bool SetMouseOver(GrCell pCell, GrPoint localLocation)
        {
            return this.mouseOverer.SetMouseOver(pCell, localLocation);
        }

        public GrCell GetMouseOver()
        {
            return this.mouseOverer.GetMouseOver();
        }

        public int GetMouseOverState()
        {
            return this.mouseOverer.GetMouseOverState();
        }

        public void SetMousePress(GrCell pCell)
        {
            this.mousePresser.SetMousePress(pCell);
        }

        public void SetMouseUnpress()
        {
            this.mousePresser.SetMouseUnpress();
        }

        public GrCell GetMousePress()
        {
            return this.mousePresser.GetMousePress();
        }

        public void Invalidate()
        {
            this.invalidator.Invalidate();
        }

        public void Invalidate(int x, int y, int width, int height)
        {
            this.invalidator.Invalidate(x, y, width, height);
        }

        public void Invalidate(GrRect rect)
        {
            this.invalidator.Invalidate(rect);
        }

        public void LockInvalidate()
        {
            this.invalidator.Lock();
        }

        public void UnlockInvalidate()
        {
            this.invalidator.Unlock();
        }

        public void ResetInvalidate()
        {
            this.invalidator.Reset();
        }

        public bool IsInvalidated()
        {
            return this.invalidator.IsInvalidated();
        }

        public bool HitTest(GrPoint location, out GrHitTest pHitTest)
        {
            GrCell pCell = this.rootRow.HitTest(location);
            pHitTest = new GrHitTest();
            if (pCell != null)
            {
                pHitTest.pHitted = pCell;
                pHitTest.localHit = location - pCell.Location;
                return true;
            }
            return false;
        }

        public GrStyle GetStyle()
        {
            return this.pStyle;
        }

        public void SetStyle(GrStyle pStyle)
        {
            this.pStyle = pStyle;
        }
        
        public void AttachObject(GrObject pObject)
        {
            if (pObject.GridCore != null)
                return;
            pObject.GridCore = this;
            pObject.InvokeOnGridCoreAttached();

            this.attachedCount++;
        }

        public void DetachObject(GrObject pObject)
        {
            if (pObject.GridCore == null)
                return;
            pObject.InvokeOnGridCoreDetached();
            pObject.GridCore = null;
            this.attachedCount--;
        }

        public void Invoke(string eventName, EventArgs e)
        {
            if (eventName.CompareTo("EditValue") == 0)
            {
                OnEditValue((GrEditEventArgs)e);
            }
            else if (eventName.CompareTo("ItemMouseEnter") == 0)
            {
                OnItemMouseEnter((GrItemMouseEventArgs)e);
            }
            else if (eventName.CompareTo("ItemMouseMove") == 0)
            {
                OnItemMouseMove((GrItemMouseEventArgs)e);
            }
            else if (eventName.CompareTo("ItemMouseLeave") == 0)
            {
                OnItemMouseLeave((GrItemMouseEventArgs)e);
            }
            else if (eventName.CompareTo("ItemMouseClick") == 0)
            {
                OnItemMouseClick((GrItemMouseEventArgs)e);
            }
            else if (eventName.CompareTo("ItemMouseDoubleClick") == 0)
            {
                OnItemMouseDoubleClick((GrItemMouseEventArgs)e);
            }
            else if (eventName.CompareTo("RowMouseEnter") == 0)
            {
                OnRowMouseEnter((GrRowMouseEventArgs)e);
            }
            else if (eventName.CompareTo("RowMouseMove") == 0)
            {
                OnRowMouseMove((GrRowMouseEventArgs)e);
            }
            else if (eventName.CompareTo("RowMouseLeave") == 0)
            {
                OnRowMouseLeave((GrRowMouseEventArgs)e);
            }
            else if (eventName.CompareTo("close") == 0)
            {
                this.stateManager.ChangeDefaultState();
            }
            else if (eventName.CompareTo("FontChanged") == 0)
            {
                OnFontChanged(e);
            }
            else
            {
                this.columnList.Invoke(eventName, e);
            }
        }

        public event EventHandler Created;

        public event EventHandler Cleared;

        public event EventHandler CapacityChanged;

        public event EventHandler FontChanged;

        public event EventHandler DisplayRectangleChanged;

        public event GrItemMouseEventHandler ItemMouseEnter;

        public event GrItemMouseEventHandler ItemMouseMove;

        public event GrItemMouseEventHandler ItemMouseLeave;

        public event GrItemMouseEventHandler ItemMouseClick;

        public event GrItemMouseEventHandler ItemMouseDoubleClick;

        public event GrRowMouseEventHandler RowMouseEnter;

        public event GrRowMouseEventHandler RowMouseMove;

        public event GrRowMouseEventHandler RowMouseLeave;
        
        protected virtual void OnCleared(EventArgs e)
        {
            Cleared(this, e);
        }

        protected virtual void OnCapacityChanged(EventArgs e)
        {
            CapacityChanged(this, e);
        }

        protected virtual void OnFontChanged(EventArgs e)
        {
            FontChanged(this, e);
        }

        protected virtual void OnDisplayRectangleChanged(EventArgs e)
        {
            if (this.DisplayRectangleChanged != null)
            {
                this.DisplayRectangleChanged(this, e);
            }
            this.Invalidate();
        }

        protected virtual void OnEditValue(GrEditEventArgs e)
        {
            e.GetItem().LockColor(true);
            this.invalidator.Invalidate();
            this.gridWindow.OnEditValue(e);
            e.GetItem().LockColor(false);
            this.invalidator.Invalidate();
        }

        protected virtual void OnItemMouseEnter(GrItemMouseEventArgs e)
        {
            if (this.ItemMouseEnter != null)
            {
                this.ItemMouseEnter(this, e);
            }
        }

        protected virtual void OnItemMouseMove(GrItemMouseEventArgs e)
        {
            if (this.ItemMouseMove != null)
            {
                this.ItemMouseMove(this, e);
            }
        }

        protected virtual void OnItemMouseLeave(GrItemMouseEventArgs e)
        {
            if (this.ItemMouseLeave != null)
            {
                this.ItemMouseLeave(this, e);
            }
        }

        protected virtual void OnItemMouseClick(GrItemMouseEventArgs e)
        {
            if (this.ItemMouseClick != null)
            {
                this.ItemMouseClick(this, e);
            }
        }

        protected virtual void OnItemMouseDoubleClick(GrItemMouseEventArgs e)
        {
            if (this.ItemMouseDoubleClick != null)
            {
                this.ItemMouseDoubleClick(this, e);
            }
        }

        protected virtual void OnRowMouseEnter(GrRowMouseEventArgs e)
        {
            if (this.RowMouseEnter != null)
            {
                this.RowMouseEnter(this, e);
            }
        }

        protected virtual void OnRowMouseMove(GrRowMouseEventArgs e)
        {
            if (this.RowMouseMove != null)
            {
                this.RowMouseMove(this, e);
            }
        }

        protected virtual void OnRowMouseLeave(GrRowMouseEventArgs e)
        {
            if (this.RowMouseLeave != null)
            {
                this.RowMouseLeave(this, e);
            }
        }

        internal void InvokePrePaint(GrGridPainter painter, GrRect clipRect)
        {
            this.PrePaint(painter, clipRect);
        }

        internal void InvokePaint(GrGridPainter painter, GrRect clipRect)
        {
            this.Paint(painter, clipRect);
        }

        internal void InvokePostPaint(GrGridPainter painter, GrRect clipRect)
        {
            this.PostPaint(painter, clipRect);
        }

        protected virtual void PrePaint(GrGridPainter painter, GrRect clipRect)
        {

        }
        
        protected virtual void Paint(GrGridPainter painter, GrRect clipRect)
        {
            //this.painting = true;
            this.rootRow.Paint(painter, clipRect);
            //this.painting = false;
        }

        protected virtual void PostPaint(GrGridPainter painter, GrRect clipRect)
        {
            //this.painting = true;
            this.stateManager.OnPaint(painter);
            //this.painting = false;
        }

        private void focuser_FocusChanging(object sender, GrFocusChangeArgs e)
        {
            if (this.stateManager.GetGridState() == GrGridState.ItemEditing)
                this.stateManager.ChangeDefaultState();
        }

        private void columnList_ColumnInserted(object sender, GrColumnEventArgs e)
        {
            this.textUpdater.AddTextBounds(this.paption);
            this.textUpdater.AddTextBounds(this.groupPanel);
        }

        private void columnList_ColumnRemoved(object sender, GrColumnEventArgs e)
        {
            this.textUpdater.AddTextBounds(this.paption);
            this.textUpdater.AddTextBounds(this.groupPanel);
        }

        private void columnList_ColumnWidthChanged(object sender, GrColumnEventArgs e)
        {
            if (this.updating == true)
                return;
            GrColumn column = e.Column;
            if (column.ItemWordWrap == true)
                this.textUpdater.AddTextBounds(column);
        }

        private void columnList_ColumnWordwrapChanged(object sender, GrColumnEventArgs e)
        {
            if (this.updating == true)
                return;
            GrColumn column = e.Column;
            this.textUpdater.AddTextBounds(column);
        }

        private void columnList_ColumnHorzAlignChanged(object sender, GrColumnEventArgs e)
        {
            GrColumn column = e.Column;
            this.textUpdater.AddTextAlign(column);
        }

        private void columnList_ColumnVertAlignChanged(object sender, GrColumnEventArgs e)
        {
            GrColumn column = e.Column;
            this.textUpdater.AddTextAlign(column);
        }

        private void columnList_ColumnPaddingChanged(object sender, GrColumnEventArgs e)
        {
            GrColumn column = e.Column;
            if (column.ItemWordWrap == true)
                this.textUpdater.AddTextBounds(column);
        }

        internal void BeginPaint()
        {
            int horz = HorzScroll.IsVisible == true ? HorzScroll.Value : HorzScroll.Minimum;
            int vert = VertScroll.IsVisible == true ? VertScroll.Value : VertScroll.Minimum;
            this.rootRow.Clip(this.displayRect, horz, vert);
        }

        internal void EndPaint()
        {
            this.invalidator.Reset();
        }

        public static GrClickEditing DefaultClickEditing;

        public void Dispose()
        {
            throw new NotImplementedException();
        }
    }
}
