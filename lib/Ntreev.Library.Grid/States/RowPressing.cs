using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid.States
{
    class RowPressing : GrStateBase
    {
        GrRow m_row;
        //bool m_handled;

        GrDataRowList m_pDataRowList;
        GrColumnList m_columnList;

        GrDataRow m_pDataRow;
        GrDataRow m_pTargetDataRow;
        int m_targetIndex;

        GrSelectionTimer m_pTimer;

        public RowPressing()
        {
            m_pTimer = new GrSelectionTimer();
            m_pTimer.Elapsed += timer_Elapsed;
            m_pTargetDataRow = null;
            m_targetIndex = GrDefineUtility.INVALID_INDEX;
        }

        public override bool GetHitTest(GrCell pHitted, GrPoint localLocation)
        {
            if (pHitted is GrRow == false)
                return false;
            GrRow row = pHitted as GrRow;
            int margin = row.GetResizingMargin();
            if (localLocation.Y < margin || localLocation.Y >= pHitted.Height - margin)
                return false;
            return true;
        }

        public override void OnBegin(GrStateEventArgs e)
        {

        }

        public override void OnEnd(GrStateEventArgs e)
        {
            m_pTargetDataRow = null;
            m_targetIndex = GrDefineUtility.INVALID_INDEX;
        }

        public override void OnMouseDown(GrStateMouseEventArgs e)
        {
            m_row = e.GetCell() as GrRow;
            m_pDataRow = m_row as GrDataRow;

            switch (m_row.GetRowType())
            {
                case GrRowType.DataRow:
                case GrRowType.InsertionRow:
                    {
                        GrDataRow pDataRow = m_row as GrDataRow;

                        if (m_pGridWindow.GetSelectionRange() == GrSelectionRange.Multi)
                        {
                            if (m_pItemSelector.CanSelect(pDataRow) == true)
                            {
                                m_pItemSelector.SelectDataRows(pDataRow, m_pItemSelector.GetRowAnchor(), m_pGridWindow.GetSelectionType());
                                m_pFocuser.Set(pDataRow);
                            }
                        }
                        else
                        {
                            switch (m_pGridWindow.GetSelectionType())
                            {
                                case GrSelectionType.Normal:
                                    {
                                        m_pItemSelector.SetSelectionGroup(pDataRow);
                                        m_pItemSelector.SetRowAnchor(pDataRow);
                                        m_pItemSelector.SelectDataRow(pDataRow, GrSelectionType.Normal);
                                        m_pFocuser.Set(pDataRow);
                                    }
                                    break;
                                default:
                                    {
                                        if (m_pItemSelector.CanSelect(pDataRow) == true)
                                        {
                                            m_pItemSelector.SelectDataRow(pDataRow, GrSelectionType.Add);
                                            m_pFocuser.Set(pDataRow);
                                        }
                                    }
                                    break;
                            }
                        }
                    }
                    break;
                case GrRowType.GroupRow:
                    {
                        GrGroupRow groupRow = m_row as GrGroupRow;
                        if (m_pGridWindow.GetSelectionRange() == GrSelectionRange.Multi)
                        {
                            if (m_pItemSelector.CanSelect(groupRow) == true)
                            {
                                m_pItemSelector.SelectDataRows(groupRow, m_pItemSelector.GetRowAnchor(), m_pGridWindow.GetSelectionType());
                                m_pFocuser.Set(groupRow);
                            }
                        }
                        else
                        {
                            switch (m_pGridWindow.GetSelectionType())
                            {
                                case GrSelectionType.Normal:
                                    {
                                        m_pItemSelector.SetSelectionGroup(groupRow);
                                        m_pItemSelector.SetRowAnchor(groupRow);
                                        m_pItemSelector.ClearSelection();
                                        m_pFocuser.Set(groupRow);
                                    }
                                    break;
                                default:
                                    {
                                        if (m_pItemSelector.CanSelect(groupRow) == true)
                                        {
                                            m_pFocuser.Set(groupRow);
                                        }
                                    }
                                    break;
                            }
                        }
                    }
                    break;
                default:
                    {
                        IDataRow pDataRow = m_row as IDataRow;
                        if (pDataRow != null)
                        {
                            m_pItemSelector.ClearSelection();
                            m_pFocuser.Set(pDataRow);
                        }
                    }
                    break;
            }
            this.GridCore.SetMousePress(m_row);
        }

        public override void OnMouseUp(GrStateMouseEventArgs e)
        {
            this.GridCore.SetMouseUnpress();
        }

        public override void OnPaintAdornments(GrGridPainter g, GrRect displayRect)
        {
            if (m_targetIndex == GrDefineUtility.INVALID_INDEX)
                return;

            int top = 0;

            if (m_targetIndex == m_pDataRowList.GetDataRowCount())
                top = this.GridCore.GetDataRect().Bottom;
            else
                top = m_pTargetDataRow.Y;

            GrRect paintRect = GrRect.FromLTRB(m_pDataRowList.X, top - 1, displayRect.Right, top + 1);
            g.FillRectangle(paintRect, GrColor.Black);
        }

        public override void OnMouseDragBegin(GrPoint location)
        {
            m_pTimer.Start();
        }

        public override void OnMouseDragMove(GrPoint location, GrHitTest hitTest)
        {
            if (hitTest.pHitted == null)
            {
                GrRect dataRect = this.GridCore.GetDataRect();
                m_pTargetDataRow = null;

                if (location.Y >= dataRect.Bottom)
                {
                    m_targetIndex = m_pDataRowList.GetDataRowCount();
                }
                else if (location.Y <= dataRect.Top)
                {
                    m_targetIndex = 0;
                    m_pTargetDataRow = m_pDataRowList.GetDataRow(m_targetIndex);
                }
                else
                {
                    m_targetIndex = GrDefineUtility.INVALID_INDEX;
                }
            }
            else
            {
                GrDataRow pDataRow = hitTest.pHitted.GetRow() as GrDataRow;

                if (pDataRow != null && pDataRow != m_pDataRowList.InsertionRow)
                    m_pTargetDataRow = pDataRow;

                if (m_pTargetDataRow == null)
                    m_targetIndex = GrDefineUtility.INVALID_INDEX;
                else
                    m_targetIndex = m_pTargetDataRow.GetDataRowIndex();
            }

            m_pTimer.SetMouseLocation(m_pGridWindow.ClientToScreen(location));
            this.GridCore.Invalidate();
        }

        public override void OnMouseDragEnd(bool cancel, GrHitTest hitTest)
        {
            m_pTimer.Stop();
            if (cancel == false && m_targetIndex != GrDefineUtility.INVALID_INDEX)
            {
                int index = m_pDataRow.GetDataRowIndex();
                int targetIndex = m_targetIndex;
                if (index < m_targetIndex)
                    targetIndex--;

                m_pDataRow.SetDataRowIndex(targetIndex);
            }
            this.GridCore.Invalidate();
        }

        public override GrGridState GetState() { return GrGridState.RowPressing; }

        public override bool GetDragable()
        {
            if (m_pDataRow == null || m_pDataRow == m_pDataRowList.InsertionRow)
                return false;

            if (this.GridCore.IsGrouped() == true || this.GridCore.GetRowMovable() == false)
                return false;

            GrColumn column = m_columnList.GetFirstSortColumn();
            if (column != null && column.GetSortType() != GrSort.None)
                return false;

            return true;
        }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            m_pDataRowList = this.GridCore.DataRowList;
            m_columnList = this.GridCore.ColumnList;
            this.GridCore.AttachObject(m_pTimer);
        }

        private void timer_Elapsed(object sender, GrElapsedEventArgs e)
        {
            m_pTimer.DoScroll();
            this.GridCore.Invalidate();
        }
    }
}
