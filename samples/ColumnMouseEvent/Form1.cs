using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Ntreev.Windows.Forms.Grid;

namespace ColumnMouseEvent
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            Column column = this.gridControl1.Columns.AddNew("아이콘 좌클릭", typeof(int));
            column.Width = 200;
            column.ColumnPainter = new UserColumnPainter();

            Random random = new Random();
            for (int i = 0; i < 10; i++)
            {
                Row row = this.gridControl1.Rows.AddNew();
                row[0].Value = random.Next();
            }
        }

        private void gridControl1_ColumnMouseUp(object sender, ColumnMouseEventArgs e)
        {
            UserColumnPainter columnPainter = e.Column.ColumnPainter as UserColumnPainter;
            if (columnPainter == null)
                return;

            Rectangle rect = new Rectangle(Point.Empty, e.Column.DisplayRectangle.Size);
            Rectangle buttonRect = columnPainter.ComputeButtonRectangle(rect);

            if (buttonRect.Contains(e.Location))
            {
                e.Handled = true;
                this.columnMenu.Show(e.Column, Cursor.Position);
            }
        }

        private void gridControl1_ColumnMouseDown(object sender, ColumnMouseEventArgs e)
        {
            UserColumnPainter columnPainter = e.Column.ColumnPainter as UserColumnPainter;
            if (columnPainter == null)
                return;

            Rectangle rect = new Rectangle(Point.Empty, e.Column.DisplayRectangle.Size);
            Rectangle buttonRect = columnPainter.ComputeButtonRectangle(rect);

            if (buttonRect.Contains(e.Location))
                e.Handled = true;
        }
    }
}