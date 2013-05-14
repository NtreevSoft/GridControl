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
            this.columnFlags = new Ntreev.Windows.Forms.Grid.Column();
            this.column3 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnDataLocation1 = new SampleApplication.ColumnDataLocation();
            this.columnEnable1 = new SampleApplication.ColumnEnable();
            this.columnTextBox1 = new Ntreev.Windows.Forms.Grid.Column();
            this.column1 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnButton123 = new SampleApplication.ColumnButton();
            this.columnRadio2 = new SampleApplication.ColumnRadio();
            this.columnLinkLabel1 = new SampleApplication.ColumnLinkLabel();
            this.ColumnComboBox = new Ntreev.Windows.Forms.Grid.Column();
            this.columnNumeric1 = new SampleApplication.ColumnNumeric();
            this.columnDate1 = new SampleApplication.ColumnDate();
            this.columnDataTypes1 = new SampleApplication.ColumnDataTypes();
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
            this.column2.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.column2.VisibleIndex = 1;
            // 
            // columnUITypeEditor1
            // 
            this.columnUITypeEditor1.ColumnName = "CheckBox";
            this.columnUITypeEditor1.DataType = typeof(bool);
            this.columnUITypeEditor1.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.columnUITypeEditor1.VisibleIndex = 2;
            // 
            // columnButton1
            // 
            this.columnButton1.ColumnName = "Cursor";
            this.columnButton1.DataType = typeof(System.Windows.Forms.Cursor);
            this.columnButton1.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.columnButton1.VisibleIndex = 3;
            // 
            // columnUITypeEditor2
            // 
            this.columnUITypeEditor2.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.columnUITypeEditor2.ColumnName = "Color";
            this.columnUITypeEditor2.DataType = typeof(System.Drawing.Color);
            this.columnUITypeEditor2.IsReadOnly = true;
            this.columnUITypeEditor2.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.columnUITypeEditor2.VisibleIndex = 4;
            // 
            // columnFlags
            // 
            this.columnFlags.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.columnFlags.ColumnName = "Flags";
            this.columnFlags.DataType = typeof(System.AttributeTargets);
            this.columnFlags.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.columnFlags.VisibleIndex = 5;
            // 
            // column3
            // 
            this.column3.ColumnName = "Enums";
            this.column3.DataType = typeof(System.Environment.SpecialFolder);
            this.column3.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.column3.VisibleIndex = 6;
            // 
            // columnDataLocation1
            // 
            this.columnDataLocation1.ColumnName = "Column0";
            this.columnDataLocation1.DataType = typeof(SampleApplication.DataLocation);
            this.columnDataLocation1.MaxWidth = 30;
            this.columnDataLocation1.MinWidth = 30;
            this.columnDataLocation1.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.columnDataLocation1.VisibleIndex = 7;
            // 
            // columnEnable1
            // 
            this.columnEnable1.ColumnName = "Column1";
            this.columnEnable1.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.columnEnable1.VisibleIndex = 8;
            // 
            // columnTextBox1
            // 
            this.columnTextBox1.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.columnTextBox1.ColumnName = "DockStyle";
            this.columnTextBox1.DataType = typeof(System.Windows.Forms.DockStyle);
            this.columnTextBox1.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.columnTextBox1.VisibleIndex = 9;
            // 
            // column1
            // 
            this.column1.CellMultiline = true;
            this.column1.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.column1.ColumnName = "Text";
            this.column1.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.column1.VisibleIndex = 10;
            // 
            // columnButton123
            // 
            this.columnButton123.CellMinHeight = 21;
            this.columnButton123.CellPadding = new System.Windows.Forms.Padding(0, 0, 1, 1);
            this.columnButton123.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.SingClick;
            this.columnButton123.ColumnName = "Button";
            this.columnButton123.IsFrozen = true;
            this.columnButton123.IsResizable = false;
            this.columnButton123.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.columnButton123.VisibleIndex = 0;
            // 
            // columnRadio2
            // 
            this.columnRadio2.ColumnName = "Radio";
            this.columnRadio2.DataType = typeof(bool);
            this.columnRadio2.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.columnRadio2.VisibleIndex = 11;
            // 
            // columnLinkLabel1
            // 
            this.columnLinkLabel1.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.SingClick;
            this.columnLinkLabel1.ColumnName = "LinkLabel";
            this.columnLinkLabel1.IsGroupable = false;
            this.columnLinkLabel1.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.columnLinkLabel1.VisibleIndex = 12;
            // 
            // ColumnComboBox
            // 
            this.ColumnComboBox.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.ColumnComboBox.ColumnName = "ComboBox";
            this.ColumnComboBox.DataType = typeof(SampleApplication.Test);
            this.ColumnComboBox.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.ColumnComboBox.VisibleIndex = 13;
            // 
            // columnNumeric1
            // 
            this.columnNumeric1.ColumnName = "Int32";
            this.columnNumeric1.DataType = typeof(int);
            this.columnNumeric1.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.columnNumeric1.VisibleIndex = 14;
            // 
            // columnDate1
            // 
            this.columnDate1.ColumnName = "Date";
            this.columnDate1.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.columnDate1.VisibleIndex = 15;
            // 
            // columnDataTypes1
            // 
            this.columnDataTypes1.ColumnName = "DataTypes";
            this.columnDataTypes1.Padding = new System.Windows.Forms.Padding(5, 3, 5, 3);
            this.columnDataTypes1.VisibleIndex = 16;
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
            this.columnFlags,
            this.column3,
            this.columnDataLocation1,
            this.columnEnable1,
            this.columnTextBox1,
            this.column1,
            this.columnButton123,
            this.columnRadio2,
            this.columnLinkLabel1,
            this.ColumnComboBox,
            this.columnNumeric1,
            this.columnDate1,
            this.columnDataTypes1});
            resources.ApplyResources(this.gridControl1, "gridControl1");
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
        private Ntreev.Windows.Forms.Grid.Column columnFlags;
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
        private ColumnDataLocation columnDataLocation1;
        private ColumnEnable columnEnable1;
        private Ntreev.Windows.Forms.Grid.Column column3;
        private ColumnDataTypes columnDataTypes1;
    }
}