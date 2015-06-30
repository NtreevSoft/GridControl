using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrGroup : GrCell
    {
        public static int SortGlyphSize = 10;

        private readonly GrColumn column;
        private GrGroupPanel groupPanel;
        private bool isExpanded = true;
        private GrSort sortType = GrSort.Up;

        private readonly Dictionary<uint, GrGroupRow> mapGrourows = new Dictionary<uint, GrGroupRow>();
        private int level = GrDefineUtility.INVALID_INDEX;

        public GrGroup(GrColumn column)
        {
            this.column = column;
        }

        public GrColumn Column
        {
            get { return this.column; }
        }

        public bool IsGrouped
        {
            get { return this.column.IsGrouped; }
            set { this.column.IsGrouped = value; }
        }

        public void SetExpanded(bool b)
        {
            if (this.groupPanel.IsGroupable == false)
                return;
            this.isExpanded = b;
            this.groupPanel.NotifyExpanded(this);
        }

        public bool GetExpanded()
        {
            return this.isExpanded;
        }

        public void SetSortType(GrSort sortType)
        {
            if (this.groupPanel.IsGroupable == false)
                return;
            this.sortType = (sortType == GrSort.Up) ? GrSort.Up : GrSort.Down;
            this.groupPanel.NotifySortChanged(this);
        }

        public GrSort GetSortType()
        {
            return this.sortType;
        }

        public int GetGroupLevel()
        {
            return this.level;
        }

        public void SetGroupLevel(int level)
        {
            if (this.level == level)
                return;

            this.level = level;
            if (this.IsGrouped == true)
                LevelChanged(this, EventArgs.Empty);
        }

        //public void SetText()
        //{
        //    base.Text = this.column.Text;
        //}


        //public override GrCellType GetCellType() { return GrCellType.Group; }

        //public override int X
        //{
        //    get { return this.pt.X; }
        //}

        //public override int Y
        //{
        //    get { return this.pt.Y; }
        //}

        //public override int Width
        //{
        //    get
        //    {
        //        GrFont pFont = GetPaintingFont();
        //        return GetTextBounds().Width +
        //            (int)((pFont.GetHeight() + pFont.GetExternalLeading()) * 0.25f) +
        //            GetPadding().Horizontal + SortGlyphSize;
        //    }
        //}

        //public override int Height
        //{
        //    get
        //    {
        //        GrFont pFont = GetPaintingFont();
        //        return (int)((pFont.GetHeight() + pFont.GetExternalLeading()) * 1.25f) + GetPadding().Vertical;
        //    }
        //}

        public override bool IsVisible
        {
            get { return true; }
            set { }
        }

        public override GrHorzAlign TextHorzAlign
        {
            get { return GrHorzAlign.Left; }
        }

        public override GrVertAlign TextVertAlign
        {
            get { return GrVertAlign.Center; }
        }

        public override bool IsDisplayable
        {
            get { return this.groupPanel.IsDisplayable; }
        }

        public override void Paint(GrGridPainter painter, GrRect clipRect)
        {
            GrRect paintRect = this.Bounds;
            GrPaintStyle paintStyle = ToPaintStyle();

            GrColor backColor = this.column.GetPaintingBackColor();
            GrColor foreColor = this.column.GetPaintingForeColor();
            GrPadding padding = GetPadding();

            painter.DrawColumn(paintStyle, paintRect, GetPaintingLineColor(), backColor, null);


            int right = paintRect.Right - padding.Right;
            int left = right - SortGlyphSize;
            int top = (paintRect.Bottom + paintRect.Top - SortGlyphSize) / 2;
            int bottom = top + SortGlyphSize;

            GrRect sortRect = GrRect.FromLTRB(left, top, right, bottom);

            painter.DrawSortGlyph(sortRect, this.sortType);
            DrawText(painter, foreColor, paintRect, null);
        }

        public override GrRow GetRow() { return this.groupPanel; }


        public event EventHandler LevelChanged;



        protected override void OnGridCoreAttached()
        {
            base.OnGridCoreAttached();
            this.groupPanel = this.GridCore.GroupPanel;
        }

        protected override void OnGridCoreDetached()
        {
            this.groupPanel = null;
            base.OnGridCoreDetached();
        }

        protected override void OnTextChanged()
        {
            base.OnTextChanged();

            GrFont pFont = GetPaintingFont();
            int width = GetTextBounds().Width + (int)((pFont.GetHeight() + pFont.GetExternalLeading()) * 0.25f) + GetPadding().Horizontal + SortGlyphSize;
            int height = GetTextBounds().Width + (int)((pFont.GetHeight() + pFont.GetExternalLeading()) * 0.25f) + GetPadding().Horizontal + SortGlyphSize;

            this.Size = new GrSize(width, height);
        }


        internal void SetGroupLevelCore(int level)
        {
            this.level = level;
        }


    }
}
