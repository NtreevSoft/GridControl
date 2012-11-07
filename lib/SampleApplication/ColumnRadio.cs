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
using System.Linq;
using System.Text;
using Ntreev.Windows.Forms.Grid;
using System.Windows.Forms;

namespace SampleApplication
{
    public class ColumnRadio : ColumnControl<RadioButton>
    {
        public ColumnRadio()
        {
            this.DataType = typeof(bool);

            this.Control.Click += new EventHandler(Control_Click);
        }

        void Control_Click(object sender, EventArgs e)
        {
            this.Control.Checked = !this.Control.Checked;
            CloseControl();
        }

        protected override RadioButton CreateControlInstance(params object[] controlArgs)
        {
            RadioButton control = base.CreateControlInstance(controlArgs);
            control.CheckAlign = System.Drawing.ContentAlignment.MiddleCenter;
            control.AutoCheck = false;
            return control;
        }

        protected override void SetControlValue(RadioButton control, object value)
        {
            base.SetControlValue(control, value);

            if (value == null)
            {
                control.Checked = false;
            }
            else
            {
                control.Checked = (bool)value;
            }
        }

        protected override object GetControlValue(RadioButton control)
        {
            return control.Checked;
        }
    }

    //public class ColumnPainter2 : ColumnPainter
    //{

    //    public override bool PaintBackground(System.Drawing.Graphics g, System.Drawing.Rectangle paintRect, IColumn column)
    //    {
    //        return false;
    //    }

    //    public override bool PaintContents(System.Drawing.Graphics g, System.Drawing.Rectangle paintRect, IColumn column)
    //    {
    //        return false;
    //    }
    //}
}
