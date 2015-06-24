using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrGroup : GrCell
    {

        public static int SortGlyphSize = 10;
        GrGroupPanel m_groupPanel;
        GrColumn m_column;
        GrPoint m_pt;
        bool m_expanded;
        bool m_grouped;
        GrSort m_sortType;

        Dictionary<uint, GrGroupRow> m_mapGrourows = new Dictionary<uint, GrGroupRow>();
        int m_level;

        public GrGroup(GrColumn column)
        {
            // TODO: Complete member initialization
            m_column = column;
            m_groupPanel = null;
            m_grouped = false;
            m_expanded = true;
            m_sortType = GrSort.Up;
            m_level = GrDefineUtility.INVALID_INDEX;
        }

        public GrColumn GetColumn()
        {
            return m_column;
        }


        public bool GetGrouped()
        {
            return m_column.GetGrouped();
        }

        public void SetGrouped(bool b)
        {
            m_column.SetGrouped(b);
        }

        public void SetExpanded(bool b)
        {
            if (m_groupPanel.GetGroupable() == false)
                return;
            m_expanded = b;
            m_groupPanel.NotifyExpanded(this);
        }
        public bool GetExpanded()
        {
            return m_expanded;
        }

        public void SetSortType(GrSort sortType)
        {
            if (m_groupPanel.GetGroupable() == false)
                return;
            m_sortType = (sortType == GrSort.Up) ? GrSort.Up : GrSort.Down;
            m_groupPanel.NotifySortChanged(this);
        }

        public GrSort GetSortType()
        {
            return m_sortType;
        }

        public int GetGroupLevel()
        {
            return m_level;
        }

        public void SetGroupLevel(int level)
        {
    if(m_level == level)
        return;

    m_level = level;
    if(GetGrouped() == true)
        LevelChanged(this, EventArgs.Empty);
}

        public void SetText()
        {
    base.SetText(m_column.GetText());
}


        public override GrCellType GetCellType() { return GrCellType.Group; }

        public override int GetX()
        {
            return m_pt.X;
        }

        public override int GetY()
        {
            return m_pt.Y;
        }

        public override int GetWidth()
        {
            GrFont pFont = GetPaintingFont();
            return GetTextBounds().Width +
                (int)((pFont.GetHeight() + pFont.GetExternalLeading()) * 0.25f) +
                GetPadding().Horizontal + SortGlyphSize;
        }

        public override int GetHeight()
        {
            GrFont pFont = GetPaintingFont();
            return (int)((pFont.GetHeight() + pFont.GetExternalLeading()) * 1.25f) + GetPadding().Vertical;
        }

        public override bool GetVisible() { return true; }

        public override GrHorzAlign GetTextHorzAlign()
        {
            return GrHorzAlign.Left;
        }

        public override GrVertAlign GetTextVertAlign()
        {
            return GrVertAlign.Center;
        }

        public override bool GetDisplayable()
        {
            return m_groupPanel.GetDisplayable();
        }

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrRect paintRect = GetRect();
            GrPaintStyle paintStyle = ToPaintStyle();

            GrColor backColor = m_column.GetPaintingBackColor();
            GrColor foreColor = m_column.GetPaintingForeColor();
            GrPadding padding = GetPadding();

            painter.DrawColumn(paintStyle, paintRect, GetPaintingLineColor(), backColor, null);

            
            int right = paintRect.Right - padding.Right;
            int left = right - SortGlyphSize;
            int top = (paintRect.Bottom + paintRect.Top - SortGlyphSize) / 2;
            int bottom = top + SortGlyphSize;

            GrRect sortRect = GrRect.FromLTRB(left, top, right, bottom);

            painter.DrawSortGlyph(sortRect, m_sortType);
            DrawText(painter, foreColor, paintRect, null);
        }

        public override GrRow GetRow() { return m_groupPanel; }


        public event EventHandler LevelChanged;



        protected override void OnGridCoreAttached()
        {
    base.OnGridCoreAttached();
    m_groupPanel = this.GridCore.GroupPanel;
}

        protected override void OnGridCoreDetached()
        {
    m_groupPanel = null;
    base.OnGridCoreDetached();
}


        internal void SetPosition(GrPoint pt)
        {
            m_pt = pt;
        }

        internal void SetGroupLevelCore(int level)
        {
            m_level = level;
        }


    }
}
