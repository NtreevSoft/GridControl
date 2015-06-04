using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrRootRow : GrRow
    {
        List<GrRow> m_vecVisibleRows;
        List<GrUpdatableRow> m_vecUpdatables;
        bool m_visibleChanged;
        bool m_fitChanged;
        bool m_heightChanged;

        int m_width;
        int m_height;

        GrRect m_bound;
        GrColumnList m_pColumnList;
        GrDataRowList m_pDataRowList;
        GrDataRow m_pInsertionRow;

        public GrRootRow()
        {

            SetText("Root");
            m_visibleChanged = true;

        }

        public void SetVisibleChanged()
        {
            m_visibleChanged = true;
            this.GridCore.Invalidate();
        }

        public void SetFitChanged()
        {
            m_fitChanged = true;
        }

        public void SetHeightChanged()
        {
            m_heightChanged = true;
        }

        public bool ShouldUpdate()
        {
            foreach (var value in m_vecUpdatables)
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
            foreach (var value in m_vecUpdatables)
            {
                value.Update(force);
            }

            if (m_visibleChanged == true || force == true)
                BuildVisibleList();

            if (m_fitChanged == true || force == true)
                AdjustRowHeight();

            if (m_heightChanged == true || force == true)
                RepositionVisibleList();

            m_width = m_pColumnList.GetBounds().GetWidth();

            m_heightChanged = false;
            m_visibleChanged = false;
            m_fitChanged = false;
        }

        public bool ShouldClip()
        {
            return true;
        }

        public void Clip(GrRect displayRect, uint horizontal, uint vertical)
        {
    int y = GetY();
    foreach(var value in m_vecVisibleRows)
    {
        y += value.GetHeight();
    }
    m_height = y - GetY();

    m_bound.SetLocation(displayRect.GetLocation());
            m_bound = GrRect.FromLTRB(displayRect.Left, displayRect.Top, m_bound.Right, m_bound.Bottom);
    //m_bound.Right = displayRect.left;
    //m_bound.Bottom = displayRect.top;

    foreach(var value in m_vecUpdatables)
    {
        if(value.ShouldClip(displayRect, horizontal, vertical) == true)
            value.Clip(displayRect, horizontal, vertical);
    }

    foreach(var value in m_vecUpdatables)
    {
        int left = m_bound.Left;
        int top = m_bound.Top;
        int right = Math.Max(value.GetBounds().Right, m_bound.Right);
        int bottom = Math.Max(value.GetBounds().Bottom, m_bound.Bottom);
        //m_bound.right = Math.Max(value.GetBounds().Right, m_bound.Right);
        //m_bound.bottom = Math.Max(value.GetBounds().Bottom, m_bound.Bottom);
        m_bound = GrRect.FromLTRB(left, top, right, bottom);
    }
}

        public GrRect GetVisibleBounds()
        {
            GrRect rect;
            int left = this.GetX();
            int top = this.GetY();
            int right = m_pColumnList.GetVisibleRight();
            int bottom = m_pDataRowList.GetVisibleBottom();
            rect = GrRect.FromLTRB(left, top, right, bottom);
            return rect;
        }

        public override int GetX()
        {
            return this.GridCore.GetDisplayRect().Left;
        }

        public override int GetY()
        {
            return this.GridCore.GetDisplayRect().Top;
        }

        public override int GetWidth() { return m_width; }
        public override int GetHeight() { return m_height; }

        public override GrRect GetBounds() { return m_bound; }

        public override void Paint(GrGridPainter pPainter, GrRect clipRect)
        {
            foreach (var value in m_vecVisibleRows)
            {
                value.Paint(pPainter, clipRect);
            }
        }

        public override GrCell HitTest(GrPoint location)
        {
            GrCell pHitted = base.HitTest(location);
            if (pHitted == null)
                return null;

            foreach (var value in m_vecVisibleRows)
            {
                GrCell pSubHitted = value.HitTest(location);
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

            this.GridCore.Created += gridCore_Created;
            this.GridCore.DisplayRectChanged += gridCore_DisplayRectChanged;
        }


        private void BuildVisibleList()
        {
            m_vecVisibleRows.Clear();

            for (int i = 0; i < GetChildCount(); i++)
            {
                GrRow pChild = GetChild(i);
                if (pChild.GetVisible() == true)
                    m_vecVisibleRows.Add(pChild);
            }
            m_heightChanged = true;
        }

        private void AdjustRowHeight()
        {
            for (int i = 0; i < GetChildCount(); i++)
            {
                GrRow pChild = GetChild(i);
                pChild.AdjustHeight();
            }
            m_heightChanged = true;
        }

        private void RepositionVisibleList()
        {
            int y = GetY();
            foreach (var value in m_vecVisibleRows)
            {
                value.SetY(y);
                y += value.GetHeight();
            }
            m_height = y - GetY();
        }

        private void gridCore_Created(object pSender, EventArgs e)
        {
            base.OnGridCoreAttached();

            for (int i = 0; i < GetChildCount(); i++)
            {
                GrUpdatableRow pUpdatableRow = GetChild(i) as GrUpdatableRow;
                if (pUpdatableRow == null)
                    continue;
                m_vecUpdatables.Add(pUpdatableRow);
            }

            m_vecUpdatables.Sort((x, y) => x.GetUpdatePriority().CompareTo(y.GetUpdatePriority()));
            //std::sort(m_vecUpdatables.begin(), m_vecUpdatables.end(), SortUpdatable());

            m_pColumnList = this.GridCore.GetColumnList();
            m_pDataRowList = this.GridCore.GetDataRowList();
            m_pInsertionRow = this.GridCore.GetInsertionRow();

            this.GridCore.GetDataRowList().DataRowInserted += dataRowList_DataRowInserted;
        }

        private void gridCore_DisplayRectChanged(object pSender, EventArgs e)
        {
            SetHeightChanged();
        }

        private void dataRowList_DataRowInserted(object pSender, GrDataRowInsertedEventArgs e)
        {
            if (e.GetInsertType() == GrDataRowInsertType.New)
                return;

            List<GrRow> childs = new List<GrRow>();

            for (int i = 0; i < GetChildCount(); i++)
            {
                GrRow pChild = GetChild(i);
                if (pChild == m_pInsertionRow)
                {
                    m_pInsertionRow = this.GridCore.GetInsertionRow();
                    childs.Add(m_pInsertionRow);
                }
                else
                {
                    childs.Add(pChild);
                }
            }

            ClearChild();

            foreach (var value in childs)
            {
                AddChild(value);
            }


            m_visibleChanged = true;
            Update(true);
        }

        //private:


        //private:
        //    class SortUpdatable
        //    {
        //    public:
        //        bool operator () ( GrUpdatableRow p1,  GrUpdatableRow p2)
        //        {
        //            return p1.GetUpdatePriority() < p2.GetUpdatePriority();
        //        }
        //    };
    }
}
