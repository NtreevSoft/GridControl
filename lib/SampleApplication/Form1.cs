#region License
//Ntreev Grid for .Net 2.0.4646.22417
//https://github.com/NtreevSoft/GridControl

//Released under the MIT License.

//Copyright (c) 2010 Ntreev Soft co., Ltd.

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
//rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
//persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
//Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
//WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
//COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
//OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#endregion


using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Threading;
using Ntreev.Windows.Forms.Grid;

namespace SampleApplication
{
    public partial class Form1 : Form
    {
        [DllImport("User32.dll")]
        public static extern Int32 SendMessage(int hWnd, int Msg, int wParam, int lParam);

        [DllImport("User32.dll")]
        public static extern Int32 PostMessage(int hWnd, int Msg, int wParam, int lParam);

        NativeWindow2 n2 = new NativeWindow2();

        
        public Form1()
        {
            string d = ((int)100).ToString("{0} wow");
            
            InitializeComponent();
            this.gridControl1.InsertionRow.Cells["Color"].IsReadOnly = true;

            List<Type> types = new List<Type>();
            types.Add(typeof(int));
            types.Add(typeof(bool));
            types.Add(typeof(string));
            types.Add(typeof(float));
            types.Add(typeof(short));
            types.Add(typeof(byte));
            types.Add(typeof(long));

            this.columnDataTypes1.DataSource = types;
            DataSet dataSet = new DataSet();
            dataSet.Tables.Add("table-1").Columns.Add("column1");

            this.gridControl1.RowChanged += gridControl1_RowChanged;
        }

        void gridControl1_ValueChanging(object sender, Ntreev.Windows.Forms.Grid.ValueChangingEventArgs e)
        {
            return;
            if (e.Cell.Tag != null)
                return;
            e.Cancel = true;
            object value = e.Cell.Value;
            bool b = false;
            if (value != null)
                b = (bool)value;
            b = !b;
            e.Cell.DisplayText = b.ToString();

            Action<Cell, bool> d = new Action<Cell, bool>((c, b2) =>
                {
                    Thread.Sleep(10000);
                    this.gridControl1.BeginInvoke((Action<Cell, bool>)((c3, b3) => 
                    {
                        c3.Tag = new object();
                        c3.Value = b3;
                        c3.DisplayText = null;
                        c3.Tag = null;
                        
                    }), c, b2);

                });

            d.BeginInvoke(e.Cell, b, null, null);
        }

        void gridControl1_RowChanged(object sender, Ntreev.Windows.Forms.Grid.RowEventArgs e)
        {
            //e.Row.CellBackColor = Color.Red;
        }

        public void SaveSettings()
        {
            
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            //n2.AssignHandle(this.PanelPane.Handle);

            //this.DockPanel.Skin.DockPaneStripSkin.ToolWindowGradient.InactiveCaptionGradient = this.DockPanel.Skin.DockPaneStripSkin.ToolWindowGradient.ActiveCaptionGradient;

            //foreach (Control item in this.PanelPane.Controls)
            //{
            //    Type type = item.GetType();
            //    if (item.GetType().IsSubclassOf(typeof(DockPaneCaptionBase)) == true)
            //    {
            //        int qewr = 0;


            //        n2.AssignHandle(item.Handle);
            //    }
            //}

            //this.PanelPane.DockChanged += new EventHandler(PanelPane_DockChanged);
           // this.PanelPane.IsActivatedChanged += new EventHandler(PanelPane_IsActivatedChanged);
            this.gridControl1.Columns.AdjustWidth();
            this.gridControl1.Rows.Count = 50;

        }

        void PanelPane_IsActivatedChanged(object sender, EventArgs e)
        {
            int qwer = 0;
        }

        void PanelPane_DockChanged(object sender, EventArgs e)
        {
            int qwer = 0;
        }


        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            if (this.ActiveControl == null)
                this.Text = "null";
            else
                this.Text = this.ActiveControl.Name;
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // TODO: 이 코드는 데이터를 'hit_ini_korDataSet.ini_data_type' 테이블에 로드합니다. 필요한 경우 이 코드를 이동하거나 제거할 수 있습니다.

        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            this.columnLinkLabel1.IsVisible = !this.columnLinkLabel1.IsVisible;
        }

        private void gridControl1_ColumnMouseDown(object sender, Ntreev.Windows.Forms.Grid.ColumnMouseEventArgs e)
        {
            
        }

        class NativeWindow2 : NativeWindow
        {
            protected override void WndProc(ref Message m)
            {

                base.WndProc(ref m);

                switch (m.Msg)
                {
                    case 0x0086:
                        {
                            if ((m.WParam) == IntPtr.Zero)
                            {
                                SendMessage(Handle.ToInt32(), 0x0086, 1, 0);
                            }
                        }
                        break;
                    case 0x001C:
                        {
                            if (m.WParam == IntPtr.Zero)
                            {
                                PostMessage(Handle.ToInt32(), 0x0086, 0, 0);
                            }
                        }
                        break;
                }
            }
        }

        private void gridControl1_Scroll(object sender, ScrollEventArgs e)
        {
            //var bounds = this.gridControl1.Rows[0].Bounds;
            var y = this.gridControl1.Rows[0].Y;
        }
    }

    [Flags]
    public enum TestFlag
    {
        None = 0,
        A = 1,
        B = 2,
        AB = 3,
        C = 4,
        D = 8,
        All = A | B | C | D,
    }
}
