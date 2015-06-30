using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrRootRow : GrRow
    {
        //private readonly List<GrRow> m_vecVisibleRows = new List<GrRow>();
        private readonly List<GrUpdatableRow> m_vecUpdatables = new List<GrUpdatableRow>();
        private bool m_visibleChanged;
        private bool m_fitChanged;
        private bool m_heightChanged;

        private int m_width;
        private int m_height;

        private GrRect m_bound;
        private GrColumnList m_columnList;
        private GrDataRowList m_pDataRowList;
        private GrDataRow m_pInsertionRow;

        public GrRootRow()
        {

            this.Text = "Root";
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

            m_width = m_columnList.GetBounds().Width;

            m_heightChanged = false;
            m_visibleChanged = false;
            m_fitChanged = false;
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
            m_height = y - this.Y;

            m_bound.Location = displayRect.Location;
            m_bound = GrRect.FromLTRB(displayRect.Left, displayRect.Top, m_bound.Right, m_bound.Bottom);
            //m_bound.Right = displayRect.left;
            //m_bound.Bottom = displayRect.top;

            foreach (var value in m_vecUpdatables)
            {
                if (value.ShouldClip(displayRect, horizontal, vertical) == true)
                    value.Clip(displayRect, horizontal, vertical);
            }

            foreach (var value in m_vecUpdatables)
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
            int left = this.X;
            int top = this.Y;
            int right = m_columnList.GetVisibleRight();
            int bottom = m_pDataRowList.GetVisibleBottom();
            rect = GrRect.FromLTRB(left, top, right, bottom);
            return rect;
        }

        //public override int X
        //{
        //    get { return this.GridCore.DisplayRectangle.Left; }
        //}

        //public override int Y
        //{
        //    get { return this.GridCore.DisplayRectangle.Top; }
        //}

        //public override int Width
        //{
        //    get { return m_width; }
        //}

        //public override int Height
        //{
        //    get { return m_height; }
        //}

        public override GrRect GetBounds() { return m_bound; }

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

            this.GridCore.Created += gridCore_Created;
            this.GridCore.DisplayRectangleChanged += gridCore_DisplayRectChanged;
        }


        private void BuildVisibleList()
        {
            //m_vecVisibleRows.Clear();

            //for (int i = 0; i < GetChildCount(); i++)
            //{
            //    GrRow pChild = GetChild(i);
            //    if (pChild.IsVisible == true)
            //        m_vecVisibleRows.Add(pChild);
            //}
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
            int y = this.Y;
            foreach (var item in this.Childs)
            {
                if (item.IsVisible == false)
                    continue;
                item.Y = y;
                y += item.Height;
            }
            m_height = y - this.Y;
        }

        private void gridCore_Created(object sender, EventArgs e)
        {
            base.OnGridCoreAttached();

            foreach (var item in this.Childs)
            {
                if (item is GrUpdatableRow == false)
                    continue;
                item.SizeChanged += item_SizeChanged;
                item.VisibleChanged += item_VisibleChanged;
                m_vecUpdatables.Add(item as GrUpdatableRow);

            }

            for (int i = 0; i < GetChildCount(); i++)
            {
                GrUpdatableRow pUpdatableRow = GetChild(i) as GrUpdatableRow;
                if (pUpdatableRow == null)
                    continue;
                m_vecUpdatables.Add(pUpdatableRow);
            }

            m_vecUpdatables.Sort((x, y) => x.GetUpdatePriority().CompareTo(y.GetUpdatePriority()));
            //std::sort(m_vecUpdatables.begin(), m_vecUpdatables.end(), SortUpdatable());

            m_columnList = this.GridCore.ColumnList;
            m_pDataRowList = this.GridCore.DataRowList;
            m_pInsertionRow = this.GridCore.InsertionRow;

            this.GridCore.DataRowList.DataRowInserted += dataRowList_DataRowInserted;
        }

        void item_VisibleChanged(object sender, EventArgs e)
        {
            
        }

        void item_SizeChanged(object sender, EventArgs e)
        {
            
        }

        private void gridCore_DisplayRectChanged(object sender, EventArgs e)
        {
            SetHeightChanged();
        }

        private void dataRowList_DataRowInserted(object sender, GrDataRowInsertedEventArgs e)
        {
            if (e.GetInsertType() == GrDataRowInsertType.New)
                return;

            List<GrRow> childs = new List<GrRow>();

            for (int i = 0; i < GetChildCount(); i++)
            {
                GrRow pChild = GetChild(i);
                if (pChild == m_pInsertionRow)
                {
                    m_pInsertionRow = this.GridCore.InsertionRow;
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
