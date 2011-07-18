using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Ntreev.Windows.Forms.Grid;

namespace ColumnBinding
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            // DataSource로 초기화 할 경우 ColumnBinding이벤트가 컬럼 수 만큼 발생한다. 
            // 특별한 컬럼을 생성에 연결하고 싶다면 아래의 이벤트를 받아서 처리해야 한다.
            this.gridControl1.ColumnBinding += new Ntreev.Windows.Forms.Grid.ColumnBindingEventHandler(this.gridControl1_ColumnBinding);

            DataTable dataTable = new DataTable("DataTable");

            DataColumn dataColumn1 = dataTable.Columns.Add("TestColumn1");  // ColumnTest1과 바인딩
            DataColumn dataColumn2 = dataTable.Columns.Add("TestColumn2");  // ColumnTest2와 바인딩
            DataColumn dataColumn3 = dataTable.Columns.Add("DefaultColumn");  // GridControl에서 제공하는 기본컬럼과 바인딩.

            dataColumn1.DataType = typeof(string);
            dataColumn2.DataType = typeof(bool);

            for (int i = 0; i < 10; i++)
            {
                dataTable.Rows.Add();
            }

            // DataSource에 dataTable을 넣으면 내부에서 초기화 과정중 ColumnBinding 이벤트를 발생시킨다.
            this.gridControl1.DataSource = dataTable;
        }

        private void gridControl1_ColumnBinding(object sender, Ntreev.Windows.Forms.Grid.ColumnBindingEventArgs e)
        {
            // e.PropertyDescriptor 정보를 참조하여 바인딩될 컬럼의 인스턴스를 생성해 e.BindingColumn에 설정한다.
            // 만약 e.BindingColumn가 null일경우 GridControl에서는 DataType에 따라 적절한 컬럼을 생성해준다.
            switch (e.PropertyDescriptor.Name)
            {
                case "TestColumn1":
                    // null 체크는 하지않아도 되지만,
                    // 이벤트 핸들러에 다른 함수들이 등록되어 있을수 있기 때문에 null체크를 하였다.
                    if (e.BindingColumn == null)
                    {
                        e.BindingColumn = new TestColumn1();
                        e.BindingColumn.Tooltip = "이 컬럼은 TestColumn1과 연결되었습니다.";
                    }
                    break;
                case "TestColumn2":
                    if (e.BindingColumn == null)
                    {
                        e.BindingColumn = new TestColumn2();
                        e.BindingColumn.Tooltip = "이 컬럼은 TestColumn2와 연결되었습니다.";
                    }
                    break;
            }
        }
    }

    class TestColumn1 : ColumnControl<TextBox>
    {
        public override bool PaintValueSupported
        {
            get
            {
                return false;
            }
        }

        protected override object GetEditingValue(TextBox control)
        {
            return control.Text;
        }

        protected override void SetEditingValue(TextBox control, object value)
        {
            if (value != null)
                control.Text = value.ToString();
        }
    }

    class TestColumn2 : ColumnControl<CheckBox>
    {
        protected override object GetEditingValue(CheckBox control)
        {
            return control.Checked;
        }

        protected override void SetEditingValue(CheckBox control, object value)
        {
            if (value == System.DBNull.Value)
                value = false;
            control.Checked = (bool)value;
        }
    }
}
