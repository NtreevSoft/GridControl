#region License
//Ntreev Grid for .Net 2.0.0.0
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
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Design;
using System.Windows.Forms;

//#if 0
//using Ntreev.Windows.Forms.Grid.GridState;
//#endif

namespace Ntreev.Windows.Forms.Grid.Design
{
    public partial class GridControlDesigner : System.Windows.Forms.Design.ControlDesigner
    {
        ISelectionService selectionService;
        IComponentChangeService componentChangeService;

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

            if(this.componentChangeService == null)
            {
                this.componentChangeService = GetService(typeof(IComponentChangeService)) as IComponentChangeService;
                this.componentChangeService.ComponentRemoved += new ComponentEventHandler(componentChangeService_ComponentRemoved);
            }

            ContainerFilterService containerFilterService = GetService(typeof(ContainerFilterService)) as ContainerFilterService;


            IServiceContainer serviceContainer = GetService(typeof(IServiceContainer)) as IServiceContainer;
            if (serviceContainer != null)
            {
                ContainerFilterService newFilter = null;

                if (containerFilterService != null)
                {
                    if (containerFilterService.GetType() != typeof(GridControlContainerFilterService))
                    {
                        newFilter = new GridControlContainerFilterService(containerFilterService);
                        serviceContainer.RemoveService(typeof(ContainerFilterService));
                    }
                }
                else
                {
                    newFilter = new GridControlContainerFilterService();
                }

                if (newFilter != null)
                {
                    serviceContainer.AddService(typeof(ContainerFilterService), newFilter);
                }
            }
        }

        protected override void PreFilterProperties(System.Collections.IDictionary properties)
        {
            string[] strArray = new string[] { "AutoScroll", "AutoScrollMargin", "AutoScrollMinSize", "AutoScrollPosition", "Cursor", };
            foreach (string item in strArray)
            {
                properties.Remove(item);
            }

            base.PreFilterProperties(properties);
        }

        public override System.Collections.ICollection AssociatedComponents
        {
            get
            {
                GridControl gridControl = this.Control as GridControl;
                if(gridControl == null)
                    return base.AssociatedComponents;
                return gridControl.Columns.ToArray();
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
            GridControl gridControl = this.Control as GridControl;

            foreach (Column item in gridControl.DisplayableColumns)
            {
                if (item.PropertyDescriptor == null)
                    continue;

                Image image;
                if (item.HasLifeline == true)
                    image = Properties.Resources.DataSource_Only;
                else
                    image = Properties.Resources.DataSource_Binding;

                int x = item.Bounds.Left - image.Width / 3;
                int y = item.Bounds.Top + image.Height / 3;

                pe.Graphics.DrawImage(image, x, y);
            }


            foreach (object item in this.selectionService.GetSelectedComponents())
            {
                Column column = item as Column;
                if (column == null)
                    continue;

                if (column.GridControl == gridControl && column.IsDisplayable == true)
                {
                    Rectangle rectancle = column.Bounds;
                    rectancle.Width--;
                    rectancle.Height--;

                    rectancle.Inflate(1, 1);
                    Pen pen = new Pen(Color.Black);
                    pen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dot;
                    
                    pe.Graphics.DrawRectangle(pen, rectancle);
                }
            }

            //if (this.selectionService.PrimarySelection is Column)
            //{
            //    Column column = this.selectionService.PrimarySelection as Column;

            //    if (column.GridControl == gridControl && column.IsDisplayable == true)
            //    {
            //        Rectangle rectancle = column.Bounds;
            //        rectancle.Width--;
            //        rectancle.Height--;
            //        pe.Graphics.DrawRectangle(Pens.Black, rectancle);
            //    }
            //}

            //pe.Graphics.DrawRectangle(System.Drawing.Pens.Red, new System.Drawing.Rectangle(0, 0, 50, 50));
        }

        protected override bool GetHitTest(System.Drawing.Point point)
        {
            GridControl gridControl = this.Control as GridControl;
            return gridControl.DesignTimeHitTest(point);
        }

        protected override void OnSetCursor()
        {
            GridControl gridControl = this.Control as GridControl;
            //if (gridControl.DesignTimeSetCursor() == true)
            //{
            //    //gridControl.CaptionRow.Text = "true";
            //    return;
            //}
            gridControl.CaptionRow.Text = gridControl.Cursor.ToString();
            base.OnSetCursor();
        }

        protected override void Dispose(bool disposing)
        {
            if (this.componentChangeService != null)
            {
                this.componentChangeService.ComponentRemoved -= new ComponentEventHandler(componentChangeService_ComponentRemoved);
                this.componentChangeService = null;
            }

            base.Dispose(disposing);
        }

        void selectionService_SelectionChanged(object sender, EventArgs e)
        {
            if (this.Control == null)
                return;
            this.Control.Invalidate();
        }

        void componentChangeService_ComponentRemoved(object sender, ComponentEventArgs e)
        {
            GridControl gridControl = this.Control as GridControl;
            if (gridControl.DataSource == e.Component)
                gridControl.DataSource = null;
        }
    }
}