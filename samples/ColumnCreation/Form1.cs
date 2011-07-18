using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Ntreev.Windows.Forms.Grid.Columns;

namespace ColumnCreation
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            // 임의의 이름을 갖는 Column을 생성하고 컬렉션에 추가한다.
            this.gridControl1.Columns.AddNew();

            // Text라는 이름을 갖는 Column을 생성하고 컬렉션에 추가한다.
            this.gridControl1.Columns.AddNew("Text");

            // Number라는 이름을 갖고, int형의 데이터를 관리하는 Column을 생성하고 컬렉션에 추가한다.
            this.gridControl1.Columns.AddNew("Number", typeof(int));

            // Number라는 이름을 갖고, EnumSample형의 데이터를 관리하는 Column을 생성하고 컬렉션에 추가한다.
            this.gridControl1.Columns.AddNew("EnumSample", typeof(EnumSample));

            // 수동으로 콤보박스 형태의 Column 생성시
            ColumnComboBox columnComboBox = new ColumnComboBox();
            columnComboBox.DataType = typeof(EnumSample);
            columnComboBox.DataSource = System.Enum.GetValues(typeof(EnumSample));
            columnComboBox.Name = "EnumSample 수동";

            this.gridControl1.Columns.Add(columnComboBox);

            for (int i = 0; i < 10; i++)
            {
                this.gridControl1.Rows.AddNew();
            }
        }

        enum EnumSample : int
        {
            EnumSample1,
            EnumSample2,
            EnumSample3,
            EnumSample4,
        }
    }
}
