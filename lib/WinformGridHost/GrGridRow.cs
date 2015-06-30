using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    class GrGridRow : IDataRow
    {
        private GrGridCell m_pCell;

        public GridControl m_parentGrid;
        public PropertyDescriptor m_propertyDescriptor;
        public Row m_parentRow;
        public GridControl m_gridControl;

        public GrGridRow(GridControl gridControl, PropertyDescriptor propertyDescriptor, Row parentRow)
        {
            m_parentGrid = gridControl;

            m_propertyDescriptor = propertyDescriptor;
            m_parentRow = parentRow;

            m_pCell = new GrGridCell(this);
        }

        public override IFocusable GetFocusable(GrColumn pColumn)
        {
            return m_pCell;
        }

        public override void Paint(GrGridPainter pPainter, GrRect clipRect)
        {
            GrRect paintRect = this.Bounds;
            GrPaintStyle paintStyle = ToPaintStyle();
            GrColor foreColor = GetPaintingForeColor();
            GrColor backColor = GetPaintingBackColor();

            if (this.GridCore.GetRowVisible() == true)
            {
                if (GetClipped() == true)
                    pPainter.DrawRow(paintStyle, paintRect, GetPaintingLineColor(), backColor, clipRect);
                else
                    pPainter.DrawRow(paintStyle, paintRect, GetPaintingLineColor(), backColor, null);

                if (m_pDataRowList.GetRowNumberVisible() == true)
                {
                    if (GetClipped() == true)
                        DrawText(pPainter, foreColor, paintRect, clipRect);
                    else
                        DrawText(pPainter, foreColor, paintRect, null);
                }
            }

            int left = paintRect.Right;
            int top = paintRect.Top;
            int right = m_pCell.X;
            int bottom = paintRect.Top;

            //paintRect.left = paintRect.right;
            //paintRect.right = m_pCell.GetX();
            paintRect = GrRect.FromLTRB(left, top, right, bottom);

            pPainter.DrawItem(GrPaintStyle.BottomLine, paintRect, GetCellLineColor(), GetCellBackColor(), null);

            m_pCell.Paint(pPainter, clipRect);
        }

        public override int GetMinHeight()
        {
            int borderSize = m_gridControl.Height - m_gridControl.ClientSize.Height;
            GrGridCore pGridCore = m_gridControl.GridCore;
            GrRect rect = pGridCore.GetVisibleBounds();

            return rect.Height + m_gridControl.Padding.Vertical + borderSize + m_pCell.GetPadding().Vertical;
        }

        public bool SetFocus()
        {
            if (this.HasGridControl() == true && m_gridControl.FocusedCell == null)
            {
                m_gridControl.ClearSelection();
                m_gridControl.FocusedCell = m_gridControl.GetFirstVisibleCell();
                m_gridControl.Select();
                return true;
            }
            return false;
        }

        public PropertyDescriptor GetPropertyDescriptor()
        {
            return m_propertyDescriptor;
        }

        public Row GetParentRow()
        {
            return m_parentRow;
        }

        public GridControl GetChildGrid()
        {
            return m_gridControl;
        }

        public void Update()
        {
            if (this.HasGridControl() == true)
            {
                Row row = m_parentRow;
                PropertyDescriptor propertyDescriptor = m_propertyDescriptor;
                System.Object value = propertyDescriptor.GetValue(row.Component);

                m_gridControl.DataSource = value;
                m_gridControl.Refresh();
                this.Height = GetMinHeight();
            }
        }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            this.GridCore.AttachObject(m_pCell);
            GrFocuser pFocuser = this.GridCore.Focuser;
            pFocuser.FocusChanged += focuser_FocusChanged;
        }

        protected override void OnGridCoreDetached()
        {
            GrFocuser pFocuser = this.GridCore.Focuser;
            pFocuser.FocusChanged += focuser_FocusChanged;
            this.GridCore.DetachObject(m_pCell);
            base.OnGridCoreDetached();
        }

        protected override void OnYChanged()
        {
            base.OnYChanged();
            if (this.HasGridControl() == true)
            {
                GrRect clientRect = m_pCell.ClientRectangle;
                m_gridControl.Location = clientRect.Location + m_pCell.Location;
            }
            //m_gridControl.Visible = true;
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

            if (this.HasGridControl() == true)
            {
                GrRect clientRect = m_pCell.ClientRectangle;
                m_gridControl.Size = clientRect.Size;
                m_gridControl.Visible = this.IsDisplayable;
            }
        }

        protected override void OnDisplayableChanged()
        {
            base.OnDisplayableChanged();
            if (this.HasGridControl() == true)
            {
                m_gridControl.Visible = this.IsDisplayable;
            }
        }

        protected override void OnVisibleChanged()
        {
            base.OnVisibleChanged();
            //bool visibled = this.GetVisible();

            if (this.HasGridControl() == false)
            {
                Row row = m_parentRow;
                PropertyDescriptor propertyDescriptor = m_propertyDescriptor;

                m_gridControl = m_parentGrid.InvokeNewChildGridControl(this);

                System.Object value = propertyDescriptor.GetValue(row.Component);

                m_gridControl.DataSource = value;
                m_gridControl.Refresh();

                GrGridCore pGridCore = m_gridControl.GridCore;
                GrRect rect = pGridCore.GetVisibleBounds();

                GrDataRowList pDataRowList = pGridCore.DataRowList;
                pDataRowList.VisibleHeightChanged += dataRowList_VisibleHeightChanged;

                this.GridCore.DisplayRectangleChanged += gridCore_DisplayRectChanged;

                this.Height = GetMinHeight();
            }
        }

        private void focuser_FocusChanged(object pSender, GrFocusChangeArgs e)
        {
            GrFocuser pFocuser = pSender as GrFocuser;
            IFocusable pFocusable = pFocuser.Get();

            if (pFocusable != m_pCell && this.HasGridControl() == true)
            {
                m_gridControl.ClearSelection();
            }
        }

        private void dataRowList_VisibleHeightChanged(object pSender, EventArgs e)
        {
            this.SetFit();
        }

        private void gridCore_DisplayRectChanged(object pSender, EventArgs e)
        {
            GrRect clientRect = m_pCell.ClientRectangle;
            m_gridControl.Size = clientRect.Size;
        }

        private bool HasGridControl()
        {
            return m_gridControl != null;
        }

    }
}
