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
using System.Windows.Forms;
using Ntreev.Windows.Forms.Grid;

namespace SampleApplication
{
    public class ColumnNumeric : ColumnControl<NumericUpDown>
    {
        public override ViewType ViewType
        {
            get
            {
                return ViewType.Text;
            }
        }

        public ColumnNumeric()
        {
            this.Control.BorderStyle = BorderStyle.None;
        }

        protected override void SetControlValue(NumericUpDown control, object value)
        {
            if (value == null)
                value = (int)0;
            control.Value = (int)value;
        }

        protected override object GetControlValue(NumericUpDown control)
        {
            return (int)control.Value;
        }
        
        protected override void SetControlLayout(NumericUpDown control, ICell cell)
        {
            control.SetBounds(0, 0, cell.ClientRectangle.Width, cell.ClientRectangle.Height);

            if (control.Font != cell.Font)
                control.Font = cell.Font;
        }
    }
}
