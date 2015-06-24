using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing.Design;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid.Columns
{
    public class ColumnComboBox : ColumnDropDown<ListBox>
    {
        public static readonly int DefaultMaxDropDownItems = 8;

        private int m_nMaxDropDownItems;
        private System.Drawing.Size m_size;
        private object m_value;
        private object m_dataSource;

        public ColumnComboBox()
        {


            m_nMaxDropDownItems = DefaultMaxDropDownItems;
            ListBox listBox = this.Control;

            listBox.BorderStyle = BorderStyle.None;
            //listBox.IntegralHeight = false;

            listBox.PreviewKeyDown += listBox_PreviewKeyDown;
            listBox.MouseMove += listBox_OnMouseMove;
            listBox.MouseClick += listBox_OnMouseClick;
            listBox.VisibleChanged += listBox_OnVisibleChanged;
        }

        [Category("Data")]
        [RefreshProperties(RefreshProperties.Repaint)]
        [AttributeProvider(typeof(IListSource))]
        [DefaultValue((string)null)]
        public object DataSource
        {
            get { return this.Control.DataSource; }
            set
            {

                BindingContext bindingContext = new BindingContext();
                CurrencyManager manager = bindingContext[value] as CurrencyManager;
                if (manager == null)
                    throw new ArgumentException("Not supported", "value");

                foreach (var item in manager.List)
                {
                    this.Control.Items.Add(item);
                }
                this.Control.SelectedIndex = -1;
            }
        }

        [Category("Data")]
        [Editor("System.Windows.Forms.Design.ListControlStringCollectionEditor, System.Design, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a", typeof(UITypeEditor))]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
        [MergableProperty(false)]
        public ListBox.ObjectCollection Items
        {
            get { return this.Control.Items; }
        }

        public int MaxDropDownItems
        {
            get
            {
                return m_nMaxDropDownItems;
            }
            set
            {
                if (value < 1)
                    throw new ArgumentOutOfRangeException("value");
                m_nMaxDropDownItems = value;
            }

        }

        protected override object GetControlValue()
        {
            return m_value;
        }

        protected override void SetControlValue(object value)
        {
            this.Control.SelectedItem = value;
            m_value = value;
        }

        protected override void SetControlLayout(ICell cell)
        {
            ListBox control = this.Control;
            base.SetControlLayout(cell);
            control.CreateGraphics().Dispose();

            int itemHeight = control.ItemHeight;
            int height = 0;
            int itemCount = control.Items.Count;

            if (itemCount > this.MaxDropDownItems)
            {
                height = itemHeight * (this.MaxDropDownItems);
            }
            else
            {
                height = itemHeight * (itemCount);
            }

            control.Height = height;
            control.CreateGraphics().Dispose();
        }

        private void listBox_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                ListBox control = this.Control;
                if (control.SelectedItem != null)
                    m_value = control.SelectedItem;
            }
        }

        private void listBox_OnMouseMove(object sender, MouseEventArgs e)
        {
            int selIndex = this.Control.IndexFromPoint(e.Location);
            if (selIndex < 0)
                return;
            this.Control.SelectedIndex = selIndex;
        }

        private void listBox_OnMouseClick(object sender, MouseEventArgs e)
        {
            ListBox control = this.Control;
            if (control.SelectedItem != null)
                m_value = control.SelectedItem;
            CloseDropDown();
        }

        private void listBox_OnVisibleChanged(object sender, System.EventArgs e)
        {
            //if(this.Control.Visible == false)
            //    this.Control.SelectedIndex = -1;
            //else
            //    this.Control.SelectedItem = m_value;
        }

        private bool ShouldSerializeMaxDropDownItems()
        {
            return m_nMaxDropDownItems != DefaultMaxDropDownItems;
        }
    }
}
