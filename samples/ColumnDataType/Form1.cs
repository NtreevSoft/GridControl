using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Ntreev.Windows.Forms.Grid;

namespace ColumnAdvancedExtension
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            ColumnFlagControl column = new ColumnFlagControl();
            column.ColumnName = "NtreevGames";
            column.Width = 200;

            this.gridControl1.Columns.Add(column);

            Row row;
            row = this.gridControl1.Rows.AddNew();
            row[0].Value = NtreevGames.Alicia | NtreevGames.Arche;

            row = this.gridControl1.Rows.AddNew();
            row[0].Value = NtreevGames.Panya | NtreevGames.Arche | NtreevGames.BaseballManager;

            row = this.gridControl1.Rows.AddNew();
            row[0].Value = NtreevGames.Trickster| NtreevGames.Alicia;
        }
    }
}
