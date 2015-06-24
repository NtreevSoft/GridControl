using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

namespace Ntreev.Windows.Forms.Grid.Design
{
    class BooleanTypeEditor : TypeEditor
    {
        private readonly VisualStyleRenderer m_rendererChecked;
        private readonly VisualStyleRenderer m_rendererCheckedHot;
        private readonly VisualStyleRenderer m_rendererUnchecked;
        private readonly VisualStyleRenderer m_rendererUncheckedHot;

        public BooleanTypeEditor(Type dataType)
            : base(dataType)
        {
            try
            {
                if (Application.RenderWithVisualStyles == true)
                {
                    m_rendererChecked = new VisualStyleRenderer(VisualStyleElement.Button.CheckBox.CheckedNormal);
                    m_rendererCheckedHot = new VisualStyleRenderer(VisualStyleElement.Button.CheckBox.CheckedHot);
                    m_rendererUnchecked = new VisualStyleRenderer(VisualStyleElement.Button.CheckBox.UncheckedNormal);
                    m_rendererUncheckedHot = new VisualStyleRenderer(VisualStyleElement.Button.CheckBox.UncheckedHot);
                }
            }
            catch (Exception)
            {

            }
        }

        public override bool CanEdit(ICell cell, EditingReason reason)
        {
            switch (reason.ReasonType)
            {
                case EditingReasonType.Mouse:
                    return cell.ClientRectangle.Contains(reason.Location);
                case EditingReasonType.Key:
                    {
                        switch (reason.Key)
                        {
                            case Keys.Space:
                            case Keys.Enter:
                                return true;
                            default:
                                return false;
                        }
                    }
                    break;
            }

            return base.CanEdit(cell, reason);

        }

        public override object EditValue(IEditorService editorService, ICell cell, object value)
        {
            Controls.InternalCheckBox checkBox = new Controls.InternalCheckBox(editorService);

            Rectangle rect = cell.ClientRectangle;

            if (value != null && value.ToString() == bool.TrueString)
                checkBox.Checked = true;
            else
                checkBox.Checked = false;

            checkBox.BackColor = cell.BackColor;
            checkBox.ForeColor = cell.ForeColor;
            checkBox.Font = cell.Font;
            checkBox.Bounds = rect;
            checkBox.Dock = DockStyle.Fill;

            editorService.ShowControl(checkBox);
            return checkBox.Checked;
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
                if (value.ToString() == System.Boolean.TrueString)
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
                    if ((cell.State & CellState.Hot) == CellState.Hot)
                        m_rendererCheckedHot.DrawBackground(graphics, rectangle);
                    else
                        m_rendererChecked.DrawBackground(graphics, rectangle);
                }
                else
                {
                    int x1 = paintRect.Width / 2 - 7 + paintRect.Left;
                    int y1 = paintRect.Height / 2 - 7 + paintRect.Top;
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
                    if ((cell.State & CellState.Hot) == CellState.Hot)
                        m_rendererUncheckedHot.DrawBackground(graphics, rectangle);
                    else
                        m_rendererUnchecked.DrawBackground(graphics, rectangle);
                }
                else
                {
                    int x1 = paintRect.Width / 2 - 7 + paintRect.Left;
                    int y1 = paintRect.Height / 2 - 7 + paintRect.Top;
                    ControlPaint.DrawCheckBox(graphics, x1, y1, 13, 13, ButtonState.Normal);
                }
            }
        }

        public override ViewType ViewType
        {
            get
            {
                return ViewType.Custom;
            }
        }
    }
}
