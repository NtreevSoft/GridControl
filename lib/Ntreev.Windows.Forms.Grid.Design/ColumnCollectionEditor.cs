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
    public class ColumnCollectionEditor : CollectionEditor
    {
        ArrayList created = new ArrayList();

        public ColumnCollectionEditor(Type type)
            : base(type)
        {
            
        }

        protected override Type[] CreateNewItemTypes()
        {
            return new Type[] { typeof(string), typeof(int), typeof(bool), typeof(float), 
                typeof(System.Drawing.Color), typeof(System.Drawing.Point), typeof(System.Drawing.Rectangle),
                typeof(EtcType), typeof(EtcColumn), };
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
            this.created.Add(instance);
            return base.GetObjectsFromInstance(instance);
        }

        protected override object SetItems(object editValue, object[] value)
        {
            ColumnCollection columns = editValue as ColumnCollection;
            columns.SetItemsByDesigner(value);
            return editValue;
        }

        protected override void CancelChanges()
        {
            base.CancelChanges();

            foreach (Column item in this.created)
            {
                item.Dispose();
            }
        }

        protected override object CreateInstance(Type itemType)
        {
            IDesignerHost d = GetService(typeof(IDesignerHost)) as IDesignerHost;

            if (itemType == typeof(EtcType))
            {
                return CreateInstanceBySelectingType();
            }
            else if (itemType == typeof(EtcColumn))
            {
                return CreateInstanceBySelectingColumn();
            }

            return ColumnCollection.CreateColumnInstance(d, itemType);
        }

        void service_ComponentChanged(object sender, ComponentChangedEventArgs e)
        {
            int qwer = 0;
        }

        protected override string GetDisplayText(object value)
        {
            Column column = value as Column;
            return column.ColumnName;
        }

        object CreateInstanceBySelectingType()
        {
            IDesignerHost designerHost = GetService(typeof(IDesignerHost)) as IDesignerHost;
            IReferenceService refService = GetService(typeof(IReferenceService)) as IReferenceService;
            ITypeResolutionService resService = GetService(typeof(ITypeResolutionService)) as ITypeResolutionService;
            DTE dte = GetService(typeof(DTE)) as DTE;

            if (dte.ActiveWindow.Project.Object is VSProject)
            {
                VSProject vsproj = dte.ActiveWindow.Project.Object as VSProject;

                List<Assembly> assemblies = new List<Assembly>();

                try
                {
                    Assembly projectAseembly = Assembly.Load(dte.ActiveWindow.Project.Name);
                    if (projectAseembly != null)
                        assemblies.Add(projectAseembly);
                }
                catch (Exception)
                {
                }

                for (int i = 1; i <= vsproj.References.Count; i++)
                {
                    Reference reference = vsproj.References.Item(i);
                    Assembly assembly = resService.GetAssembly(new AssemblyName(reference.Name));
                    assemblies.Add(assembly);
                }

                AssembliesForm form = new AssembliesForm(assemblies.ToArray());


                if (form.ShowDialog() == DialogResult.OK)
                {
                    return ColumnCollection.CreateColumnInstance(this.Context, form.SelectedType);
                }
            }

            return null;
        }

        object CreateInstanceBySelectingColumn()
        {
            IDesignerHost designerHost = GetService(typeof(IDesignerHost)) as IDesignerHost;
            IReferenceService refService = GetService(typeof(IReferenceService)) as IReferenceService;
            ITypeResolutionService resService = GetService(typeof(ITypeResolutionService)) as ITypeResolutionService;
            DTE dte = GetService(typeof(DTE)) as DTE;

            if (dte.ActiveWindow.Project.Object is VSProject)
            {
                VSProject vsproj = dte.ActiveWindow.Project.Object as VSProject;

                List<Assembly> assemblies = new List<Assembly>();

                try
                {
                    Assembly projectAseembly = Assembly.Load(dte.ActiveWindow.Project.Name);
                    if (projectAseembly != null)
                        assemblies.Add(projectAseembly);
                }
                catch (Exception)
                {
                }

                for (int i = 1; i <= vsproj.References.Count; i++)
                {
                    Reference reference = vsproj.References.Item(i);
                    Assembly assembly = resService.GetAssembly(new AssemblyName(reference.Name));
                    assemblies.Add(assembly);
                }

                AssembliesForm form = new AssembliesForm(assemblies.ToArray(), typeof(Column));


                if (form.ShowDialog() == DialogResult.OK)
                {
                    return ColumnCollection.CreateColumnInstanceCore(this.Context, form.SelectedType);
                }
            }

            return null;

        }

        class EtcType
        {
            public override string ToString()
            {
                return "EtcType";
            }
        }

        class EtcColumn
        {
            public override string ToString()
            {
                return "EtcColumn";
            }
        }
    }
}