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
        private readonly GridControl gridControl;
        private readonly Orientation type;
        private int min;
        private int max;
        private int value;
        private int smallChange;
        private int largeChange;
        private bool isVisible;

        public WinFormScroll(GridControl gridControl, Orientation type)
        {
            this.gridControl = gridControl;
            this.type = type;
            this.min = 0;
            this.max = 100;
            this.value = 0;
            this.smallChange = 1;
            this.largeChange = 10;
        }

        public override int Value
        {
            get { return this.value; }
            set
            {
                int oldValue = this.value;
                if (this.SetValueCore(value) == false)
                    return;

                ScrollEventArgs se = new ScrollEventArgs(ScrollEventType.ThumbPosition, oldValue, this.value, (ScrollOrientation)this.type);
                this.gridControl.InvokeScroll(se);
            }
        }

        public override int SmallChange
        {
            get { return this.smallChange; }
            set { this.smallChange = value; }
        }

        public override int LargeChange
        {
            get { return this.largeChange; }
            set
            {
                this.largeChange = value;
                NativeMethods.SetScrollPage(this.gridControl.Handle, this.type, this.largeChange);
            }
        }

        public override int Maximum
        {
            get { return this.max; }
            set
            {
                this.max = value;
                NativeMethods.SetScrollRange(this.gridControl.Handle, this.type, this.min, this.max);
            }
        }

        public override int Minimum
        {
            get { return this.min; }
            set
            {
                this.min = value;
                NativeMethods.SetScrollRange(this.gridControl.Handle, this.type, this.min, this.max);
            }
        }

        public override bool IsVisible
        {
            get { return this.isVisible; }
            set
            {
                if (this.type == 0 && this.gridControl.HScrollInternal == false)
                    value = false;
                else if (this.type == Orientation.Vertical && this.gridControl.VScrollInternal == false)
                    value = false;

                this.isVisible = value;

                NativeMethods.SetScrollVisible(this.gridControl.Handle, this.type, this.isVisible);
            }
        }

        public void WndProc(IntPtr handle, IntPtr wParam)
        {
            int nValue = this.value;

            ScrollEventType scrollType;

            switch ((uint)NativeMethods.LoWord(wParam))
            {
                case NativeMethods.SB_ENDSCROLL:
                    {
                        scrollType = ScrollEventType.EndScroll;
                    }
                    break;
                case NativeMethods.SB_LEFT:
                    {
                        nValue = this.min;
                        scrollType = ScrollEventType.First;
                    }
                    break;
                case NativeMethods.SB_RIGHT:
                    {
                        nValue = this.max;
                        scrollType = ScrollEventType.Last;
                    }
                    break;
                case NativeMethods.SB_LINELEFT:
                    {
                        nValue -= this.smallChange;
                        scrollType = ScrollEventType.SmallDecrement;
                    }
                    break;
                case NativeMethods.SB_LINERIGHT:
                    {
                        nValue += this.smallChange;
                        scrollType = ScrollEventType.SmallIncrement;
                    }
                    break;
                case NativeMethods.SB_PAGELEFT:
                    {
                        nValue -= this.largeChange;
                        scrollType = ScrollEventType.LargeDecrement;
                    }
                    break;
                case NativeMethods.SB_PAGERIGHT:
                    {
                        nValue += this.largeChange;
                        scrollType = ScrollEventType.LargeIncrement;
                    }
                    break;
                case NativeMethods.SB_THUMBTRACK:
                    {
                        if (NativeMethods.GetScrollTrackPosition(handle, this.type, ref nValue) == false)
                            return;
                        scrollType = ScrollEventType.ThumbTrack;
                    }
                    break;
                default:
                    return;
            }

            SetValue(nValue, (int)scrollType);
        }

        private void SetValue(int value, int scrollEventType)
        {
            int oldValue = this.value;
            if (this.SetValueCore(value) == false)
                return;

            ScrollEventArgs se = new ScrollEventArgs((ScrollEventType)scrollEventType, oldValue, this.value, (ScrollOrientation)this.type);
            this.gridControl.InvokeScroll(se);
        }

        private bool SetValueCore(int value)
        {
            int oldValue = this.value;
            int newValue = this.ValidateValue(value);

            if (oldValue == newValue)
                return false;

            this.value = newValue;
            NativeMethods.SetScrollValue(this.gridControl.Handle, this.type, newValue);
            return true;
        }
    }
}
