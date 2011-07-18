namespace DataSourceUsage
{
    partial class Form1
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

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.gridControl1 = new Ntreev.Windows.Forms.Grid.GridControl();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPageFromArray = new System.Windows.Forms.TabPage();
            this.tabPageFromList = new System.Windows.Forms.TabPage();
            this.tabPageFromDataTable = new System.Windows.Forms.TabPage();
            this.tabPageFromDataSet = new System.Windows.Forms.TabPage();
            this.tabPageFromBindingList = new System.Windows.Forms.TabPage();
            this.gridControl2 = new Ntreev.Windows.Forms.Grid.GridControl();
            this.gridControl3 = new Ntreev.Windows.Forms.Grid.GridControl();
            this.gridControl4 = new Ntreev.Windows.Forms.Grid.GridControl();
            this.gridControl5 = new Ntreev.Windows.Forms.Grid.GridControl();
            this.tabControl1.SuspendLayout();
            this.tabPageFromArray.SuspendLayout();
            this.tabPageFromList.SuspendLayout();
            this.tabPageFromDataTable.SuspendLayout();
            this.tabPageFromDataSet.SuspendLayout();
            this.tabPageFromBindingList.SuspendLayout();
            this.SuspendLayout();
            // 
            // gridControl1
            // 
            this.gridControl1.AllowDrop = true;
            this.gridControl1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.gridControl1.DataSource = null;
            this.gridControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gridControl1.FocusedCell = null;
            this.gridControl1.Location = new System.Drawing.Point(3, 3);
            this.gridControl1.Name = "gridControl1";
            this.gridControl1.ReadOnly = false;
            this.gridControl1.Size = new System.Drawing.Size(470, 330);
            this.gridControl1.TabIndex = 0;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPageFromArray);
            this.tabControl1.Controls.Add(this.tabPageFromList);
            this.tabControl1.Controls.Add(this.tabPageFromDataTable);
            this.tabControl1.Controls.Add(this.tabPageFromDataSet);
            this.tabControl1.Controls.Add(this.tabPageFromBindingList);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(484, 362);
            this.tabControl1.TabIndex = 1;
            // 
            // tabPageFromArray
            // 
            this.tabPageFromArray.Controls.Add(this.gridControl1);
            this.tabPageFromArray.Location = new System.Drawing.Point(4, 22);
            this.tabPageFromArray.Name = "tabPageFromArray";
            this.tabPageFromArray.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageFromArray.Size = new System.Drawing.Size(476, 336);
            this.tabPageFromArray.TabIndex = 0;
            this.tabPageFromArray.Text = "Array";
            this.tabPageFromArray.UseVisualStyleBackColor = true;
            // 
            // tabPageFromList
            // 
            this.tabPageFromList.Controls.Add(this.gridControl2);
            this.tabPageFromList.Location = new System.Drawing.Point(4, 22);
            this.tabPageFromList.Name = "tabPageFromList";
            this.tabPageFromList.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageFromList.Size = new System.Drawing.Size(476, 336);
            this.tabPageFromList.TabIndex = 1;
            this.tabPageFromList.Text = "List";
            this.tabPageFromList.UseVisualStyleBackColor = true;
            // 
            // tabPageFromDataTable
            // 
            this.tabPageFromDataTable.Controls.Add(this.gridControl3);
            this.tabPageFromDataTable.Location = new System.Drawing.Point(4, 22);
            this.tabPageFromDataTable.Name = "tabPageFromDataTable";
            this.tabPageFromDataTable.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageFromDataTable.Size = new System.Drawing.Size(476, 336);
            this.tabPageFromDataTable.TabIndex = 2;
            this.tabPageFromDataTable.Text = "DataTable";
            this.tabPageFromDataTable.UseVisualStyleBackColor = true;
            // 
            // tabPageFromDataSet
            // 
            this.tabPageFromDataSet.Controls.Add(this.gridControl4);
            this.tabPageFromDataSet.Location = new System.Drawing.Point(4, 22);
            this.tabPageFromDataSet.Name = "tabPageFromDataSet";
            this.tabPageFromDataSet.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageFromDataSet.Size = new System.Drawing.Size(476, 336);
            this.tabPageFromDataSet.TabIndex = 3;
            this.tabPageFromDataSet.Text = "DataSet";
            this.tabPageFromDataSet.UseVisualStyleBackColor = true;
            // 
            // tabPageFromBindingList
            // 
            this.tabPageFromBindingList.Controls.Add(this.gridControl5);
            this.tabPageFromBindingList.Location = new System.Drawing.Point(4, 22);
            this.tabPageFromBindingList.Name = "tabPageFromBindingList";
            this.tabPageFromBindingList.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageFromBindingList.Size = new System.Drawing.Size(476, 336);
            this.tabPageFromBindingList.TabIndex = 4;
            this.tabPageFromBindingList.Text = "BindingList";
            this.tabPageFromBindingList.UseVisualStyleBackColor = true;
            // 
            // gridControl2
            // 
            this.gridControl2.AllowDrop = true;
            this.gridControl2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.gridControl2.DataSource = null;
            this.gridControl2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gridControl2.FocusedCell = null;
            this.gridControl2.Location = new System.Drawing.Point(3, 3);
            this.gridControl2.Name = "gridControl2";
            this.gridControl2.ReadOnly = false;
            this.gridControl2.Size = new System.Drawing.Size(470, 330);
            this.gridControl2.TabIndex = 0;
            // 
            // gridControl3
            // 
            this.gridControl3.AllowDrop = true;
            this.gridControl3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.gridControl3.DataSource = null;
            this.gridControl3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gridControl3.FocusedCell = null;
            this.gridControl3.Location = new System.Drawing.Point(3, 3);
            this.gridControl3.Name = "gridControl3";
            this.gridControl3.ReadOnly = false;
            this.gridControl3.Size = new System.Drawing.Size(470, 330);
            this.gridControl3.TabIndex = 0;
            // 
            // gridControl4
            // 
            this.gridControl4.AllowDrop = true;
            this.gridControl4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.gridControl4.DataSource = null;
            this.gridControl4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gridControl4.FocusedCell = null;
            this.gridControl4.Location = new System.Drawing.Point(3, 3);
            this.gridControl4.Name = "gridControl4";
            this.gridControl4.ReadOnly = false;
            this.gridControl4.Size = new System.Drawing.Size(470, 330);
            this.gridControl4.TabIndex = 0;
            // 
            // gridControl5
            // 
            this.gridControl5.AllowDrop = true;
            this.gridControl5.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.gridControl5.DataSource = null;
            this.gridControl5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gridControl5.FocusedCell = null;
            this.gridControl5.Location = new System.Drawing.Point(3, 3);
            this.gridControl5.Name = "gridControl5";
            this.gridControl5.ReadOnly = false;
            this.gridControl5.Size = new System.Drawing.Size(470, 330);
            this.gridControl5.TabIndex = 0;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(484, 362);
            this.Controls.Add(this.tabControl1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.tabControl1.ResumeLayout(false);
            this.tabPageFromArray.ResumeLayout(false);
            this.tabPageFromList.ResumeLayout(false);
            this.tabPageFromDataTable.ResumeLayout(false);
            this.tabPageFromDataSet.ResumeLayout(false);
            this.tabPageFromBindingList.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Ntreev.Windows.Forms.Grid.GridControl gridControl1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPageFromArray;
        private System.Windows.Forms.TabPage tabPageFromList;
        private System.Windows.Forms.TabPage tabPageFromDataTable;
        private System.Windows.Forms.TabPage tabPageFromDataSet;
        private System.Windows.Forms.TabPage tabPageFromBindingList;
        private Ntreev.Windows.Forms.Grid.GridControl gridControl2;
        private Ntreev.Windows.Forms.Grid.GridControl gridControl3;
        private Ntreev.Windows.Forms.Grid.GridControl gridControl4;
        private Ntreev.Windows.Forms.Grid.GridControl gridControl5;
    }
}

