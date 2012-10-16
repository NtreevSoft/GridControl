namespace SampleApplication
{
    partial class Form2
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
            this.column1 = new Ntreev.Windows.Forms.Grid.Column();
            this.column2 = new Ntreev.Windows.Forms.Grid.Column();
            this.column3 = new Ntreev.Windows.Forms.Grid.Column();
            this.column4 = new Ntreev.Windows.Forms.Grid.Column();
            this.gridControl1 = new Ntreev.Windows.Forms.Grid.GridControl();
            this.dataSet1 = new System.Data.DataSet();
            this.dataTable1 = new System.Data.DataTable();
            this.dataColumn1 = new System.Data.DataColumn();
            this.dataColumn2 = new System.Data.DataColumn();
            this.dataColumn3 = new System.Data.DataColumn();
            this.dataTable2 = new System.Data.DataTable();
            this.dataColumn4 = new System.Data.DataColumn();
            this.dataColumn5 = new System.Data.DataColumn();
            this.dataColumn6 = new System.Data.DataColumn();
            ((System.ComponentModel.ISupportInitialize)(this.dataSet1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataTable1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataTable2)).BeginInit();
            this.SuspendLayout();
            // 
            // column1
            // 
            this.column1.ColumnName = "Column1";
            this.column1.PriorityOnFrozen = 3;
            this.column1.PriorityOnUnfrozen = 3;
            // 
            // column2
            // 
            this.column2.ColumnName = "Column2";
            this.column2.PriorityOnFrozen = 3;
            this.column2.PriorityOnUnfrozen = 3;
            // 
            // column3
            // 
            this.column3.ColumnName = "Column3";
            this.column3.PriorityOnFrozen = 3;
            this.column3.PriorityOnUnfrozen = 3;
            // 
            // column4
            // 
            this.column4.ColumnName = "Relation1";
            // 
            // gridControl1
            // 
            this.gridControl1.BackgroundColor = System.Drawing.SystemColors.ActiveCaption;
            this.gridControl1.CaptionRow.Height = 21;
            this.gridControl1.Columns.AddRange(new Ntreev.Windows.Forms.Grid.Column[] {
            this.column1,
            this.column2,
            this.column3,
            this.column4});
            this.gridControl1.DataMember = "Table1";
            this.gridControl1.DataSource = this.dataSet1;
            this.gridControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gridControl1.Location = new System.Drawing.Point(0, 0);
            this.gridControl1.Name = "gridControl1";
            this.gridControl1.Size = new System.Drawing.Size(525, 424);
            this.gridControl1.TabIndex = 0;
            // 
            // dataSet1
            // 
            this.dataSet1.DataSetName = "NewDataSet";
            this.dataSet1.Relations.AddRange(new System.Data.DataRelation[] {
            new System.Data.DataRelation("Relation1", "Table1", "Table2", new string[] {
                        "Column1"}, new string[] {
                        "Column1"}, false)});
            this.dataSet1.Tables.AddRange(new System.Data.DataTable[] {
            this.dataTable1,
            this.dataTable2});
            // 
            // dataTable1
            // 
            this.dataTable1.Columns.AddRange(new System.Data.DataColumn[] {
            this.dataColumn1,
            this.dataColumn2,
            this.dataColumn3});
            this.dataTable1.Constraints.AddRange(new System.Data.Constraint[] {
            new System.Data.UniqueConstraint("Constraint1", new string[] {
                        "Column1"}, true)});
            this.dataTable1.PrimaryKey = new System.Data.DataColumn[] {
        this.dataColumn1};
            this.dataTable1.TableName = "Table1";
            // 
            // dataColumn1
            // 
            this.dataColumn1.AllowDBNull = false;
            this.dataColumn1.ColumnName = "Column1";
            // 
            // dataColumn2
            // 
            this.dataColumn2.ColumnName = "Column2";
            // 
            // dataColumn3
            // 
            this.dataColumn3.ColumnName = "Column3";
            // 
            // dataTable2
            // 
            this.dataTable2.Columns.AddRange(new System.Data.DataColumn[] {
            this.dataColumn4,
            this.dataColumn5,
            this.dataColumn6});
            this.dataTable2.Constraints.AddRange(new System.Data.Constraint[] {
            new System.Data.ForeignKeyConstraint("Relation1", "Table1", new string[] {
                        "Column1"}, new string[] {
                        "Column1"}, System.Data.AcceptRejectRule.None, System.Data.Rule.Cascade, System.Data.Rule.Cascade)});
            this.dataTable2.TableName = "Table2";
            // 
            // dataColumn4
            // 
            this.dataColumn4.AllowDBNull = false;
            this.dataColumn4.ColumnName = "Column1";
            // 
            // dataColumn5
            // 
            this.dataColumn5.ColumnName = "Column2";
            // 
            // dataColumn6
            // 
            this.dataColumn6.ColumnName = "Column3";
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(525, 424);
            this.Controls.Add(this.gridControl1);
            this.Name = "Form2";
            this.Text = "Form2";
            ((System.ComponentModel.ISupportInitialize)(this.dataSet1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataTable1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataTable2)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Ntreev.Windows.Forms.Grid.GridControl gridControl1;
        private Ntreev.Windows.Forms.Grid.Column column1;
        private Ntreev.Windows.Forms.Grid.Column column2;
        private Ntreev.Windows.Forms.Grid.Column column3;
        private Ntreev.Windows.Forms.Grid.Column column4;
        private System.Data.DataSet dataSet1;
        private System.Data.DataTable dataTable1;
        private System.Data.DataColumn dataColumn1;
        private System.Data.DataColumn dataColumn2;
        private System.Data.DataColumn dataColumn3;
        private System.Data.DataTable dataTable2;
        private System.Data.DataColumn dataColumn4;
        private System.Data.DataColumn dataColumn5;
        private System.Data.DataColumn dataColumn6;
    }
}