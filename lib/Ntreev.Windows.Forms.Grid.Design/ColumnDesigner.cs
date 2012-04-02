#region License
//Ntreev Grid for .Net 2.0.4475.19551
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
        bool isMovable = true;
        bool isResizable = true;
        bool isVisible = true;

        public ColumnDesigner()
        {
                
        }

        public override void Initialize(System.ComponentModel.IComponent component)
        {
            base.Initialize(component);

            Column column = this.Component as Column;

            this.IsVisible = column.IsVisible;
            this.IsResizable = column.IsResizable;
            this.isMovable = column.IsMovable;
        }

        public bool IsMovable
        {
            get { return this.isMovable; }
            set { this.isMovable = value; }
        }

        public bool IsResizable
        {
            get { return this.isResizable; }
            set { this.isResizable = value; }
        }

        public bool IsVisible
        {
            get { return this.isVisible; }
            set { this.isVisible = value; }
        }

        [DesignOnly(true)]
        public bool HasLifeline
        {
            get
            {
                Column column = this.Component as Column;
                return column.HasLifeline;
            }
            set
            {
                Column column = this.Component as Column;
                column.HasLifeline = value;
            }
        }

        protected override void PostFilterProperties(System.Collections.IDictionary properties)
        {
            base.PostFilterProperties(properties);
        }

        protected override void PreFilterProperties(System.Collections.IDictionary properties)
        {
            base.PreFilterProperties(properties);

            string[] strArray = new string[] { "IsVisible", "IsResizable", "IsMovable", };
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

            properties["GridControl"] = System.ComponentModel.TypeDescriptor.CreateProperty(typeof(ColumnDesigner), "GridControl", typeof(GridControl), attributes);
            properties["HasLifeline"] = System.ComponentModel.TypeDescriptor.CreateProperty(typeof(ColumnDesigner), "HasLifeline", typeof(bool), attributes);
        }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        GridControl GridControl
        {
            get
            {
                Column column = this.Component as Column;
                return column.GridControl;
            }
            set
            {
                Column column = this.Component as Column;
                if (column.Index == -1)
                {
                    value.Columns.Add(column);
                }
            }
        }
    }
}