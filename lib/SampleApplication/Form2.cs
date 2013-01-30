using Ntreev.Crema.Data;
using Ntreev.Windows.Forms.Grid;
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
            //dataSet.ReadXmlSchema(@".\ItemContent.xsd");
            //dataSet.ReadXml(@".\ItemContent.xml");

            dataSet.ReadXmlSchema(@"F:\Crema\repositories\cherry\xml\Mina26\Combat_abc.xsd");
            //dataSet.ReadXmlSchema(@"G:\NTG\Crema\data\test\xml\Root\Root\wow\NtreevSoft.xsd");
            dataSet.ReadXml(@"F:\Crema\repositories\cherry\xml\Mina26\Combat_abc.xml");
            //dataSet.ReadXml(@"G:\NTG\Crema\data\test\xml\Root\Root\wow\NtreevSoft.xml");

            //this.gridControl1.ColumnBindingCreation = Ntreev.Windows.Forms.Grid.ColumnBindingCreation.None;

            INIDataTable table = dataSet.GetFirstPrimaryTable();

            //for (int i = 0; i < 10; i++)
            //{
            //    var q = table.Columns.Add();
            //    table.Columns.Remove(q);
            //}

            table.Columns.Add();
           
            this.gridControl1.DataSource = dataSet.GetFirstPrimaryTable();
            this.columnID.VisibleIndex = 1000;
            this.columnID.IsFrozen = true;
            this.dataGridView1.DataSource = dataSet.Tables[0];
            this.dataGridView2.DataSource = dataSet.Tables[1];
            //this.dataGridView3.DataSource = dataSet.Tables[2];
        }


        private void gridControl1_RowInserting(object sender, RowInsertingEventArgs e)
        {
            return;
            e.Cancel = true;


            
            this.BeginInvoke((Action<DataRow>)((row) =>
                {
                    INIDataTable table = (this.gridControl1.DataSource as INIDataTable);
                    INIDataRow dataRow = table.NewRow();

                    foreach (INIDataColumn item in table.Columns)
                    {
                        dataRow[item] = row[item];
                    }

                    table.Rows.Add(dataRow);

                    this.gridControl1.InsertionRow.CancelEdit();
                    //this.gridControl1.ClearSelection();
                    //this.gridControl1.InsertionRow.Clear();

                }), (e.Component as DataRowView).Row);
        }

        private void gridControl1_RowChanged(object sender, RowEventArgs e)
        {
            Color c = Color.FromArgb(100, 101, 23, 32);
            e.Row.ResetCellBackColor();
        }

        private void gridControl1_ColumnBinding(object sender, ColumnBindingEventArgs e)
        {
            GridControl control = sender as GridControl;
            //if (string.IsNullOrEmpty(control.DataMember) == false)
            {
                INIDataTable dataTable = control.DataSource as INIDataTable;
                INIDataColumn dataColumn = dataTable.Columns[e.PropertyDescriptor.Name];

                if (e.BindingColumn == null)
                {
                    e.BindingColumn = new Column();

                    if (dataColumn != null)
                    {
                        e.BindingColumn.DataType = dataColumn.DataType;
                        if (dataColumn.DataType == typeof(string))
                            e.BindingColumn.CellMultiline = true;
                    }
                }

                if (dataColumn != null && string.IsNullOrEmpty(dataColumn.Description) == false)
                {
                    e.BindingColumn.Tooltip = dataColumn.Description;
                }
            }
        }
    }
}
  