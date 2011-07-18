namespace ColumnMouseEvent
{
    partial class ColumnMenu
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
            this.components = new System.ComponentModel.Container();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolStripMenuItemAlignment = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemLeft = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemCenter = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemRight = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemLineAlignment = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemTop = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemMiddle = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemBottom = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemSort = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemSortNone = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemSortUp = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemSortDown = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStrip1.SuspendLayout();
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemAlignment,
            this.toolStripMenuItemLineAlignment,
            this.toolStripMenuItemSort});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(153, 70);
            // 
            // toolStripMenuItemAlignment
            // 
            this.toolStripMenuItemAlignment.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemLeft,
            this.toolStripMenuItemCenter,
            this.toolStripMenuItemRight});
            this.toolStripMenuItemAlignment.Name = "toolStripMenuItemAlignment";
            this.toolStripMenuItemAlignment.Size = new System.Drawing.Size(152, 22);
            this.toolStripMenuItemAlignment.Text = "Alignment";
            this.toolStripMenuItemAlignment.DropDownOpening += new System.EventHandler(this.toolStripMenuItemAlignment_DropDownOpening);
            // 
            // toolStripMenuItemLeft
            // 
            this.toolStripMenuItemLeft.Name = "toolStripMenuItemLeft";
            this.toolStripMenuItemLeft.Size = new System.Drawing.Size(152, 22);
            this.toolStripMenuItemLeft.Text = "Left";
            this.toolStripMenuItemLeft.Click += new System.EventHandler(this.toolStripMenuItemLeft_Click);
            // 
            // toolStripMenuItemCenter
            // 
            this.toolStripMenuItemCenter.Name = "toolStripMenuItemCenter";
            this.toolStripMenuItemCenter.Size = new System.Drawing.Size(152, 22);
            this.toolStripMenuItemCenter.Text = "Center";
            this.toolStripMenuItemCenter.Click += new System.EventHandler(this.toolStripMenuItemCenter_Click);
            // 
            // toolStripMenuItemRight
            // 
            this.toolStripMenuItemRight.Name = "toolStripMenuItemRight";
            this.toolStripMenuItemRight.Size = new System.Drawing.Size(152, 22);
            this.toolStripMenuItemRight.Text = "Right";
            this.toolStripMenuItemRight.Click += new System.EventHandler(this.toolStripMenuItemRight_Click);
            // 
            // toolStripMenuItemLineAlignment
            // 
            this.toolStripMenuItemLineAlignment.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemTop,
            this.toolStripMenuItemMiddle,
            this.toolStripMenuItemBottom});
            this.toolStripMenuItemLineAlignment.Name = "toolStripMenuItemLineAlignment";
            this.toolStripMenuItemLineAlignment.Size = new System.Drawing.Size(152, 22);
            this.toolStripMenuItemLineAlignment.Text = "LineAlignment";
            this.toolStripMenuItemLineAlignment.DropDownOpening += new System.EventHandler(this.toolStripMenuItemLineAlignment_DropDownOpening);
            // 
            // toolStripMenuItemTop
            // 
            this.toolStripMenuItemTop.Name = "toolStripMenuItemTop";
            this.toolStripMenuItemTop.Size = new System.Drawing.Size(114, 22);
            this.toolStripMenuItemTop.Text = "Top";
            this.toolStripMenuItemTop.Click += new System.EventHandler(this.toolStripMenuItemTop_Click);
            // 
            // toolStripMenuItemMiddle
            // 
            this.toolStripMenuItemMiddle.Name = "toolStripMenuItemMiddle";
            this.toolStripMenuItemMiddle.Size = new System.Drawing.Size(114, 22);
            this.toolStripMenuItemMiddle.Text = "Middle";
            this.toolStripMenuItemMiddle.Click += new System.EventHandler(this.toolStripMenuItemMiddle_Click);
            // 
            // toolStripMenuItemBottom
            // 
            this.toolStripMenuItemBottom.Name = "toolStripMenuItemBottom";
            this.toolStripMenuItemBottom.Size = new System.Drawing.Size(114, 22);
            this.toolStripMenuItemBottom.Text = "Bottom";
            this.toolStripMenuItemBottom.Click += new System.EventHandler(this.toolStripMenuItemBottom_Click);
            // 
            // toolStripMenuItemSort
            // 
            this.toolStripMenuItemSort.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemSortNone,
            this.toolStripMenuItemSortUp,
            this.toolStripMenuItemSortDown});
            this.toolStripMenuItemSort.Name = "toolStripMenuItemSort";
            this.toolStripMenuItemSort.ShortcutKeyDisplayString = "";
            this.toolStripMenuItemSort.Size = new System.Drawing.Size(152, 22);
            this.toolStripMenuItemSort.Text = "Sort";
            this.toolStripMenuItemSort.DropDownOpening += new System.EventHandler(this.toolStripMenuItemSort_DropDownOpening);
            // 
            // toolStripMenuItemSortNone
            // 
            this.toolStripMenuItemSortNone.Name = "toolStripMenuItemSortNone";
            this.toolStripMenuItemSortNone.Size = new System.Drawing.Size(106, 22);
            this.toolStripMenuItemSortNone.Text = "None";
            this.toolStripMenuItemSortNone.Click += new System.EventHandler(this.toolStripMenuItemSortNone_Click);
            // 
            // toolStripMenuItemSortUp
            // 
            this.toolStripMenuItemSortUp.Name = "toolStripMenuItemSortUp";
            this.toolStripMenuItemSortUp.Size = new System.Drawing.Size(106, 22);
            this.toolStripMenuItemSortUp.Text = "Up";
            this.toolStripMenuItemSortUp.Click += new System.EventHandler(this.toolStripMenuItemSortUp_Click);
            // 
            // toolStripMenuItemSortDown
            // 
            this.toolStripMenuItemSortDown.Name = "toolStripMenuItemSortDown";
            this.toolStripMenuItemSortDown.Size = new System.Drawing.Size(106, 22);
            this.toolStripMenuItemSortDown.Text = "Down";
            this.toolStripMenuItemSortDown.Click += new System.EventHandler(this.toolStripMenuItemSortDown_Click);
            this.contextMenuStrip1.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemAlignment;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemLeft;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemCenter;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemRight;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemLineAlignment;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemTop;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemMiddle;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemBottom;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemSort;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemSortNone;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemSortUp;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemSortDown;

    }
}
