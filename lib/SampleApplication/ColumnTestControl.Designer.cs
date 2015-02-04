namespace SampleApplication
{
    partial class ColumnTestControl
    {
        /// <summary> 
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 구성 요소 디자이너에서 생성한 코드

        /// <summary> 
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.columnButton123 = new SampleApplication.ColumnButton();
            this.column2 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnUITypeEditor1 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnButton1 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnUITypeEditor2 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnFlags = new Ntreev.Windows.Forms.Grid.Column();
            this.columnLinkedItemSelector1 = new SampleApplication.ColumnLinkedItemSelector();
            this.column3 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnDataLocation1 = new SampleApplication.ColumnDataLocation();
            this.columnEnable1 = new SampleApplication.ColumnEnable();
            this.columnTextBox1 = new Ntreev.Windows.Forms.Grid.Column();
            this.column1 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnRadio2 = new SampleApplication.ColumnRadio();
            this.columnLinkLabel1 = new SampleApplication.ColumnLinkLabel();
            this.ColumnComboBox = new Ntreev.Windows.Forms.Grid.Column();
            this.columnNumeric1 = new Ntreev.Windows.Forms.Grid.Column();
            this.columnDate1 = new SampleApplication.ColumnDate();
            this.columnDataTypes1 = new SampleApplication.ColumnDataTypes();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.gridControl1 = new Ntreev.Windows.Forms.Grid.GridControl();
            this.propertyGrid1 = new System.Windows.Forms.PropertyGrid();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // columnButton123
            // 
            this.columnButton123.CellPadding = new System.Windows.Forms.Padding(0, 0, 1, 1);
            this.columnButton123.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.SingClick;
            this.columnButton123.ColumnName = "Button";
            this.columnButton123.IsFrozen = true;
            this.columnButton123.IsResizable = false;
            // 
            // column2
            // 
            this.column2.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.column2.ColumnName = "Icon";
            this.column2.DataType = typeof(System.Drawing.Icon);
            // 
            // columnUITypeEditor1
            // 
            this.columnUITypeEditor1.CellBackColor = System.Drawing.Color.Red;
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
            this.columnUITypeEditor2.IsReadOnly = true;
            // 
            // columnFlags
            // 
            this.columnFlags.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.columnFlags.ColumnName = "Flags";
            this.columnFlags.DataType = typeof(SampleApplication.TestFlag);
            // 
            // columnLinkedItemSelector1
            // 
            this.columnLinkedItemSelector1.CellPadding = new System.Windows.Forms.Padding(0, 0, 1, 1);
            this.columnLinkedItemSelector1.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.SingClick;
            this.columnLinkedItemSelector1.ColumnName = "Column2";
            // 
            // column3
            // 
            this.column3.ColumnName = "Enums";
            this.column3.DataType = typeof(System.Environment.SpecialFolder);
            // 
            // columnDataLocation1
            // 
            this.columnDataLocation1.ColumnName = "Column0";
            this.columnDataLocation1.DataType = typeof(SampleApplication.DataLocation);
            this.columnDataLocation1.MaxWidth = 30;
            this.columnDataLocation1.MinWidth = 30;
            this.columnDataLocation1.Width = 30;
            // 
            // columnEnable1
            // 
            this.columnEnable1.ColumnName = "Column1";
            // 
            // columnTextBox1
            // 
            this.columnTextBox1.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.columnTextBox1.ColumnName = "DockStyle";
            this.columnTextBox1.DataType = typeof(System.Windows.Forms.DockStyle);
            // 
            // column1
            // 
            this.column1.CellMultiline = true;
            this.column1.CellWordWrap = true;
            this.column1.ClickEditType = Ntreev.Windows.Forms.Grid.ClickEditType.FocusedClick;
            this.column1.ColumnName = "Text";
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
            // columnDataTypes1
            // 
            this.columnDataTypes1.ColumnName = "DataTypes";
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
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
            this.splitContainer1.Size = new System.Drawing.Size(778, 482);
            this.splitContainer1.SplitterDistance = 599;
            this.splitContainer1.TabIndex = 0;
            // 
            // gridControl1
            // 
            this.gridControl1.AutoFitColumn = true;
            this.gridControl1.AutoFitRow = true;
            this.gridControl1.CaptionRow.Tag = "false";
            this.gridControl1.Columns.AddRange(new Ntreev.Windows.Forms.Grid.Column[] {
            this.column2,
            this.columnUITypeEditor1,
            this.columnButton1,
            this.columnUITypeEditor2,
            this.columnFlags,
            this.columnLinkedItemSelector1,
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
            this.gridControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gridControl1.ForeColor = System.Drawing.Color.Black;
            this.gridControl1.IsRowNumberVisible = false;
            this.gridControl1.Location = new System.Drawing.Point(0, 0);
            this.gridControl1.Name = "gridControl1";
            this.gridControl1.Size = new System.Drawing.Size(599, 482);
            this.gridControl1.TabIndex = 0;
            // 
            // propertyGrid1
            // 
            this.propertyGrid1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGrid1.Location = new System.Drawing.Point(0, 0);
            this.propertyGrid1.Name = "propertyGrid1";
            this.propertyGrid1.SelectedObject = this.gridControl1;
            this.propertyGrid1.Size = new System.Drawing.Size(175, 482);
            this.propertyGrid1.TabIndex = 0;
            // 
            // ColumnTestControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Name = "ColumnTestControl";
            this.Size = new System.Drawing.Size(778, 482);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        private Ntreev.Windows.Forms.Grid.GridControl gridControl1;
        private Ntreev.Windows.Forms.Grid.Column columnUITypeEditor1;
        private Ntreev.Windows.Forms.Grid.Column columnButton1;
        private ColumnLinkedItemSelector columnLinkedItemSelector1;
        private Ntreev.Windows.Forms.Grid.Column columnUITypeEditor2;
        private Ntreev.Windows.Forms.Grid.Column columnFlags;
        private Ntreev.Windows.Forms.Grid.Column columnTextBox1;
        private Ntreev.Windows.Forms.Grid.Column column1;
        private Ntreev.Windows.Forms.Grid.Column ColumnComboBox;
        private ColumnButton columnButton123;
        private Ntreev.Windows.Forms.Grid.Column column2;
        private ColumnLinkLabel columnLinkLabel1;
        private Ntreev.Windows.Forms.Grid.Column columnNumeric1;
        private ColumnDate columnDate1;
        private ColumnRadio columnRadio2;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.PropertyGrid propertyGrid1;
        private ColumnDataLocation columnDataLocation1;
        private ColumnEnable columnEnable1;
        private Ntreev.Windows.Forms.Grid.Column column3;
        private ColumnDataTypes columnDataTypes1;

        #endregion
    }
}
