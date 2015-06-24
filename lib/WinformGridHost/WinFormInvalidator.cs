using Ntreev.Library.Grid;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    class WinFormInvalidator : GrInvalidator
    {
        private readonly GridControl gridControl;
        private RectType rectType;
        private int lockRef;
        private GrRect rect;

        public WinFormInvalidator(GridControl gridControl)
        {
            this.gridControl = gridControl;
        }

        public void Invalidate()
        {
            if (this.rectType == RectType.Full)
                return;

            if (this.lockRef == 0)
            {
#if _PRINT_INVALIDATE_RECT
            Console.Write("full : ");
#endif
                this.gridControl.Invalidate(false);
            }
            this.rectType = RectType.Full;
        }

        public void Invalidate(int x, int y, int width, int height)
        {
            switch (this.rectType)
            {
                case RectType.Full:
                    return;
                case RectType.Custom:
                    {
                        int left = Math.Min(x, this.rect.Left);
                        int top = Math.Min(y, this.rect.Top);
                        int right = Math.Max(x + width, this.rect.Right);
                        int bottom = Math.Max(y + height, this.rect.Bottom);

                        this.rect = GrRect.FromLTRB(left, top, right, bottom);
                    }
                    break;
                case RectType.Empty:
                    {
                        this.rect = new GrRect(x, y, width, height);
                    }
                    break;
            }

            if (this.lockRef == 0)
            {
#if _PRINT_INVALIDATE_RECT
            Console.Write("custom : ");
#endif
                this.gridControl.Invalidate(this.rect, false);
            }

            this.rectType = RectType.Custom;
        }

        public void Lock()
        {
            this.lockRef++;
        }

        public void Unlock()
        {
            this.lockRef--;
            if (this.lockRef < 0)
                throw new System.Exception("Invalidator의 잠금해제 횟수가 잠금 횟수보다 큽니다.");

            if (this.rectType == RectType.Custom)
            {
#if _PRINT_INVALIDATE_RECT
            Console.Write("custom by unlock {0} : ", c++);
#endif

                this.gridControl.Invalidate(this.rect, false);
            }
            else if (this.rectType == RectType.Full)
            {
#if _PRINT_INVALIDATE_RECT
            Console.Write("full by unlock : {0}", w++);
#endif
                this.gridControl.Invalidate(false);
            }
            this.rectType = RectType.Empty;
        }

        public void Reset()
        {
            if (this.lockRef == 0)
                this.rectType = RectType.Empty;
        }

        public bool IsInvalidated()
        {
            return this.rectType != RectType.Empty;
        }

        #region enums

        enum RectType
        {
            Empty = 0,
            Custom,
            Full,
        }

        #endregion
    }
}
