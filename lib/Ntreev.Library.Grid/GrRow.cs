using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public abstract class GrRow : GrCell
    {
        public static int DefaultHeight = 21;

        //private bool isVisible;

        private readonly List<GrRow> childs = new List<GrRow>();
        private GrRow parent;
        private int depth;
        private int id;

        //private int y;
        //private int height;

        private bool isResizable;
        private bool fitting;

        private static int snID;

        public GrRow()
        {
            this.id = snID++;
            //this.y = 0;
            //this.isVisible = true;
            this.isResizable = true;

            //this.height = DefaultHeight;
            this.parent = null;
            this.depth = 0;
            this.fitting = false;

            //this.Height = DefaultHeight;
        }

        //public void SetY(int y)
        //{
        //    if (this.y == y)
        //        return;
        //    this.y = y;
        //    this.OnYChanged();
        //}

        //public void SetHeight(int height)
        //{
        //    if (this.height == height)
        //        return;

        //    this.height = height;
        //    this.OnHeightChanged();
        //}

        public void SetResizable(bool b)
        {
            this.isResizable = b;
        }

        public void AdjustHeight()
        {
            if (this.fitting == false)
                return;

            int height = this.GetMinHeight();

            if (this.Height != height)
            {
                this.Height = height;
                this.OnHeightAdjusted();
            }
            this.fitting = false;
        }

        //public override int Y
        //{
        //    get { return this.y; }
        //}

        //public override int Height
        //{
        //    get { return this.height; }
        //}

        public virtual bool GetResizable()
        {
            return this.isResizable;
        }

        public virtual int GetMinHeight()
        {
            GrFont pFont = this.GetPaintingFont();
            int minHeight1 = pFont.GetHeight() + pFont.GetExternalLeading();
            int minHeight2 = this.GetTextBounds().Height + this.GetPadding().Vertical;
            if (minHeight2 < minHeight1)
                return minHeight1;
            return minHeight2;
        }

        public override GrRow GetRow() { return this; }

        //public override GrCellType GetCellType() { return GrCellType.Row; }

        //public override int X
        //{
        //    get { return this.GridCore.DisplayRectangle.Left; }
        //}

        public override bool IsDisplayable
        {
            get { return true; }
        }

        //public override bool IsVisible
        //{
        //    get { return this.isVisible; }
        //}

        public override GrHorzAlign TextHorzAlign
        {
            get { return GrHorzAlign.Center; }
        }

        public override GrVertAlign TextVertAlign
        {
            get { return GrVertAlign.Center; }
        }

        public virtual GrRowType GetRowType() { return GrRowType.Dummy; }

        public virtual bool ShouldBringIntoView() { return false; }

        public virtual GrCell HitTest(GrPoint location)
        {
            if (this.IsDisplayable == false)
                return null;

            if (this.Bounds.Contains(location) == false)
                return null;

            return this;
        }

        public IReadOnlyList<GrRow> Childs
        {
            get { return this.childs; }
        }

        //public int GetChildCount()
        //{
        //    return this.childs.Count;
        //}

        //public GrRow GetChild(int index)
        //{
        //    return this.childs[index];
        //}

        public GrRow Parent
        {
            get { return this.parent; }
        }

        public int Depth
        {
            get { return this.depth; }
        }

        public int GetResizingMargin()
        {
            GrFont pFont = this.GetPaintingFont();
            int margin = (int)((float)pFont.GetHeight() * 0.25f);

            int height = this.Height;
            if (margin * 3 > height)
                margin = (int)((float)height / 3.0f);

            return margin;
        }

        public void SetFit()
        {
            this.fitting = true;

            OnFitted();
        }

        public override void Paint(GrGridPainter painter, GrRect clipRect) { }

        //public virtual GrRect GetBounds() { return this.Bounds; }

        public override GrPadding GetPadding() { return GrPadding.Default; }

        public virtual void Sort(GrSort sortType)
        {
            switch (sortType)
            {
                case GrSort.Up:
                    Sort(GrSortFunc.SortRowsUp, 0);
                    break;
                case GrSort.Down:
                    Sort(GrSortFunc.SortRowsDown, 0);
                    break;
                case GrSort.None:
                    Sort(GrSortFunc.SortRowsNone, 0);
                    break;
                default:
                    break;
            }
        }

        public virtual void Sort(GrFuncSortRow fnSort, object userData)
        {
            this.childs.Sort((x, y) => fnSort(this.GridCore, x, y, userData));
        }

        public void AddChild(GrRow row)
        {
            this.childs.Add(row);
            row.parent = this;
            this.OnChildAdded(row);
        }

        public void ReserveChild(int reserve)
        {
            this.childs.Clear();
            this.childs.Capacity = reserve;
        }

        public void ClearChild()
        {
            foreach (var value in this.childs)
            {
                value.parent = null;
                value.depth = 0;
            }
            this.childs.Clear();
        }

        public int GetID() { return this.id; }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();

            foreach (var value in this.childs)
            {
                this.GridCore.AttachObject(value);
            }

            this.GridCore.DisplayRectangleChanged += GridCore_DisplayRectangleChanged;
        }

        void GridCore_DisplayRectangleChanged(object sender, EventArgs e)
        {
            this.Width = this.GridCore.DisplayRectangle.Width;
        }

        protected override void OnTextSizeChanged()
        {
            base.OnTextSizeChanged();

            if (this.GridCore.AutoFitRow == true && this.Height != this.GetTextBounds().Height)
            {
                this.SetFit();
            }
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);
            this.SetTextAlignChanged();
        }

        protected virtual void OnYChanged() { }

        protected virtual void OnHeightAdjusted() { }

        protected virtual void OnChildAdded(GrRow row)
        {
            this.UpdateDepth(row);

            if (this.GridCore != null)
            {
                this.GridCore.AttachObject(row);
            }
        }

        protected virtual void OnFitted()
        {
            this.SetTextAlignChanged();
        }

        public void InvalidateRow()
        {
            if (this.GridCore == null)
                return;
            GrRect rect = this.Bounds;
            //rect.Right = this.GridCore.GetDisplayRect().Right;
            rect.Width = this.GridCore.DisplayRectangle.Right - rect.Left;
            rect.Expand(2, 2, 2, 2);

            this.GridCore.Invalidate(rect);
        }

        protected void UpdateDepth(GrRow row)
        {
            GrRow pParent = row.Parent;
            if (pParent == null)
                row.depth = 0;
            else
                row.depth = pParent.depth + 1;

            foreach (var value in row.childs)
            {
                this.UpdateDepth(value);
            }
        }
        
        //class SortDesc
        //{
        //public:
        //    SortDesc(GrGridCore pGridCore, FuncSortRow fn, object* userData)
        //        : this.GridCore(pGridCore), this.fn(fn), this.userData(userData)
        //    {
        //    }

        //    bool operator () ( GrRow row1,  GrRow row2)
        //    {
        //        return (this.fn)(this.GridCore, row1, row2, this.userData);
        //    }

        //    GrGridCore this.GridCore;
        //    FuncSortRow this.fn;
        //    void this.userData;
        //}
    }
}
