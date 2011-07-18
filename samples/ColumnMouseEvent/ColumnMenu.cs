using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Ntreev.Windows.Forms.Grid;
using System.Drawing;

namespace ColumnMouseEvent
{
    public partial class ColumnMenu : Component
    {
        Column _column;

        public ColumnMenu()
        {
            InitializeComponent();
        }

        public ColumnMenu(IContainer container)
        {
            container.Add(this);

            InitializeComponent();
        }

        public void Show(Column column, Point location)
        {
            _column = column;
            this.contextMenuStrip1.Show(location);
        }

        private void toolStripMenuItemLeft_Click(object sender, EventArgs e)
        {
            _column.CellAlignment = StringAlignment.Near;
        }

        private void toolStripMenuItemCenter_Click(object sender, EventArgs e)
        {
            _column.CellAlignment = StringAlignment.Center;
        }

        private void toolStripMenuItemRight_Click(object sender, EventArgs e)
        {
            _column.CellAlignment = StringAlignment.Far;
        }

        private void toolStripMenuItemTop_Click(object sender, EventArgs e)
        {
            _column.CellLineAlignment = StringAlignment.Near;
        }

        private void toolStripMenuItemMiddle_Click(object sender, EventArgs e)
        {
            _column.CellLineAlignment = StringAlignment.Center;
        }

        private void toolStripMenuItemBottom_Click(object sender, EventArgs e)
        {
            _column.CellLineAlignment = StringAlignment.Far;
        }

        private void toolStripMenuItemSortNone_Click(object sender, EventArgs e)
        {
            _column.SortType = SortType.None;
        }

        private void toolStripMenuItemSortUp_Click(object sender, EventArgs e)
        {
            _column.SortType = SortType.Up;
        }

        private void toolStripMenuItemSortDown_Click(object sender, EventArgs e)
        {
            _column.SortType = SortType.Down;
        }

        private void toolStripMenuItemAlignment_DropDownOpening(object sender, EventArgs e)
        {
            this.toolStripMenuItemLeft.Checked = _column.CellAlignment == StringAlignment.Near;
            this.toolStripMenuItemCenter.Checked = _column.CellAlignment == StringAlignment.Center;
            this.toolStripMenuItemRight.Checked = _column.CellAlignment == StringAlignment.Far;
        }

        private void toolStripMenuItemLineAlignment_DropDownOpening(object sender, EventArgs e)
        {
            this.toolStripMenuItemTop.Checked = _column.CellLineAlignment == StringAlignment.Near;
            this.toolStripMenuItemMiddle.Checked = _column.CellLineAlignment == StringAlignment.Center;
            this.toolStripMenuItemBottom.Checked = _column.CellLineAlignment == StringAlignment.Far;
        }

        private void toolStripMenuItemSort_DropDownOpening(object sender, EventArgs e)
        {
            this.toolStripMenuItemSortNone.Checked = _column.SortType == SortType.None;
            this.toolStripMenuItemSortUp.Checked = _column.SortType == SortType.Up;
            this.toolStripMenuItemSortDown.Checked = _column.SortType == SortType.Down;
        }
    }
}
