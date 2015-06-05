using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public abstract class GrRow : GrCell
    {
        public static int DefaultHeight = 21;

        protected bool m_visible;

        List<GrRow> m_vecChilds;
        GrRow m_pParent;
        int m_depth;
        uint m_id;

        int m_y;
        int m_height;

        bool m_resizable;
        bool m_fitting;


        static uint m_snID;

        public GrRow()
        {
            m_id = m_snID++;
            m_y = 0;
            m_visible = true;
            m_resizable = true;

            m_height = DefaultHeight;
            m_pParent = null;
            m_depth = 0;
            m_fitting = false;
        }

        public void SetY(int y)
        {
            if (m_y == y)
                return;
            m_y = y;
            this.OnYChanged();
        }

        public void SetHeight(int height)
        {
            if (m_height == height)
                return;

            m_height = height;
            this.OnHeightChanged();
        }

        public void SetResizable(bool b)
        {
            m_resizable = b;
        }

        public void AdjustHeight()
        {
            if (m_fitting == false)
                return;

            int height = this.GetMinHeight();

            if (m_height != height)
            {
                this.SetHeight(height);
                this.OnHeightAdjusted();
            }
            m_fitting = false;
        }

        public override int GetY()
        {
            return m_y;
        }

        public override int GetHeight()
        {
            return m_height;
        }

        public virtual bool GetResizable()
        {
            return m_resizable;
        }

        public virtual int GetMinHeight()
        {
            GrFont pFont = this.GetPaintingFont();
            int minHeight1 = pFont.GetHeight() + pFont.GetExternalLeading();
            int minHeight2 = this.GetTextBounds().Height + this.GetPadding().GetVertical();
            if (minHeight2 < minHeight1)
                return minHeight1;
            return minHeight2;
        }

        public override GrRow GetRow() { return this; }

        public override GrCellType GetCellType() { return GrCellType.Row; }

        public override int GetX()
        {
            return this.GridCore.GetDisplayRect().Left;
        }

        public override bool GetDisplayable() { return true; }

        public override bool GetVisible()
        {
            return m_visible;
        }

        public override GrHorzAlign GetTextHorzAlign()
        {
            return GrHorzAlign.Center;
        }

        public override GrVertAlign GetTextVertAlign()
        {
            return GrVertAlign.Center;
        }

        public virtual GrRowType GetRowType() { return GrRowType.Dummy; }

        public virtual bool ShouldBringIntoView() { return false; }
        public virtual GrCell HitTest(GrPoint location)
        {
            if (this.GetDisplayable() == false)
                return null;

            GrRect bound = this.GetBounds();
            if (bound.Contains(location) == false)
                return null;

            return this;
        }

        public int GetChildCount()
        {
            return m_vecChilds.Count;
        }

        public GrRow GetChild(int index)
        {
            return m_vecChilds[index];
        }

        public GrRow GetParent()
        {
            return m_pParent;
        }

        public int GetDepth()
        {
            return m_depth;
        }

        public int GetResizingMargin()
        {
            GrFont pFont = this.GetPaintingFont();
            int margin = (int)((float)pFont.GetHeight() * 0.25f);

            int height = this.GetHeight();
            if (margin * 3 > height)
                margin = (int)((float)height / 3.0f);

            return margin;
        }

        public void SetFit()
        {
            m_fitting = true;

            OnFitted();
        }

        public override void Paint(GrGridPainter pPainter, GrRect clipRect) { }

        public virtual GrRect GetBounds() { return GetRect(); }

        public override GrPadding GetPadding() { return GrPadding.Default; }

        public virtual void Sort(GrSort sortType)
        {
            throw new NotImplementedException();
        }

        public virtual void Sort(FuncSortRow fnSort, object userData)
        {
            throw new NotImplementedException();
        }

        public void AddChild(GrRow pRow)
        {
            m_vecChilds.Add(pRow);
            pRow.m_pParent = this;
            this.OnChildAdded(pRow);
        }

        public void ReserveChild(int reserve)
        {
            m_vecChilds.Clear();
            m_vecChilds.Capacity = reserve;
        }

        public void ClearChild()
        {
            foreach (var value in m_vecChilds)
            {
                value.m_pParent = null;
                value.m_depth = 0;
            }
            m_vecChilds.Clear();
        }

        public uint GetID() { return m_id; }

        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();

            foreach (var value in m_vecChilds)
            {
                this.GridCore.AttachObject(value);
            }
        }

        protected override void OnTextSizeChanged()
        {
            base.OnTextSizeChanged();

            if (this.GridCore.GetAutoFitRow() == true && this.GetHeight() != this.GetTextBounds().Height)
            {
                this.SetFit();
            }
        }

        protected virtual void OnHeightChanged()
        {
            this.SetTextAlignChanged();
        }

        protected virtual void OnYChanged() { }

        protected virtual void OnHeightAdjusted() { }

        protected virtual void OnChildAdded(GrRow pRow)
        {
            this.UpdateDepth(pRow);

            if (this.GridCore != null)
            {
                this.GridCore.AttachObject(pRow);
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
            GrRect rect = this.GetRect();
            //rect.Right = this.GridCore.GetDisplayRect().Right;
            rect.Width = this.GridCore.GetDisplayRect().Right - rect.Left;
            rect.Expand(2, 2, 2, 2);

            this.GridCore.Invalidate(rect);
        }

        protected void UpdateDepth(GrRow pRow)
        {
            GrRow pParent = pRow.GetParent();
            if (pParent == null)
                pRow.m_depth = 0;
            else
                pRow.m_depth = pParent.m_depth + 1;

            foreach (var value in pRow.m_vecChilds)
            {
                this.UpdateDepth(value);
            }
        }





        //class SortDesc
        //{
        //public:
        //    SortDesc(GrGridCore pGridCore, FuncSortRow fn, object* userData)
        //        : this.GridCore(pGridCore), m_fn(fn), m_userData(userData)
        //    {
        //    }

        //    bool operator () ( GrRow pRow1,  GrRow pRow2)
        //    {
        //        return (m_fn)(this.GridCore, pRow1, pRow2, m_userData);
        //    }

        //    GrGridCore this.GridCore;
        //    FuncSortRow m_fn;
        //    void m_userData;
        //}
    }
}
