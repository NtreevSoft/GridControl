using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ColumnExtension
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            DataTable dataTable = new DataTable("DataTable");
            dataTable.Columns.Add("Boolean", typeof(bool));
            dataTable.Columns.Add("DateTime", typeof(DateTime));
            dataTable.Columns.Add("Color", typeof(Color));

            for (int i = 0; i < 10; i++)
            {
                dataTable.Rows.Add();
            }

            this.gridControl1.DataSource = dataTable;
        }

        private void gridControl1_ColumnBinding(object sender, Ntreev.Windows.Forms.Grid.ColumnBindingEventArgs e)
        {
            switch (e.PropertyDescriptor.Name)
            {
                case "Boolean":
                    e.BindingColumn = new ColumnUserControl();
                    break;
                case "DateTime":
                    e.BindingColumn = new ColumnUserDropDown();
                    break;
                case "Color":
                    e.BindingColumn = new ColumnUserModal();
                    break;
            }
        }
    }
}
