using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ColumnAdvancedExtension
{
    public partial class FlagControl : UserControl
    {
        Type _flagType;
        int _oldValue = 0;

        public FlagControl()
        {
            InitializeComponent();
        }

        public int Value
        {
            get
            {
                int value = 0;
                foreach (Control item in this.Controls)
                {
                    CheckBox checkBox = item as CheckBox;
                    if (checkBox == null || checkBox.Checked == false)
                        continue;
                    value |= (int)item.Tag;
                }
                return value;
            }
            set
            {
                _oldValue = this.Value;

                foreach (Control item in this.Controls)
                {
                    CheckBox checkBox = item as CheckBox;
                    if (checkBox == null)
                        continue;
                    int flag = (int)item.Tag;
                    checkBox.Checked = (value & flag) != 0 ? true : false;
                }
            }
        }

        public Type FlagType
        {
            get
            {
                return _flagType;
            }
            set
            {
                if (value.IsEnum == false)
                    throw new System.ArgumentException("Enum Type만 가능합니다.");

                object[] flagAttrs = value.GetCustomAttributes(typeof(FlagsAttribute), true);
                if(flagAttrs.Length == 0)
                    throw new System.ArgumentException("FlagsAttribute를 갖는 Enum Type만 가능합니다.");
             
                if (_flagType == value)
                    return;

                this.SuspendLayout();

                for (int i = this.Controls.Count-1; i >=0 ; i--)
                {
                    Control item = this.Controls[i];
                    if(item is CheckBox)
                        this.Controls.Remove(item);
                }

                int y = this.Padding.Top;
                int buttonHeight = this.Bottom - this.button1.Top;
                foreach(object item in Enum.GetValues(value))
                {
                    CheckBox checkBox = new CheckBox();
                    checkBox.Name = item.ToString();
                    checkBox.Text = item.ToString();
                    checkBox.Tag = item;
                    checkBox.AutoSize = true;
                    checkBox.Location = new Point(this.Padding.Left, y);
                    
                    this.Controls.Add(checkBox);

                    y += checkBox.Height + 1;
                }

                this.Height = y + buttonHeight;
                this.ResumeLayout(true);

                _flagType = value;
            }
        }

        public event EventHandler EditOK;

        public event EventHandler EditCanceled;

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == Keys.Enter || keyData == Keys.Escape)
            {
                OnPreviewKeyDown(new PreviewKeyDownEventArgs(keyData));
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (EditOK != null)
                EditOK(this, EventArgs.Empty);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Value = _oldValue;
            if (EditCanceled != null)
                EditCanceled(this, EventArgs.Empty);
        }
    }
}
