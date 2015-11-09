using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid.States
{
    class ItemPressing : GrStateBase
    {
        GrItem m_pItem;
        GrSelectionTimer m_pTimer;
        GrPoint m_location;

        public ItemPressing()
        {
            m_pTimer = new GrSelectionTimer();
        }

        public override bool GetHitTest(GrCell pHitted, GrPoint localLocation)
        {
            if (pHitted is GrItem == false)
                return false;

            GrItem pItem = pHitted as GrItem;
            if (pItem.HitMouseOverTest(localLocation) != (int)GrMouseOverState.In)
                return false;

            if (pItem.IsReadOnly == true)
                return true;

            GrClickEditing editingType = pItem.Column.ItemClickEditing;
            if (editingType == GrClickEditing.Default)
            {
                editingType = this.GridCore.GetClickEditing();
            }

            switch (editingType)
            {
                case GrClickEditing.Click:
                case GrClickEditing.DoubleClick:
                    {
                        GrEditingReason reason = new GrEditingReason(localLocation);
                        if (m_pGridWindow.CanEdit(pItem, reason) == false)
                            return true;
                    }
                    break;
                case GrClickEditing.FocusedClick:
                    {
                        if (pItem.IsFocused == true)
                            return false;
                    }
                    break;
            }


            //if(pItem.GetFocused() == true)
            // return false;

            return true;
        }

        public override void OnMouseDown(GrStateMouseEventArgs e)
        {
            m_pItem = e.GetCell() as GrItem;

            if (m_pGridWindow.GetSelectionRange() == GrSelectionRange.Multi)
            {
                if (m_pItemSelector.CanSelect(m_pItem) == true)
                {
                    GrRange columnRange = m_pItemSelector.GetColumnSelections(m_pItem);
                    GrRange rowRange = m_pItemSelector.GetRowSelections(m_pItem);

                    if (rowRange != GrRange.Empty)
                    {
                        m_pItemSelector.BeginSelecting();
                        m_pItemSelector.Selecting(columnRange, rowRange);
                        m_pFocuser.SetFocusing(m_pItem);
                    }
                }
            }
            else
            {
                switch (m_pGridWindow.GetSelectionType())
                {
                    case GrSelectionType.Add:
                        {
                            if (m_pItemSelector.CanSelect(m_pItem) == true)
                            {
                                if (this.GridCore.GetFullRowSelect() == true)
                                    m_pItemSelector.SelectDataRow(m_pItem.DataRow, GrSelectionType.Add);
                                else
                                    m_pItemSelector.SelectItem(m_pItem, GrSelectionType.Add);
                                m_pItemSelector.SetAnchor(m_pItem);
                                m_pFocuser.Set(m_pItem);
                            }
                        }
                        break;
                    case GrSelectionType.Normal:
                        {
                            m_pItemSelector.SetSelectionGroup(m_pItem);
                            m_pItemSelector.SetAnchor(m_pItem);
                            if (this.GridCore.GetFullRowSelect() == true)
                                m_pItemSelector.SelectDataRow(m_pItem.DataRow, GrSelectionType.Normal);
                            else
                                m_pItemSelector.SelectItem(m_pItem, GrSelectionType.Normal);
                            m_pFocuser.Set(m_pItem);

                            if (m_pItem.IsReadOnly == false)
                            {
                                //static GrEditingReason editingReason;
                                GrEditingReason editingReason = new GrEditingReason();
                                GrClickEditing clickEdit = m_pItem.Column.ItemClickEditing;
                                if (clickEdit == GrClickEditing.Default)
                                    clickEdit = this.GridCore.GetClickEditing();

                                GrEditingReason reason = new GrEditingReason(e.GetLocalHit());
                                if (clickEdit == GrClickEditing.Click && m_pGridWindow.CanEdit(m_pItem, reason) == true)
                                {
                                    editingReason.editingType = GrEditingType.Mouse;
                                    editingReason.location = e.GetLocation();
                                    e.SetNextState(GrGridState.ItemEditing);
                                    e.SetUserData(editingReason);
                                }
                            }
                        }
                        break;
                    default:
                        break;
                }

                this.GridCore.SetMousePress(m_pItem);
                if (e.GetNextState() == GrGridState.ItemPressing)
                    this.GridCore.BringIntoView(m_pFocuser.GetItem());
            }
        }

        public override void OnMouseClick(GrStateMouseEventArgs e)
        {
            GrItemMouseEventArgs e1 = new GrItemMouseEventArgs(m_pItem, e.GetLocation(), e.GetModifierKeys());
            this.GridCore.Invoke("ItemMouseClick", e1);
        }

        public override void OnMouseDoubleClick(GrStateMouseEventArgs e)
        {
            GrEditingReason reason = new GrEditingReason(e.GetLocalHit());
            if (m_pItem.IsReadOnly == true || m_pGridWindow.CanEdit(m_pItem, reason) == false)
            {
                GrItemMouseEventArgs e1 = new GrItemMouseEventArgs(m_pItem, e.GetLocation(), e.GetModifierKeys());
                this.GridCore.Invoke("ItemMouseDoubleClick", e1);
            }
        }

        public override void OnMouseUp(GrStateMouseEventArgs e)
        {
            if (m_pItemSelector.IsSelecting())
            {
                m_pItemSelector.EndSelecting(m_pGridWindow.GetSelectionType());
                this.GridCore.BringIntoView(m_pFocuser.GetItem());
            }
        }

        public override void OnMouseDragBegin(GrPoint location)
        {
            if (m_pItemSelector.IsSelecting() == false)
                m_pItemSelector.BeginSelecting();

            switch (m_pGridWindow.GetSelectionType())
            {
                case GrSelectionType.Add:
                    break;
                case GrSelectionType.Normal:
                    //m_pItemSelector.ClearSelection();
                    break;
                default:
                    break;
            }

            this.GridCore.SetMouseOver(null, GrPoint.Empty);
            m_pTimer.Start();
        }

        public override void OnMouseDragMove(GrPoint location, GrHitTest hitTest)
        {
            FindSelection fs = new FindSelection(this.GridCore, location);
            m_pItemSelector.Selecting(fs.GetColumnRange(), fs.GetRowRange());
            m_pFocuser.SetFocusing(fs.GetFocus());
            m_pTimer.SetMouseLocation(m_pGridWindow.ClientToScreen(location));
            m_location = location;
        }

        public override void OnMouseDragEnd(bool cancel, GrHitTest hitTest)
        {
            m_pTimer.Stop();

            if (m_pItemSelector.IsSelecting())
            {
                m_pItemSelector.EndSelecting(m_pGridWindow.GetSelectionType());
                this.GridCore.BringIntoView(m_pFocuser.GetItem());
            }
        }

        public override GrGridState GetState() { return GrGridState.ItemPressing; }

        public override bool GetDragable() { return true; }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            this.GridCore.AttachObject(m_pTimer);

            m_pTimer.SetInterval(new TimeSpan(300));
            m_pTimer.Elapsed += timer_Elapsed;
        }

        private void timer_Elapsed(object sender, GrElapsedEventArgs e)
        {
            if (m_pTimer.DoScroll() == true)
            {
                FindSelection fs = new FindSelection(this.GridCore, m_location);

                m_pItemSelector.Selecting(fs.GetColumnRange(), fs.GetRowRange());
                m_pFocuser.SetFocusing(fs.GetFocus());
                this.GridCore.Invalidate();
            }
        }

        class FindSelection
        {
            IFocusable m_pFocus;
            int m_columnBegin;
            int m_columnEnd;
            int m_rowBegin;
            int m_rowEnd;

            GrItemSelector m_pItemSelector;
            GrFocuser m_pFocuser;
            GrGridCore m_pGridCore;

            public FindSelection(GrGridCore pGridCore, GrPoint location)
            {
                m_pGridCore = pGridCore;
                m_pItemSelector = pGridCore.ItemSelector;
                m_pFocuser = pGridCore.Focuser;


                GrRange columnRange = m_pGridCore.ColumnList.HitTest(location.X, m_pItemSelector.GetColumnAnchor());
                GrRange rowRange = m_pGridCore.DataRowList.HitTest(location.Y, m_pItemSelector.GetRowAnchor());

                m_columnBegin = columnRange.Minimum;
                m_columnEnd = columnRange.Maximum;
                m_rowBegin = rowRange.Minimum;
                m_rowEnd = rowRange.Maximum;

                GrColumn pFocusColumn = GetFocusColumn(m_columnBegin, m_columnEnd);
                IDataRow pFocusRow = GetFocusRow(m_rowBegin, m_rowEnd);

                m_pFocus = pFocusRow.GetFocusable(pFocusColumn);
            }

            public IFocusable GetFocus() { return m_pFocus; }

            public GrRange GetColumnRange() { return new GrRange(m_columnBegin, m_columnEnd); }

            public GrRange GetRowRange() { return new GrRange(m_rowBegin, m_rowEnd); }

            private IDataRow GetFocusRow(int begin, int end)
            {
                GrDataRowList dataRowList = m_pGridCore.DataRowList;
                IDataRow pAnchorRow = m_pItemSelector.GetRowAnchor();
                if (pAnchorRow.GetVisibleIndex() == begin)
                    return dataRowList.GetVisibleRow(end - 1);
                return dataRowList.GetVisibleRow(begin);
            }

            private GrColumn GetFocusColumn(int begin, int end)
            {
                GrColumnList columnList = m_pGridCore.ColumnList;
                GrColumn pAnchorColumn = m_pItemSelector.GetColumnAnchor();

                if (pAnchorColumn.GetVisibleIndex() == begin)
                    return columnList.GetVisibleColumn(end - 1);
                return columnList.GetVisibleColumn(begin);
            }
        }
    }
}
