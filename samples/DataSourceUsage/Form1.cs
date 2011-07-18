using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Ntreev.Windows.Forms.Grid;

namespace DataSourceUsage
{
    public partial class Form1 : Form
    {
        static readonly int maxRowCount = 10;

        public Form1()
        {
            InitializeComponent();
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            InitializeFromArray(this.gridControl1);
            InitializeFromList(this.gridControl2);
            InitializeFromDataTable(this.gridControl3);
            InitializeFromDataSet(this.gridControl4);
            InitializeFromBindingList(this.gridControl5);
        }

        void InitializeFromArray(GridControl gridControl)
        {
            gridControl.DataSource = new TestStruct[Form1.maxRowCount];
        }

        void InitializeFromList(GridControl gridControl)
        {
            List<TestStruct> listDataSource = new List<TestStruct>(Form1.maxRowCount);
            for (int i = 0; i < listDataSource.Capacity; i++)
            {
                listDataSource.Add(new TestStruct());
            }

            gridControl.DataSource = listDataSource;
        }

        void InitializeFromDataTable(GridControl gridControl)
        {
            DataTable dataTable = new DataTable("table");

            dataTable.Columns.Add("Text", typeof(string));
            dataTable.Columns.Add("Number", typeof(int));
            dataTable.Columns.Add("Check", typeof(bool));

            for (int i = 0; i < Form1.maxRowCount; i++)
            {
                dataTable.Rows.Add();
            }

            gridControl.DataSource = dataTable;
        }

        void InitializeFromDataSet(GridControl gridControl)
        {
            DataSet dataSet = new DataSet("dataSet");

            DataTable dataTable1 = dataSet.Tables.Add("table1");

            dataTable1.Columns.Add("Text1", typeof(string));
            dataTable1.Columns.Add("Number1", typeof(int));
            dataTable1.Columns.Add("Check1", typeof(bool));

            for (int i = 0; i < Form1.maxRowCount; i++)
            {
                dataTable1.Rows.Add();
            }

            DataTable dataTable2 = dataSet.Tables.Add("table2");

            dataTable2.Columns.Add("Text2", typeof(string));
            dataTable2.Columns.Add("Number2", typeof(int));
            dataTable2.Columns.Add("Check2", typeof(bool));

            for (int i = 0; i < Form1.maxRowCount; i++)
            {
                dataTable2.Rows.Add();
            }

            gridControl.DataSource = dataSet;
            gridControl.DataMember = "table2";
        }

        void InitializeFromBindingList(GridControl gridControl)
        {
            UserDataSource userDataSource = new UserDataSource(Form1.maxRowCount);
            for (int i = 0; i < userDataSource.Capacity; i++)
            {
                userDataSource.Add(new TestStruct());
            }

            gridControl.DataSource = userDataSource;
        }
    }
}
