using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Ntreev.Windows.Forms.Grid;
using System.Drawing;

namespace ColumnPainting
{
    class UserColumnPainter : ColumnPainter
    {
        StringFormat _stringFormat = new StringFormat();
        int _clickCount = 0;

        public UserColumnPainter()
        {
            _stringFormat.Alignment = StringAlignment.Center;
            _stringFormat.LineAlignment = StringAlignment.Near;
        }

        public void Clicked()
        {
            _clickCount++;
        }

        /// <summary>
        /// 배경을 그리는 부분입니다.
        /// </summary>
        /// <returns>
        /// 반환값이 false인 경우 기본 배경을 그려줍니다.
        /// </returns>
        public override bool PaintBackground(System.Drawing.Graphics g, System.Drawing.Rectangle renderRect, IColumnDescriptor columnDescriptor, IStyle style)
        {
            return false;
        }

        /// <summary>
        /// 컨텐츠를 그리는 부분입니다.
        /// </summary>
        /// <returns>
        /// 반환값이 false인 경우 기본 컨텐츠를 그려줍니다.
        /// </returns>
        public override bool PaintContents(System.Drawing.Graphics g, System.Drawing.Rectangle renderRect, IColumnDescriptor columnDescriptor, IStyle style)
        {
            string text = string.Format("{0}({1})", columnDescriptor.ColumnState, _clickCount);
            g.DrawString(text, style.Font, Brushes.Black, renderRect, _stringFormat);
            return true;
        }
    }
}
