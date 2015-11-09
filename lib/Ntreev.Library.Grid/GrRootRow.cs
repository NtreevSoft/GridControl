using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrRootRow : GrRow
    {
        //private readonly List<GrRow> vecVisibleRows = new List<GrRow>();
        private readonly List<GrUpdatableRow> updatables = new List<GrUpdatableRow>();
        private bool visibleChanged;
        private bool fitChanged;
        private bool heightChanged;

        private GrRect bound;
        private GrColumnList columnList;
        private GrDataRowList pDataRowList;
        private GrDataRow pInsertionRow;

        public GrRootRow()
        {
            this.Text = "Root";
            this.visibleChanged = true;
        }

        public void SetVisibleChanged()
        {
            this.visibleChanged = true;
            this.GridCore.Invalidate();
        }

        public void SetFitChanged()
        {
            this.fitChanged = true;
        }

        public void SetHeightChanged()
        {
            this.heightChanged = true;
        }

        public bool ShouldUpdate()
        {
            foreach (var value in this.updatables)
            {
                if (value.ShouldUpdate() == true)
                    return true;
            }
            return false;
        }

        public void Update()
        {
            this.Update(false);
        }

        public void Update(bool force)
        {
            foreach (var item in this.updatables)
            {
                item.Update(force);
            }

            if (this.visibleChanged == true || force == true)
                BuildVisibleList();

            if (this.fitChanged == true || force == true)
                AdjustRowHeight();

            if (this.heightChanged == true || force == true)
                RepositionVisibleList();

            this.Width = this.columnList.Bounds.Width;

            this.heightChanged = false;
            this.visibleChanged = false;
            this.fitChanged = false;
        }

        public bool ShouldClip()
        {
            return true;
        }

        public void Clip(GrRect displayRect, int horizontal, int vertical)
        {
            int y = this.Y;
            foreach (var item in this.Childs)
            {
                if (item.IsVisible == false)
                    continue;
                y += item.Height;
            }
            this.Height = y - this.Y;

            this.bound.Location = displayRect.Location;
            this.bound = GrRect.FromLTRB(displayRect.Left, displayRect.Top, this.bound.Right, this.bound.Bottom);
            //this.bound.Right = displayRect.left;
            //this.bound.Bottom = displayRect.top;

            foreach (var item in this.updatables)
            {
                if (item.ShouldClip(displayRect, horizontal, vertical) == true)
                    item.Clip(displayRect, horizontal, vertical);
            }

            foreach (var item in this.updatables)
            {
                int left = this.bound.Left;
                int top = this.bound.Top;
                int right = Math.Max(item.Bounds.Right, this.bound.Right);
                int bottom = Math.Max(item.Bounds.Bottom, this.bound.Bottom);
                //this.bound.right = Math.Max(value.GetBounds().Right, this.bound.Right);
                //this.bound.bottom = Math.Max(value.GetBounds().Bottom, this.bound.Bottom);
                this.bound = GrRect.FromLTRB(left, top, right, bottom);
            }
        }

        public GrRect GetVisibleBounds()
        {
            GrRect rect;
            int left = this.X;
            int top = this.Y;
            int right = this.columnList.GetVisibleRight();
            int bottom = this.pDataRowList.GetVisibleBottom();
            rect = GrRect.FromLTRB(left, top, right, bottom);
            return rect;
        }

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            foreach (var item in this.Childs)
            {
                if (item.IsVisible == false)
                    continue;
                item.Paint(painter, clipRect);
            }
        }

        public override GrCell HitTest(GrPoint location)
        {
            GrCell pHitted = base.HitTest(location);
            if (pHitted == null)
                return null;

            foreach (var item in this.Childs)
            {
                if (item.IsVisible == false)
                    continue;

                GrCell pSubHitted = item.HitTest(location);
                if (pSubHitted != null)
                {
                    return pSubHitted;
                }
            }

            return null;
        }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();

            this.GridCore.Created += GridCore_Created;
            this.GridCore.DisplayRectangleChanged += GridCore_DisplayRectangleChanged;
        }

        private void BuildVisibleList()
        {
            //this.vecVisibleRows.Clear();

            //for (int i = 0; i < GetChildCount(); i++)
            //{
            //    GrRow pChild = GetChild(i);
            //    if (pChild.IsVisible == true)
            //        this.vecVisibleRows.Add(pChild);
            //}
            this.heightChanged = true;
        }

        private void AdjustRowHeight()
        {
            foreach(var item in this.Childs)
            {
                item.AdjustHeight();
            }
            this.heightChanged = true;
        }

        private void RepositionVisibleList()
        {
            int y = this.Y;
            foreach (var item in this.Childs)
            {
                if (item.IsVisible == false)
                    continue;
                item.Y = y;
                y += item.Height;
            }
            this.Height = y - this.Y;
        }

        private void GridCore_Created(object sender, EventArgs e)
        {
            base.OnGridCoreAttached();

            foreach (var item in this.Childs)
            {
                if (item is GrUpdatableRow == false)
                    continue;
                item.SizeChanged += item_SizeChanged;
                item.VisibleChanged += item_VisibleChanged;
                this.updatables.Add(item as GrUpdatableRow);

            }

            this.updatables.Sort((x, y) => x.UpdatePriority.CompareTo(y.UpdatePriority));
            //std::sort(this.vecUpdatables.begin(), this.vecUpdatables.end(), SortUpdatable());

            this.columnList = this.GridCore.ColumnList;
            this.pDataRowList = this.GridCore.DataRowList;
            this.pInsertionRow = this.GridCore.InsertionRow;

            this.GridCore.DataRowList.DataRowInserted += dataRowList_DataRowInserted;
        }

        private void item_VisibleChanged(object sender, EventArgs e)
        {
            
        }

        private void item_SizeChanged(object sender, EventArgs e)
        {
            
        }

        private void GridCore_DisplayRectangleChanged(object sender, EventArgs e)
        {
            this.Size = this.GridCore.DisplayRectangle.Size;
            SetHeightChanged();
        }

        private void dataRowList_DataRowInserted(object sender, GrDataRowInsertedEventArgs e)
        {
            if (e.GetInsertType() == GrDataRowInsertType.New)
                return;

            List<GrRow> childs = new List<GrRow>();

            foreach(var item in this.Childs)
            {
                if (item == this.pInsertionRow)
                {
                    this.pInsertionRow = this.GridCore.InsertionRow;
                    childs.Add(this.pInsertionRow);
                }
                else
                {
                    childs.Add(item);
                }
            }

            ClearChild();

            foreach (var value in childs)
            {
                AddChild(value);
            }


            this.visibleChanged = true;
            Update(true);
        }
    }
}
