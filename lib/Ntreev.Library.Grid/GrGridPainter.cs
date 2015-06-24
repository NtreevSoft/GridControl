using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public interface GrGridPainter : IDisposable
    {
        void BeginPaint(object painterDevice);

        void DrawRowSplitter(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor fillColor);
        void DrawDropDown(GrRect paintRect, GrControlState state, GrRect? pClipRect);
        void DrawModal(GrRect paintRect, GrControlState state, GrRect? pClipRect);
        void DrawExpander(GrPaintStyle paintStyle, GrRect paintRect, GrControlState state, bool opened, GrColor foreColor, GrColor backColor);
        void DrawSortGlyph(GrRect paintRect, GrSort sortType);

        void DrawColumn(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor fillColor, GrRect? pClipRect);
        void DrawRow(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor fillColor, GrRect? pClipRect);
        void DrawItem(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor fillColor, GrRect? pClipRect);
        void DrawGroupHeader(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor fillColor, GrRect? pClipRect);

        void DrawText(GrFont pFont, string text, int textLength, GrRect paintRect, GrColor color, GrRect? pClipRect);
        void DrawColumnText(GrFont pFont, string text, int textLength, GrRect paintRect, GrColor color, GrRect? pClipRect);

        void DrawResizingLine(int x1, int y1, int x2, int y2);
        void DrawSplitterMovingLine(GrRect paintRect);

        void DrawRectangle(GrRect rect, GrColor color);
        void DrawLine(int x1, int y1, int x2, int y2, GrColor color);

        void FillRectangle(GrRect rect, GrColor color);

        void EndPaint();
        void SetFont(GrFont pFont);

        IntPtr GetDevice();
        void ReleaseDevice(IntPtr painterDevice);
    };
}
