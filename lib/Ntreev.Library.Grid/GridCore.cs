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
        GrGroupPanel m_pGroupPanel;
        GrColumnList m_pColumnList;
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
            m_pGroupPanel = new GrGroupPanel();
            m_pColumnList = new GrColumnList();
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
            AttachObject(m_pGroupPanel);
            AttachObject(m_pColumnList);
            AttachObject(m_pDataRowList);
            AttachObject(m_pSplitterRow);
            AttachObject(m_pStateManager);
            AttachObject(m_pGridWindow);
            AttachObject(m_pFocusMover);

            m_pRootRow.AddChild(m_pCaption);
            m_pRootRow.AddChild(m_pGroupPanel);
            m_pRootRow.AddChild(m_pColumnList);
            m_pRootRow.AddChild(m_pDataRowList.GetInsertionRow());
            m_pRootRow.AddChild(m_pSplitterRow);
            m_pRootRow.AddChild(m_pDataRowList);

            m_pColumnList.ColumnInserted += columnList_ColumnInserted;
            m_pColumnList.ColumnRemoved += columnList_ColumnRemoved;
            m_pColumnList.ColumnWidthChanged += columnList_ColumnWidthChanged;
            m_pColumnList.ColumnWordwrapChanged += columnList_ColumnWordwrapChanged;
            m_pColumnList.ColumnHorzAlignChanged += columnList_ColumnHorzAlignChanged;
            m_pColumnList.ColumnVertAlignChanged += columnList_ColumnVertAlignChanged;
            m_pColumnList.ColumnPaddingChanged += columnList_ColumnPaddingChanged;

            m_pFocuser.FocusChanging += focuser_FocusChanging;

            this.Created(this, EventArgs.Empty);
        }


        public void Reserve(int columnCount, int rowCount)
        {
            m_reservedColumn = columnCount + 2;
            m_reservedRow = rowCount + 2;

            m_pDataRowList.Reserve(m_reservedRow);
            m_pColumnList.Reserve(m_reservedColumn);

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
        public GrColumnList GetColumnList() { return m_pColumnList; }
        public GrDataRow GetInsertionRow() { return m_pDataRowList.GetInsertionRow(); }
        public GrCaption GetCaptionRow() { return m_pCaption; }
        public GrGroupPanel GetGroupPanel() { return m_pGroupPanel; }
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
            uint horz = GetHorzScroll().GetVisible() == true ? GetHorzScroll().GetValue() : GetHorzScroll().GetMinimum();
            uint vert = GetVertScroll().GetVisible() == true ? GetVertScroll().GetValue() : GetVertScroll().GetMinimum();
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
            GrRect dataRect;
            dataRect.Left = m_pColumnList.GetUnfrozenX();
            dataRect.Top = m_pDataRowList.GetY();
            dataRect.Right = Math.Min(GetBounds().Right, m_displayRect.Right);
            dataRect.bottom = Math.Min(GetBounds().Bottom, m_displayRect.Bottom);
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
                m_pColumnList.SetWidthChanged();
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
            m_pColumnList.SetVisibleChanged();
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
            return m_pGroupPanel.GetGroupCount() > 0 ? true : false;
        }

        public bool IsUpdating() { return m_updating; }

        public bool GetGroupable()
        {
            return m_pGroupPanel.GetGroupable();
        }
        public void SetGroupable(bool b)
        {
            m_pGroupPanel.SetGroupable(b);
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
        bool GetRowMovable()
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
        void SetBackColor(GrColor backColor)
        {
            if (m_backColor == backColor)
                return;
            m_backColor = backColor;
            Invalidate();
        }
        void SetLineColor(GrColor lineColor)
        {
            if (m_lineColor == lineColor)
                return;
            m_lineColor = lineColor;
            Invalidate();
        }
        void SetFont(GrFont pFont)
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
            m_pColumnList.BringIntoView(pItem.GetColumn());
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

        public bool HitTest(GrPoint location, GrHitTest pHitTest)
        {
            GrCell pCell = m_pRootRow.HitTest(location);

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
            if (pObject.m_pGridCore != null)
                return;
            pObject.m_pGridCore = this;
            pObject.OnGridCoreAttached();

            m_attachedCount++;
        }
        public void DetachObject(GrObject pObject)
        {
            if (pObject.m_pGridCore == null)
                return;
            pObject.OnGridCoreDetached();
            pObject.m_pGridCore = null;
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
                m_pColumnList.Invoke(eventName, e);
            }
        }

        public event EventHandler Created;
        public event EventHandler Cleared;
        public event EventHandler CapacityChanged;
        public event EventHandler FontChanged;
        public event EventHandler DisplayRectChanged;

        public event ItemMouseEventHandler ItemMouseEnter;
        public event ItemMouseEventHandler ItemMouseMove;
        public event ItemMouseEventHandler ItemMouseLeave;
        public event ItemMouseEventHandler ItemMouseClick;
        public event ItemMouseEventHandler ItemMouseDoubleClick;

        public event RowMouseEventHandler RowMouseEnter;
        public event RowMouseEventHandler RowMouseMove;
        public event RowMouseEventHandler RowMouseLeave;


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
            ItemMouseEnter(this, e);
        }
        protected virtual void OnItemMouseMove(GrItemMouseEventArgs e)
        {
            ItemMouseMove(this, e);
        }

        protected virtual void OnItemMouseLeave(GrItemMouseEventArgs e)
        {
            ItemMouseLeave(this, e);
        }
        protected virtual void OnItemMouseClick(GrItemMouseEventArgs e)
        {
            ItemMouseClick(this, e);
        }
        protected virtual void OnItemMouseDoubleClick(GrItemMouseEventArgs e)
        {
            ItemMouseDoubleClick(this, e);
        }

        protected virtual void OnRowMouseEnter(GrRowMouseEventArgs e)
        {
            RowMouseEnter(this, e);
        }
        protected virtual void OnRowMouseMove(GrRowMouseEventArgs e)
        {
            RowMouseMove(this, e);
        }
        protected virtual void OnRowMouseLeave(GrRowMouseEventArgs e)
        {
            RowMouseLeave(this, e);
        }


        protected virtual void PrePaint(GrGridPainter pPainter, GrRect clipRect)
        {

        }
        protected virtual void Paint(GrGridPainter pPainter, GrRect clipRect)
        {
            m_painting = true;
            m_pRootRow.Paint(pPainter, clipRect);
            m_painting = false;
        }

        protected virtual void PostPaint(GrGridPainter pPainter, GrRect clipRect)
        {
            m_painting = true;
            m_pStateManager.OnPaint(pPainter);
            m_painting = false;
        }



        void focuser_FocusChanging(object sender, GrFocusChangeArgs e)
        {
            if (m_pStateManager.GetGridState() == GrGridState.ItemEditing)
                m_pStateManager.ChangeDefaultState();
        }

        void columnList_ColumnInserted(object sender, GrColumnEventArgs e)
        {
            m_pTextUpdater.AddTextBounds(m_pCaption);
            m_pTextUpdater.AddTextBounds(m_pGroupPanel);
        }

        void columnList_ColumnRemoved(object sender, GrColumnEventArgs e)
        {
            m_pTextUpdater.AddTextBounds(m_pCaption);
            m_pTextUpdater.AddTextBounds(m_pGroupPanel);
        }
        void columnList_ColumnWidthChanged(object sender, GrColumnEventArgs e)
        {
            if (m_updating == true)
                return;
            GrColumn pColumn = e.GetColumn();
            if (pColumn.GetItemWordWrap() == true)
                m_pTextUpdater.AddTextBounds(pColumn);
        }
        void columnList_ColumnWordwrapChanged(object sender, GrColumnEventArgs e)
        {
            if (m_updating == true)
                return;
            GrColumn pColumn = e.GetColumn();
            m_pTextUpdater.AddTextBounds(pColumn);
        }
        void columnList_ColumnHorzAlignChanged(object sender, GrColumnEventArgs e)
        {
            GrColumn pColumn = e.GetColumn();
            m_pTextUpdater.AddTextAlign(pColumn);
        }
        void columnList_ColumnVertAlignChanged(object sender, GrColumnEventArgs e)
        {
            GrColumn pColumn = e.GetColumn();
            m_pTextUpdater.AddTextAlign(pColumn);
        }
        void columnList_ColumnPaddingChanged(object sender, GrColumnEventArgs e)
        {
            GrColumn pColumn = e.GetColumn();
            if (pColumn.GetItemWordWrap() == true)
                m_pTextUpdater.AddTextBounds(pColumn);
        }

        void BeginPaint()
        {
            int horz = GetHorzScroll().GetVisible() == true ? GetHorzScroll().GetValue() : GetHorzScroll().GetMinimum();
            int vert = GetVertScroll().GetVisible() == true ? GetVertScroll().GetValue() : GetVertScroll().GetMinimum();
            m_pRootRow.Clip(m_displayRect, horz, vert);
        }
        void EndPaint()
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
