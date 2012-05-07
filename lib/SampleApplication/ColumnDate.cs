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

namespace SampleApplication
{
    public class ColumnDate : ColumnControl<MaskedTextBox>
    {
        public override ViewType ViewType
        {
            get
            {
                return ViewType.Text;
            }
        }

        public ColumnDate()
        {
            this.Control.BorderStyle = BorderStyle.None;
            this.Control.Mask = "0000년 90월 90일";
            this.Control.ValidatingType = typeof(System.DateTime);    
        }

        protected override void SetControlValue(MaskedTextBox control, object value)
        {
            if (value != null)
                control.Text = value.ToString();
            else
                control.Text = string.Empty;
        }

        protected override object GetControlValue(MaskedTextBox control)
        {
            return control.Text;
        }
    }
}
