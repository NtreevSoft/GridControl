#region License
//Ntreev Grid for .Net 2.0.5792.31442
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
using System.Windows.Forms;
using System.ComponentModel;
using System.Drawing.Design;
using System.Drawing;
using System.Windows.Forms.Design;
using EnvDTE;
using VSLangProj;
using System.Collections;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public class ColumnCollectionEditor : CollectionEditor, IServiceProvider
    {
        public ColumnCollectionEditor(Type type)
            : base(type)
        {
            
        }

        protected override Type[] CreateNewItemTypes()
        {
            return new Type[] { typeof(string), typeof(int), typeof(bool), typeof(float), 
                typeof(System.Drawing.Color), typeof(System.Drawing.Point), typeof(System.Drawing.Rectangle),
                new EtcType(), new EtcColumn(), };
        }

        protected override bool CanRemoveInstance(object value)
        {
            Column column = value as Column;
            if (column.PropertyDescriptor != null)
                return false;
            return base.CanRemoveInstance(value);
        }

        protected override IList GetObjectsFromInstance(object instance)
        {
            if (instance == null)
                return null;
            return base.GetObjectsFromInstance(instance);
        }

        protected override object SetItems(object editValue, object[] value)
        {
            ColumnCollection columns = editValue as ColumnCollection;
            columns.SetItemsByDesigner(value);
            return editValue;
        }

        protected override object CreateInstance(Type itemType)
        {
            IDesignerHost d = GetService(typeof(IDesignerHost)) as IDesignerHost;

            if (itemType.Name == "EtcType...")
            {
                return CreateInstanceBySelectingType();
            }
            else if (itemType.Name == "EtcColumn...")
            {
                return CreateInstanceBySelectingColumn();
            }

            Column column = ColumnCollection.CreateColumnInstance(d);
            column.DataType = itemType;
            return column;
        }

        void service_ComponentChanged(object sender, ComponentChangedEventArgs e)
        {
            
        }

        protected override string GetDisplayText(object value)
        {
            Column column = value as Column;
            return column.ColumnName;
        }

        object CreateInstanceBySelectingType()
        {
            TypeSelectorForm form = TypeSelectorForm.FromVSProject(this);

            if (form != null && form.ShowDialog() == DialogResult.OK)
            {
                Column column = ColumnCollection.CreateColumnInstance(this.Context);
                column.DataType = form.SelectedType;
                return column;
            }

            return null;
        }

        object CreateInstanceBySelectingColumn()
        {
            TypeSelectorForm form = TypeSelectorForm.FromVSProject(this);

            if (form != null)
            {
                form.Filter = typeof(Column);
                if (form.ShowDialog() == DialogResult.OK)
                {
                    return ColumnCollection.CreateColumnInstance(this.Context, form.SelectedType);
                }
            }

            return null;
        }

        class EtcType : TypeDelegator
        {
            public override string Name
            {
                get
                {
                    return "EtcType...";
                }
            }
        }
        
        class EtcColumn : TypeDelegator
        {
            public override string Name
            {
                get
                {
                    return "EtcColumn...";
                }
            }
        }

        #region IServiceProvider 멤버

        object IServiceProvider.GetService(Type serviceType)
        {
            return GetService(serviceType);
        }

        #endregion
    }
}