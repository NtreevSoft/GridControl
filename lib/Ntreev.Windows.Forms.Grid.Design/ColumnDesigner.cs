#region License
//Ntreev Grid for .Net 2.0.4461.30274
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
using System.ComponentModel.Design;
using System.ComponentModel;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public class ColumnDesigner : ComponentDesigner
    {
        public ColumnDesigner()
        {
                
        }


        public override void Initialize(System.ComponentModel.IComponent component)
        {
            base.Initialize(component);

            Column column = this.Component as Column;

            this.IsVisible = column.IsVisible;
            this.IsResizable = column.IsResizable;
            this.IsMovable = column.IsMovable;
        }

        protected override void PreFilterProperties(System.Collections.IDictionary properties)
        {
            base.PreFilterProperties(properties);

            string[] strArray = new string[] { "IsVisible", "IsResizable", "IsMovable"};
            Attribute[] attributes = new Attribute[0];
            for (int i = 0; i < strArray.Length; i++)
            {
                PropertyDescriptor oldPropertyDescriptor = (PropertyDescriptor)properties[strArray[i]];
                if (oldPropertyDescriptor != null)
                {
                    PropertyDescriptor p = System.ComponentModel.TypeDescriptor.CreateProperty(typeof(ColumnDesigner), 
                                                            oldPropertyDescriptor, attributes);
                    properties[strArray[i]] = p;
                }
            }
        }

        public bool IsMovable
        {
            get;
            set;
        }

        public bool IsResizable
        {
            get;
            set;
        }

        public bool IsVisible
        {
            get; 
            set;
        }
    }
}