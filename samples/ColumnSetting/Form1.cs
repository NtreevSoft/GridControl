using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Ntreev.Windows.Forms.Grid;
using Ntreev.Windows.Forms.Grid.Columns;

namespace ColumnProperties
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            Column columnFrozen = this.gridControl1.Columns.AddNew("FrozenColumn");
            columnFrozen.Title = "Frozen";
            columnFrozen.IsFrozen = true;
            columnFrozen.Tooltip = "이 컬럼은 스크롤에 영향을 받지 않습니다.";

            Column columnFixed = this.gridControl1.Columns.AddNew("FixedColumn");
            columnFixed.Title = "Movable";
            columnFixed.IsMovable = false;
            columnFixed.Tooltip = "이 컬럼은 컬럼간 이동이 제한됩니다.";

            Column columnUnresizable = this.gridControl1.Columns.AddNew("UnresizableColumn");
            columnUnresizable.Title = "Unresizable";
            columnUnresizable.IsResizable = false;
            columnUnresizable.Tooltip = "이 컬럼은 마우스로 이용한 크기 변경을 할 수가 없습니다.";

            Column columnHidden = this.gridControl1.Columns.AddNew("HiddenColumn");
            columnHidden.Title = "Hidden";
            columnHidden.IsVisible = false;
            columnHidden.Tooltip = "이 컬럼은 보이지 않습니다.";

            Column columnReadOnly = this.gridControl1.Columns.AddNew("ReadOnlyColumn");
            columnReadOnly.Title = "ReadOnly";
            columnReadOnly.IsReadOnly = true;
            columnReadOnly.Tooltip = "이 컬럼은 수정할 수 없습니다.";

            for (int i = 0; i < 10; i++)
            {
                Row row = this.gridControl1.Rows.AddNew();

                foreach (Cell item in row.Cells)
                {
                    item.Value = "text";
                }
            }
        }
    }
}
