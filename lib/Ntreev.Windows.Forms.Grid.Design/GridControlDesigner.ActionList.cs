#region License
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
                    items.Add(new DesignerActionPropertyItem("CaptionText", Resources.CaptionText));
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

            public string CaptionText
            {
                get
                {
                    return this.gridControl.CaptionRow.Text;
                }
                set
                {
                    this.gridControl.CaptionRow.Text = value;
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
                    this.gridControl.DataSource = value;
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
                    this.gridControl.DataMember = value;
                }
            }

            void Clear()
            {
                 this.gridControl.Clear();
            }
        }
    }
    class ContextMenu : Behavior
    {

    }

}