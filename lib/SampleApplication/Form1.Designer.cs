#region License
//Ntreev Grid for .Net 2.0.4646.22417
//https://github.com/NtreevSoft/GridControl

//Released under the MIT License.

//Copyright (c) 2010 Ntreev Soft co., Ltd.

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
//rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
//persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
//Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
//WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
//COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
//OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#endregion


namespace SampleApplication
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.dataSet1 = new System.Data.DataSet();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.column2 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnUITypeEditor1 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnButton1 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnUITypeEditor2 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnComboBox1 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnTextBox1 = new Ntreev.Windows.Forms.Grid.Column();
            this.column1 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnButton123 = new SampleApplication.ColumnButton();
            this.columnRadio2 = new SampleApplication.ColumnRadio();
            this.columnLinkLabel1 = new SampleApplication.ColumnLinkLabel();
            this.ColumnComboBox = new Ntreev.Windows.Forms.Grid.Column();
            this.columnNumeric1 = new SampleApplication.ColumnNumeric();
            this.columnDate1 = new SampleApplication.ColumnDate();
            this.gridControl1 = new Ntreev.Windows.Forms.Grid.GridControl();
            this.propertyGrid1 = new System.Windows.Forms.PropertyGrid();
            this.style1 = new Ntreev.Windows.Forms.Grid.Style();
            ((System.ComponentModel.ISupportInitialize)(this.dataSet1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // dataSet1
            // 
            this.dataSet1.DataSetName = "NewDataSet";
            // 
            // splitContainer1
            // 
            resources.ApplyResources(this.splitContainer1, "splitContainer1");
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.BackColor = System.Drawing.SystemColors.ControlDark;
            this.splitContainer1.Panel1.Controls.Add(this.gridControl1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.propertyGrid1);
            // 
            // column2
            // 
            this.column2.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.column2.ColumnName = "Icon";
            this.column2.DataType = typeof(System.Drawing.Icon);
            // 
            // columnUITypeEditor1
            // 
            this.columnUITypeEditor1.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.SingClick;
            this.columnUITypeEditor1.ColumnName = "CheckBox";
            this.columnUITypeEditor1.DataType = typeof(bool);
            // 
            // columnButton1
            // 
            this.columnButton1.ColumnName = "Cursor";
            this.columnButton1.DataType = typeof(System.Windows.Forms.Cursor);
            // 
            // columnUITypeEditor2
            // 
            this.columnUITypeEditor2.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.columnUITypeEditor2.ColumnName = "Color";
            this.columnUITypeEditor2.DataType = typeof(System.Drawing.Color);
            // 
            // columnComboBox1
            // 
            this.columnComboBox1.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.columnComboBox1.ColumnName = "Flags";
            this.columnComboBox1.DataType = typeof(System.AttributeTargets);
            // 
            // columnTextBox1
            // 
            this.columnTextBox1.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.columnTextBox1.ColumnName = "DockStyle";
            this.columnTextBox1.DataType = typeof(System.Windows.Forms.DockStyle);
            // 
            // column1
            // 
            this.column1.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.column1.ColumnName = "Text";
            // 
            // columnButton123
            // 
            this.columnButton123.CellMinHeight = 21;
            this.columnButton123.CellPadding = new System.Windows.Forms.Padding(0, 0, 1, 1);
            this.columnButton123.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.SingClick;
            this.columnButton123.ColumnName = "Button";
            this.columnButton123.IsFrozen = true;
            this.columnButton123.IsResizable = false;
            // 
            // columnRadio2
            // 
            this.columnRadio2.ColumnName = "Radio";
            this.columnRadio2.DataType = typeof(bool);
            // 
            // columnLinkLabel1
            // 
            this.columnLinkLabel1.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.SingClick;
            this.columnLinkLabel1.ColumnName = "LinkLabel";
            this.columnLinkLabel1.IsGroupable = false;
            // 
            // ColumnComboBox
            // 
            this.ColumnComboBox.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.ColumnComboBox.ColumnName = "ComboBox";
            this.ColumnComboBox.DataType = typeof(SampleApplication.Test);
            // 
            // columnNumeric1
            // 
            this.columnNumeric1.ColumnName = "Int32";
            this.columnNumeric1.DataType = typeof(int);
            // 
            // columnDate1
            // 
            this.columnDate1.ColumnName = "Date";
            // 
            // gridControl1
            // 
            this.gridControl1.AutoFitColumn = true;
            this.gridControl1.AutoFitRow = true;
            this.gridControl1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.gridControl1.CaptionRow.Tag = "false";
            this.gridControl1.Columns.AddRange(new Ntreev.Windows.Forms.Grid.Column[] {
            this.column2,
            this.columnUITypeEditor1,
            this.columnButton1,
            this.columnUITypeEditor2,
            this.columnComboBox1,
            this.columnTextBox1,
            this.column1,
            this.columnButton123,
            this.columnRadio2,
            this.columnLinkLabel1,
            this.ColumnComboBox,
            this.columnNumeric1,
            this.columnDate1});
            resources.ApplyResources(this.gridControl1, "gridControl1");
            this.gridControl1.IsColumnMovable = false;
            this.gridControl1.IsRowNumberVisible = false;
            this.gridControl1.IsRowResizable = false;
            this.gridControl1.Name = "gridControl1";
            this.gridControl1.ColumnMouseDown += new Ntreev.Windows.Forms.Grid.ColumnMouseEventHandler(this.gridControl1_ColumnMouseDown);
            // 
            // propertyGrid1
            // 
            resources.ApplyResources(this.propertyGrid1, "propertyGrid1");
            this.propertyGrid1.Name = "propertyGrid1";
            this.propertyGrid1.SelectedObject = this.gridControl1;
            // 
            // style1
            // 
            this.style1.CaptionBackColor = System.Drawing.Color.Silver;
            this.style1.RowBackColor = System.Drawing.Color.DimGray;
            // 
            // Form1
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Name = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataSet1)).EndInit();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Ntreev.Windows.Forms.Grid.GridControl gridControl1;
        private Ntreev.Windows.Forms.Grid.Column columnUITypeEditor1;
        private Ntreev.Windows.Forms.Grid.Column columnButton1;
        private Ntreev.Windows.Forms.Grid.Column columnUITypeEditor2;
        private Ntreev.Windows.Forms.Grid.Column columnComboBox1;
        private Ntreev.Windows.Forms.Grid.Column columnTextBox1;
        private Ntreev.Windows.Forms.Grid.Column column1;
        private Ntreev.Windows.Forms.Grid.Column ColumnComboBox;
        private ColumnButton columnButton123;
        private Ntreev.Windows.Forms.Grid.Column column2;
        private ColumnLinkLabel columnLinkLabel1;
        private System.Data.DataSet dataSet1;
        private ColumnNumeric columnNumeric1;
        private ColumnDate columnDate1;
        private ColumnRadio columnRadio2;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.PropertyGrid propertyGrid1;
        private Ntreev.Windows.Forms.Grid.Style style1;
    }
}