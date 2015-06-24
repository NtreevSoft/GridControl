using Ntreev.Windows.Forms.Grid.Design;
using Ntreev.Windows.Forms.Grid.Natives;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    public abstract class ColumnControl<TControl> : Column where TControl : Control
    {
        private TControl m_control;
        private TControl m_viewControl;

        private IControlPainter m_controlPainter;
        private IEditorService m_editorService;

        //Form m_form;
        //private CellMouseEventHandler m_eventCellMouseMove;
        //private InvalidateEventHandler m_viewControlInvalidate;
        private Cell m_viewedCell;

        private MessageFilter messageFilter;

        protected ColumnControl()
        {
            m_controlPainter = new ControlPainter();
            m_control = CreateControlInstance(null);

            if (this.ViewType != ViewType.Text)
            {
                m_viewControl = CreateControlInstance(null);
                //m_viewControl.Visible = true;
                //m_eventCellMouseMove = new CellMouseEventHandler(this, &ColumnControl<TControl>.gridControl_CellMouseMove);
                //m_viewControlInvalidate = new InvalidateEventHandler(this, &ColumnControl<TControl>.viewControl_Invalidated);
                this.NativeRef.SetItemMinSize(m_viewControl.PreferredSize);
            }

            NativeRef.m_customItemPaint = this.ViewType != ViewType.Text;
        }


        protected ColumnControl(params object[] controlArgs)
        {
            m_controlPainter = new ControlPainter();
            m_control = CreateControlInstance(controlArgs);

            if (this.ViewType != ViewType.Text)
            {
                m_viewControl = CreateControlInstance(controlArgs);
                //m_eventCellMouseMove = new CellMouseEventHandler(this, &ColumnControl<TControl>.gridControl_CellMouseMove);
                //m_viewControlInvalidate = new InvalidateEventHandler(this, &ColumnControl<TControl>.viewControl_Invalidated);
                this.NativeRef.SetItemMinSize(m_viewControl.PreferredSize);
            }

            NativeRef.m_customItemPaint = this.ViewType != ViewType.Text;
        }


        public override void PaintValue(Graphics graphics, Rectangle paintRect, ICell cell, object value)
        {
            if (m_viewControl == null)
                return;

            Point location = this.GridControl.PointToClient(Cursor.Position);
            location -= (Size)cell.Bounds.Location;
            location.X -= cell.Padding.Left;
            location.Y -= cell.Padding.Left;

            this.SetControlValue(m_viewControl, value);
            this.SetControlLayout(m_viewControl, cell);

            IntPtr hdc = graphics.GetHdc();

            Cursor cursor = null;

            if (this.Site == null)
            {
                if (m_viewControl.Bounds.Contains(location) == true)
                {
                    location = m_viewControl.PointToScreen(location);
                    Control hitTest = this.GridControl.GetChildAt(m_viewControl, location);
                    if (hitTest == null)
                        hitTest = m_viewControl;
                    location = hitTest.PointToClient(location);

                    Message msg = new Message();
                    msg.HWnd = hitTest.Handle;
                    msg.Msg = (int)WindowMessages.WM_MOUSEMOVE;
                    msg.WParam = IntPtr.Zero;
                    msg.LParam = NativeMethods.MakeLParam(location.X, location.Y);
                    NativeMethods.SendMessage(msg);

                    msg.HWnd = hitTest.Handle;
                    msg.Msg = (int)WindowMessages.WM_SETCURSOR;
                    msg.WParam = hitTest.Handle;
                    msg.LParam = NativeMethods.MakeLParam(1, (int)WindowMessages.WM_MOUSEMOVE);
                    NativeMethods.SendMessage(msg);

                    cursor = Cursor.Current;
                }
                else
                {
                    Message msg = new Message();
                    msg.HWnd = m_viewControl.Handle;
                    msg.Msg = (int)WindowMessages.WM_MOUSELEAVE;
                    msg.WParam = IntPtr.Zero;
                    msg.LParam = new IntPtr(1);
                    NativeMethods.SendMessage(msg);
                }
            }

            try
            {
                m_controlPainter.TransparentBlt(hdc, m_viewControl, paintRect, value);
            }
            catch (Exception exception)
            {
                throw exception;
            }
            finally
            {
                graphics.ReleaseHdc();
            }

            if (cursor != null)
                this.GridControl.Cursor = cursor;
        }


        [Browsable(false)]
        public TControl Control
        {
            get
            {
                return m_control;
            }
        }

        public override ViewType ViewType
        {
            get
            {
                return ViewType.Custom;
            }
        }

        protected virtual TControl CreateControlInstance(object[] controlArgs)
        {
            if (controlArgs == null)
                return Activator.CreateInstance(typeof(TControl)) as TControl;
            return Activator.CreateInstance(typeof(TControl), controlArgs) as TControl;
        }

        protected virtual object GetControlValue(TControl control)
        {
            return null;
        }

        protected virtual void SetControlValue(TControl control, object value)
        {

        }

        protected virtual void SetControlLayout(TControl control, ICell cell)
        {
            control.SetBounds(0, 0, cell.ClientRectangle.Width, cell.ClientRectangle.Height);
            if (control.BackColor != cell.PaintingBackColor)
                control.BackColor = cell.PaintingBackColor;
            if (control.ForeColor != cell.PaintingForeColor)
                control.ForeColor = cell.PaintingForeColor;
            if (control.Font != cell.Font)
                control.Font = cell.Font;
        }

        protected override object EditValue(IEditorService editorService, ICell cell, object value)
        {
            SetControlLayout(m_control, cell);
            SetControlValue(m_control, value);
            m_editorService = editorService;
            editorService.ShowControl(m_control);
            m_editorService = null;
            return GetControlValue(m_control);
        }

        protected override bool CanEdit(ICell cell, EditingReason reason)
        {
            if (reason.ReasonType == EditingReasonType.Mouse)
                return cell.ClientRectangle.Contains(reason.Location);
            return false;
        }

        protected void CloseControl()
        {
            if (m_editorService == null)
                return;

            m_editorService.Close();
        }

        //~ColumnControl();

        protected override void OnGridControlAttached()
        {
            base.OnGridControlAttached();

            if (m_viewControl != null)
            {
                this.GridControl.CellMouseMove += gridControl_CellMouseMove;
                this.messageFilter = new MessageFilter(m_viewControl);
                Application.AddMessageFilter(this.messageFilter);
            }
        }

        protected override void OnGridControlDetached()
        {
            if (this.messageFilter != null)
            {
                Application.RemoveMessageFilter(this.messageFilter);
                this.messageFilter = null;
                this.GridControl.CellMouseMove -= gridControl_CellMouseMove;
            }

            base.OnGridControlDetached();
        }



        internal override sealed EditStyle GetEditStyle()
        {
            return Design.EditStyle.Control;
        }

        internal override void OnCellBoundUpdate(Cell cell, object value)
        {
            if (m_viewControl == null)
                return;

            this.SetControlLayout(m_viewControl, cell);
            this.SetControlValue(m_viewControl, value);
            cell.NativeRef.SetTextBounds(m_viewControl.PreferredSize);
        }

        private void gridControl_CellMouseMove(object sender, CellMouseEventArgs e)
        {
            if (this.IsVisible == false || e.Cell.Column != this || e.Cell.IsReadOnly == true || this.Site != null)
                return;

            Cell cell = e.Cell;
            //Point location = e.Location;
            //location.X -= cell.Padding.Left;
            //location.Y -= cell.Padding.Left;

            //this.SetControlLayout(m_viewControl, cell);

            //if(m_viewControl.Bounds.Contains(location) == false)
            //	return;

            //this.SetControlValue(m_viewControl, cell.Value);

            //Message msg;
            //Control hitTest = this.GetChildAt(m_viewControl, location);
            //location = hitTest.PointToClient(location);
            //msg.HWnd = hitTest.Handle;
            //msg.Msg = (int)WindowMessages.WM_MOUSEMOVE;
            //msg.WParam = IntPtr(0);
            //msg.LParam = Native.Methods.MakeLParam(location.X, location.Y);
            //Native.Methods.SendMessage(msg);

            //msg.HWnd = hitTest.Handle;
            //msg.Msg = (int)WindowMessages.WM_SETCURSOR;
            //msg.WParam = hitTest.Handle;
            //msg.LParam = Native.Methods.MakeLParam(1, (int)WindowMessages.WM_MOUSEMOVE);
            //Native.Methods.SendMessage(msg);

            //this.GridControl.Cursor = Cursor.Current;
            cell.Invalidate();
            //System.Console.WriteLine(cell.Location);

            e.Handled = true;
        }

        private void viewControl_Invalidated(object sender, InvalidateEventArgs e)
        {

        }






        class MessageFilter : IMessageFilter
        {
            private TControl m_control;
            public MessageFilter(TControl control)
            {
                m_control = control;
            }


            bool IMessageFilter.PreFilterMessage(ref Message m)
            {
                if (m.Msg == (int)WindowMessages.WM_MOUSELEAVE)
                {
                    if (m.HWnd == m_control.Handle && m.LParam == IntPtr.Zero)
                    {
                        return true;
                    }
                }

                return false;
            }

        }


    }
}
