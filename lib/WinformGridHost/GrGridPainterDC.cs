using Ntreev.Library.Grid;
using Ntreev.Windows.Forms.Grid.Natives;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid
{
    class GrGridPainterDC : GrGridPainter
    {
        private const int CBXS_NORMAL = 1;
        private const int CBXS_HOT = 2;
        private const int CBXS_PRESSED = 3;
        private const int CBXS_DISABLED = 4;

        private const int DFCS_INACTIVE = 0x0100;
        private const int DFCS_PUSHED = 0x0200;
        private const int DFCS_CHECKED = 0x0400;

        private const int DFCS_TRANSPARENT = 0x0800;
        private const int DFCS_HOT = 0x1000;

        private const int DFC_CAPTION = 1;
        private const int DFC_MENU = 2;
        private const int DFC_SCROLL = 3;
        private const int DFC_BUTTON = 4;
        private const int DFC_POPUPMENU = 5;

        IntPtr m_hdc;
        IntPtr m_hWnd;
        PAINTSTRUCT m_ps;

        IntPtr m_hOverlayDC;
        IntPtr m_hOverlayBmp, m_hOverlayBmpOld;

        IntPtr m_hColorDC;
        IntPtr m_hColorBmp, m_hColorBmpOld;

        IntPtr m_hMouseOverBrush;
        IntPtr m_hSelectedBrush;
        IntPtr m_hSelectedMouseOverBrush;
        IntPtr m_hWhiteBrush;
        IntPtr m_hFocusedPen;
        //IntPtr m_hSelRectPen;
        IntPtr m_hSizingPen;
        IntPtr m_hSplitterMovingPen;

        IntPtr m_hFont;
        IntPtr m_hOldFont;

        IntPtr m_hColumnTheme;
        IntPtr m_hDropDownTheme;
        IntPtr m_hModalTheme;
        IntPtr m_hTreeTheme;
        IntPtr m_hTooltip;

        uint m_dwMajorVersion;


        GrPoint m_ptTransform;

        public GrGridPainterDC(IntPtr hWnd)
        {
            m_hWnd = hWnd;
            m_hColumnTheme = NativeMethods.OpenThemeData(m_hWnd, "HEADER");
            m_hDropDownTheme = NativeMethods.OpenThemeData(m_hWnd, "COMBOBOX");
            m_hModalTheme = NativeMethods.OpenThemeData(m_hWnd, "BUTTON");
            m_hTreeTheme = NativeMethods.OpenThemeData(m_hWnd, "TREEVIEW");
            m_hTooltip = NativeMethods.OpenThemeData(m_hWnd, "TOOLTIP");

            OSVERSIONINFO osInfo = new OSVERSIONINFO();
            //memset(&osInfo, 0, sizeof(OSVERSIONINFOW));
            osInfo.dwOSVersionInfoSize = (uint)Marshal.SizeOf(typeof(OSVERSIONINFO));
            NativeMethods.GetVersionEx(ref osInfo);

            m_dwMajorVersion = osInfo.dwMajorVersion;

            m_hOverlayBmp = NativeMethods.CreateBitmap(4, 4, 1, 32, IntPtr.Zero);
            m_hColorBmp = NativeMethods.CreateBitmap(4, 4, 1, 32, IntPtr.Zero);

            m_hMouseOverBrush = NativeMethods.CreateSolidBrush(240, 240, 240);
            m_hSelectedBrush = NativeMethods.CreateSolidBrush(47, 106, 197);
            m_hSelectedMouseOverBrush = NativeMethods.CreateSolidBrush(71, 130, 221);

            m_hWhiteBrush = NativeMethods.GetStockObject(StockObjects.WHITE_BRUSH);
            //m_hSelRectPen = CreatePen(PS_SOLID, 1, RGB(208,215,229));

            LOGBRUSH lb;
            lb.lbStyle = BrushStyle.BS_SOLID;
            lb.lbColor = RGB(125, 125, 125);
            lb.lbHatch = 0;
            uint[] Style = { 1, 1 };
            //m_hSizingPen = ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_FLAT, 1, &lb, 2, Style);
            m_hSizingPen = NativeMethods.CreatePen(PenStyle.PS_SOLID, 1, new GrColor(133, 151, 188));

            lb.lbColor = RGB(50, 50, 50);
            //m_hFocusedPen = ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_FLAT, 1, &lb, 2, Style);
            m_hFocusedPen = NativeMethods.CreatePen(PenStyle.PS_SOLID, 1, new GrColor(133, 151, 188));

            lb.lbStyle = BrushStyle.BS_HATCHED;
            lb.lbColor = RGB(0, 0, 0);
            lb.lbHatch = HatchStyle.HS_CROSS;
            m_hSplitterMovingPen = NativeMethods.ExtCreatePen(PenStyle.PS_GEOMETRIC | PenStyle.PS_ENDCAP_FLAT, (uint)GrColumnSplitter.DefaultSplitterWidth, ref lb, 2, Style);
        }
        public void Dispose()
        {
            NativeMethods.DeleteDC(m_hOverlayDC);
            NativeMethods.DeleteObject(m_hSplitterMovingPen);
            NativeMethods.DeleteObject(m_hSizingPen);
            //DeleteObject(m_hSelRectPen);
            NativeMethods.DeleteObject(m_hFocusedPen);
            NativeMethods.DeleteObject(m_hSelectedMouseOverBrush);
            NativeMethods.DeleteObject(m_hSelectedBrush);
            NativeMethods.DeleteObject(m_hMouseOverBrush);

            //DestroyWindow(m_hTooltipWnd);
            NativeMethods.DeleteObject(m_hOverlayBmp);
            NativeMethods.CloseThemeData(m_hTooltip);
            NativeMethods.CloseThemeData(m_hTreeTheme);
            NativeMethods.CloseThemeData(m_hModalTheme);
            NativeMethods.CloseThemeData(m_hDropDownTheme);
            NativeMethods.CloseThemeData(m_hColumnTheme);
        }

        public void BeginPaint(object hdc)
        {
            m_hdc = (IntPtr)hdc;

            m_hOverlayDC = NativeMethods.CreateCompatibleDC(m_hdc);
            m_hOverlayBmpOld = NativeMethods.SelectObject(m_hOverlayDC, m_hOverlayBmp);

            m_hFont = IntPtr.Zero;
            m_hOldFont = IntPtr.Zero;

            NativeMethods.SetBkMode(m_hdc, BkMode.TRANSPARENT);

            m_ptTransform = GrPoint.Empty;
        }


        public void DrawRowSplitter(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor backColor)
        {
            DrawItem(paintStyle, paintRect, lineColor, backColor, null);
        }

        public void DrawDropDown(GrRect paintRect, GrControlState state, GrRect? pClipRect)
        {
            RECT rtPaint = (Rectangle)paintRect;
            RECT rtClip = pClipRect.HasValue == true ? (Rectangle)pClipRect.Value : (Rectangle)rtPaint;
            rtClip.Offset(m_ptTransform.X, m_ptTransform.Y);

            if (m_hDropDownTheme != IntPtr.Zero)
            {
                switch (state)
                {
                    case GrControlState.Normal:
                        NativeMethods.DrawThemeBackground(m_hDropDownTheme, m_hdc, UxThemeParts.CP_DROPDOWNBUTTON, UxThemeStates.CBXS_NORMAL, ref rtPaint, ref rtClip);
                        break;
                    case GrControlState.Hot:
                        NativeMethods.DrawThemeBackground(m_hDropDownTheme, m_hdc, UxThemeParts.CP_DROPDOWNBUTTON, UxThemeStates.CBXS_HOT, ref rtPaint, ref rtClip);
                        break;
                    case GrControlState.Pressed:
                        NativeMethods.DrawThemeBackground(m_hDropDownTheme, m_hdc, UxThemeParts.CP_DROPDOWNBUTTON, UxThemeStates.CBXS_PRESSED, ref rtPaint, ref rtClip);
                        break;
                }
            }
            else
            {
                DrawFrameControlStates flag = 0;
                switch (state)
                {
                    case GrControlState.Normal:
                        break;
                    case GrControlState.Hot:
                        flag = DrawFrameControlStates.DFCS_HOT;
                        break;
                    case GrControlState.Pressed:
                        flag = DrawFrameControlStates.DFCS_PUSHED;
                        break;
                }

                NativeMethods.DrawFrameControl(m_hdc, ref rtPaint, DrawFrameControlTypes.DFC_SCROLL, DrawFrameControlStates.DFCS_SCROLLDOWN | flag);
            }
        }

        public void DrawModal(GrRect paintRect, GrControlState state, GrRect? pClipRect)
        {
            RECT rtPaint = (Rectangle)paintRect;
            RECT rtClip = pClipRect.HasValue == true ? (Rectangle)pClipRect.Value : (Rectangle)rtPaint;
            rtClip.Offset(m_ptTransform.X, m_ptTransform.Y);

            switch (state)
            {
                case GrControlState.Normal:
                    NativeMethods.DrawThemeBackground(m_hModalTheme, m_hdc, UxThemeParts.BP_PUSHBUTTON, UxThemeStates.PBS_NORMAL, ref rtPaint, ref rtClip);
                    break;
                case GrControlState.Hot:
                    NativeMethods.DrawThemeBackground(m_hModalTheme, m_hdc, UxThemeParts.BP_PUSHBUTTON, UxThemeStates.PBS_HOT, ref rtPaint, ref rtClip);
                    break;
                case GrControlState.Pressed:
                    NativeMethods.DrawThemeBackground(m_hModalTheme, m_hdc, UxThemeParts.BP_PUSHBUTTON, UxThemeStates.PBS_PRESSED, ref rtPaint, ref rtClip);
                    break;
            }

            rtPaint.Bottom = rtPaint.Top + (int)((float)(rtPaint.Bottom - rtPaint.Top) * (4.5f / 5.0f));
            NativeMethods.DrawThemeText(m_hModalTheme, m_hdc, UxThemeParts.BP_PUSHBUTTON, UxThemeStates.PBS_NORMAL, "...", 3, DrawTextFlags.DT_CENTER | DrawTextFlags.DT_BOTTOM | DrawTextFlags.DT_SINGLELINE, 0, ref rtPaint);
        }

        public void DrawExpander(GrPaintStyle paintStyle, GrRect paintRect, GrControlState state, bool opened, GrColor foreColor, GrColor backColor)
        {
            RECT rt = (Rectangle)paintRect;

            IntPtr br = NativeMethods.CreateSolidBrush(backColor.R, backColor.G, backColor.B);
            NativeMethods.FillRect(m_hdc, ref rt, br);
            NativeMethods.DeleteObject(br);

            if (opened == true)
            {
                if (m_hTreeTheme != IntPtr.Zero && m_dwMajorVersion >= 6)
                {
                    NativeMethods.DrawThemeBackground(m_hTreeTheme, m_hdc, UxThemeParts.TVP_GLYPH, UxThemeStates.GLPS_OPENED, ref rt, ref rt);
                }
                else
                {
                    GrPoint ptCenter = paintRect.GetCenter();

                    rt.Left = ptCenter.X - 4;
                    rt.Top = ptCenter.Y - 4;
                    rt.Right = ptCenter.X + 5;
                    rt.Bottom = ptCenter.Y + 5;

                    NativeMethods.Rectangle(m_hdc, rt.Left, rt.Top, rt.Right, rt.Bottom);
                    NativeMethods.MoveToEx(m_hdc, rt.Left + 2, ptCenter.Y, IntPtr.Zero);
                    NativeMethods.LineTo(m_hdc, rt.Right - 2, ptCenter.Y);
                }
            }
            else
            {
                if (m_hTreeTheme != IntPtr.Zero && m_dwMajorVersion >= 6)
                {
                    NativeMethods.DrawThemeBackground(m_hTreeTheme, m_hdc, UxThemeParts.TVP_GLYPH, UxThemeStates.GLPS_CLOSED, ref rt, ref rt);
                }
                else
                {
                    GrPoint ptCenter = paintRect.GetCenter();

                    rt.Left = ptCenter.X - 4;
                    rt.Top = ptCenter.Y - 4;
                    rt.Right = ptCenter.X + 5;
                    rt.Bottom = ptCenter.Y + 5;

                    NativeMethods.Rectangle(m_hdc, rt.Left, rt.Top, rt.Right, rt.Bottom);
                    NativeMethods.MoveToEx(m_hdc, rt.Left + 2, ptCenter.Y, IntPtr.Zero);
                    NativeMethods.LineTo(m_hdc, rt.Right - 2, ptCenter.Y);

                    NativeMethods.MoveToEx(m_hdc, ptCenter.X, rt.Top + 2, IntPtr.Zero);
                    NativeMethods.LineTo(m_hdc, ptCenter.X, rt.Bottom - 2);
                }
            }

            IntPtr hPen = NativeMethods.CreatePen(PenStyle.PS_SOLID, 1, foreColor);
            IntPtr hOldPen = NativeMethods.SelectObject(m_hdc, (IntPtr)hPen);

            NativeMethods.MoveToEx(m_hdc, rt.Right - 1, rt.Top, IntPtr.Zero);
            NativeMethods.LineTo(m_hdc, rt.Right - 1, rt.Bottom);

            if (paintStyle.HasFlag(GrPaintStyle.BottomLine) == true)
            {
                NativeMethods.MoveToEx(m_hdc, rt.Left, rt.Bottom - 1, IntPtr.Zero);
                NativeMethods.LineTo(m_hdc, rt.Right, rt.Bottom - 1);
            }

            NativeMethods.SelectObject(m_hdc, hOldPen);
            NativeMethods.DeleteObject(hPen);
        }

        public void DrawSortGlyph(GrRect paintRect, GrSort sortType)
        {
            RECT rtPaint = (Rectangle)paintRect;

            switch (sortType)
            {
                case GrSort.Up:
                    {
                        if (m_hColumnTheme != IntPtr.Zero && m_dwMajorVersion >= 6)
                        {
                            NativeMethods.DrawThemeBackground(m_hColumnTheme, m_hdc, UxThemeParts.HP_HEADERSORTARROW, UxThemeStates.HSAS_SORTEDUP, ref rtPaint, ref rtPaint);
                        }
                        else
                        {
                            GrPoint ptCenter = paintRect.GetCenter();

                            NativeMethods.MoveToEx(m_hdc, ptCenter.X, ptCenter.Y - 2, IntPtr.Zero);
                            NativeMethods.LineTo(m_hdc, ptCenter.X + 1, ptCenter.Y - 2);

                            NativeMethods.MoveToEx(m_hdc, ptCenter.X - 1, ptCenter.Y - 1, IntPtr.Zero);
                            NativeMethods.LineTo(m_hdc, ptCenter.X + 2, ptCenter.Y - 1);

                            NativeMethods.MoveToEx(m_hdc, ptCenter.X - 2, ptCenter.Y, IntPtr.Zero);
                            NativeMethods.LineTo(m_hdc, ptCenter.X + 3, ptCenter.Y);

                            NativeMethods.MoveToEx(m_hdc, ptCenter.X - 3, ptCenter.Y + 1, IntPtr.Zero);
                            NativeMethods.LineTo(m_hdc, ptCenter.X + 4, ptCenter.Y + 1);
                        }
                    }
                    break;
                case GrSort.Down:
                    {
                        if (m_hColumnTheme != IntPtr.Zero && m_dwMajorVersion >= 6)
                        {
                            NativeMethods.DrawThemeBackground(m_hColumnTheme, m_hdc, UxThemeParts.HP_HEADERSORTARROW, UxThemeStates.HSAS_SORTEDDOWN, ref rtPaint, ref rtPaint);
                        }
                        else
                        {
                            GrPoint ptCenter = paintRect.GetCenter();

                            NativeMethods.MoveToEx(m_hdc, ptCenter.X, ptCenter.Y + 1, IntPtr.Zero);
                            NativeMethods.LineTo(m_hdc, ptCenter.X + 1, ptCenter.Y + 1);

                            NativeMethods.MoveToEx(m_hdc, ptCenter.X - 1, ptCenter.Y, IntPtr.Zero);
                            NativeMethods.LineTo(m_hdc, ptCenter.X + 2, ptCenter.Y);

                            NativeMethods.MoveToEx(m_hdc, ptCenter.X - 2, ptCenter.Y - 1, IntPtr.Zero);
                            NativeMethods.LineTo(m_hdc, ptCenter.X + 3, ptCenter.Y - 1);

                            NativeMethods.MoveToEx(m_hdc, ptCenter.X - 3, ptCenter.Y - 2, IntPtr.Zero);
                            NativeMethods.LineTo(m_hdc, ptCenter.X + 4, ptCenter.Y - 2);
                        }
                    }
                    break;
                case GrSort.None:
                    break;
            }
        }

        public void DrawColumn(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor backColor, GrRect? pClipRect)
        {
            this.DrawGradientHeader(paintStyle, paintRect, lineColor, backColor, pClipRect, false);
        }

        public void DrawRow(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor backColor, GrRect? pClipRect)
        {
            this.DrawGradientHeader(paintStyle, paintRect, lineColor, backColor, pClipRect, false);
        }

        public void DrawItem(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor backColor, GrRect? pClipRect)
        {
            RECT rt = (Rectangle)paintRect;
            if (pClipRect.HasValue == true)
            {
                if (rt.Right > pClipRect.Value.Right)
                    rt.Right = pClipRect.Value.Right;
                if (rt.Bottom > pClipRect.Value.Bottom)
                    rt.Bottom = pClipRect.Value.Bottom;
            }

            if (paintStyle.HasFlag(GrPaintStyle.Focused) == true)
            {
                GrColor backColor1 = backColor;
                GrColor backColor2 = GrColor.ModifyBrightness(backColor, 0.9f);
                TRIVERTEX[] vert = new TRIVERTEX[2];
                GRADIENT_RECT[] grect = new GRADIENT_RECT[1];

                vert[0].x = rt.Left;
                vert[0].y = rt.Top;

                vert[0].Red = (ushort)(backColor1.R << 8);
                vert[0].Green = (ushort)(backColor1.G << 8);
                vert[0].Blue = (ushort)(backColor1.B << 8);
                vert[0].Alpha = (ushort)(backColor1.A << 8);

                vert[1].x = rt.Right;
                vert[1].y = rt.Bottom;

                vert[1].Red = (ushort)(backColor2.R << 8);
                vert[1].Green = (ushort)(backColor2.G << 8);
                vert[1].Blue = (ushort)(backColor2.B << 8);
                vert[1].Alpha = (ushort)(backColor2.A << 8);

                grect[0].UpperLeft = 0;
                grect[0].LowerRight = 1;

                NativeMethods.GradientFill(m_hdc, vert, 2, grect, 1, GradientFillMode.RECT_V);
            }
            else
            {
                IntPtr hb = NativeMethods.CreateSolidBrush(backColor);
                NativeMethods.FillRect(m_hdc, ref rt, hb);
                NativeMethods.DeleteObject(hb);
            }

            IntPtr hPen = NativeMethods.CreatePen(PenStyle.PS_SOLID, 1, lineColor);
            IntPtr hOldPen = NativeMethods.SelectObject(m_hdc, hPen);

            if (paintStyle.HasFlag(GrPaintStyle.RightLine) == true && paintRect.Right <= rt.Right)
            {
                NativeMethods.MoveToEx(m_hdc, rt.Right - 1, rt.Top, IntPtr.Zero);
                NativeMethods.LineTo(m_hdc, rt.Right - 1, rt.Bottom - 1);
            }

            if (paintStyle.HasFlag(GrPaintStyle.BottomLine) == true && paintRect.Bottom <= rt.Bottom)
            {
                NativeMethods.MoveToEx(m_hdc, rt.Left, rt.Bottom - 1, IntPtr.Zero);
                NativeMethods.LineTo(m_hdc, rt.Right, rt.Bottom - 1);
            }
            NativeMethods.SelectObject(m_hdc, hOldPen);
            NativeMethods.DeleteObject(hPen);

            if (paintStyle.HasFlag(GrPaintStyle.Focused) == true)
            {
                IntPtr hOld = NativeMethods.SelectObject(m_hdc, (IntPtr)m_hFocusedPen);
                IntPtr hOb = NativeMethods.SelectObject(m_hdc, NativeMethods.GetStockObject(StockObjects.NULL_BRUSH));
                NativeMethods.Rectangle(m_hdc, rt.Left, rt.Top, rt.Right - 1, rt.Bottom - 1);
                NativeMethods.SelectObject(m_hdc, hOb);
                NativeMethods.SelectObject(m_hdc, hOld);
            }
        }

        public void DrawGroupHeader(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor fillColor, GrRect? pClipRect)
        {
            this.DrawGradientHeader(paintStyle, paintRect, lineColor, fillColor, pClipRect.Value, true);
        }

        public void DrawText(GrFont pFont, string text, int textLength, GrRect paintRect, GrColor color, GrRect? pClipRect)
        {
            RECT rtPaint = (Rectangle)paintRect;
            rtPaint.Offset(m_ptTransform.X, m_ptTransform.Y);

            SetFont(pFont);
            NativeMethods.SetTextColor(m_hdc, color);
            if (pClipRect.HasValue == true)
            {
                RECT rtClip = (Rectangle)pClipRect.Value;
                rtClip.Offset(m_ptTransform.X, m_ptTransform.Y);
                NativeMethods.ExtTextOut(m_hdc, rtPaint.Left, rtPaint.Top, ETOOptions.ETO_CLIPPED, ref rtClip, text, (uint)textLength, IntPtr.Zero);
            }
            else
            {
                NativeMethods.ExtTextOut(m_hdc, rtPaint.Left, rtPaint.Top, 0, IntPtr.Zero, text, (uint)textLength, IntPtr.Zero);
            }
        }

        public void DrawColumnText(GrFont pFont, string text, int textLength, GrRect paintRect, GrColor color, GrRect? pClipRect)
        {
            RECT rtPaint = (Rectangle)paintRect;
            if (pClipRect.HasValue == true)
            {
                if (rtPaint.Right > pClipRect.Value.Right)
                    rtPaint.Right = pClipRect.Value.Right;
                if (rtPaint.Bottom > pClipRect.Value.Bottom)
                    rtPaint.Bottom = pClipRect.Value.Bottom;
            }
            rtPaint.Offset(m_ptTransform.X, m_ptTransform.Y);

            SetFont(pFont);
            NativeMethods.SetTextColor(m_hdc, color);

            if (pClipRect.HasValue)
            {
                if (rtPaint.Right > pClipRect.Value.Right)
                    rtPaint.Right = pClipRect.Value.Right;
                if (rtPaint.Bottom > pClipRect.Value.Bottom)
                    rtPaint.Bottom = pClipRect.Value.Bottom;
            }

            NativeMethods.DrawText(m_hdc, text, textLength, ref rtPaint, DrawTextFlags.DT_CENTER | DrawTextFlags.DT_VCENTER | DrawTextFlags.DT_END_ELLIPSIS | DrawTextFlags.DT_SINGLELINE);
        }

        public void DrawTooltip(GrRect paintRect)
        {
            RECT rtPaint = (Rectangle)paintRect;
            NativeMethods.DrawThemeBackground(m_hTooltip, m_hdc, UxThemeParts.TTP_BALLOON, UxThemeStates.TTCS_NORMAL, ref rtPaint, ref rtPaint);
        }

        public void DrawResizingLine(int x1, int y1, int x2, int y2)
        {
            //if(x1 < ps.rcPaint.Left || x2 > ps.rcPaint.Right)
            // int wqer=0;
            IntPtr hOldPen;
            hOldPen = NativeMethods.SelectObject(m_hdc, m_hSizingPen);
            NativeMethods.MoveToEx(m_hdc, x1, y1, IntPtr.Zero);
            NativeMethods.LineTo(m_hdc, x2, y2);
            NativeMethods.SelectObject(m_hdc, hOldPen);
        }

        public void DrawSplitterMovingLine(GrRect paintRect)
        {
            BinaryRasterOperations n = NativeMethods.SetROP2(m_hdc, BinaryRasterOperations.R2_MERGEPEN);

            uint[] b = { 0x00ffffff, 0, 0, 0x00ffffff };
            IntPtr hBitmap = NativeMethods.CreateBitmap(2, 2, 32, 1, b);
            IntPtr hBrush = NativeMethods.CreatePatternBrush(hBitmap);

            NativeMethods.FillRect(m_hdc, paintRect, hBrush);

            NativeMethods.DeleteObject(hBrush);
            NativeMethods.SetROP2(m_hdc, n);
        }

        public void DrawRectangle(GrRect rect, GrColor color)
        {
            IntPtr hPen = NativeMethods.CreatePen(PenStyle.PS_SOLID, 1, color);
            IntPtr hOldPen = NativeMethods.SelectObject(m_hdc, hPen);
            // left
            NativeMethods.MoveToEx(m_hdc, rect.Left, rect.Top, IntPtr.Zero);
            NativeMethods.LineTo(m_hdc, rect.Left, rect.Bottom - 1);
            // top
            NativeMethods.MoveToEx(m_hdc, rect.Left, rect.Top, IntPtr.Zero);
            NativeMethods.LineTo(m_hdc, rect.Right, rect.Top);
            // right
            NativeMethods.MoveToEx(m_hdc, rect.Right - 1, rect.Top, IntPtr.Zero);
            NativeMethods.LineTo(m_hdc, rect.Right - 1, rect.Bottom - 1);
            // bottom
            NativeMethods.MoveToEx(m_hdc, rect.Left, rect.Bottom - 1, IntPtr.Zero);
            NativeMethods.LineTo(m_hdc, rect.Right, rect.Bottom - 1);
            NativeMethods.SelectObject(m_hdc, hOldPen);
            NativeMethods.DeleteObject(hPen);
        }

        public void DrawLine(int x1, int y1, int x2, int y2, GrColor color)
        {
            IntPtr hBrush = NativeMethods.CreateSolidBrush(color);
            NativeMethods.MoveToEx(m_hdc, x1, y1, IntPtr.Zero);
            NativeMethods.LineTo(m_hdc, x2, y2);
            NativeMethods.DeleteObject(hBrush);
        }

        public void FillRectangle(GrRect rect, GrColor color)
        {
            IntPtr hBrush = NativeMethods.CreateSolidBrush(color);
            NativeMethods.FillRect(m_hdc, rect, hBrush);
            NativeMethods.DeleteObject(hBrush);
        }


        public void EndPaint()
        {
            NativeMethods.SelectObject(m_hdc, m_hOldFont);
            NativeMethods.SelectObject(m_hOverlayDC, m_hOverlayBmpOld);

            m_hdc = IntPtr.Zero;
        }

        public void SetFont(GrFont pFont)
        {
            GrFontDC pFontDC = pFont as GrFontDC;

            IntPtr hCurFont = pFontDC.GetFontHandle();

            if (m_hFont != hCurFont)
            {
                IntPtr hOldFont = NativeMethods.SelectObject(m_hdc, hCurFont);
                if (m_hOldFont == IntPtr.Zero)
                    m_hOldFont = hOldFont;
                m_hFont = hCurFont;
            }
        }

        public IntPtr GetDevice() { return m_hdc; }

        public void ReleaseDevice(IntPtr dc) { }

        private void DrawThemeHeader(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor backColor, GrRect? pClipRect)
        {
            RECT rtPaint = (Rectangle)paintRect;
            RECT rtClip = (Rectangle)paintRect;
            rtPaint.Offset(m_ptTransform.X, m_ptTransform.Y);
            rtClip.Offset(m_ptTransform.X, m_ptTransform.Y);
            rtPaint.Bottom++;
            GrColor paintColor = backColor;

            BLENDFUNCTION blend;
            blend.BlendFlags = 0;
            blend.AlphaFormat = BLENDFUNCTION.AC_SRC_ALPHA;
            blend.BlendOp = BLENDFUNCTION.AC_SRC_OVER;
            blend.SourceConstantAlpha = 15;

            if (pClipRect.HasValue)
            {
                rtClip.Right = Math.Min(rtClip.Right, pClipRect.Value.Right);
                rtClip.Bottom = Math.Min(rtClip.Bottom, pClipRect.Value.Bottom);
            }

            if (m_hColumnTheme != IntPtr.Zero)
            {
                UxThemeStates nStateID = UxThemeStates.HIS_NORMAL;
                if (paintStyle.HasFlag(GrPaintStyle.Mouseover) == true)
                    nStateID = UxThemeStates.HIS_HOT;
                if (paintStyle.HasFlag(GrPaintStyle.Pressed) == true)
                    nStateID = UxThemeStates.HIS_PRESSED;

                NativeMethods.DrawThemeBackground(m_hColumnTheme, m_hdc, UxThemeParts.HP_HEADERITEM, nStateID, ref rtPaint, ref rtClip);
            }
            else
            {
                DrawFrameControlStates nStateID = 0;
                if (paintStyle.HasFlag(GrPaintStyle.Mouseover) == true)
                    nStateID = DrawFrameControlStates.DFCS_HOT;
                if (paintStyle.HasFlag(GrPaintStyle.Pressed) == true)
                    nStateID = DrawFrameControlStates.DFCS_PUSHED;
                NativeMethods.DrawFrameControl(m_hdc, ref rtPaint, DrawFrameControlTypes.DFC_BUTTON, DrawFrameControlStates.DFCS_BUTTONPUSH | nStateID);
            }

            IntPtr hb = NativeMethods.CreateSolidBrush(paintColor);
            RECT rt = new RECT(0, 0, 4, 4);

            paintColor.A = 255;
            uint[] bits = new uint[16];
            //c.A(columnAlpha/255.0f);
            for (int i = 0; i < 16; i++)
            {
                bits[i] = (uint)paintColor.ToARGB();
            }

            IntPtr hBitmap = NativeMethods.CreateBitmap(4, 4, 1, 32, bits);
            IntPtr hOldBitmap = NativeMethods.SelectObject(m_hOverlayDC, hBitmap);
            //FillRect(m_hOverlayDC, &rt, hb);
            NativeMethods.DeleteObject(hb);


            //if(dwPaintStyle & GrPaintStyle.Selected)
            {
                RECT alphaRect = pClipRect == null ? rtPaint : rtClip;

                NativeMethods.AlphaBlend(m_hdc, alphaRect.Left, alphaRect.Top, alphaRect.Right - alphaRect.Left, alphaRect.Bottom - alphaRect.Top, m_hOverlayDC, 0, 0, 4, 4, blend);
            }

            NativeMethods.SelectObject(m_hOverlayDC, hOldBitmap);
            NativeMethods.DeleteObject(hBitmap);

            if (paintStyle.HasFlag(GrPaintStyle.Focused) == true)
            {
                IntPtr hOld = NativeMethods.SelectObject(m_hdc, m_hFocusedPen);
                IntPtr hOb = NativeMethods.SelectObject(m_hdc, NativeMethods.GetStockObject(StockObjects.NULL_BRUSH));
                NativeMethods.Rectangle(m_hdc, rtPaint.Left, rtPaint.Top, rtPaint.Right, rtPaint.Bottom - 2);
                NativeMethods.SelectObject(m_hdc, hOb);
                NativeMethods.SelectObject(m_hdc, hOld);
            }

            //SetBkMode(m_hdc, nOldBk);
        }

        private void DrawGradientHeader(GrPaintStyle paintStyle, GrRect paintRect, GrColor lineColor, GrColor backColor, GrRect? pClipRect, bool focusedLine)
        {
            GrColor backColor1, backColor2;

            if (paintStyle.HasFlag(GrPaintStyle.Pressed) == true)
            {
                backColor1 = GrColor.ModifyBrightness(backColor, 0.9f);
                backColor2 = GrColor.ModifyBrightness(backColor1, 0.9f);
            }
            else if (paintStyle.HasFlag(GrPaintStyle.Mouseover) == true)
            {
                backColor1 = backColor;
                backColor2 = GrColor.ModifyBrightness(backColor, 0.95f);
            }
            else if (paintStyle.HasFlag(GrPaintStyle.Selected) == true)
            {
                backColor1 = GrColor.ModifyBrightness(backColor, 0.95f);
                backColor2 = GrColor.ModifyBrightness(backColor1, 0.9f);
            }
            else if (paintStyle.HasFlag(GrPaintStyle.Focused) == true)
            {
                backColor1 = GrColor.ModifyBrightness(backColor, 0.925f);
                backColor2 = GrColor.ModifyBrightness(backColor1, 0.9f);
            }
            else
            {
                backColor1 = backColor;
                backColor2 = GrColor.ModifyBrightness(backColor, 0.9f);
            }

            RECT rt = (Rectangle)paintRect;

            if (pClipRect != null)
            {
                if (rt.Right > pClipRect.Value.Right)
                    rt.Right = pClipRect.Value.Right;
                if (rt.Bottom > pClipRect.Value.Bottom)
                    rt.Bottom = pClipRect.Value.Bottom;
            }

            TRIVERTEX[] vert = new TRIVERTEX[2];
            GRADIENT_RECT[] grect = new GRADIENT_RECT[1];

            vert[0].x = rt.Left;
            vert[0].y = rt.Top;

            vert[0].Red = (ushort)(backColor1.R << 8);
            vert[0].Green = (ushort)(backColor1.G << 8);
            vert[0].Blue = (ushort)(backColor1.B << 8);
            vert[0].Alpha = (ushort)(backColor1.A << 8);

            vert[1].x = rt.Right;
            vert[1].y = rt.Bottom;

            vert[1].Red = (ushort)(backColor2.R << 8);
            vert[1].Green = (ushort)(backColor2.G << 8);
            vert[1].Blue = (ushort)(backColor2.B << 8);
            vert[1].Alpha = (ushort)(backColor2.A << 8);

            grect[0].UpperLeft = 0;
            grect[0].LowerRight = 1;


            //IntPtr hb = CreateSolidBrush(_RGB(backColor));
            NativeMethods.GradientFill(m_hdc, vert, 2, grect, 1, GradientFillMode.RECT_V);
            //DeleteObject(hb);

            IntPtr hPen = NativeMethods.CreatePen(PenStyle.PS_SOLID, 1, lineColor);
            IntPtr hOldPen = NativeMethods.SelectObject(m_hdc, hPen);

            if (paintStyle.HasFlag(GrPaintStyle.RightLine) == true)
            {
                NativeMethods.MoveToEx(m_hdc, rt.Right - 1, rt.Top, IntPtr.Zero);
                NativeMethods.LineTo(m_hdc, rt.Right - 1, rt.Bottom - 1);
            }

            if (paintStyle.HasFlag(GrPaintStyle.BottomLine) == true)
            {
                NativeMethods.MoveToEx(m_hdc, rt.Left, rt.Bottom - 1, IntPtr.Zero);
                NativeMethods.LineTo(m_hdc, rt.Right, rt.Bottom - 1);
            }
            NativeMethods.SelectObject(m_hdc, hOldPen);
            NativeMethods.DeleteObject(hPen);

            if (focusedLine == true && paintStyle.HasFlag(GrPaintStyle.Focused) == true)
            {
                IntPtr hOld = NativeMethods.SelectObject(m_hdc, (IntPtr)m_hFocusedPen);
                IntPtr hOb = NativeMethods.SelectObject(m_hdc, NativeMethods.GetStockObject(StockObjects.NULL_BRUSH));
                NativeMethods.Rectangle(m_hdc, rt.Left, rt.Top, rt.Right - 1, rt.Bottom - 1);
                NativeMethods.SelectObject(m_hdc, hOb);
                NativeMethods.SelectObject(m_hdc, hOld);
            }
        }

        private uint RGB(int r, int g, int b)
        {
            return (uint)r + (((uint)g) << 8) + (((uint)b) << 16);
        }


        const int TRANSPARENT = 1;
        const int OPAQUE = 2;



    }
}
