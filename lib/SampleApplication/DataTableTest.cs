using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Ntreev.Windows.Forms.Grid;
using System.Threading;

namespace SampleApplication
{
    public partial class DataTableTest : UserControl
    {
        public DataTableTest()
        {
            InitializeComponent();
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            //this.gridControl1
            //this.gridControl1.DataSource = this.dataTable2;
            this.gridControl1.DataSource = this.dataTable1;
            this.column2.VisibleIndex = 1;
            //this.dataSet1.DefaultViewManager

            //this.gridControl1.DataMember = this.dataTable2.TableName;
            //this.gridControl1.DataMember = this.dataTable1.TableName;

            this.dataTable1.DefaultView.Sort = string.Format("{0} ASC", this.dataColumn1.ColumnName);

            Random random = new Random();
            for (int i = 10; i >= 0; i--)
            {
                var row = this.dataTable1.NewRow();

                //row.RowError = "row error" + i;
                //row.SetColumnError(1, "column error" + i);
                row[0] = i;
                row[1] = i.ToString();

                this.dataTable1.Rows.Add(row);
            }

            this.dataTable1.AcceptChanges();

            this.gridControl1.RowMoving += gridControl1_RowMoving;
            this.gridControl1.RowMoved += gridControl1_RowMoved;
            this.gridControl1.DataBindingComplete += gridControl1_DataBindingComplete;
            this.dataTable1.RowChanged += dataTable1_RowChanged;
            this.dataTable1.TableNewRow += dataTable1_TableNewRow;
            this.dataTable1.ColumnChanging += dataTable1_ColumnChanging;
            //this.dataTable1.ColumnChanged += dataTable1_ColumnChanged;
            this.gridControl1.Reseted += gridControl1_Reseted;
        }

        void dataTable1_TableNewRow(object sender, DataTableNewRowEventArgs e)
        {
            e.Row[this.dataColumn2] = "wqer";
        }

        void dataTable1_RowChanged(object sender, DataRowChangeEventArgs e)
        {
            if (e.Action == DataRowAction.Add)
            {
                this.ResetIndex(e.Row, (int)e.Row[this.dataColumn1], true);
            }
        }

        void dataTable1_ColumnChanging(object sender, DataColumnChangeEventArgs e)
        {
            if (e.Column != this.dataColumn1 || e.Row.RowState == DataRowState.Detached)
                return;

            e.ProposedValue = ResetIndex(e.Row, (int)e.ProposedValue, false);
        }

        private int ResetIndex(DataRow dataRow, int proposedValue, bool b)
        {
            this.dataTable1.ColumnChanging -= dataTable1_ColumnChanging;
            this.dataTable1.DefaultView.Sort = null;

            List<DataRow> items = new List<DataRow>(this.dataTable1.Rows.Count);
            foreach (DataRow item in this.dataTable1.Rows)
            {
                if (item.RowState == DataRowState.Added)
                    continue;
                items.Add(item);
            }
            items.Sort((x, y) =>
            {
                int x1 = (int)x[this.dataColumn1];
                //if (b == true && x == dataRow)
                //    x1 = int.MaxValue;
                int y1 = (int)y[this.dataColumn1];
                //if (b == true && y == dataRow)
                //    y1 = int.MaxValue;

                return x1.CompareTo(y1);
            });

            int index = (int)proposedValue;


            index = Math.Min(index, items.Count);

            if (index >= items.Count)
            {
                items.Remove(dataRow);
                items.Add(dataRow);
            }
            else
            {
                var targetRow = items[index];
                items.Remove(dataRow);
                items.Insert(items.IndexOf(targetRow), dataRow);
            }

            index = 0;
            foreach (var item in items)
            {
                item[this.dataColumn1] = index++;
            }

            this.dataTable1.AcceptChanges();
            this.dataTable1.DefaultView.Sort = string.Format("{0} ASC", this.dataColumn1.ColumnName);
            this.dataTable1.ColumnChanging += dataTable1_ColumnChanging;

            return (int)dataRow[this.dataColumn1];
        }

        void gridControl1_Reseted(object sender, EventArgs e)
        {
            //this.row.Select();
            //this.row.Focus();
        }

        void gridControl1_DataBindingComplete(object sender, EventArgs e)
        {
            //int qwer = 0;
        }

        Row row;

        void gridControl1_RowMoving(object sender, Ntreev.Windows.Forms.Grid.RowMovingEventArgs e)
        {
            e.Cancel = true;
            this.row = e.Row;
            e.Row[0] = e.Index;
        }

        void gridControl1_RowMoved(object sender, Ntreev.Windows.Forms.Grid.RowEventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            //var row = this.dataTable1.NewRow();

            //row[0] = this.dataTable1.Rows.Count;
            //row[1] = this.dataTable1.Rows.Count.ToString();
            ////row[0] = i.ToString();


            //this.dataTable1.Rows.Add(row);
            //if (dataTable1.Columns.Contains("wow1") == true)
            //    this.dataTable1.Columns.Remove("wow1");
            //else
            //    this.dataTable1.Columns.Add("wow1");
            Random random = new Random();
            var row = this.dataTable1.Rows[random.Next(this.dataTable1.Rows.Count)];
            row.RowError = "abc";

        }

        private void gridControl1_RowInserting(object sender, RowInsertingEventArgs e)
        {
            return;
            e.Cancel = true;

            Task.Run(() =>
            {
                Thread.Sleep(100);
                this.Invoke(new Action(() =>
                {
                    var row = this.dataTable1.NewRow();

                    row[0] = 0;
                    row[1] = this.dataTable1.Rows.Count.ToString();

                    this.dataTable1.Rows.Add(row);

                    var row1 = this.gridControl1.Rows.Last();
                    row1.GridControl.ClearSelection();
                    row1.Select();
                    row1.Focus();
                    row1.BringIntoView();
                    row1.GridControl.ClearInsertion();
                }));
            });
        }

        private void buttonInvalidValue_Click(object sender, EventArgs e)
        {
            Random random = new Random();
            var row = this.dataTable1.Rows[random.Next(this.dataTable1.Rows.Count)];

            row[this.dataColumn2] = "abc";
        }

        private void buttonError_Click(object sender, EventArgs e)
        {
            Random random = new Random();
            var row = this.gridControl1.Rows[random.Next(this.gridControl1.Rows.Count)];
            row.Error = "error";
        }

        private void buttonSourceError_Click(object sender, EventArgs e)
        {
            Random random = new Random();
            var row = this.dataTable1.Rows[random.Next(this.dataTable1.Rows.Count)];
            row.RowError = "source Error";
        }
    }
}
