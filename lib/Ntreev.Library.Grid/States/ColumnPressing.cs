﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid.States
{
    class ColumnPressing : GrStateBase
    {
        GrColumnList m_columnList;
        GrColumn m_column;
        GrColumn m_pOldDataColumn;
        bool m_handled;
        GrPoint m_locationStart;
        GrCursor m_cursor;

        GrCell m_targetCell;

        GrSelectionTimer2 m_pTimer;
        TargetType m_targetType;

        public ColumnPressing()
        {
            m_cursor = GrCursor.Default;
            m_column = null;
            m_pOldDataColumn = null;
            m_handled = false;
            m_targetType = TargetType.Unknown;

            m_pTimer = new GrSelectionTimer2();
            m_pTimer.Elapsed += timer_Elapsed;
        }
        //virtual ~ColumnPressing();

        public override bool GetHitTest(GrCell pHitted, GrPoint localLocation)
        {
            if (pHitted.GetCellType() != GrCellType.Column)
                return false;
            GrColumn column = pHitted as GrColumn;

            if (column.GetResizable() == false)
                return true;

            int margin = column.GetResizingMargin();

            if (localLocation.X < margin ||
                localLocation.X >= pHitted.GetWidth() - margin)
                return false;

            if (column.GetClipped() == true)
            {
                int x = localLocation.X + column.GetX();
                if (x >= this.GridCore.GetDisplayRect().Right - margin)
                    return false;
            }
            return true;
        }

        public override void OnMouseDown(GrStateMouseEventArgs e)
            {
        GrColumnList columnList = this.GridCore.GetColumnList();
        m_pOldDataColumn = m_column;
        m_column = e.GetCell() as GrColumn;

        GrColumnMouseEventArgs ce = new GrColumnMouseEventArgs(m_column, e.GetLocalHit(), e.GetModifierKeys());
        columnList.Invoke("ColumnMouseDown", ce);
        m_handled = ce.GetHandled();

        if(m_handled == false)
        {
            if(m_pGridWindow.GetSelectionRange() == GrSelectionRange.Multi)
            {
                m_pItemSelector.SelectColumns(m_column, m_pItemSelector.GetColumnAnchor(), m_pGridWindow.GetSelectionType());
                m_pFocuser.Set(m_column);
            }
            else
            {
                switch(m_pGridWindow.GetSelectionType())
                {
                case GrSelectionType.Normal:
                    {
                        m_pItemSelector.SelectColumn(m_column, GrSelectionType.Normal);
                        m_pItemSelector.SetColumnAnchor(m_column);
                        m_pFocuser.Set(m_column);

                        IDataRow pFocusedRow = m_pFocuser.GetFocusedRow();
                        if(pFocusedRow != null)
                            m_pItemSelector.SetRowAnchor(pFocusedRow);
                    }
                    break;
                default:
                    {
                        m_pItemSelector.SelectColumn(m_column, GrSelectionType.Add);
                        m_pFocuser.Set(m_column);
                    }
                    break;
                }
            }
            this.GridCore.SetMousePress(m_column);
            m_locationStart = e.GetLocation();
        }
        else
        {
            m_locationStart = GrPoint.Empty;
        }

        this.GridCore.Invalidate(m_column.GetRect());
    }

        public override void OnMouseUp(GrStateMouseEventArgs e)
        {
            if (m_handled == true)
            {
                GrColumnList columnList = this.GridCore.GetColumnList();
                GrColumnMouseEventArgs ce = new GrColumnMouseEventArgs(m_column, e.GetLocalHit(), e.GetModifierKeys());
                columnList.Invoke("ColumnMouseUp", ce);
                m_handled = ce.GetHandled();
            }

            if (m_handled == false)
            {
                if (GetSortable(e.GetLocation()) == true)
                {
                    int n = ((int)m_column.GetSortType() + 1) % Enum.GetNames(typeof(GrSort)).Length;
                    GrSort sortType = (GrSort)n;

                    m_column.SetSortType(sortType);

                    this.GridCore.Update();
                    this.GridCore.BringIntoView(m_pFocuser.GetItem());
                }
            }

            m_pTimer.Stop();
            this.GridCore.Invalidate(m_column.GetRect());
            this.GridCore.SetMouseUnpress();
        }

        public override void OnPaintAdornments(GrGridPainter g, GrRect displayRect)
            {
        const int padding = 2;
        switch(m_targetType)
        {
        case TargetType.Frozen:
            {
                int x;
                if(m_targetCell == null)
                {
                    x = m_columnList.GetColumnSplitter().GetX();
                }
                else
                {
                    x = (m_targetCell as GrColumn).GetX();
                }
                GrRect paintRect = GrRect.FromLTRB(x - padding, m_columnList.GetY(), x + padding, m_columnList.GetY() + m_columnList.GetHeight());
                g.FillRectangle(paintRect, GrColor.Black);
            }
            break;
        case TargetType.Unfrozen:
            {
                int x;
                if(m_targetCell == null)
                {
                    x = Math.Min(displayRect.Right, m_columnList.GetBounds().Right);
                }
                else
                {
                    x = (m_targetCell as GrColumn).GetX();
                }
                GrRect paintRect = GrRect.FromLTRB(x - padding, m_columnList.GetY(), x + padding, m_columnList.GetY() + m_columnList.GetHeight());
                g.FillRectangle(paintRect, GrColor.Black);
            }
            break;
        case TargetType.GroupList:
            {
                if(m_targetCell != null)
                {
                    int x = m_targetCell.GetX();
                    GrRect paintRect = GrRect.FromLTRB(x - padding, m_targetCell.GetY(), x + padding, m_targetCell.GetY() + m_targetCell.GetHeight());
                    g.FillRectangle(paintRect, GrColor.Black);
                }
            }
            break;
        default:
            return;
        }
    }

        public override void OnMouseDragBegin(GrPoint location)
        {
            m_pTimer.Start();
        }

        public override void OnMouseDragMove(GrPoint location, GrHitTest hitTest)
        {
            m_cursor = GrCursor.No;

            if (hitTest.pHitted == null)
                return;

            m_pTimer.SetMouseLocation(location);

            m_targetType = TargetType.Unknown;

            switch (hitTest.pHitted.GetCellType())
            {
                case GrCellType.Column:
                    {
                        if (this.GridCore.GetColumnMovable() == false || m_column.GetMovable() == false)
                            break;

                        if (m_pTimer.CanHScroll() == true)
                            break;

                        GrColumn pTarget = hitTest.pHitted as GrColumn;
                        if (pTarget != m_column)
                        {
                            if (pTarget.GetFrozen() == true)
                            {
                                int targetIndex;
                                if (hitTest.localHit.X < pTarget.GetWidth() / 2)
                                    targetIndex = pTarget.GetFrozenIndex();
                                else
                                    targetIndex = pTarget.GetFrozenIndex() + 1;

                                if (m_column.GetFrozen() == false)
                                {
                                    if (this.GridCore.GetColumnFreezable() == false)
                                        m_targetType = TargetType.Unknown;
                                    else
                                        m_targetType = TargetType.Frozen;
                                }
                                else if (targetIndex != m_column.GetFrozenIndex() + 1 && targetIndex != m_column.GetFrozenIndex())
                                {
                                    m_targetType = TargetType.Frozen;
                                }
                                else
                                {
                                    m_targetType = TargetType.Unknown;
                                }

                                if (targetIndex < m_columnList.GetFrozenColumnCount())
                                    m_targetCell = m_columnList.GetFrozenColumn(targetIndex);
                                else
                                    m_targetCell = null;
                            }
                            else
                            {
                                int targetIndex;
                                if (hitTest.localHit.X < pTarget.GetWidth() / 2)
                                    targetIndex = pTarget.GetUnfrozenIndex();
                                else
                                    targetIndex = pTarget.GetUnfrozenIndex() + 1;

                                if (m_column.GetFrozen() == true)
                                {
                                    if (this.GridCore.GetColumnFreezable() == false)
                                        m_targetType = TargetType.Unknown;
                                    else
                                        m_targetType = TargetType.Unfrozen;
                                }
                                else if (targetIndex != m_column.GetUnfrozenIndex() + 1 && targetIndex != m_column.GetUnfrozenIndex())
                                {
                                    m_targetType = TargetType.Unfrozen;
                                }
                                else
                                {
                                    m_targetType = TargetType.Unknown;
                                }

                                if (targetIndex < m_columnList.GetUnfrozenColumnCount())
                                    m_targetCell = m_columnList.GetUnfrozenColumn(targetIndex);
                                else
                                    m_targetCell = null;
                            }
                        }
                    }
                    break;
                case GrCellType.Group:
                    {
                        if (m_column.GetGroupable() == false)
                            break;

                        if (m_column.GetGrouped() == false)
                        {
                            m_targetCell = hitTest.pHitted as GrGroup;
                            m_targetType = TargetType.GroupList;
                        }
                    }
                    break;
                case GrCellType.Row:
                    {
                        if (m_column.GetGroupable() == false)
                            break;

                        if (m_column.GetGrouped() == false && hitTest.pHitted is GrGroupPanel)
                        {
                            m_targetCell = null;
                            m_targetType = TargetType.GroupList;
                        }
                    }
                    break;
                default:
                    break;
            }

            switch (m_targetType)
            {
                case TargetType.GroupList:
                    {
                        m_cursor = GrCursor.Add;
                    }
                    break;
                case TargetType.Frozen:
                case TargetType.Unfrozen:
                    {
                        m_cursor = GrCursor.Move;
                    }
                    break;
                default:
                    break;
            }

            this.GridCore.Invalidate();
        }

        public override void OnMouseDragEnd(bool cancel, GrHitTest hitTest)
        {
            if (cancel == false)
            {
                switch (m_targetType)
                {
                    case TargetType.Frozen:
                        {
                            GrColumn pTarget = m_targetCell as GrColumn;
                            int index = m_columnList.GetFrozenColumnCount();
                            if (pTarget != null)
                                index = pTarget.GetVisibleIndex();

                            m_column.SetVisibleIndex(index);
                            m_column.SetFrozen(true);
                        }
                        break;
                    case TargetType.Unfrozen:
                        {
                            int index = m_columnList.GetVisibleColumnCount();
                            GrColumn pTarget = m_targetCell as GrColumn;
                            if (pTarget != null)
                                index = pTarget.GetVisibleIndex();

                            if (index > m_column.GetVisibleIndex())
                                index--;

                            m_column.SetVisibleIndex(index);
                            m_column.SetFrozen(false);
                        }
                        break;
                    case TargetType.GroupList:
                        {
                            if (m_targetCell != null)
                            {
                                GrGroup pTargetGroupInfo = m_targetCell as GrGroup;
                                GrGroup pGroup = m_column.GetGroup();
                                pGroup.SetGroupLevel(pTargetGroupInfo.GetGroupLevel());
                            }
                            m_column.SetGrouped(true);
                        }
                        break;
                    default:
                        break;
                }
            }
            m_targetType = TargetType.Unknown;
            m_cursor = GrCursor.Default;
            m_pTimer.Stop();
            this.GridCore.Invalidate();
        }

        public override GrGridState GetState() { return GrGridState.ColumnPressing; }

        public override bool GetDragable() { return true; }

        public override GrCursor GetCursor() { return m_cursor; }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            m_columnList = this.GridCore.GetColumnList();
            this.GridCore.AttachObject(m_pTimer);
        }

        private void timer_Elapsed(object sender, GrElapsedEventArgs e)
        {
            m_pTimer.DoScroll();
            this.GridCore.Invalidate();
        }

        private bool GetSortable(GrPoint location)
        {
            if (this.GridCore.GetColumnSortable() == false)
                return false;

            if (m_targetType == TargetType.Unknown &&
                m_pOldDataColumn == m_column &&
                location == m_locationStart)
            {
                return true;
            }
            return false;
        }

        enum TargetType
        {
            Unknown,

            Frozen,
            Unfrozen,
            GroupList,
        }

        class GrSelectionTimer2 : GrSelectionTimer
        {
            public GrSelectionTimer2()
            {
                SetVScrollEnabled(false);
                SetIntervalAcceleration(false);
                SetScrollAcceleration(false);
            }

            protected override GrRect GetInsideRectangle()
            {
                GrColumnList columnList = this.GridCore.GetColumnList();
                const int gap = 20;
                GrRect rect = this.GridCore.GetDataRect();
                return GrRect.FromLTRB(rect.Left + gap, columnList.GetY(), rect.Right - gap, rect.Bottom);
            }
            protected override GrRect GetOutsideRectangle()
            {
                GrColumnList columnList = this.GridCore.GetColumnList();
                GrRect rect = this.GridCore.GetDataRect();
                return GrRect.FromLTRB(rect.Left, columnList.GetY(), rect.Right, rect.Bottom);
            }
        }
    }
}
