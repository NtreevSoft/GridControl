#region License
//Ntreev Grid for .Net 1.0
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
using Ntreev.Windows.Forms.Grid;
using Ntreev.Windows.Forms.Grid.Columns;
using System.Reflection;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public class ColumnCollectionEditor : CollectionEditor
    {
        public ColumnCollectionEditor(Type type)
            : base(type)
        {
            
        }

        protected override Type[] CreateNewItemTypes()
        {
            return new Type[] { typeof(ColumnTextBox), typeof(ColumnComboBox), typeof(ColumnCheckBox), };
        }

        protected override bool CanRemoveInstance(object value)
        {
            Column column = value as Column;
            if (column.PropertyDescriptor != null)
                return false;
            return base.CanRemoveInstance(value);
        }

        protected override object SetItems(object editValue, object[] value)
        {
            ColumnCollection columns = editValue as ColumnCollection;
            columns.SetItemsByDesigner(value);
            return editValue;
        }
    }
}