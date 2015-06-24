using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    public class GrGridCore : IDisposable
    {
        GrRootRow m_pRootRow;
        GrCaption m_pCaption;
        GrGroupPanel m_groupPanel;
        GrColumnList m_columnList;
        GrRowSplitter m_pSplitterRow;
        GrDataRowList m_pDataRowList;
        GrFocusMover m_pFocusMover;

        GrStateManager m_pStateManager;
        GrGridWindow m_pGridWindow;
        GrInvalidator m_pInvalidator;

        GrRect m_displayRect;

        GrItemSelector m_pItemSelector;
        GrFocuser m_pFocuser;
        GrTextUpdater m_pTextUpdater;
        GrMouseOverer m_pMouseOverer;
        GrMousePresser m_pMousePresser;


        //ulong m_flag;

        int m_reservedColumn;
        int m_reservedRow;

        bool m_autoFitColumn;
        GrAutoFitColumnType m_autoFitColumnType;
        bool m_autoFitRow;
        int m_columnSplitterWidth;
        int m_rowSplitterHeight;

        bool m_updating;
        bool m_painting;
        bool m_fullRowSelect;
        bool m_selectionVisible;
        bool m_rowHighlight;
        GrRowHighlightType m_rowHighlightType;
        bool m_columnMovable;
        bool m_columnSortable;
        bool m_columnResizable;
        bool m_columnFreezable;
        bool m_readOnly;
        bool m_rowResizable;
        bool m_rowMovable;
        bool m_rowVisible;
        bool m_hideSelection;
        bool m_multiSelect;
        bool m_fillBlank;
        GrClickEditing m_clickEditing;

        GrColor m_foreColor;
        GrColor m_backColor;
        GrColor m_lineColor;
        GrFont m_pFont;
        GrPadding m_padding;

        GrStyle m_pStyle;

        int m_attachedCount;
        int m_createdCell;

        public GrGridCore(GrGridWindow pGridWindow)
        {
            this.m_pGridWindow = pGridWindow;
            m_createdCell = 0;
            m_attachedCount = 0;

            m_updating = false;

            m_autoFitColumn = false;
            m_autoFitColumnType = GrAutoFitColumnType.ColumnIncluded;
            m_autoFitRow = false;
            m_columnSplitterWidth = 10;
            m_rowSplitterHeight = 3;

            m_reservedColumn = 0;
            m_reservedRow = 0;

            m_fullRowSelect = false;
            m_selectionVisible = true;
            m_rowHighlight = false;
            m_rowHighlightType = GrRowHighlightType.Fill;
            m_columnMovable = true;
            m_columnSortable = true;
            m_columnResizable = true;
            m_columnFreezable = true;
            m_readOnly = false;
            m_rowResizable = true;
            m_rowMovable = false;
            m_hideSelection = false;
            m_multiSelect = true;
            m_rowVisible = true;
            m_fillBlank = false;
            m_clickEditing = DefaultClickEditing;

            m_foreColor = GrStyle.Default.ForeColor;
            m_backColor = GrStyle.Default.BackColor;
            m_lineColor = GrStyle.Default.LineColor;
            m_padding = GrStyle.Default.Padding;
            m_pFont = GrStyle.Default.Font;
            m_pStyle = null;

            m_pItemSelector = new GrItemSelectorInternal();
            m_pFocuser = new GrFocuserInternal();
            m_pTextUpdater = new GrTextUpdater();
            m_pMouseOverer = new GrMouseOverer();
            m_pMousePresser = new GrMousePresser();
            m_pRootRow = new GrRootRow();
            m_pCaption = new GrCaption();
            m_groupPanel = new GrGroupPanel();
            m_columnList = new GrColumnList();
            m_pDataRowList = new GrDataRowList();
            m_pSplitterRow = new GrRowSplitter();
            m_pFocusMover = new GrFocusMover();
            m_pStateManager = new GrStateManager();

            m_pInvalidator = m_pGridWindow.GetInvalidator();

            AttachObject(m_pItemSelector);
            AttachObject(m_pFocuser);
            AttachObject(m_pTextUpdater);
            AttachObject(m_pMouseOverer);
            AttachObject(m_pMousePresser);
            AttachObject(m_pRootRow);
            AttachObject(m_pCaption);
            AttachObject(m_groupPanel);
            AttachObject(m_columnList);
            AttachObject(m_pDataRowList);
            AttachObject(m_pSplitterRow);
            AttachObject(m_pStateManager);
            AttachObject(m_pGridWindow);
            AttachObject(m_pFocusMover);

            m_pRootRow.AddChild(m_pCaption);
            m_pRootRow.AddChild(m_groupPanel);
            m_pRootRow.AddChild(m_columnList);
            m_pRootRow.AddChild(m_pDataRowList.GetInsertionRow());
            m_pRootRow.AddChild(m_pSplitterRow);
            m_pRootRow.AddChild(m_pDataRowList);

            m_columnList.ColumnInserted += columnList_ColumnInserted;
            m_columnList.ColumnRemoved += columnList_ColumnRemoved;
            m_columnList.ColumnWidthChanged += columnList_ColumnWidthChanged;
            m_columnList.ColumnWordwrapChanged += columnList_ColumnWordwrapChanged;
            m_columnList.ColumnHorzAlignChanged += columnList_ColumnHorzAlignChanged;
            m_columnList.ColumnVertAlignChanged += columnList_ColumnVertAlignChanged;
            m_columnList.ColumnPaddingChanged += columnList_ColumnPaddingChanged;

            m_pFocuser.FocusChanging += focuser_FocusChanging;

            this.Created(this, EventArgs.Empty);
        }

        public void Reserve(int columnCount, int rowCount)
        {
            m_reservedColumn = columnCount + 2;
            m_reservedRow = rowCount + 2;

            m_pDataRowList.Reserve(m_reservedRow);
            m_columnList.Reserve(m_reservedColumn);

            this.OnCapacityChanged(EventArgs.Empty);
        }

        public int GetReservedColumn()
        {
            return m_reservedColumn;
        }

        public int GetReservedRow()
        {
            return m_reservedRow;
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
            if (m_updating == true)
                return false;

            m_updating = true;

            m_pTextUpdater.UpdateTextBounds();
            m_pRootRow.Update(force);
            m_pTextUpdater.UpdateTextBounds();
            m_pTextUpdater.UpdateTextAlign();

            int i = 0;
            while (m_pRootRow.ShouldUpdate() == true)
            {
                m_pTextUpdater.UpdateTextBounds();
                m_pRootRow.Update(force);
                m_pTextUpdater.UpdateTextBounds();
                m_pTextUpdater.UpdateTextAlign();
                i++;

                //#ifdef _MANAGED
                //        //System::Console::Write("횟수");
                //        //System::Console::WriteLine(i);
                //#endif
            }

            m_updating = false;
            return true;
        }

        public GrRootRow GetRootRow() { return m_pRootRow; }

        public GrDataRowList GetDataRowList() { return m_pDataRowList; }

        public GrColumnList GetColumnList() { return m_columnList; }

        public GrDataRow GetInsertionRow() { return m_pDataRowList.GetInsertionRow(); }

        public GrCaption GetCaptionRow() { return m_pCaption; }

        public GrGroupPanel GetGroupPanel() { return m_groupPanel; }

        public GrItemSelector GetItemSelector() { return m_pItemSelector; }

        public GrFocuser GetFocuser() { return m_pFocuser; }

        public GrFocusMover GetFocusMover() { return m_pFocusMover; }

        public GrTextUpdater GetTextUpdater() { return m_pTextUpdater; }

        public GrScroll GetHorzScroll() { return m_pGridWindow.GetHorzScroll(); }

        public GrScroll GetVertScroll() { return m_pGridWindow.GetVertScroll(); }

        public GrGridWindow GetGridWindow() { return m_pGridWindow; }

        public GrStateManager GetStateManager() { return m_pStateManager; }

        public GrRect GetDisplayRect()
        {
            return m_displayRect;
        }

        public void SetDisplayRect(GrRect displayRect)
        {
            if (m_displayRect == displayRect)
                return;
            m_displayRect = displayRect;
            this.Update();
            int horz = GetHorzScroll().GetVisible() == true ? GetHorzScroll().GetValue() : GetHorzScroll().GetMinimum();
            int vert = GetVertScroll().GetVisible() == true ? GetVertScroll().GetValue() : GetVertScroll().GetMinimum();
            m_pRootRow.Clip(m_displayRect, horz, vert);
            OnDisplayRectChanged(EventArgs.Empty);
        }

        public GrRect GetBounds()
        {
            return m_pRootRow.GetBounds();
        }

        public GrRect GetVisibleBounds()
        {
            return m_pRootRow.GetVisibleBounds();
        }

        public GrRect GetDataRect()
        {

            int left = m_columnList.GetUnfrozenX();
            int top = m_pDataRowList.GetY();
            int right = Math.Min(GetBounds().Right, m_displayRect.Right);
            int bottom = Math.Min(GetBounds().Bottom, m_displayRect.Bottom);
            GrRect dataRect = GrRect.FromLTRB(left, top, right, bottom);
            return dataRect;
        }

        public bool GetAutoFitColumn()
        {
            return m_autoFitColumn;
        }

        public void SetAutoFitColumn(bool b)
        {
            m_autoFitColumn = b;
        }

        public GrAutoFitColumnType GetAutoFitColumnType()
        {
            return m_autoFitColumnType;
        }

        public void GetAutoFitColumnType(GrAutoFitColumnType value)
        {
            if (m_autoFitColumnType == value)
                return;
            m_autoFitColumnType = value;

            if (m_autoFitColumn == true)
            {
                m_columnList.SetWidthChanged();
            }
        }

        public bool GetAutoFitRow()
        {
            return m_autoFitRow;
        }

        public void SetAutoFitRow(bool b)
        {
            m_autoFitRow = b;
        }

        public bool GetInsertionRowVisible()
        {
            return GetInsertionRow().GetVisible();
        }

        public void SetInsertionRowVisible(bool b)
        {
            GetInsertionRow().SetVisible(b);
        }

        public bool GetRowHighlight()
        {
            return m_rowHighlight;
        }

        public void SetRowHighlight(bool b)
        {
            if (m_rowHighlight == b)
                return;
            m_rowHighlight = b;
            Invalidate();
        }

        public GrRowHighlightType GetRowHighlightType()
        {
            return m_rowHighlightType;
        }

        public void SetRowHighlightType(GrRowHighlightType type)
        {
            if (m_rowHighlightType == type)
                return;
            m_rowHighlightType = type;
            Invalidate();
        }

        public bool GetFullRowSelect()
        {
            return m_fullRowSelect;
        }

        public void SetFullRowSelect(bool b)
        {
            m_fullRowSelect = b;
        }

        public bool GetSelectionVisible()
        {
            return m_selectionVisible;
        }

        public void SetSelectionVisible(bool b)
        {
            m_selectionVisible = b;
        }

        public bool GetRowVisible()
        {
            return m_rowVisible;
        }

        public void SetRowVisible(bool b)
        {
            if (m_rowVisible == b)
                return;
            m_rowVisible = b;
            m_columnList.SetVisibleChanged();
            Invalidate();
        }

        public bool GetFillBlank()
        {
            return m_fillBlank;
        }

        public void SetFillBlank(bool b)
        {
            if (m_fillBlank == b)
                return;
            m_fillBlank = b;
            Invalidate();
        }

        public bool IsGrouped()
        {
            return m_groupPanel.GetGroupCount() > 0 ? true : false;
        }

        public bool IsUpdating() { return m_updating; }

        public bool GetGroupable()
        {
            return m_groupPanel.GetGroupable();
        }

        public void SetGroupable(bool b)
        {
            m_groupPanel.SetGroupable(b);
            m_pRootRow.SetVisibleChanged();
        }

        public bool GetColumnMovable()
        {
            return m_columnMovable;
        }

        public void SetColumnMovable(bool b)
        {
            m_columnMovable = b;
        }

        public bool GetColumnSortable()
        {
            return m_columnSortable;
        }

        public void SetColumnSortable(bool b)
        {
            m_columnSortable = b;
        }

        public bool GetColumnResizable()
        {
            return m_columnResizable;
        }

        public void SetColumnResizable(bool b)
        {
            m_columnResizable = b;
        }

        public bool GetColumnFreezable()
        {
            return m_columnFreezable;
        }

        public void SetColumnFreezable(bool b)
        {
            m_columnFreezable = b;
        }

        public bool GetReadOnly()
        {
            return m_readOnly;
        }

        public void SetReadOnly(bool value)
        {
            if (m_readOnly == value)
                return;
            m_readOnly = value;
            Invalidate();
        }

        public bool GetRowResizable()
        {
            return m_rowResizable;
        }

        public void SetRowResizable(bool b)
        {
            m_rowResizable = b;
        }

        public bool GetRowMovable()
        {
            return m_rowMovable;
        }

        public void SetRowMovable(bool b)
        {
            m_rowMovable = b;
        }

        public bool GetHideSelection()
        {
            return m_hideSelection;
        }

        public void SetHideSelection(bool b)
        {
            m_hideSelection = b;
        }

        public bool GetMultiSelect()
        {
            return m_multiSelect;
        }

        public void SetMultiSelect(bool b)
        {
            m_multiSelect = b;
        }

        public GrClickEditing GetClickEditing()
        {
            return m_clickEditing;
        }

        public void SetClickEditing(GrClickEditing clickEditing)
        {
            if (clickEditing == GrClickEditing.Default)
                clickEditing = DefaultClickEditing;
            m_clickEditing = clickEditing;
        }

        public GrColor GetForeColor()
        {
            return m_foreColor;
        }

        public GrColor GetBackColor()
        {
            return m_backColor;
        }

        public GrColor GetLineColor()
        {
            return m_lineColor;
        }

        public GrFont GetFont()
        {
            return m_pFont;
        }

        public GrPadding GetPadding()
        {
            return m_padding;
        }

        public void SetForeColor(GrColor foreColor)
        {
            if (m_foreColor == foreColor)
                return;
            m_foreColor = foreColor;
            Invalidate();
        }

        public void SetBackColor(GrColor backColor)
        {
            if (m_backColor == backColor)
                return;
            m_backColor = backColor;
            Invalidate();
        }

        public void SetLineColor(GrColor lineColor)
        {
            if (m_lineColor == lineColor)
                return;
            m_lineColor = lineColor;
            Invalidate();
        }

        public void SetFont(GrFont pFont)
        {
            if (m_pFont == pFont)
                return;
            m_pFont = pFont;
            OnFontChanged(EventArgs.Empty);
        }

        public void SetPadding(GrPadding padding)
        {
            if (m_padding == padding)
                return;
            m_padding = padding;
            Invalidate();
        }

        static GrEditingReason temp;

        public void EditItem(GrItem pItem, GrEditingReason reason)
        {

            temp = reason;

            if (pItem.GetDisplayable() == false)
            {
                pItem.BringIntoView();
            }
            m_pStateManager.ChangeState(GrGridState.ItemEditing, pItem, temp);
        }

        public void BringIntoView(GrItem pItem)
        {
            if (pItem == null)
                return;

            Update();
            m_columnList.BringIntoView(pItem.GetColumn());
            m_pDataRowList.BringIntoView(pItem.GetDataRow());
        }

        public bool SetMouseOver(GrCell pCell, GrPoint localLocation)
        {
            return m_pMouseOverer.SetMouseOver(pCell, localLocation);
        }

        public GrCell GetMouseOver()
        {
            return m_pMouseOverer.GetMouseOver();
        }

        public int GetMouseOverState()
        {
            return m_pMouseOverer.GetMouseOverState();
        }

        public void SetMousePress(GrCell pCell)
        {
            m_pMousePresser.SetMousePress(pCell);
        }

        public void SetMouseUnpress()
        {
            m_pMousePresser.SetMouseUnpress();
        }

        public GrCell GetMousePress()
        {
            return m_pMousePresser.GetMousePress();
        }

        public void Invalidate()
        {
            m_pInvalidator.Invalidate();
        }

        public void Invalidate(int x, int y, int width, int height)
        {
            m_pInvalidator.Invalidate(x, y, width, height);
        }

        public void Invalidate(GrRect rect)
        {
            m_pInvalidator.Invalidate(rect);
        }

        public void LockInvalidate()
        {
            m_pInvalidator.Lock();
        }

        public void UnlockInvalidate()
        {
            m_pInvalidator.Unlock();
        }

        public void ResetInvalidate()
        {
            m_pInvalidator.Reset();
        }

        public bool IsInvalidated()
        {
            return m_pInvalidator.IsInvalidated();
        }

        public bool HitTest(GrPoint location, out GrHitTest pHitTest)
        {
            GrCell pCell = m_pRootRow.HitTest(location);
            pHitTest = new GrHitTest();
            if (pCell != null)
            {
                pHitTest.pHitted = pCell;
                pHitTest.localHit = location - pCell.GetLocation();
                return true;
            }
            return false;
        }

        public GrStyle GetStyle()
        {
            return m_pStyle;
        }

        public void SetStyle(GrStyle pStyle)
        {
            m_pStyle = pStyle;
        }
        
        public void AttachObject(GrObject pObject)
        {
            if (pObject.GridCore != null)
                return;
            pObject.GridCore = this;
            pObject.InvokeOnGridCoreAttached();

            m_attachedCount++;
        }

        public void DetachObject(GrObject pObject)
        {
            if (pObject.GridCore == null)
                return;
            pObject.InvokeOnGridCoreDetached();
            pObject.GridCore = null;
            m_attachedCount--;
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
                m_pStateManager.ChangeDefaultState();
            }
            else if (eventName.CompareTo("FontChanged") == 0)
            {
                OnFontChanged(e);
            }
            else
            {
                m_columnList.Invoke(eventName, e);
            }
        }

        public event EventHandler Created;

        public event EventHandler Cleared;

        public event EventHandler CapacityChanged;

        public event EventHandler FontChanged;

        public event EventHandler DisplayRectChanged;

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

        protected virtual void OnDisplayRectChanged(EventArgs e)
        {
            DisplayRectChanged(this, e);
            Invalidate();
        }

        protected virtual void OnEditValue(GrEditEventArgs e)
        {
            e.GetItem().LockColor(true);
            m_pInvalidator.Invalidate();
            m_pGridWindow.OnEditValue(e);
            e.GetItem().LockColor(false);
            m_pInvalidator.Invalidate();
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
            m_painting = true;
            m_pRootRow.Paint(painter, clipRect);
            m_painting = false;
        }

        protected virtual void PostPaint(GrGridPainter painter, GrRect clipRect)
        {
            m_painting = true;
            m_pStateManager.OnPaint(painter);
            m_painting = false;
        }

        private void focuser_FocusChanging(object sender, GrFocusChangeArgs e)
        {
            if (m_pStateManager.GetGridState() == GrGridState.ItemEditing)
                m_pStateManager.ChangeDefaultState();
        }

        private void columnList_ColumnInserted(object sender, GrColumnEventArgs e)
        {
            m_pTextUpdater.AddTextBounds(m_pCaption);
            m_pTextUpdater.AddTextBounds(m_groupPanel);
        }

        private void columnList_ColumnRemoved(object sender, GrColumnEventArgs e)
        {
            m_pTextUpdater.AddTextBounds(m_pCaption);
            m_pTextUpdater.AddTextBounds(m_groupPanel);
        }

        private void columnList_ColumnWidthChanged(object sender, GrColumnEventArgs e)
        {
            if (m_updating == true)
                return;
            GrColumn column = e.Column;
            if (column.GetItemWordWrap() == true)
                m_pTextUpdater.AddTextBounds(column);
        }

        private void columnList_ColumnWordwrapChanged(object sender, GrColumnEventArgs e)
        {
            if (m_updating == true)
                return;
            GrColumn column = e.Column;
            m_pTextUpdater.AddTextBounds(column);
        }

        private void columnList_ColumnHorzAlignChanged(object sender, GrColumnEventArgs e)
        {
            GrColumn column = e.Column;
            m_pTextUpdater.AddTextAlign(column);
        }

        private void columnList_ColumnVertAlignChanged(object sender, GrColumnEventArgs e)
        {
            GrColumn column = e.Column;
            m_pTextUpdater.AddTextAlign(column);
        }

        private void columnList_ColumnPaddingChanged(object sender, GrColumnEventArgs e)
        {
            GrColumn column = e.Column;
            if (column.GetItemWordWrap() == true)
                m_pTextUpdater.AddTextBounds(column);
        }

        internal void BeginPaint()
        {
            int horz = GetHorzScroll().GetVisible() == true ? GetHorzScroll().GetValue() : GetHorzScroll().GetMinimum();
            int vert = GetVertScroll().GetVisible() == true ? GetVertScroll().GetValue() : GetVertScroll().GetMinimum();
            m_pRootRow.Clip(m_displayRect, horz, vert);
        }

        internal void EndPaint()
        {
            m_pInvalidator.Reset();
        }

        public static GrClickEditing DefaultClickEditing;

        public void Dispose()
        {
            throw new NotImplementedException();
        }
    }
}
