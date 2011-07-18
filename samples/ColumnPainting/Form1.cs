using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Ntreev.Windows.Forms.Grid;

namespace ColumnPainting
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            for (int i = 0; i < 10; i++)
            {
                Column column = this.gridControl1.Columns.AddNew();
                column.ColumnPainter = new UserColumnPainter();
            }

            for (int i = 0; i < 10; i++)
            {
                this.gridControl1.Rows.AddNew();
            }
        }

        private void gridControl1_ColumnMouseUp(object sender, ColumnMouseEventArgs e)
        {
            UserColumnPainter userPaint = e.Column.ColumnPainter as UserColumnPainter;
            if (userPaint == null)
                return;
            userPaint.Clicked();
        }
    }
}
