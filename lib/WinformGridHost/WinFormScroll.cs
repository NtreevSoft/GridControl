using Ntreev.Library.Grid;
using Ntreev.Windows.Forms.Grid.Natives;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Ntreev.Windows.Forms.Grid
{
    class WinFormScroll : GrScroll
    {
         GridControl m_gridControl;
        //gcroot<System.Windows.Forms.ScrollProperties> m_scroll;
        Orientation m_type;
        int m_min;
        int m_max;
        int m_value;
        int m_smallChange;
        int m_largeChange;
        bool m_visible;

        public WinFormScroll(GridControl gridControl, Orientation type)
        {
            m_gridControl = gridControl;
            m_type = type;
            m_min = 0;
            m_max = 100;
            m_value = 0;
            m_smallChange = 1;
            m_largeChange = 10;
        }

        public override int GetValue()
        {
            return m_value;
        }

        public override void SetValue(int value)
        {


            int oldValue = m_value;
            if (this.SetValueCore(value) == false)
                return;

            ScrollEventArgs se = new ScrollEventArgs(ScrollEventType.ThumbPosition, oldValue, m_value, (ScrollOrientation)m_type);
            m_gridControl.InvokeScroll(se);
        }

        public override int GetSmallChange()
        {
            return m_smallChange;
        }

        public override void SetSmallChange(int value)
        {
            m_smallChange = value;
        }

        public override int GetLargeChange()
        {
            return m_largeChange;
        }

        public override void SetLargeChange(int value)
        {
            m_largeChange = value;
            NativeMethods.SetScrollPage(m_gridControl.Handle, m_type, m_largeChange);
        }

        public override int GetMaximum()
        {
            return m_max;

        }

        public override void SetMaximum(int value)
        {
            m_max = value;
            NativeMethods.SetScrollRange(m_gridControl.Handle, m_type, m_min, m_max);
        }

        public override int GetMinimum()
        {
            return m_min;
        }

        public override void SetMinimum(int value)
        {
            m_min = value;
            NativeMethods.SetScrollRange(m_gridControl.Handle, m_type, m_min, m_max);
        }

        public override bool GetVisible()
        {
            return m_visible;
        }

        public override void SetVisible(bool value)
        {
            if (m_type == 0 && m_gridControl.HScrollInternal == false)
                value = false;
            else if (m_type == Orientation.Vertical && m_gridControl.VScrollInternal == false)
                value = false;

            m_visible = value;

            NativeMethods.SetScrollVisible(m_gridControl.Handle, m_type, m_visible);
        }
        
        public void WndProc(IntPtr handle, IntPtr wParam)
        {


            int nValue = m_value;

            ScrollEventType ScrollType;

            switch ((uint)NativeMethods.LoWord(wParam))
            {
                case NativeMethods.SB_ENDSCROLL:
                    {
                        ScrollType = ScrollEventType.EndScroll;
                    }
                    break;
                case NativeMethods.SB_LEFT:
                    {
                        nValue = m_min;
                        ScrollType = ScrollEventType.First;
                    }
                    break;
                case NativeMethods.SB_RIGHT:
                    {
                        nValue = m_max;
                        ScrollType = ScrollEventType.Last;
                    }
                    break;
                case NativeMethods.SB_LINELEFT:
                    {
                        nValue -= m_smallChange;
                        ScrollType = ScrollEventType.SmallDecrement;
                    }
                    break;
                case NativeMethods.SB_LINERIGHT:
                    {
                        nValue += m_smallChange;
                        ScrollType = ScrollEventType.SmallIncrement;
                    }
                    break;
                case NativeMethods.SB_PAGELEFT:
                    {
                        nValue -= m_largeChange;
                        ScrollType = ScrollEventType.LargeDecrement;
                    }
                    break;
                case NativeMethods.SB_PAGERIGHT:
                    {
                        nValue += m_largeChange;
                        ScrollType = ScrollEventType.LargeIncrement;
                    }
                    break;
                case NativeMethods.SB_THUMBTRACK:
                    {
                        if (NativeMethods.GetScrollTrackPosition(handle, m_type, ref nValue) == false)
                            return;
                        ScrollType = ScrollEventType.ThumbTrack;
                    }
                    break;
                default:
                    return;
            }

            SetValue(nValue, (int)ScrollType);
        }
        
        private void SetValue(int value, int scrollEventType)
        {


            int oldValue = m_value;
            if (this.SetValueCore(value) == false)
                return;

            ScrollEventArgs se = new ScrollEventArgs((ScrollEventType)scrollEventType, oldValue, m_value, (ScrollOrientation)m_type);
            m_gridControl.InvokeScroll(se);
        }

        private bool SetValueCore(int value)
        {
            int oldValue = m_value;
            int newValue = this.ValidateValue(value);

            if (oldValue == newValue)
                return false;

            m_value = newValue;
            NativeMethods.SetScrollValue(m_gridControl.Handle, m_type, newValue);
            return true;
        }
    }
}
