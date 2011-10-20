﻿#region License
//Ntreev Grid for .Net 1.0.4300.26762
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
using System.ComponentModel.Design.Serialization;

namespace Ntreev.Windows.Forms.Grid.Design
{
    [DesignerSerializer(typeof(RowCollectionCodeDomSerializer), typeof(CodeDomSerializer))]
    public class RowCollectionEditor : CollectionEditor
    {
        GridControl gridControl = null;
        public RowCollectionEditor(Type type)
            : base(type)
        {
            
        }

        public override object EditValue(System.ComponentModel.ITypeDescriptorContext context, IServiceProvider provider, object value)
        {
            RowCollection rows = value as RowCollection;
            this.gridControl = rows.GridControl;

            //if (this.gridControl.DataSource != null && provider.GetService(typeof(IDesignerHost)) != null)
            //    return value;

            return base.EditValue(context, provider, value);
        }

        protected override object CreateInstance(Type itemType)
        {
            return this.gridControl.AddNewRow();
        }

        protected override object SetItems(object editValue, object[] value)
        {
            return editValue;
        }

        protected override void DestroyInstance(object instance)
        {
            Row row = instance as Row;
            this.gridControl.Rows.Remove(row);
        }
    }
}