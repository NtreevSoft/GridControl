using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid.Design.Controls
{
    [ToolboxItemAttribute(false)]
    [DesignTimeVisibleAttribute(false)]
    public partial class FlagControl : UserControl
    {
        private Type m_flagType;
        private IEditorService m_editorService;
        private object m_value;
        private List<CheckBox> m_checkBoxes;

        public FlagControl()
        {
            this.InitializeComponent();
        }

        public FlagControl(IEditorService editorService, Type flagType)
        {
            m_editorService = editorService;
            m_flagType = flagType;
            this.InitializeComponent();
            this.InitializeCheckBox();
        }

        public object Value
        {
            get { return m_value; }
            set
            {
                m_value = value;
                if (Utility.IsNullOrDBNull(m_value) == false)
                {
                    this.UpdateValueToControl(m_value.GetHashCode(), null);
                }
            }
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == Keys.Enter)
            {
                UpdateValue();
                m_editorService.Close();
                return true;
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void InitializeCheckBox()
        {
            object[] flagAttrs = m_flagType.GetCustomAttributes(typeof(FlagsAttribute), true);
            if (flagAttrs.Length == 0)
                throw new ArgumentException("FlagsAttribute를 갖는 Enum Type만 가능합니다.");

            this.SuspendLayout();

            for (int i = this.Controls.Count - 1; i >= 0; i--)
            {
                CheckBox checkBox = this.Controls[i] as CheckBox;

                if (checkBox != null)
                {
                    checkBox.CheckedChanged -= checkBox_CheckChanged;
                    this.Controls.Remove(checkBox);
                }
            }

            int y = this.Padding.Top;
            int buttonHeight = this.Bottom - this.buttonOk.Top;
            foreach (string item in Enum.GetNames(m_flagType))
            {
                object value = Enum.Parse(m_flagType, item);
                CheckBox checkBox = new CheckBox();
                checkBox.Name = item;
                checkBox.Text = item;
                checkBox.Tag = value;

                int code = value.GetHashCode();

                if (code == 0)
                    checkBox.ForeColor = System.Drawing.Color.Red;
                else if (this.IsSingleBit(code) == false)
                    checkBox.Font = new System.Drawing.Font(checkBox.Font, System.Drawing.FontStyle.Bold);

                checkBox.AutoSize = true;
                checkBox.Location = new Point(this.Padding.Left, y);

                this.Controls.Add(checkBox);
                m_checkBoxes.Add(checkBox);
                checkBox.CheckedChanged += checkBox_CheckChanged;

                y += checkBox.Height + 1;
            }

            this.Height = y + buttonHeight;
            this.ResumeLayout(true);
        }
        private void UpdateValue()
        {
            m_value = System.Enum.ToObject(m_flagType, this.GetInt32Value());
        }

        private void UpdateValueToControl(int value, CheckBox exception)
        {
            foreach (CheckBox item in m_checkBoxes)
            {
                item.CheckedChanged -= checkBox_CheckChanged;
            }

            foreach (InternalCheckBox item in m_checkBoxes)
            {
                if (item == exception)
                    continue;

                int itemValue = item.Tag.GetHashCode();

                if (value == itemValue)
                {
                    item.CheckState = CheckState.Checked;
                }
                else if ((value & itemValue) != 0)
                {
                    if (this.IsSingleBit(itemValue) == true || (value & itemValue) == itemValue)
                        item.CheckState = CheckState.Checked;
                    else
                        item.CheckState = CheckState.Indeterminate;
                }
                else
                {
                    item.CheckState = CheckState.Unchecked;
                }
            }

            foreach (InternalCheckBox item in m_checkBoxes)
            {
                item.CheckedChanged += checkBox_CheckChanged;
            }
        }

        private bool IsSingleBit(int value)
        {
            for (int i = 0; i < 32; i++)
            {
                if (value == 1 << i)
                {
                    return true;
                }
            }
            return false;
        }

        private int GetInt32Value()
        {
            int value = 0;
            foreach (InternalCheckBox item in m_checkBoxes)
            {
                int controlValue = item.Tag.GetHashCode();
                //if(item.CheckState != CheckState.Checked || this.IsSingleBit(controlValue) == false)
                //	continue;
                if (item.CheckState != CheckState.Checked)
                    continue;
                value |= controlValue;
            }
            return value;
        }

        private void buttonOk_Click(object sender, EventArgs e)
        {
            UpdateValue();
            m_editorService.Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            m_editorService.Close();
        }

        private void checkBox_CheckChanged(object sender, EventArgs e)
        {
            CheckBox checkBox = sender as CheckBox;
            int controlValue = checkBox.Tag.GetHashCode();
            int value = this.GetInt32Value();

            if (checkBox.Checked == true)
            {
                if (controlValue == 0)
                    value = 0;
                else
                    value |= checkBox.Tag.GetHashCode();
            }
            else
            {
                value &= ~checkBox.Tag.GetHashCode();
            }

            //m_value = System.Enum.ToObject(m_flagType, value);
            this.UpdateValueToControl(value, controlValue != 0 ? checkBox : null);
        }
    }
}
