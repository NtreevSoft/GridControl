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
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel.Design;
using System.ComponentModel;
using System.Globalization;
using System.Reflection;
using System.ComponentModel.Design.Serialization;
using Ntreev.Windows.Forms.Grid.GridState;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Design;

namespace Ntreev.Windows.Forms.Grid.Design
{
    public class GridControlDesigner : System.Windows.Forms.Design.ControlDesigner
    {
        DesignerActionListCollection mactionLists;
        ISelectionService selectionService;

        public GridControlDesigner()
        {

        }

        public override void Initialize(System.ComponentModel.IComponent component)
        {
            base.Initialize(component);

            if (this.selectionService == null)
            {
                this.selectionService = GetService(typeof(ISelectionService)) as ISelectionService;
                this.selectionService.SelectionChanged += new EventHandler(selectionService_SelectionChanged);
            }
        }

        public override void InitializeNewComponent(System.Collections.IDictionary defaultValues)
        {
            base.InitializeNewComponent(defaultValues);

            if (this.selectionService == null)
            {
                this.selectionService = GetService(typeof(ISelectionService)) as ISelectionService;
                this.selectionService.SelectionChanged += new EventHandler(selectionService_SelectionChanged);
            }
        }

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
                //    if (this.designerVerbs == null)
                //    {
                //        this.designerVerbs = new DesignerVerbCollection();
                //        this.designerVerbs.Add(new DesignerVerb(SR.GetString("DataGridViewEditColumnsVerb"), new EventHandler(this.OnEditColumns)));
                //        this.designerVerbs.Add(new DesignerVerb(SR.GetString("DataGridViewAddColumnVerb"), new EventHandler(this.OnAddColumn)));
                //    }

                return base.Verbs;
            }
        }

        protected override InheritanceAttribute InheritanceAttribute
        {
            get
            {
                if ((base.InheritanceAttribute != InheritanceAttribute.Inherited) && (base.InheritanceAttribute != InheritanceAttribute.InheritedReadOnly))
                {
                    return base.InheritanceAttribute;
                }
                return InheritanceAttribute.InheritedReadOnly;
            }
        }

        protected override void OnPaintAdornments(System.Windows.Forms.PaintEventArgs pe)
        {
            base.OnPaintAdornments(pe);

            if (this.selectionService.PrimarySelection is Column)
            {
                Column column = this.selectionService.PrimarySelection as Column;

                Rectangle rectancle = column.DisplayRectangle;
                rectancle.Width--;
                rectancle.Height--;
                 pe.Graphics.DrawRectangle(Pens.Black, rectancle);
                
            }

            //pe.Graphics.DrawRectangle(System.Drawing.Pens.Red, new System.Drawing.Rectangle(0, 0, 50, 50));
        }

        protected override bool GetHitTest(System.Drawing.Point point)
        {
            GridControl gridControl = this.Control as GridControl;
            if (gridControl == null)
            {
                 return false;
            }

            point = gridControl.PointToClient(point);

            try
            {
                State state = gridControl.StateManager.GetHitTest(point);

                switch (state)
                {
                    case State.ColumnPressing:
                    case State.ColumnResizing:
                    case State.ColumnSplitterMoving:
                    case State.RowResizing:
                    case State.GroupingInfoPressing:
                    case State.GroupingCellPressing:
                    case State.GroupingExpandPressing:
                         return true;

                }
            }
            catch (Exception)
            {
                return false;
            }


            if (gridControl.DisplayRectangle.Contains(point) == false)
            {
                 return true;
            }

             return false;
        }

        void selectionService_SelectionChanged(object sender, EventArgs e)
        {
            if (this.Control == null)
                return;
            this.Control.Invalidate();
        }

        #region classes

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
                    items.Add(new DesignerActionPropertyItem("CaptionText", "제목"));
                    items.Add(new DesignerActionPropertyItem("DataSource", "DataSource"));
                    items.Add(new DesignerActionPropertyItem("DataMember", "DataMember"));
                    items.Add(new DesignerActionPropertyItem("Columns", "열 편집"));
                    items.Add(new DesignerActionPropertyItem("Rows", "행 편집"));

                    items.Add(new DesignerActionMethodItem(this, "Clear", "전체 삭제"));
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

        #endregion
    }
}