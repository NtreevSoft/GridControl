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
using System.ComponentModel.Design;
using System.ComponentModel;
using System.Drawing.Design;
using Ntreev.Windows.Forms.Grid.Design.Properties;
using System.Windows.Forms.Design.Behavior;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public partial class GridControlDesigner : System.Windows.Forms.Design.ControlDesigner
    {
        DesignerActionListCollection mactionLists;
        //DesignerVerbCollection designerVerbs;
 
        public override DesignerActionListCollection ActionLists
        {
            get
            {
                if (this.mactionLists == null)
                {
                    this.mactionLists = new DesignerActionListCollection();
                    this.mactionLists.Add(new ActionList(this.Component));
                    this.mactionLists[0].AutoShow = true;
                }

                return this.mactionLists;
            }
        }

        public override DesignerVerbCollection Verbs
        {
            get
            {
                //if (this.designerVerbs == null)
                //{
                //    this.designerVerbs = new DesignerVerbCollection();
                //    this.designerVerbs.Add(new DesignerVerb(SR.GetString("DataGridViewEditColumnsVerb"), new EventHandler(this.OnEditColumns)));
                //    this.designerVerbs.Add(new DesignerVerb(SR.GetString("DataGridViewAddColumnVerb"), new EventHandler(this.OnAddColumn)));
                //}
                return base.Verbs;
            }
        }

        public class ActionList : System.ComponentModel.Design.DesignerActionList
        {
            GridControl gridControl;

            public ActionList(System.ComponentModel.IComponent component)
                : base(component)
            {
                this.gridControl = component as GridControl;
            }

            public override DesignerActionItemCollection GetSortedActionItems()
            {
                DesignerActionItemCollection items = new DesignerActionItemCollection();

                try
                {
                    items.Add(new DesignerActionPropertyItem("Caption", Resources.Caption));
                    items.Add(new DesignerActionPropertyItem("DataSource", Resources.DataSource));
                    items.Add(new DesignerActionPropertyItem("DataMember", Resources.DataMember));
                    items.Add(new DesignerActionPropertyItem("Columns", Resources.ColumnsEditing));
                    items.Add(new DesignerActionPropertyItem("Rows", Resources.RowsEditing));

                    items.Add(new DesignerActionMethodItem(this, "Clear", Resources.Clear));
                }
                catch (System.Exception e)
                {
                    System.Windows.Forms.MessageBox.Show(e.Message);
                }

                return items;
            }

            public ColumnCollection Columns
            {
                get
                {
                    return this.gridControl.Columns;
                }
            }

            public RowCollection Rows
            {
                get
                {
                    return this.gridControl.Rows;
                }
            }

            public string Caption
            {
                get
                {
                    return this.gridControl.Caption;
                }
                set
                {
                    IDesignerHost host = GetService(typeof(IDesignerHost)) as IDesignerHost;
                    PropertyDescriptor member = System.ComponentModel.TypeDescriptor.GetProperties(this.gridControl)["Caption"];
                    IComponentChangeService service = GetService(typeof(IComponentChangeService)) as IComponentChangeService;
                    DesignerTransaction transaction = host.CreateTransaction("GridControl.Caption Transaction");
                    try
                    {
                        service.OnComponentChanging(this.gridControl, member);
                        this.gridControl.Caption = value;
                        service.OnComponentChanged(this.gridControl, member, null, null);
                        transaction.Commit();
                        transaction = null;
                    }
                    finally
                    {
                        if (transaction != null)
                        {
                            transaction.Cancel();
                        }
                    }
                    
                }
            }

            [AttributeProvider(typeof(IListSource))]
            public object DataSource
            {
                get
                {
                    return this.gridControl.DataSource;
                }
                set
                {
                    IDesignerHost host = GetService(typeof(IDesignerHost)) as IDesignerHost;
                    PropertyDescriptor member = System.ComponentModel.TypeDescriptor.GetProperties(this.gridControl)["DataSource"];
                    IComponentChangeService service = GetService(typeof(IComponentChangeService)) as IComponentChangeService;
                    DesignerTransaction transaction = host.CreateTransaction("GridControl.DataSource Transaction");
                    try
                    {
                        service.OnComponentChanging(this.gridControl, member);
                        this.gridControl.DataSource = value;
                        service.OnComponentChanged(this.gridControl, member, null, null);
                        transaction.Commit();
                        transaction = null;
                    }
                    finally
                    {
                        if (transaction != null)
                        {
                            transaction.Cancel();
                        }
                    }
                }
            }

            [Editor("System.Windows.Forms.Design.DataMemberListEditor, System.Design, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a", typeof(UITypeEditor))]
            public string DataMember
            {
                get
                {
                    return this.gridControl.DataMember;
                }
                set
                {
                    IDesignerHost host = GetService(typeof(IDesignerHost)) as IDesignerHost;
                    PropertyDescriptor member = System.ComponentModel.TypeDescriptor.GetProperties(this.gridControl)["DataMember"];
                    IComponentChangeService service = GetService(typeof(IComponentChangeService)) as IComponentChangeService;
                    DesignerTransaction transaction = host.CreateTransaction("GridControl.DataMember Transaction");
                    try
                    {
                        service.OnComponentChanging(this.gridControl, member);
                        this.gridControl.DataMember = value;
                        service.OnComponentChanged(this.gridControl, member, null, null);
                        transaction.Commit();
                        transaction = null;
                    }
                    finally
                    {
                        if (transaction != null)
                        {
                            transaction.Cancel();
                        }
                    }
                }
            }

            void Clear()
            {
                IDesignerHost host = GetService(typeof(IDesignerHost)) as IDesignerHost;
                IComponentChangeService service = GetService(typeof(IComponentChangeService)) as IComponentChangeService;
                DesignerTransaction transaction = host.CreateTransaction("GridControl.Clear() Transaction");
                try
                {
                    service.OnComponentChanging(this.gridControl, null);
                    this.gridControl.Clear();
                    service.OnComponentChanged(this.gridControl, null, null, null);
                    transaction.Commit();
                    transaction = null;
                }
                finally
                {
                    if (transaction != null)
                    {
                        transaction.Cancel();
                    }
                }
            }
        }
    }
}