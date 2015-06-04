using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrGroup : GrCell
    {

        public static int SortGlyphSize = 10;
        GrGroupPanel m_pGroupPanel;
        GrColumn m_pColumn;
        GrPoint m_pt;
        bool m_expanded;
        bool m_grouped;
        GrSort m_sortType;

        Dictionary<uint, GrGroupRow> m_mapGroupRows = new Dictionary<uint, GrGroupRow>();
        int m_level;

        public GrGroup(GrColumn pColumn)
        {
            // TODO: Complete member initialization
            m_pColumn = pColumn;
            m_pGroupPanel = null;
            m_grouped = false;
            m_expanded = true;
            m_sortType = GrSort.Up;
            m_level = GrDefineUtility.INVALID_INDEX;
        }

        public GrColumn GetColumn()
        {
            return m_pColumn;
        }


        public bool GetGrouped()
        {
            return m_pColumn.GetGrouped();
        }

        public void SetGrouped(bool b)
        {
            m_pColumn.SetGrouped(b);
        }

        public void SetExpanded(bool b)
        {
            if (m_pGroupPanel.GetGroupable() == false)
                return;
            m_expanded = b;
            m_pGroupPanel.NotifyExpanded(this);
        }
        public bool GetExpanded()
        {
            return m_expanded;
        }

        public void SetSortType(GrSort sortType)
        {
            if (m_pGroupPanel.GetGroupable() == false)
                return;
            m_sortType = (sortType == GrSort.Up) ? GrSort.Up : GrSort.Down;
            m_pGroupPanel.NotifySortChanged(this);
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
    base.SetText(m_pColumn.GetText());
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
                GetPadding().GetHorizontal() + SortGlyphSize;
        }

        public override int GetHeight()
        {
            GrFont pFont = GetPaintingFont();
            return (int)((pFont.GetHeight() + pFont.GetExternalLeading()) * 1.25f) + GetPadding().GetVertical();
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
            return m_pGroupPanel.GetDisplayable();
        }

        public override void Paint(GrGridPainter pPainter, GrRect clipRect)
        {
            GrRect paintRect = GetRect();
            GrPaintStyle paintStyle = ToPaintStyle();

            GrColor backColor = m_pColumn.GetPaintingBackColor();
            GrColor foreColor = m_pColumn.GetPaintingForeColor();
            GrPadding padding = GetPadding();

            pPainter.DrawColumn(paintStyle, paintRect, GetPaintingLineColor(), backColor, null);

            
            int right = paintRect.Right - padding.Right;
            int left = right - SortGlyphSize;
            int top = (paintRect.Bottom + paintRect.Top - SortGlyphSize) / 2;
            int bottom = top + SortGlyphSize;

            GrRect sortRect = GrRect.FromLTRB(left, top, right, bottom);

            pPainter.DrawSortGlyph(sortRect, m_sortType);
            DrawText(pPainter, foreColor, paintRect, null);
        }

        public override GrRow GetRow() { return m_pGroupPanel; }


        public event EventHandler LevelChanged;



        protected override void OnGridCoreAttached()
        {
    base.OnGridCoreAttached();
    m_pGroupPanel = this.GridCore.GetGroupPanel();
}

        protected override void OnGridCoreDetached()
        {
    m_pGroupPanel = null;
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
