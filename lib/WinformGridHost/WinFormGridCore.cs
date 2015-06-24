using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.Design;
using System.Drawing;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    class WinFormGridCore : GrGridCore
    {
        public GridControl m_gridControl;

        public WinFormGridCore(GridControl gridControl, GrGridWindow pGridWindow)
            : base(pGridWindow)
            {
                m_gridControl = gridControl;

                GrGridCore pGridCore = this;
		pGridCore.DisplayRectChanged+=gridCore_DisplayRectChanged;

        GrColumnList pColumnList = GetColumnList();
        pColumnList.ColumnMouseDown+=columnList_ColumnMouseDown;
        pColumnList.ColumnMouseEnter+=columnList_ColumnMouseEnter;
        pColumnList.ColumnMouseLeave+=columnList_ColumnMouseLeave;
        pColumnList.ColumnMouseMove+=columnList_ColumnMouseMove;
        pColumnList.ColumnMouseUp+=columnList_ColumnMouseUp;
        pColumnList.ColumnWidthChanged+=columnList_ColumnWidthChanged;
        pColumnList.ColumnFrozenChanged+=columnList_ColumnFrozenChanged;
		pColumnList.ColumnVisibleIndexChanged+=columnList_ColumnVisibleIndexChanged;

        GrFocuser pFocuser = GetFocuser();
        pFocuser.FocusChanging+=focuser_FocusChanging;
        pFocuser.FocusChanged+=focuser_FocusChanged;

        GrItemSelector pItemSelector = GetItemSelector();
        pItemSelector.SelectedRowsChanged+=itemSelector_SelectedRowsChanged;
        pItemSelector.SelectedColumnsChanged+=itemSelector_SelectedColumnsChanged;
        pItemSelector.SelectionChanged+=itemSelector_SelectionChanged;

        GrCaption pCaption = GetCaptionRow();
        pCaption.HeightChanged+=caption_HeightChanged;

		GrDataRowList pDataRowList = GetDataRowList();
		pDataRowList.DataRowMoved+=dataRowList_DataRowMoved;
		pDataRowList.DataRowMoving+=dataRowList_DataRowMoving;
		pDataRowList.VisibleHeightChanged+=dataRowList_VisibleHeightChanged;
            }

        protected override void OnItemMouseEnter(GrItemMouseEventArgs e)
        {
            base.OnItemMouseEnter(e);
            m_gridControl.InvokeCellMouseEnter(FromNative.Get(e.GetItem()));
        }

        protected override void OnItemMouseMove(GrItemMouseEventArgs e)
        {
            base.OnItemMouseMove(e);
            if (m_gridControl.InvokeCellMouseMove(FromNative.Get(e.GetItem()), e.GetLocation()) == true)
            {
                e.SetHandled(true);
            }
        }

        protected override void OnItemMouseLeave(GrItemMouseEventArgs e)
        {
            base.OnItemMouseLeave(e);
            m_gridControl.InvokeCellMouseLeave(FromNative.Get(e.GetItem()));
        }

        protected override void OnItemMouseClick(GrItemMouseEventArgs e)
        {
            base.OnItemMouseClick(e);
            m_gridControl.InvokeCellClick(FromNative.Get(e.GetItem()));
        }

        protected override void OnItemMouseDoubleClick(GrItemMouseEventArgs e)
        {
            base.OnItemMouseDoubleClick(e);
            m_gridControl.InvokeCellDoubleClick(FromNative.Get(e.GetItem()));
        }

        protected override void OnRowMouseEnter(GrRowMouseEventArgs e)
        {
            base.OnRowMouseEnter(e);
            GrDataRow pDataRow = e.GetRow() as GrDataRow;
            if (pDataRow == null)
                return;

            try
            {
                Row row = FromNative.Get(pDataRow);
                string error = row.Error;
                string sourceError = row.SourceError;
                if (error != string.Empty)
                {
                    m_gridControl.ToolTip.Show(error);
                }
                else if (sourceError != string.Empty)
                {
                    m_gridControl.ToolTip.Show(sourceError);
                }
            }
            catch (Exception)
            {
            }
        }

        protected override void OnRowMouseMove(GrRowMouseEventArgs e)
        {

        }

        protected override void OnRowMouseLeave(GrRowMouseEventArgs e)
        {
            base.OnRowMouseLeave(e);
            m_gridControl.ToolTip.Hide();
        }

        protected override void PostPaint(GrGridPainter pPainter, GrRect clipRect)
        {
            Graphics graphics = Graphics.FromHdc(pPainter.GetDevice());
            m_gridControl.PostPaint(graphics, clipRect);
            graphics.Dispose();
            base.PostPaint(pPainter, clipRect);
        }

        private void gridCore_DisplayRectChanged(object pSender, EventArgs e)
        {
            m_gridControl.InvokeDisplayRectangleChanged();
        }

        private void columnList_ColumnMouseDown(object pSender, GrColumnMouseEventArgs e)
        {
            Column column = FromNative.Get(e.GetColumn());
            if (m_gridControl.Site == null)
            {
                bool handled = m_gridControl.InvokeColumnMouseDown(column, e.GetLocation());
                e.SetHandled(handled);
                return;
            }

            ISelectionService selectionService = m_gridControl.GetInternalService(typeof(ISelectionService)) as ISelectionService;
            object[] components = new object[] { column, };
            selectionService.SetSelectedComponents(components);
            e.SetHandled(true);
        }

        private void columnList_ColumnMouseUp(object pSender, GrColumnMouseEventArgs e)
        {
            Column column = FromNative.Get(e.GetColumn());
            bool handled = m_gridControl.InvokeColumnMouseUp(column, e.GetLocation());
            e.SetHandled(handled);
        }

        private void columnList_ColumnMouseEnter(object pSender, GrColumnMouseEventArgs e)
        {
            Column column = FromNative.Get(e.GetColumn());
            m_gridControl.InvokeColumnMouseEnter(column, e.GetLocation());
        }

        private void columnList_ColumnMouseLeave(object pSender, GrColumnMouseEventArgs e)
        {
            Column column = FromNative.Get(e.GetColumn());
            m_gridControl.InvokeColumnMouseLeave(column);
        }

        private void columnList_ColumnMouseMove(object pSender, GrColumnMouseEventArgs e)
        {
            Column column = FromNative.Get(e.GetColumn());
            if (m_gridControl.InvokeColumnMouseMove(column, e.GetLocation()) == true)
            {
                e.SetHandled(true);
            }
        }

        private void columnList_ColumnWidthChanged(object pSender, GrColumnEventArgs e)
        {
            Column column = FromNative.Get(e.GetColumn());
            m_gridControl.InvokeColumnWidthChanged(column);
        }

        private void columnList_ColumnFrozenChanged(object pSender, GrColumnEventArgs e)
        {
            Column column = FromNative.Get(e.GetColumn());
            m_gridControl.InvokeColumnFrozenChanged(column);
        }

        private void columnList_ColumnVisibleIndexChanged(object pSender, GrColumnEventArgs e)
        {
            Column column = FromNative.Get(e.GetColumn());
            m_gridControl.InvokeColumnVisibleIndexChanged(column);
        }

        private void focuser_FocusChanging(object pSender, GrFocusChangeArgs e)
        {
            m_gridControl.InvokeFocusChanging();
        }

        private void focuser_FocusChanged(object pSender, GrFocusChangeArgs e)
        {
            m_gridControl.InvokeFocusChanged();
        }

        private void itemSelector_SelectedRowsChanged(object pSender, EventArgs e)
        {
            m_gridControl.InvokeSelectedRowsChanged();
        }

        private void itemSelector_SelectedColumnsChanged(object pSender, EventArgs e)
        {
            m_gridControl.InvokeSelectedColumnsChanged();

        }

        private void itemSelector_SelectionChanged(object pSender, EventArgs e)
        {
            m_gridControl.InvokeSelectionChanged();

        }

        private void caption_HeightChanged(object pSender, EventArgs e)
        {
            if (m_gridControl.Site == null)
                return;


            IComponentChangeService service = m_gridControl.GetInternalService(typeof(IComponentChangeService)) as IComponentChangeService;
            PropertyDescriptor propertyDescriptor = TypeDescriptor.GetProperties(m_gridControl)["Caption"];
            service.OnComponentChanging(m_gridControl, propertyDescriptor);
            service.OnComponentChanged(m_gridControl, propertyDescriptor, null, null);
        }
        
        private void dataRowList_DataRowMoved(object pSender, GrDataRowEventArgs e)
        {
            Row row = FromNative.Get(e.GetDataRow());
            m_gridControl.InvokeRowMoved(row);

        }

        private void dataRowList_DataRowMoving(object pSender, GrDataRowMovingEventArgs e)
        {
            Row row = FromNative.Get(e.GetDataRow());
            if (m_gridControl.InvokeRowMoving(row, e.GetIndex()) == false)
                e.SetCancel(true);
        }

        private void dataRowList_VisibleHeightChanged(object pSender, EventArgs e)
        {
            m_gridControl.InvokeVisibleHeightChanged();
        }
    }
}
