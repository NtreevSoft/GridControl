using Ntreev.INI3.Data;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SampleApplication
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();

            INIDataSet dataSet = new INIDataSet();
            dataSet.ReadSchema(@"G:\NTG\INI3\data\test\xml\Item\ItemContent.xsd");
            dataSet.ReadXml(@"G:\NTG\INI3\data\test\xml\Item\ItemContent.xml");

            this.gridControl1.DataSource = dataSet.GetFirstPrimaryTable();
        }
    }
}
