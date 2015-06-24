using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid.Natives
{
    static partial class NativeMethods
    {
        [DllImport("uxtheme.dll", ExactSpelling = true, CharSet = CharSet.Unicode)]
        public static extern IntPtr OpenThemeData(IntPtr hWnd, string classList);

        [DllImport("kernel32")]
        public static extern bool GetVersionEx(ref OSVERSIONINFO osvi);

        [DllImport("gdi32.dll")]
        public static extern IntPtr CreateBitmap(int nWidth, int nHeight, uint cPlanes, uint cBitsPerPel, IntPtr lpvBits);

        [DllImport("gdi32.dll")]
        public static extern IntPtr CreateBitmap(int nWidth, int nHeight, uint cPlanes, uint cBitsPerPel, uint[] lpvBits);

        [DllImport("gdi32.dll")]
        public static extern IntPtr CreateSolidBrush(uint crColor);

        public static IntPtr CreateSolidBrush(int r, int g, int b)
        {
            return CreateSolidBrush(RGB(r, g, b));
        }

        [DllImport("user32.dll")]
        public static extern int FillRect(IntPtr hDC, [In] ref RECT lprc, IntPtr hbr);

        [DllImport("user32.dll")]
        public static extern bool SetRect(out RECT lprc, int xLeft, int yTop, int xRight, int yBottom);

        [DllImport("gdi32.dll")]
        public static extern IntPtr GetStockObject(StockObjects fnObject);

        [DllImport("gdi32.dll")]
        public static extern IntPtr CreatePen(PenStyle fnPenStyle, int nWidth, uint crColor);



        [DllImport("gdi32.dll", ExactSpelling = true, PreserveSig = true, SetLastError = true)]
        public static extern IntPtr ExtCreatePen(PenStyle dwPenStyle, uint dwWidth, [In] ref LOGBRUSH lplb, uint dwStyleCount, uint[] lpStyle);

        [DllImport("gdi32.dll", EntryPoint = "DeleteDC")]
        public static extern bool DeleteDC([In] IntPtr hdc);

        [DllImport("gdi32.dll", EntryPoint = "DeleteObject")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool DeleteObject([In] IntPtr hObject);

        [DllImport("uxtheme.dll", ExactSpelling = true)]
        public static extern int CloseThemeData(IntPtr hTheme);

        [DllImport("gdi32.dll", EntryPoint = "CreateCompatibleDC", SetLastError = true)]
        public static extern IntPtr CreateCompatibleDC([In] IntPtr hdc);

        [DllImport("gdi32.dll", EntryPoint = "SelectObject")]
        public static extern IntPtr SelectObject([In] IntPtr hdc, [In] IntPtr hgdiobj);

        [DllImport("gdi32.dll")]
        public static extern int SetBkMode(IntPtr hdc, BkMode iBkMode);

        [DllImport("uxtheme", ExactSpelling = true)]
        public static extern int DrawThemeBackground(IntPtr hTheme, IntPtr hdc, UxThemeParts iPartId, UxThemeStates iStateId, ref RECT pRect, ref RECT pClipRect);

        [DllImport("user32.dll")]
        public static extern bool DrawFrameControl(IntPtr hdc, ref RECT lprc, DrawFrameControlTypes uType, DrawFrameControlStates uState);

        [DllImport("uxtheme", ExactSpelling = true, CharSet = CharSet.Unicode)]
        public static extern int DrawThemeText(IntPtr hTheme, IntPtr hdc, UxThemeParts iPartId, UxThemeStates iStateId, string text, int textLength, DrawTextFlags textFlags, uint textFlags2, ref RECT pRect);

        [DllImport("user32.dll")]
        public static extern int DrawText(IntPtr hDC, string lpString, int nCount, ref RECT lpRect, DrawTextFlags uFormat);

        [DllImport("gdi32.dll")]
        public static extern bool Rectangle(IntPtr hdc, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect);

        public static bool Rectangle(IntPtr hdc, RECT rect)
        {
            return Rectangle(hdc, rect.Left, rect.Top, rect.Right, rect.Bottom);
        }

        [DllImport("gdi32.dll")]
        public static extern bool MoveToEx(IntPtr hdc, int X, int Y, IntPtr lpPoint);

        [DllImport("gdi32.dll")]
        public static extern bool LineTo(IntPtr hdc, int nXEnd, int nYEnd);

        [DllImport("gdi32.dll", EntryPoint = "GdiGradientFill", ExactSpelling = true)]
        public static extern bool GradientFill(IntPtr hdc, TRIVERTEX[] pVertex, uint dwNumVertex, GRADIENT_RECT[] pMesh, uint dwNumMesh, GradientFillMode dwMode);

        [DllImport("gdi32.dll")]
        public static extern uint SetTextColor(IntPtr hdc, uint crColor);

        [DllImport("gdi32.dll", EntryPoint = "ExtTextOutW")]
        public static extern bool ExtTextOut(IntPtr hdc, int X, int Y, ETOOptions fuOptions, [In] ref RECT lprc, [MarshalAs(UnmanagedType.LPWStr)] string lpString, uint cbCount, [In] IntPtr lpDx);

        [DllImport("gdi32.dll", EntryPoint = "ExtTextOutW")]
        public static extern bool ExtTextOut(IntPtr hdc, int X, int Y, ETOOptions fuOptions, IntPtr lprc, [MarshalAs(UnmanagedType.LPWStr)] string lpString, uint cbCount, [In] IntPtr lpDx);

        [DllImport("gdi32.dll")]
        public static extern BinaryRasterOperations SetROP2(IntPtr hdc, BinaryRasterOperations fnDrawMode);

        [DllImport("gdi32.dll")]
        public static extern IntPtr CreatePatternBrush(IntPtr hbmp);

        [DllImport("gdi32.dll", CharSet = CharSet.Auto)]
        public static extern bool GetTextMetrics(IntPtr hdc, out TEXTMETRIC lptm);

        [DllImport("gdi32.dll")]
        public static extern bool GetTextExtentPoint32(IntPtr hdc, string lpString, int cbString, out SIZE lpSize);

        [DllImport("gdi32.dll", EntryPoint = "GdiAlphaBlend")]
        public static extern bool AlphaBlend(IntPtr hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, IntPtr hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, BLENDFUNCTION blendFunction);

        [DllImport("gdi32.dll")]
        public static extern int GetObject(IntPtr hgdiobj, int cbBuffer, IntPtr lpvObject);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern int GetScrollPos(IntPtr hWnd, Orientation nBar);

        [DllImport("user32.dll")]
        public static extern int SetScrollPos(IntPtr hWnd, Orientation nBar, int nPos, bool bRedraw);

        [DllImport("user32.dll")]
        public static extern bool GetScrollRange(IntPtr hWnd, Orientation nBar, out int lpMinPos, out int lpMaxPos);

        [DllImport("user32.dll")]
        public static extern bool SetScrollRange(IntPtr hWnd, Orientation nBar, int nMinPos, int nMaxPos, bool bRedraw);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool ShowScrollBar(IntPtr hWnd, Orientation wBar, [MarshalAs(UnmanagedType.Bool)] bool bShow);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool GetScrollInfo(IntPtr hwnd, Orientation fnBar, ref SCROLLINFO lpsi);

        [DllImport("user32.dll")]
        public static extern int SetScrollInfo(IntPtr hwnd, Orientation fnBar, [In] ref SCROLLINFO lpsi, bool fRedraw);

        [DllImport("user32.dll", SetLastError = true)]
        public static extern IntPtr CreateWindowEx(WindowStylesEx dwExStyle, string lpClassName, string lpWindowName, WindowStyles dwStyle, int x, int y, int nWidth, int nHeight, IntPtr hWndParent, IntPtr hMenu, IntPtr hInstance, IntPtr lpParam);

        [DllImport("user32.dll", EntryPoint = "GetWindowLong")]
        public static extern IntPtr GetWindowLongPtr(IntPtr hWnd, int nIndex);

        [DllImport("user32.dll", ExactSpelling = true, CharSet = CharSet.Auto)]
        public static extern IntPtr GetParent(IntPtr hWnd);

        public static IntPtr SendMessage(Message msg)
        {
            //System::Diagnostics::Trace::WriteLine(System::String::Format("SendMessage : {0}", (WM)msg.Msg));
            return SendMessage(msg.HWnd, (WindowMessages)msg.Msg, msg.WParam, msg.LParam);
        }

        [DllImport("user32")]
        public static extern IntPtr SendMessage(IntPtr handle, WindowMessages message, IntPtr wParam, IntPtr lParam);

        public static IntPtr PostMessage(Message msg)
        {
            return PostMessage(msg.HWnd, (WindowMessages)msg.Msg, msg.WParam, msg.LParam);
        }

        [DllImport("user32")]
        public static extern IntPtr PostMessage(IntPtr handle, WindowMessages message, IntPtr wParam, IntPtr lParam);

        [DllImport("imm32")]
        public static extern Char ImmGetVirtualKey(IntPtr handle);

        [DllImport("user32")]
        private static extern int ScrollWindow(IntPtr hWnd, int nXAmount, int nYAmount, ref RECT rectScrollRegion, ref RECT rectClip);

        [DllImport("user32.dll", CharSet = CharSet.Unicode)]
        public static extern short VkKeyScan(char ch);

        [DllImport("user32.dll")]
        public static extern IntPtr WindowFromPoint(POINT Point);

        [DllImport("user32.dll")]
        public static extern IntPtr SetFocus(IntPtr hWnd);

        [DllImport("user32.dll")]
        public static extern void mouse_event(uint dwFlags, uint dx, uint dy, uint dwData, IntPtr dwExtraInfo);

        [DllImport("user32.dll")]
        public static extern IntPtr GetMessageExtraInfo();

        [DllImport("coredll.dll")]
        public static extern void keybd_event(byte bVk, byte bScan, int dwFlags, int dwExtraInfo);

        [DllImport("user32.dll")]
        public static extern uint MsgWaitForMultipleObjectsEx(uint nCount, IntPtr[] pHandles, uint dwMilliseconds, uint dwWakeMask, uint dwFlags);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool PeekMessage(out MSG lpMsg, IntPtr hWnd, uint wMsgFilterMin, uint wMsgFilterMax, uint wRemoveMsg);

        [DllImport("user32.dll")]
        public static extern bool TranslateMessage([In] ref MSG lpMsg);

        [DllImport("user32.dll")]
        public static extern IntPtr DispatchMessage([In] ref MSG lpmsg);

        [DllImport("user32.dll")]
        public static extern bool IsDialogMessage(IntPtr hDlg, [In] ref MSG lpMsg);

        [DllImport("user32.dll")]
        public static extern bool WaitMessage();

        //[DllImport("gdi32.dll", EntryPoint = "CreateCompatibleDC", SetLastError = true)]
        //public static extern IntPtr CreateCompatibleDC([In] IntPtr hdc);

        //[DllImport("gdi32.dll", EntryPoint = "SelectObject")]
        //public static extern IntPtr SelectObject([In] IntPtr hdc, [In] IntPtr hgdiobj);

        //[DllImport("gdi32.dll", CharSet = CharSet.Auto)]
        //public static extern bool GetTextMetrics(IntPtr hdc, out TEXTMETRIC lptm);

        //[DllImport("gdi32.dll", EntryPoint = "DeleteDC")]
        //public static extern bool DeleteDC([In] IntPtr hdc);

        [DllImport("user32.dll")]
        public static extern uint GetWindowThreadProcessId(IntPtr hWnd, IntPtr ProcessId);

        [DllImport("user32.dll", SetLastError = true)]
        public static extern bool GetWindowRect(IntPtr hwnd, out RECT lpRect);

        [DllImport("gdi32.dll")]
        public static extern IntPtr CreateRectRgn(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect);

        [DllImport("gdi32.dll")]
        public static extern int GetClipRgn(IntPtr hdc, IntPtr hrgn);

        [DllImport("gdi32.dll")]
        public static extern int SelectClipRgn(IntPtr hdc, IntPtr hrgn);

        [DllImport("gdi32.dll", EntryPoint = "BitBlt", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool BitBlt([In] IntPtr hdc, int nXDest, int nYDest, int nWidth, int nHeight, [In] IntPtr hdcSrc, int nXSrc, int nYSrc, TernaryRasterOperations dwRop);

        [DllImport("gdi32.dll")]
        public static extern bool SetWorldTransform(IntPtr hdc, [In] ref XFORM lpXform);

        [DllImport("gdi32.dll")]
        public static extern bool GetWorldTransform(IntPtr hdc, out XFORM lpXform);

        [DllImport("gdi32.dll")]
        public static extern int SetGraphicsMode(IntPtr hdc, int iMode);

        [DllImport("user32.dll", SetLastError = true)]
        public static extern IntPtr GetWindow(IntPtr hWnd, uint uCmd);



        public static uint RGB(int r, int g, int b)
        {
            return (uint)r + (((uint)g) << 8) + (((uint)b) << 16);
        }
    }
}
