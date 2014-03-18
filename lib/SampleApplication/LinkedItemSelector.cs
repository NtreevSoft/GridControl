using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;
using System.Collections;

namespace SampleApplication
{
    public partial class LinkedItemSelector : UserControl
    {
        private const int defaultButtonWidth = 17;
        private readonly VisualStyleRenderer rendererNormal = new VisualStyleRenderer(VisualStyleElement.ComboBox.DropDownButton.Normal);
        private readonly VisualStyleRenderer rendererHot = new VisualStyleRenderer(VisualStyleElement.ComboBox.DropDownButton.Hot);
        private readonly VisualStyleRenderer rendererPressed = new VisualStyleRenderer(VisualStyleElement.ComboBox.DropDownButton.Pressed);
        private readonly VisualStyleRenderer rendererDisabled = new VisualStyleRenderer(VisualStyleElement.ComboBox.DropDownButton.Disabled);

        private bool isDropButtonDown;
        private MouseOver mouseOverState;
        private Rectangle dropButtonRect;
        private object selectedItem;
        private readonly ObjectCollection items;

        public LinkedItemSelector()
        {
            this.items = new ObjectCollection(this);
            InitializeComponent();
        }

        public event EventHandler ItemClicked;

        public object SelectedItem
        {
            get { return this.selectedItem; }
            set
            {
                this.selectedItem = value;
                if (this.selectedItem != null)
                {
                    this.linkLabel1.Text = this.selectedItem.ToString();
                }
                else
                {
                    this.linkLabel1.Text = "(null)";
                }
            }
        }

        public ObjectCollection Items
        {
            get { return this.items; }
        }

        protected virtual void OnItemClicked(EventArgs e)
        {
            if (this.ItemClicked != null)
            {
                this.ItemClicked(this, e);
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if (this.isDropButtonDown == true)
            {
                this.rendererPressed.DrawBackground(e.Graphics, this.DropButtonRectangle);
            }
            else
            {
                switch (this.MouseOverState)
                {
                    case MouseOver.None:
                        break;
                    case MouseOver.Client:
                        this.rendererNormal.DrawBackground(e.Graphics, this.DropButtonRectangle);
                        break;
                    case MouseOver.DropButton:
                        this.rendererHot.DrawBackground(e.Graphics, this.DropButtonRectangle);
                        break;
                }
            }
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);
            this.dropButtonRect = new Rectangle(this.ClientRectangle.Right - defaultButtonWidth, 0, defaultButtonWidth, this.ClientRectangle.Height);
            this.Invalidate();
        }

        protected override void OnMouseEnter(EventArgs e)
        {
            base.OnMouseEnter(e);
            Console.WriteLine("OnMouseEnter");
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);
            if (this.ClientRectangle.Contains(e.Location) == false)
            {
                this.MouseOverState = MouseOver.None;
                return;
            }
            this.MouseOverState = this.dropButtonRect.Contains(e.Location) ? MouseOver.DropButton : MouseOver.Client;
        }

        protected override void OnMouseLeave(EventArgs e)
        {
            base.OnMouseLeave(e);
            this.MouseOverState = MouseOver.None;
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);
            this.IsDropButtonDown = this.dropButtonRect.Contains(e.Location);

            DropDownForm form = new DropDownForm(this);
            form.Show(this);
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
            this.IsDropButtonDown = false;
        }

        internal Point LocationToAttach
        {
            get
            {
                return new Point(this.Left, this.Bottom);
            }
        }

        internal void CancelByEscape(Message msg, Keys keyData)
        {
            msg.HWnd = this.Handle;
            this.ProcessCmdKey(ref msg, keyData);
        }

        private Rectangle DropButtonRectangle
        {
            get { return this.dropButtonRect; }
        }

        private MouseOver MouseOverState
        {
            get { return this.mouseOverState; }
            set
            {
                if (this.mouseOverState == value)
                    return;
                this.mouseOverState = value;
                this.Invalidate();
            }
        }

        private bool IsDropButtonDown
        {
            get { return this.isDropButtonDown; }
            set
            {
                if (this.isDropButtonDown == value)
                    return;
                this.isDropButtonDown = value;
                this.Invalidate();
            }
        }

        enum MouseOver
        {
            None,
            Client,
            DropButton,
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            if (this.selectedItem == null)
                return;
            this.OnItemClicked(EventArgs.Empty);
        }

        public class ObjectCollection : CollectionBase
        {
            internal ObjectCollection(LinkedItemSelector selector)
            {

            }

        }

        public class DropDownForm : Form
        {
            private readonly LinkedItemSelector selector;
            private ListBox listBox = new ListBox();

            public DropDownForm(LinkedItemSelector selector)
            {
                this.selector = selector;

                this.FormBorderStyle = FormBorderStyle.None;
                this.ControlBox = false;
                this.FormBorderStyle = FormBorderStyle.FixedSingle;
                this.MaximizeBox = false;
                this.MinimizeBox = false;
                this.ShowInTaskbar = false;
                this.TopMost = true;
                this.StartPosition = FormStartPosition.Manual;

                this.Location = this.selector.PointToScreen(this.selector.LocationToAttach);
                this.Width = this.selector.Width;

                this.listBox.Items.AddRange(this.selector.items.Cast<object>().ToArray());
                this.listBox.SelectedItem = this.selector.selectedItem;
            }

            protected override void WndProc(ref Message m)
            {
                if (this.selector.ChildWndProc(ref m) == true)
                    return;
                base.WndProc(ref m);
            }

        }

        internal bool ChildWndProc(ref Message m)
        {
            if (m.Msg == 0x0100)
            {
                if (m.WParam.ToInt32() == (int)Keys.Escape)
                {
                    m.HWnd = this.Parent.Handle;
                    this.PreProcessMessage(ref m);
                    //base.WndProc(ref m);
                    return true;
                }
            }

            return false;
        }
    }
}
