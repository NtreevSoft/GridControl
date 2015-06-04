namespace SampleApplication
{
    partial class DataTableTest
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
            this.dataSet1 = new System.Data.DataSet();
            this.dataTable1 = new System.Data.DataTable();
            this.dataColumn1 = new System.Data.DataColumn();
            this.dataColumn2 = new System.Data.DataColumn();
            this.dataTable2 = new System.Data.DataTable();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.column2 = new Ntreev.Windows.Forms.Grid.Column();
            this.gridControl1 = new Ntreev.Windows.Forms.Grid.GridControl();
            this.panel1 = new System.Windows.Forms.Panel();
            this.buttonDeleteRow = new System.Windows.Forms.Button();
            this.buttonSourceError = new System.Windows.Forms.Button();
            this.buttonError = new System.Windows.Forms.Button();
            this.buttonInvalidValue = new System.Windows.Forms.Button();
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.panel2 = new System.Windows.Forms.Panel();
            ((System.ComponentModel.ISupportInitialize)(this.dataSet1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataTable1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataTable2)).BeginInit();
            this.tableLayoutPanel1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // dataSet1
            // 
            this.dataSet1.DataSetName = "NewDataSet";
            this.dataSet1.Tables.AddRange(new System.Data.DataTable[] {
            this.dataTable1,
            this.dataTable2});
            // 
            // dataTable1
            // 
            this.dataTable1.Columns.AddRange(new System.Data.DataColumn[] {
            this.dataColumn1,
            this.dataColumn2});
            this.dataTable1.TableName = "Table1";
            // 
            // dataColumn1
            // 
            this.dataColumn1.AllowDBNull = false;
            this.dataColumn1.ColumnName = "Column1";
            this.dataColumn1.DataType = typeof(int);
            // 
            // dataColumn2
            // 
            this.dataColumn2.ColumnName = "Column2";
            // 
            // dataTable2
            // 
            this.dataTable2.TableName = "Table2";
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 74F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 26F));
            this.tableLayoutPanel1.Controls.Add(this.panel1, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.panel2, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(812, 468);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // column2
            // 
            this.column2.ColumnName = "Column2";
            this.column2.DataType = typeof(int);
            // 
            // gridControl1
            // 
            this.gridControl1.CaptionRow.Height = 21;
            this.gridControl1.Columns.AddRange(new Ntreev.Windows.Forms.Grid.Column[] {
            this.column2});
            this.gridControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gridControl1.IsRowMovable = true;
            this.gridControl1.Location = new System.Drawing.Point(0, 0);
            this.gridControl1.Name = "gridControl1";
            this.gridControl1.Size = new System.Drawing.Size(592, 460);
            this.gridControl1.TabIndex = 0;
            this.gridControl1.RowUnbinded += new Ntreev.Windows.Forms.Grid.RowEventHandler(this.gridControl1_RowUnbinded);
            this.gridControl1.RowInserting += new Ntreev.Windows.Forms.Grid.RowInsertingEventHandler(this.gridControl1_RowInserting);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.buttonDeleteRow);
            this.panel1.Controls.Add(this.buttonSourceError);
            this.panel1.Controls.Add(this.buttonError);
            this.panel1.Controls.Add(this.buttonInvalidValue);
            this.panel1.Controls.Add(this.linkLabel1);
            this.panel1.Controls.Add(this.textBox1);
            this.panel1.Controls.Add(this.button1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(603, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(206, 462);
            this.panel1.TabIndex = 1;
            // 
            // buttonDeleteRow
            // 
            this.buttonDeleteRow.Location = new System.Drawing.Point(21, 330);
            this.buttonDeleteRow.Name = "buttonDeleteRow";
            this.buttonDeleteRow.Size = new System.Drawing.Size(100, 23);
            this.buttonDeleteRow.TabIndex = 5;
            this.buttonDeleteRow.Text = "DeleteRow";
            this.buttonDeleteRow.UseVisualStyleBackColor = true;
            this.buttonDeleteRow.Click += new System.EventHandler(this.buttonDeleteRow_Click);
            // 
            // buttonSourceError
            // 
            this.buttonSourceError.Location = new System.Drawing.Point(21, 359);
            this.buttonSourceError.Name = "buttonSourceError";
            this.buttonSourceError.Size = new System.Drawing.Size(100, 23);
            this.buttonSourceError.TabIndex = 5;
            this.buttonSourceError.Text = "SourceError";
            this.buttonSourceError.UseVisualStyleBackColor = true;
            this.buttonSourceError.Click += new System.EventHandler(this.buttonSourceError_Click);
            // 
            // buttonError
            // 
            this.buttonError.Location = new System.Drawing.Point(21, 388);
            this.buttonError.Name = "buttonError";
            this.buttonError.Size = new System.Drawing.Size(100, 23);
            this.buttonError.TabIndex = 4;
            this.buttonError.Text = "Error";
            this.buttonError.UseVisualStyleBackColor = true;
            this.buttonError.Click += new System.EventHandler(this.buttonError_Click);
            // 
            // buttonInvalidValue
            // 
            this.buttonInvalidValue.Location = new System.Drawing.Point(34, 151);
            this.buttonInvalidValue.Name = "buttonInvalidValue";
            this.buttonInvalidValue.Size = new System.Drawing.Size(129, 23);
            this.buttonInvalidValue.TabIndex = 3;
            this.buttonInvalidValue.Text = "Invalid Value";
            this.buttonInvalidValue.UseVisualStyleBackColor = true;
            this.buttonInvalidValue.Click += new System.EventHandler(this.buttonInvalidValue_Click);
            // 
            // linkLabel1
            // 
            this.linkLabel1.AutoSize = true;
            this.linkLabel1.Location = new System.Drawing.Point(32, 307);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(61, 12);
            this.linkLabel1.TabIndex = 2;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "linkLabel1";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(34, 240);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 21);
            this.textBox1.TabIndex = 1;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(34, 44);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(129, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // panel2
            // 
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel2.Controls.Add(this.gridControl1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(3, 3);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(594, 462);
            this.panel2.TabIndex = 2;
            // 
            // DataTableTest
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "DataTableTest";
            this.Size = new System.Drawing.Size(812, 468);
            ((System.ComponentModel.ISupportInitialize)(this.dataSet1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataTable1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataTable2)).EndInit();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        private Ntreev.Windows.Forms.Grid.GridControl gridControl1;
        private System.Data.DataSet dataSet1;
        private System.Data.DataTable dataTable1;
        private System.Data.DataColumn dataColumn1;
        private System.Data.DataColumn dataColumn2;
        private System.Data.DataTable dataTable2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.LinkLabel linkLabel1;
        private Ntreev.Windows.Forms.Grid.Column column2;

        #endregion
        private System.Windows.Forms.Button buttonInvalidValue;
        private System.Windows.Forms.Button buttonSourceError;
        private System.Windows.Forms.Button buttonError;
        private System.Windows.Forms.Button buttonDeleteRow;
        private System.Windows.Forms.Panel panel2;
    }
}
