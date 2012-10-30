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
    }
}
