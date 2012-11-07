using Ntreev.Snowflaker.Data;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SampleApplication
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();

            INIDataSet dataSet = new INIDataSet();
            dataSet.ReadXmlSchema(@".\ItemContent.xsd");
            dataSet.ReadXml(@".\ItemContent.xml");

            this.gridControl1.ColumnBindingCreation = Ntreev.Windows.Forms.Grid.ColumnBindingCreation.None;

            INIDataTable table = dataSet.GetFirstPrimaryTable();

            for (int i = 0; i < 10; i++)
            {
                var q = table.Columns.Add();
                table.Columns.Remove(q);
            }

            table.Columns.Add();
           
            this.gridControl1.DataSource = dataSet.GetFirstPrimaryTable();
            this.dataGridView1.DataSource = dataSet.Tables[0];
            this.dataGridView2.DataSource = dataSet.Tables[1];
            this.dataGridView3.DataSource = dataSet.Tables[2];
        }
    }
}
