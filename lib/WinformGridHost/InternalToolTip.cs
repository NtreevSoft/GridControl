using Ntreev.Windows.Forms.Grid.Natives;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid
{
    public class ToolTip : GridObject
    {
        private bool m_showed;

        private readonly ToolTipItemCollection m_toolTips = new ToolTipItemCollection();

        internal ToolTip(GridControl gridControl, int count)
            : base(gridControl)
        {
            for (int i = 0; i < count; i++)
            {
                m_toolTips.Add(new ToolTipItem());
            }
            m_showed = false;
        }

        public void Show(string text)
        {
            if (m_showed == true)
                return;
            ToolTipItem current = m_toolTips.Current;
            current.Show(text, this.GridControl.Handle);
            m_toolTips.MoveNext();
            m_showed = true;
            //Debug.WriteLine("Show tooltip : {0}", text);
        }

        public void Hide()
        {
            if (m_showed == false)
                return;
            ToolTipItem previous = m_toolTips.Previous;
            previous.Hide();
            m_showed = false;
        }



        class ToolTipItem
        {
            public ToolTipItem()
            {
                m_created = false;
                //InitCommonControls();
            }

            public void Show(string text, IntPtr handle)
            {
                //TOOLINFO ti = new TOOLINFO();
                //if(m_created == false)
                //{
                //    IntPtr hwndParent = handle;
                //    IntPtr inst = NativeMethods.GetWindowLongPtr(hwndParent, (int)GWL.GWL_HINSTANCE);

                //    IntPtr hTooltip = NativeMethods.CreateWindowEx(WindowStylesEx.WS_EX_TOPMOST, "tooltips_class32", string.Empty,
                //        WindowStyles.WS_POPUP | WindowStyles.TTS_NOPREFIX | WindowStyles.TTS_ALWAYSTIP | WindowStyles.TTS_BALLOON,
                //        -1, -1, -1, -1,
                //        hwndParent, IntPtr.Zero, inst, IntPtr.Zero);

                //    m_tooltip = hTooltip;
                //    string strText = "wow";

                //    IntPtr hwnd = handle;
                //    IntPtr parent = NativeMethods.GetParent(hwnd);
                //    while(parent != IntPtr.Zero) 
                //    {
                //        hwnd = parent;
                //        parent = NativeMethods.GetParent(hwnd);
                //    }

                //    ti.cbSize = Marshal.SizeOf(typeof(TOOLINFO));
                //    ti.hwnd = parent;
                //    ti.uFlags = ToolInfoFlags.TTF_IDISHWND | ToolInfoFlags.TTF_SUBCLASS;
                //    ti.uId = hwndParent;
                //    ti.lpszText = strText;
                //    ti.hinst = inst;
                //    NativeMethods.SendMessage(m_tooltip, WindowMessages.TTM_ADDTOOLW, IntPtr.Zero, ti);
                //    m_created = true;
                //}

                //::SendMessage((IntPtr)m_tooltip, TTM_ACTIVATE, (WPARAM)TRUE, 0);

                //wchar_t* strTooltip = new wchar_t[text->Length + 1];
                //wcscpy_s(strTooltip, text->Length + 1, ToNativeString::Convert(text));
                //ti.lpszText = strTooltip;
                //::SendMessage((IntPtr)m_tooltip, TTM_SETTOOLINFO, 0, (LPARAM)&ti);

                //delete [] strTooltip;
            }
            public void Hide()
            {
                //::SendMessage((IntPtr)m_tooltip, TTM_ACTIVATE, (WPARAM)FALSE, 0);
            }


            private IntPtr m_tooltip;
            private bool m_created;
        }

        class ToolTipItemCollection : List<ToolTipItem>
        {
            private int m_previousIndex;
            private int m_currentIndex;

            public ToolTipItemCollection()
            {

            }

            public void MoveNext()
            {
                if (Count == 0)
                    throw new NullReferenceException();
                m_previousIndex = m_currentIndex;
                m_currentIndex = (m_currentIndex + 1) % Count;
            }

            public ToolTipItem Current
            {
                get { return this[m_currentIndex];}
            }

            public ToolTipItem Previous
            {
                get { return this[m_previousIndex]; }
            }


        }
    }
}
