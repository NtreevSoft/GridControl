using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SimpleGridControl
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            DataTable dataTable = new DataTable("DataTable");
            dataTable.Columns.Add("Int32", typeof(int));
            dataTable.Columns.Add("Boolean", typeof(bool));
            dataTable.Columns.Add("Color", typeof(Color));
            dataTable.Columns.Add("Alignment", typeof(StringAlignment));

            for (int i = 0; i < 10; i++)
            {
                dataTable.Rows.Add();
            }

            this.gridControl1.DataSource = dataTable;
        }
    }
}
