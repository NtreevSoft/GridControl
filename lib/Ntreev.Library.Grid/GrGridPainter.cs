using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public abstract class GrGridPainter
    {
        public abstract void BeginPaint(object painterDevice);

        public abstract void DrawRowSplitter(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor fillColor);
        public abstract void DrawDropDown(GrRect paintRect, GrControlState state, GrRect? pClipRect);
        public abstract void DrawModal(GrRect paintRect, GrControlState state, GrRect? pClipRect);
        public abstract void DrawExpander(GrPaintStyle paintStyle, GrRect paintRect, GrControlState state, bool opened, GrColor foreColor, GrColor backColor);
        public abstract void DrawSortGlyph(GrRect paintRect, GrSort sortType);

        public abstract void DrawColumn(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor fillColor, GrRect? pClipRect);
        public abstract void DrawRow(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor fillColor, GrRect? pClipRect);
        public abstract void DrawItem(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor fillColor, GrRect? pClipRect);
        public abstract void DrawGroupHeader(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor fillColor, GrRect? pClipRect);

        public abstract void DrawText(GrFont pFont, string text, int textLength, GrRect paintRect, GrColor color, GrRect? pClipRect);
        public abstract void DrawColumnText(GrFont pFont, string text, int textLength, GrRect paintRect, GrColor color, GrRect? pClipRect);

        public abstract void DrawResizingLine(int x1, int y1, int x2, int y2);
        public abstract void DrawSplitterMovingLine(GrRect paintRect);

        public abstract void DrawRectangle(GrRect rect, GrColor color);
        public abstract void DrawLine(int x1, int y1, int x2, int y2, GrColor color);

        public abstract void FillRectangle(GrRect rect, GrColor color);

        public abstract void EndPaint();
        public abstract void SetFont(GrFont pFont);

        public abstract object GetDevice();
        public abstract void ReleaseDevice(object painterDevice);
    };
}
