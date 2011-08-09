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

namespace Ntreev.Windows.Forms.Grid.Design
{
    public class GridControlDesigner : System.Windows.Forms.Design.ControlDesigner
    {
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
                    items.Add(new DesignerActionPropertyItem("Columns", "열 편집"));
                    items.Add(new DesignerActionPropertyItem("Rows", "행 편집"));
                    items.Add(new DesignerActionMethodItem(this, "AddNewColumn", "열 추가"));
                    items.Add(new DesignerActionMethodItem(this, "AddNewRow", "행 추가"));
                }
                catch (System.Exception e)
                {
                    System.Windows.Forms.MessageBox.Show(e.Message);
                }

                return items;
            }

            void AddNewColumn()
            {
                this.gridControl.Columns.AddNew();
                this.gridControl.Refresh();
            }

            void AddNewRow()
            {
                this.gridControl.Rows.AddNew();
                this.gridControl.Refresh();

                IComponentChangeService changeService = GetService(typeof(IComponentChangeService)) as IComponentChangeService;

                PropertyDescriptorCollection props = System.ComponentModel.TypeDescriptor.GetProperties(this.gridControl);
                PropertyDescriptor propertyDescriptor = props["Rows"];
                if (changeService != null)
                    changeService.OnComponentChanged(this.gridControl, propertyDescriptor, this.gridControl.Rows, this.gridControl.Rows);
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
        }

        //DesignerVerbCollection designerVerbs;
        System.ComponentModel.Design.DesignerActionListCollection mactionLists;

        public GridControlDesigner()
        {
            //System.Windows.Forms.MessageBox.Show("GridControlDesigner");
            //System.Windows.Forms.MessageBox.Show(typeof(RowCollectionEditor).AssemblyQualifiedName);

        }

        public override void Initialize(System.ComponentModel.IComponent component)
        {
            base.Initialize(component);
        }

        public override void InitializeNewComponent(System.Collections.IDictionary defaultValues)
        {
            base.InitializeNewComponent(defaultValues);
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

            //pe.Graphics.DrawRectangle(System.Drawing.Pens.Red, new System.Drawing.Rectangle(0, 0, 50, 50));
        }

        protected override bool GetHitTest(System.Drawing.Point point)
        {
            GridControl gridControl = this.Control as GridControl;
            if (gridControl == null)
                return false;

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
                return true;

            //return gridControl.GetCellAt(point) != null;
            return false;
        }
    }
}