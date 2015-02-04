using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SampleApplication
{
    public partial class ColumnTestControl : UserControl
    {
        public ColumnTestControl()
        {
            InitializeComponent();
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            this.gridControl1.Columns.AdjustWidth();
            this.gridControl1.Rows.Count = 50;

            this.gridControl1.Rows[0].CellBackColor = Color.Red;
        }
    }
}
