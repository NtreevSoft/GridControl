using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

namespace Ntreev.Windows.Forms.Grid.Columns
{
    public class ColumnCheckBox : ColumnControl<CheckBox>
    {
        private VisualStyleRenderer m_rendererChecked;
        private VisualStyleRenderer m_rendererUnchecked;

        public ColumnCheckBox()
        {
            this.Control.CheckAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.Control.CheckedChanged += checkBox_CheckedChanged;

            try
            {
                m_rendererChecked = new VisualStyleRenderer(VisualStyleElement.Button.CheckBox.CheckedNormal);
                m_rendererUnchecked = new VisualStyleRenderer(VisualStyleElement.Button.CheckBox.UncheckedNormal);
            }
            catch
            {

            }
        }

        public override void PaintValue(Graphics graphics, Rectangle paintRect, ICell cell, object value)
        {
            bool isChecked = false;
            if (value == null)
            {
                isChecked = false;
            }
            else if (value.GetType() == typeof(bool))
            {
                isChecked = (bool)value;
            }
            else
            {
                if (value.ToString() == bool.TrueString)
                    isChecked = true;
                else
                    isChecked = false;
            }

            if (isChecked == true)
            {
                if (m_rendererChecked != null)
                {
                    Rectangle rectangle = paintRect;
                    if (rectangle.Width % 2 == 1)
                        rectangle.Width--;
                    if (rectangle.Height % 2 == 1)
                        rectangle.Height--;

                    m_rendererChecked.DrawBackground(graphics, rectangle);
                }
                else
                {
                    int x1 = paintRect.Width / 2 - 6 + paintRect.Left;
                    int y1 = paintRect.Height / 2 - 6 + paintRect.Top;
                    ControlPaint.DrawCheckBox(graphics, x1, y1, 13, 13, ButtonState.Checked);
                }
            }
            else
            {
                if (m_rendererUnchecked != null)
                {
                    Rectangle rectangle = paintRect;
                    if (rectangle.Width % 2 == 1)
                        rectangle.Width--;
                    if (rectangle.Height % 2 == 1)
                        rectangle.Height--;
                    m_rendererUnchecked.DrawBackground(graphics, rectangle);
                }
                else
                {
                    int x1 = paintRect.Width / 2 - 6 + paintRect.Left;
                    int y1 = paintRect.Height / 2 - 6 + paintRect.Top;
                    ControlPaint.DrawCheckBox(graphics, x1, y1, 13, 13, ButtonState.Normal);
                }
            }
        }

        protected override object GetControlValue(CheckBox control)
        {
            if (DataType == typeof(bool))
            {
                return control.Checked;
            }
            return control.Checked.ToString();
        }

        protected override void SetControlValue(CheckBox control, object value)
        {
            if (value == null)
            {
                control.Checked = false;
                return;
            }

            if (value.GetType() == typeof(bool))
                control.Checked = (bool)value;
            else
                control.Checked = value.ToString() == bool.TrueString;
        }

        private void checkBox_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
