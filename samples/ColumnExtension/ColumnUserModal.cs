using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Ntreev.Windows.Forms.Grid;
using System.Windows.Forms;
using System.Drawing;

namespace ColumnExtension
{
    class ColumnUserModal : ColumnModal, IValuePainter
    {
        SolidBrush brush = new SolidBrush(Color.Empty);

        public ColumnUserModal()
        {
            this.DataType = typeof(Color);
        }

        protected override void OnEditValue(EditValueEventArgs e)
        {
            base.OnEditValue(e);
            ColorDialog colorDialog = new ColorDialog();

            if(e.Value != null)
                colorDialog.Color = ValidateValue(e.Value);
            if (colorDialog.ShowDialog() == DialogResult.OK)
                e.Value = colorDialog.Color;
        }

        /// <summary>
        /// 값의 유효성을 확인합니다.
        /// </summary>
        /// <param name="value">유효성을 확인할 값의 <see cref="System.Object"/>입니다.</param>
        /// <returns>
        /// 유효성 확인하여 수정된 <see cref="System.Drawing.Color"/>의 값을 반환합니다.
        /// </returns>
        private Color ValidateValue(object value)
        {
            if (value == null || value.GetType() != typeof(Color))
                return Color.Empty;
            return (Color)value;
        }

        public void PaintValue(Graphics graphic, Rectangle renderRect, Rectangle clipRect, IStyle style, object value)
        {
            brush.Color = ValidateValue(value);
            clipRect.Height--;
            graphic.FillRectangle(brush, clipRect);
            graphic.DrawRectangle(Pens.Black, clipRect);
        }

        public bool PaintValueSupported
        {
            get { return true; }
        }
    }
}
