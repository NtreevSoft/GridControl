using Ntreev.INI3.Data;
using Ntreev.INI3.IO;
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
            dataSet.ReadSchema(@".\ItemContent.xsd");
            dataSet.ReadXml(@".\ItemContent.xml");

            this.gridControl1.CellMouseEnter += gridControl1_CellMouseEnter;
            this.gridControl1.CellMouseLeave += gridControl1_CellMouseLeave;

            //using(INIStringWriter sw = new INIStringWriter())
            //{
            //    dataSet.WriteXml(sw);
            //    string s = sw.ToString();
            //    int qwer = 0;
            //}

            //INIDataTable table = dataSet.Tables[1];
            //INIDataRow row = table.NewRow("<ID>1</ID>");

            //row["ID"] = 100;
            //table.Rows.Add(row);

            this.gridControl1.DataSource = dataSet.GetFirstPrimaryTable();
            this.dataGridView1.DataSource = dataSet.Tables[0];
            this.dataGridView2.DataSource = dataSet.Tables[1];
            this.dataGridView3.DataSource = dataSet.Tables[2];
        }

        void gridControl1_CellMouseLeave(object sender, Ntreev.Windows.Forms.Grid.CellEventArgs e)
        {
            Rectangle bounds = e.Cell.Bounds;
            bounds.Inflate(2, 2);
            bounds.Height += this.Font.Height;
            this.Invalidate(bounds);
        }

        void gridControl1_CellMouseEnter(object sender, Ntreev.Windows.Forms.Grid.CellEventArgs e)
        {
            Rectangle bounds = e.Cell.Bounds;
            bounds.Inflate(2, 2);
            bounds.Height += this.Font.Height;
            this.Invalidate(bounds);
        }
    }
}
