#region License
//Ntreev Grid for .Net 2.0.4510.20986
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
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Ntreev.Windows.Forms.Grid;
using System.Drawing;

namespace SampleApplication
{
    public class ColumnLinkLabel : ColumnControl<LinkLabel>
    {
        LinkLabel2 testControl = new LinkLabel2();
        public ColumnLinkLabel()
        {
            this.Control.LinkClicked += new LinkLabelLinkClickedEventHandler(Control_LinkClicked);
        }

        void Control_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            //CloseControl();
        }

        protected override LinkLabel CreateControlInstance(params object[] controlArgs)
        {
            return new LinkLabel2();
        }

        protected override void SetControlValue(LinkLabel control, object value)
        {
            base.SetControlValue(control, value);
            if (value != null)
                control.Text = value.ToString();
            else
                control.Text = "(null)";
        }

        protected override void SetControlLayout(LinkLabel control, ICell cell)
        {
            if ((cell.State & CellState.Hot) == CellState.Hot)
            {
                int qwer = 0;
            }
            base.SetControlLayout(control, cell);
        }

        protected override void OnGridControlAttached()
        {
            base.OnGridControlAttached();
            this.GridControl.CursorChanged += new EventHandler(GridControl_CursorChanged);
        }

        protected override bool CanEdit(ICell cell, Ntreev.Windows.Forms.Grid.EditingReason reason)
        {
            if (base.CanEdit(cell, reason) == false)
                return false;

            SetControlLayout(this.testControl, cell);
            SetControlValue(this.testControl, cell.Value);
            return (this.testControl as LinkLabel2).LinkTest(reason.Location) != null;
        }

        void GridControl_CursorChanged(object sender, EventArgs e)
        {
            //System.Diagnostics.Trace.WriteLine(Cursor.Current);
        }

        protected override void OnGridControlDetached()
        {
            base.OnGridControlDetached();
        }

        class LinkLabel2 : LinkLabel
        {
            public Link LinkTest(Point location)
            {
                Link d = PointInLink(location.X, location.Y);
                return d;
            }
        }
    }
}
