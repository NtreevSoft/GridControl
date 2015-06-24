using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid.Natives
{
    static partial class NativeMethods
    {
        private const uint MOUSEEVENTF_ABSOLUTE = 0x8000;
        private const uint MOUSEEVENTF_LEFTDOWN = 0x0002;
        private const uint MOUSEEVENTF_LEFTUP = 0x0004;
        private const uint MOUSEEVENTF_MIDDLEDOWN = 0x0020;
        private const uint MOUSEEVENTF_MIDDLEUP = 0x0040;
        private const uint MOUSEEVENTF_MOVE = 0x0001;
        private const uint MOUSEEVENTF_RIGHTDOWN = 0x0008;
        private const uint MOUSEEVENTF_RIGHTUP = 0x0010;
        private const uint MOUSEEVENTF_XDOWN = 0x0080;
        private const uint MOUSEEVENTF_XUP = 0x0100;
        private const uint MOUSEEVENTF_WHEEL = 0x0800;
        private const uint MOUSEEVENTF_HWHEEL = 0x01000;

        private const uint PM_NOREMOVE = 0x0000;
        private const uint PM_REMOVE = 0x0001;
        private const uint PM_NOYIELD = 0x0002;

        #region SIF

        public const uint SIF_RANGE = 0x1;
        public const uint SIF_PAGE = 0x2;
        public const uint SIF_POS = 0x4;
        public const uint SIF_DISABLENOSCROLL = 0x8;
        public const uint SIF_TRACKPOS = 0x10;
        public const uint SIF_ALL = (SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS);

        #endregion

        #region GW

        public const uint GW_HWNDFIRST = 0;
        public const uint GW_HWNDLAST = 1;
        public const uint GW_HWNDNEXT = 2;
        public const uint GW_HWNDPREV = 3;
        public const uint GW_OWNER = 4;
        public const uint GW_CHILD = 5;
        public const uint GW_ENABLEDPOPUP = 6;

        #endregion

        public const uint PRF_CHECKVISIBLE = 0x00000001;
        public const uint PRF_NONCLIENT = 0x00000002;
        public const uint PRF_CLIENT = 0x00000004;
        public const uint PRF_ERASEBKGND = 0x00000008;
        public const uint PRF_CHILDREN = 0x00000010;
        public const uint PRF_OWNED = 0x00000020;


        #region SB
        public const uint SB_LINEUP = 0;
        public const uint SB_LINELEFT = 0;
        public const uint SB_LINEDOWN = 1;
        public const uint SB_LINERIGHT = 1;
        public const uint SB_PAGEUP = 2;
        public const uint SB_PAGELEFT = 2;
        public const uint SB_PAGEDOWN = 3;
        public const uint SB_PAGERIGHT = 3;
        public const uint SB_THUMBPOSITION = 4;
        public const uint SB_THUMBTRACK = 5;
        public const uint SB_TOP = 6;
        public const uint SB_LEFT = 6;
        public const uint SB_BOTTOM = 7;
        public const uint SB_RIGHT = 7;
        public const uint SB_ENDSCROLL = 8;

        #endregion

        public static IntPtr CreatePen(PenStyle fnPenStyle, int nWidth, GrColor crColor)
        {
            return CreatePen(fnPenStyle, nWidth, RGB(crColor.R, crColor.G, crColor.B));
        }

        public static IntPtr CreateSolidBrush(GrColor crColor)
        {
            return CreateSolidBrush(RGB(crColor.R, crColor.G, crColor.B));
        }

        public static uint SetTextColor(IntPtr hdc, GrColor crColor)
        {
            return SetTextColor(hdc, RGB(crColor.R, crColor.G, crColor.B));
        }

        public static int FillRect(IntPtr hDC, GrRect lprc, IntPtr hbr)
        {
            RECT rect = new RECT(lprc.Left, lprc.Top, lprc.Right, lprc.Bottom);
            return FillRect(hDC, ref rect, hbr);
        }



        public static int HiWord(IntPtr param)
        {
            return unchecked((short)((uint)param >> 16));
        }

        public static int LoWord(IntPtr param)
        {
            return unchecked((short)param);
        }

        public static IntPtr MakeLParam(int l, int h)
        {
            return new IntPtr((h << 16) | (l & 0xffff));
        }

        public static int ScrollWindow(IntPtr hWnd, int nXAmount, int nYAmount, Rectangle rectScrollRegion, Rectangle rectClip)
        {
            RECT r1 = new RECT(rectScrollRegion);
            RECT r2 = new RECT(rectClip);
            return ScrollWindow(hWnd, nXAmount, nYAmount, ref r1, ref r2);
        }

        public static bool GetScrollTrackPosition(IntPtr handle, Orientation scrollType, ref int pValue)
        {
            SCROLLINFO si = new SCROLLINFO();
            si.cbSize = Marshal.SizeOf(typeof(SCROLLINFO));
            si.fMask = SIF_TRACKPOS;
            if (GetScrollInfo(handle, scrollType, ref si) == false)
                return false;
            pValue = si.nTrackPos;
            return true;
        }

        public static void InvokeLButtonDownEvent(Point location)
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo());
        }

        public static void InvokeLButtonUpEvent(Point location)
        {
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, IntPtr.Zero);
        }

        public static void keybd_event(int key)
        {
            keybd_event((byte)key, 0, 0, 0);
        }

        public static void DoEvents(List<Message> mm)
        {
            MSG message;
            while (PeekMessage(out message, IntPtr.Zero, 0, 0, PM_REMOVE) == true)
            {
                Message msg = new Message();
                msg.Msg = (int)message.message;
                msg.HWnd = message.hwnd;
                msg.WParam = message.wParam;
                msg.LParam = message.lParam;

                if ((WindowMessages)msg.Msg == WindowMessages.WM_LBUTTONUP)
                {
                    mm.Add(msg);
                    continue;
                }

                if (Application.FilterMessage(ref msg) == false)
                {
                    Control control = Control.FromHandle(msg.HWnd);
                    if (control == null || control.PreProcessControlMessage(ref msg) != PreProcessControlState.MessageProcessed)
                    {
                        message.wParam = msg.WParam;
                        message.lParam = msg.LParam;
                        TranslateMessage(ref message);
                        DispatchMessage(ref message);
                    }
                }
            }
            MsgWaitForMultipleObjectsEx(0, null, 250, 0xff, 4);
        }

        public static void DoEventsModal(Form form)
        {
            IntPtr hWnd = form.Handle;
            do
            {
                MSG msg;

                if (PeekMessage(out msg, IntPtr.Zero, 0, 0, PM_REMOVE) == true)
                {
                    if ((WindowMessages)msg.message == WindowMessages.WM_QUIT)
                    {
                        form.DialogResult = DialogResult.Cancel;
                        PostMessage(IntPtr.Zero, WindowMessages.WM_QUIT, IntPtr.Zero, IntPtr.Zero);
                        break;
                    }

                    Message message = new Message();
                    message.HWnd = msg.hwnd;
                    message.Msg = (int)msg.message;
                    message.WParam = msg.wParam;
                    message.LParam = msg.lParam;

                    if (Application.FilterMessage(ref message) == true)
                        continue;

                    if ((msg.message >= 0x100) && (msg.message <= 0x108))
                    {
                        Control target = Control.FromChildHandle(message.HWnd);

                        if (target != null)
                        {
                            if (target.PreProcessControlMessage(ref message) == PreProcessControlState.MessageProcessed)
                                continue;
                        }
                        else
                        {
                            if (IsDialogMessage(hWnd, ref msg) == false)
                                continue;
                        }
                    }

                    msg.wParam = message.WParam;
                    msg.lParam = message.LParam;

                    TranslateMessage(ref msg);
                    DispatchMessage(ref msg);
                }
                else
                {
                    WaitMessage();
                }
            }
            while (form.DialogResult == DialogResult.None);
        }

        public static void TranslateAndDispatchMessage(Message msg)
        {
            MSG message = new MSG();
            message.hwnd = msg.HWnd;
            message.wParam = msg.WParam;
            message.lParam = msg.LParam;
            message.message = (uint)msg.Msg;
            TranslateMessage(ref message);
            DispatchMessage(ref message);
        }

        public static int GetFontInternalLeading(Font font)
        {
            IntPtr hFont = font.ToHfont();
            IntPtr hdc = CreateCompatibleDC(IntPtr.Zero);

            IntPtr hOldFont = SelectObject(hdc, hFont);

            TEXTMETRIC textMetric;
            GetTextMetrics(hdc, out textMetric);

            int internalLeading = textMetric.tmInternalLeading;

            SelectObject(hdc, hOldFont);
            DeleteDC(hdc);
            return internalLeading;
        }

        public static uint GetWindowThreadProcessId(IntPtr handle)
        {
            return GetWindowThreadProcessId(handle, IntPtr.Zero);
        }

        public static Rectangle GetWindowRect(IntPtr handle)
        {
            RECT rt;
            if (GetWindowRect(handle, out rt) == false)
                return System.Drawing.Rectangle.Empty;

            return rt;
            //return Rectangle.FromLTRB(rt.left, rt.top, rt.right, rt.bottom);
        }

        public static IntPtr GetRootWindow(IntPtr handle)
        {
            IntPtr hwnd = handle;
            IntPtr parent = GetParent(hwnd);
            while (parent != IntPtr.Zero)
            {
                hwnd = parent;
                parent = GetParent(hwnd);
            }

            return hwnd;
        }

        public static IntPtr GetOwnerWindow(IntPtr handle)
        {
            return GetWindow(handle, GW_OWNER);
        }

        public static void SetScrollValue(IntPtr handle, Orientation type, int value)
        {
            int oldValue = GetScrollPos(handle, type);
            if (oldValue == value)
                return;
            SetScrollPos(handle, type, value, true);
        }

        public static void SetScrollRange(IntPtr handle, Orientation type, int min, int max)
        {
            int oldmin, oldmax;
            GetScrollRange(handle, type, out oldmin, out oldmax);
            if (oldmin == min && oldmax == max)
                return;
            SetScrollRange(handle, type, min, max, true);
        }

        public static void SetScrollPage(IntPtr handle, Orientation type, int page)
        {

            SCROLLINFO si = new SCROLLINFO();
            si.cbSize = Marshal.SizeOf(typeof(SCROLLINFO));
            si.fMask = SIF_PAGE;
            if (GetScrollInfo(handle, type, ref si) == true && (int)si.nPage == page)
                return;
            si.nPage = (uint)page;
            SetScrollInfo(handle, type, ref si, true);
        }

        public static void SetScrollVisible(IntPtr handle, Orientation type, bool visible)
        {
            ShowScrollBar(handle, type, visible);
        }

        

        


        

        


    }
}
