using Ntreev.Windows.Forms.Grid.Natives;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.Design;
using System.Windows.Forms.VisualStyles;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public partial class TypeEditorForm : Form, IWindowsFormsEditorService, IServiceProvider, ITypeDescriptorContext, IEditorService
    {
        private static readonly int m_paddingSize = 12;

        private bool m_dragging;
        private bool m_resizing;
        private bool m_resizable;
        private bool m_isin;
        private bool m_closed;
        private bool m_defaultResult;

        private IntPtr m_mainHandle;
        private GridControl m_gridControl;

        private Point m_mouseDownPoint;
        private Point m_mouseMovePoint;
        private Rectangle m_resizableRect;
        private Size m_firstSize;
        private Rectangle m_bounds;
        private Rectangle m_clientBounds;
        private Control m_control;
        private Size m_controlSize;
        private ICell m_cell;
        private EditingReason m_reason;

        private System.Windows.Forms.VisualStyles.VisualStyleRenderer m_gripperRenderer;

        private MessageFilter m_filter = new MessageFilter();

        public TypeEditorForm()
        {
            InitializeComponent();
        }

        public TypeEditorForm(GridControl gridControl, ICell cell, EditingReason reason)
        {

            m_gridControl = gridControl;
            m_cell = cell;
            m_reason = reason;
            InitializeComponent();

            m_gridControl.EditClosed += gridControl_EditClosed;

            m_bounds = m_gridControl.RectangleToScreen(cell.Bounds);
            m_clientBounds = cell.ClientRectangle;
            m_clientBounds.Offset(cell.Bounds.Location);
            m_clientBounds = m_gridControl.RectangleToScreen(m_clientBounds);

            Rectangle gs = GetClipScreenRect(m_gridControl);

            m_clientBounds = Rectangle.Intersect(m_clientBounds, gs);
            m_mainHandle = NativeMethods.GetRootWindow(m_gridControl.Handle);

            try
            {
                m_gripperRenderer = new VisualStyleRenderer(VisualStyleElement.Status.Gripper.Normal);
            }
            catch (Exception)
            {

            }
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);

            m_filter.Stop();
            m_gridControl.EditClosed -= gridControl_EditClosed;
            this.Controls.Clear();

            if (m_control != null)
            {
                m_control.Resize -= control_Resize;
                m_control.Size = m_controlSize;
            }
            m_control = null;
            if (m_gripperRenderer != null)
            {
                m_gripperRenderer = null;
            }

            m_gridControl.Select();
        }

        public void ShowControl(Control control)
        {


            if (m_reason.ReasonType == EditingReasonType.Mouse)
            {
                Message msg = new Message();
                Point location = m_reason.Location;
                msg.HWnd = m_gridControl.Handle;
                msg.Msg = (int)WindowMessages.WM_LBUTTONUP;
                msg.WParam = new IntPtr(1);
                msg.LParam = NativeMethods.MakeLParam(location.X, location.Y);
                NativeMethods.SendMessage(msg);
            }

            m_filter.Start(m_mainHandle, this);
            IntPtr handle = this.Handle;

            this.SuspendLayout();
            control.ImeMode = m_gridControl.ImeMode;

            this.ImeMode = control.ImeMode;
            this.BackColor = control.BackColor;
            this.ForeColor = control.ForeColor;
            this.Font = control.Font;
            this.Controls.Add(control);
            this.FormBorderStyle = FormBorderStyle.None;
            this.Bounds = m_clientBounds;
            this.ResumeLayout(false);

            m_gridControl.Update();
            this.Visible = true;
            this.Activate();

            this.PreProcessEvent(control);
            List<Message> events = new List<Message>();
            //NativeMethods.DoEvents(events);
            this.PostProcessEvent(control);

            if (events.Count > 0)
            {
                Message m = (Message)events[0];
                NativeMethods.SendMessage(m);
            }

            NativeMethods.DoEventsModal(this);
            m_filter.Stop();

            m_gridControl.ImeMode = control.ImeMode;
        }

        public void DropDownControl(Control control)
        {

            m_filter.Start(m_mainHandle, this);

            IntPtr handle = this.Handle;

            this.SuspendLayout();
            control.ImeMode = m_gridControl.ImeMode;
            m_controlSize = control.Size;
            this.Controls.Add(control);
            Size size = new Size(control.Width + 2, control.Height + 2);
            if (m_resizable == true)
                size.Height += m_paddingSize;
            m_control = control;

            control.Dock = DockStyle.Fill;
            control.Visible = true;
            this.ResumeLayout(true);

            control.Resize += control_Resize;

            size.Width = System.Math.Max(m_bounds.Width, size.Width);
            size.Height = System.Math.Max(m_bounds.Height, size.Height);
            this.SetBounds(m_bounds.X, m_bounds.Bottom, size.Width, size.Height);
            this.Visible = true;

            NativeMethods.SendMessage(this.Handle, WindowMessages.WM_LBUTTONDOWN, IntPtr.Zero, IntPtr.Zero);
            NativeMethods.SendMessage(this.Handle, WindowMessages.WM_LBUTTONUP, IntPtr.Zero, IntPtr.Zero);

            NativeMethods.DoEventsModal(this);
            m_filter.Stop();
        }

        public void ShowDialog(Control control)
        {
            this.ControlBox = true;
            this.MaximizeBox = true;
            this.MinimizeBox = true;
            this.ShowInTaskbar = true;
            this.StartPosition = FormStartPosition.CenterScreen;
            this.FormBorderStyle = FormBorderStyle.Sizable;
            this.Controls.Add(control);

            ShowDialog();
        }

        public void CancelEdit()
        {
            this.DialogResult = DialogResult.Cancel;
        }

        public bool IsDropDownResizable
        {
            set
            {
                if (value == true)
                    this.Padding = new Padding(0, 0, 0, m_paddingSize);
                else
                    this.Padding = new Padding(0, 0, 0, 0);
                m_resizable = value;
            }

        }

        public bool DefaultResult
        {
            set
            {
                m_defaultResult = value;

            }
        }

        public EditingReasonType ReasonType
        {
            get
            {
                return m_reason.ReasonType;
            }
        }

        public Rectangle Rectangle
        {
            set
            {
                value.Height += ((Size.Height - ClientSize.Height) + Padding.Vertical);

                Rectangle screenRect = Screen.FromRectangle(value).WorkingArea;

                if (value.Height > screenRect.Height)
                    value.Height = screenRect.Height;

                if (value.Bottom > screenRect.Bottom)
                    value.Y = screenRect.Bottom - value.Height;

                if (value.Left < screenRect.Left)
                    value.X = screenRect.Left;
                else if (value.Right > screenRect.Right)
                    value.X = screenRect.Right - value.Width;

                this.Location = value.Location;
                this.Size = value.Size;
            }

        }

        public GridControl GridControl
        {
            get
            {
                return m_gridControl;
            }
        }

        public DialogResult Result
        {
            get
            {
                return this.DialogResult;
            }
        }

        public IntPtr MainHandle
        {
            get
            {
                return m_mainHandle;
            }
        }



        public event EventHandler EscapeKeyPress;

        public event EventHandler EnterKeyPress;

        public event EventHandler TabKeyPress;

        //protected: // methods

        //    /// <summary>
        //    /// 사용 중인 모든 리소스를 정리합니다.
        //    /// </summary>
        //    ~TypeEditorForm();

        protected override void OnSizeChanged(System.EventArgs e)
        {
            base.OnSizeChanged(e);
            const int size = 50;
            m_resizableRect = new Rectangle(ClientRectangle.Right - size, ClientRectangle.Bottom - size, size, size);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            if (m_resizable == true)
            {
                Rectangle rect = new Rectangle((this.Width) - 18, (this.Height) - 20, 20, 20);
                m_gripperRenderer.DrawBackground(e.Graphics, rect);
            }
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
            this.Capture = false;
            m_dragging = false;
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);
            if (m_resizable == false)
                return;

            if (m_resizableRect.Contains(e.Location) && e.Button == MouseButtons.Left)
            {
                m_mouseDownPoint = e.Location;
                m_mouseMovePoint = e.Location;
                m_firstSize = this.Size;
                m_dragging = true;
                this.Capture = true;
            }
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            if (m_resizable == false)
                return;

            if (m_resizableRect.Contains(e.Location))
                this.Cursor = Cursors.SizeNWSE;
            else
                this.Cursor = Cursors.Arrow;

            if (m_dragging == false || e.Button != MouseButtons.Left)
                return;

            if (e.Location == m_mouseMovePoint)
                return;

            Point offset = e.Location - (Size)m_mouseDownPoint;
            this.Size = m_firstSize + (Size)offset;

            m_mouseMovePoint = e.Location;
            this.Invalidate();
        }

        protected override void OnMouseLeave(System.EventArgs e)
        {
            base.OnMouseLeave(e);
            Cursor = Cursors.Default;
        }

        protected override void OnLayout(LayoutEventArgs levent)
        {
            try
            {
                this.m_resizing = true;
                base.OnLayout(levent);
            }
            finally
            {
                this.m_resizing = false;
            }
        }

        protected override void OnVisibleChanged(System.EventArgs e)
        {
            if (this.Visible == false)
            {
                Point cursorPos = Cursor.Position;

                cursorPos = m_gridControl.PointToClient(cursorPos);
                m_isin = m_gridControl.DisplayRectangle.Contains(cursorPos);
            }

            base.OnVisibleChanged(e);
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            bool result = base.ProcessCmdKey(ref msg, keyData);

            return result;
        }

        protected override bool ProcessDialogKey(Keys keyData)
        {
            bool result = base.ProcessDialogKey(keyData);
            if (keyData == Keys.Enter && result == false)
                (this as IEditorService).Close();
            if (keyData == Keys.Escape && result == false)
                this.CancelEdit();
            return result;
        }




        void IWindowsFormsEditorService.CloseDropDown()
        {
            (this as IEditorService).Close();
        }

        DialogResult IWindowsFormsEditorService.ShowDialog(Form dialog)
        {
            this.DialogResult = dialog.ShowDialog();
            return this.DialogResult;
        }

        object IServiceProvider.GetService(Type serviceType)
        {
            if (serviceType == typeof(IWindowsFormsEditorService))
                return this;
            if (serviceType == typeof(IEditorService))
                return this;
            if (serviceType == typeof(ITypeDescriptorContext))
                return this;
            if (serviceType == typeof(IServiceProvider))
                return this;
            if (serviceType == typeof(GridControl))
                return this.GridControl;
            if (serviceType == typeof(ITextCacheProvider))
                return m_cell.Column as ITextCacheProvider;

            return GetService(serviceType);
        }

        void ITypeDescriptorContext.OnComponentChanged()
        {

        }
        bool ITypeDescriptorContext.OnComponentChanging()
        {
            return false;
        }

        void IEditorService.Close()
        {
            this.DialogResult = DialogResult.OK;
        }

        private void control_Resize(object sender, System.EventArgs e)
        {
            if ((m_control != null) && m_resizing == false)
            {
                int width = this.Width;
                Size size = new Size(m_control.Width + 2, m_control.Height + 2);

                try
                {
                    this.m_resizing = true;
                    this.SuspendLayout();
                    this.Size = size;
                }
                finally
                {
                    this.m_resizing = false;
                    this.ResumeLayout(false);
                }
                this.Left -= this.Width - width;
            }
        }

        private void PreProcessEvent(Control control)
        {

            switch (m_reason.ReasonType)
            {
                case EditingReasonType.Mouse:
                    {
                        Message msg = new Message();
                        Point location = m_gridControl.PointToScreen(m_reason.Location);
                        Control hitTest = m_gridControl.GetChildAt(control, location);
                        if (hitTest == null)
                            hitTest = control;
                        location = hitTest.PointToClient(location);

                        msg.HWnd = hitTest.Handle;
                        msg.Msg = (int)WindowMessages.WM_MOUSEMOVE;
                        msg.WParam = IntPtr.Zero;
                        msg.LParam = NativeMethods.MakeLParam(location.X, location.Y);
                        //NativeMethods.SendMessage(msg);

                        msg.HWnd = hitTest.Handle;
                        msg.Msg = (int)WindowMessages.WM_LBUTTONDOWN;
                        msg.WParam = new IntPtr(1);
                        msg.LParam = NativeMethods.MakeLParam(location.X, location.Y);
                        //NativeMethods.SendMessage(msg);
                    }
                    break;
                case EditingReasonType.Key:
                    {
                        Message msg = new Message();
                        msg.HWnd = ActiveControl.Handle;
                        msg.Msg = (int)WindowMessages.WM_KEYDOWN;
                        msg.WParam = new IntPtr((int)m_reason.Key);
                        if (m_reason.Key != Keys.Enter)
                            NativeMethods.SendMessage(msg);
                    }
                    break;
                case EditingReasonType.Char:
                    {
                        int vk = NativeMethods.VkKeyScan(m_reason.Char);

                        Message msg = new Message();
                        msg.HWnd = ActiveControl.Handle;
                        msg.Msg = (int)WindowMessages.WM_KEYDOWN;
                        msg.WParam = new IntPtr(vk);
                        NativeMethods.TranslateAndDispatchMessage(msg);
                    }
                    break;
                case EditingReasonType.Ime:
                    {
                        Message msg = new Message();
                        msg.HWnd = ActiveControl.Handle;
                        msg.Msg = (int)WindowMessages.WM_KEYDOWN;
                        msg.WParam = new IntPtr((int)Keys.ProcessKey);
                        msg.Result = new IntPtr((int)0x1e0001);
                        NativeMethods.TranslateAndDispatchMessage(msg);
                    }
                    break;
            }
        }

        private void PostProcessEvent(Control control)
        {


            switch (m_reason.ReasonType)
            {
                case EditingReasonType.Mouse:
                    {
                        Message msg = new Message();
                        Point location = m_gridControl.PointToScreen(m_reason.Location);
                        Control hitTest = m_gridControl.GetChildAt(control, location);
                        if (hitTest == null)
                            hitTest = control;
                        location = hitTest.PointToClient(location);

                        msg.HWnd = hitTest.Handle;
                        msg.Msg = (int)WindowMessages.WM_MOUSEMOVE;
                        msg.WParam = IntPtr.Zero;
                        msg.LParam = NativeMethods.MakeLParam(location.X, location.Y);
                        NativeMethods.SendMessage(msg);

                        msg.HWnd = hitTest.Handle;
                        msg.Msg = (int)WindowMessages.WM_LBUTTONDOWN;
                        msg.WParam = new IntPtr(1);
                        msg.LParam = NativeMethods.MakeLParam(location.X, location.Y);
                        NativeMethods.SendMessage(msg);
                    }
                    break;
            }
        }

        private Rectangle GetClipScreenRect(Control control)
        {
            Control parent = control.Parent;

            if (parent == null)
            {
                return control.RectangleToScreen(control.DisplayRectangle);
            }

            Rectangle s = control.RectangleToScreen(control.DisplayRectangle);
            Rectangle e = GetClipScreenRect(parent);

            return Rectangle.Intersect(e, s);
        }

        private void gridControl_EditClosed(object sender, System.EventArgs e)
        {
            this.CancelEdit();
        }

        IContainer ITypeDescriptorContext.Container
        {
            get
            {
                return m_gridControl.Container;
            }
        }

        object ITypeDescriptorContext.Instance
        {
            get
            {
                Row row = m_gridControl.FocusedRow as Row;
                if (row == null)
                    return null;
                return row.Component;
            }
        }

        PropertyDescriptor ITypeDescriptorContext.PropertyDescriptor
        {
            get
            {
                return null;
            }
        }




        class MessageFilter : NativeWindow, IMessageFilter
        {
            private TypeEditorForm m_dropDownForm;

            public void Start(IntPtr mainHandle, Design.TypeEditorForm dropDownForm)
            {
                Application.AddMessageFilter(this);
                AssignHandle(mainHandle);

                m_dropDownForm = dropDownForm;
            }
            public void Stop()
            {
                Application.RemoveMessageFilter(this);
                ReleaseHandle();
            }

            protected override void WndProc(ref Message m)
            {
                switch ((WindowMessages)m.Msg)
                {
                    case WindowMessages.WM_NCACTIVATE:
                        {
                            if (((int)m.WParam) == 0)
                            {
                                m.WParam = new IntPtr(1);
                                m.LParam = IntPtr.Zero;
                            }
                        }
                        break;
                    case WindowMessages.WM_ACTIVATEAPP:
                        {
                            if ((int)m.WParam == 0)
                            {
                                (m_dropDownForm as IEditorService).Close();
                                NativeMethods.PostMessage(Handle, WindowMessages.WM_NCACTIVATE, IntPtr.Zero, IntPtr.Zero);
                            }
                        }
                        break;
                    case WindowMessages.WM_ACTIVATE:
                        {
                            if (m.WParam == new IntPtr(1))
                            {

                            }
                        }
                        break;
                }

                base.WndProc(ref m);
            }


            bool IMessageFilter.PreFilterMessage(ref Message m)
            {

                WindowMessages wm = (WindowMessages)m.Msg;

                switch (wm)
                {
                    case WindowMessages.WM_NCLBUTTONDBLCLK:
                    case WindowMessages.WM_NCLBUTTONDOWN:
                    case WindowMessages.WM_NCRBUTTONDOWN:
                    case WindowMessages.WM_NCMBUTTONDOWN:
                    case WindowMessages.WM_LBUTTONDBLCLK:
                    case WindowMessages.WM_LBUTTONDOWN:
                    case WindowMessages.WM_RBUTTONDOWN:
                    case WindowMessages.WM_MBUTTONDOWN:
                        {
                            Point cursorPos = Cursor.Position;

                            if (!m_dropDownForm.Bounds.Contains(cursorPos))
                            {
                                if (m.HWnd != m_dropDownForm.GridControl.Handle)
                                {
                                    IntPtr mainHandle = NativeMethods.GetRootWindow(m.HWnd);
                                    IntPtr mainHandle2 = NativeMethods.GetRootWindow(NativeMethods.WindowFromPoint(cursorPos));

                                    if (mainHandle2 != m_dropDownForm.MainHandle)
                                    {
                                        uint idcont = NativeMethods.GetWindowThreadProcessId(m.HWnd);
                                        uint idform = NativeMethods.GetWindowThreadProcessId(m_dropDownForm.Handle);

                                        if (idcont == idform)
                                            break;
                                    }
                                }

                                (m_dropDownForm as IEditorService).Close();

                                GridControl gridControl = m_dropDownForm.GridControl;
                                Point localPos = gridControl.PointToClient(cursorPos);

                                Cell cell = gridControl.GetCellAt(localPos);
                                if (cell != null)
                                {
                                    if (cell.IsFocused == false)
                                    {
                                        m.HWnd = gridControl.Handle;
                                        m.LParam = NativeMethods.MakeLParam(localPos.X, localPos.Y);
                                        NativeMethods.SendMessage(m);
                                    }
                                }
                                else
                                {
                                    NativeMethods.PostMessage(m);
                                }
                                return true;
                            }
                        }
                        break;
                }

                return false;
            }
        }
    }
}
