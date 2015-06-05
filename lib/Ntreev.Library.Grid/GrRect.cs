using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public struct GrRect
    {
        private int left, top, right, bottom;

        public GrRect(int x, int y, int width, int height)
        {
            this.left = x;
            this.top = y;
            this.right = x + width;
            this.bottom = y + height;
        }

        public GrRect(GrPoint location, GrSize size)
        {
            this.left = location.X;
            this.top = location.Y;
            this.right = location.X + size.Width;
            this.bottom = location.Y + size.Height;
        }

        public GrRect(GrPoint pt1, GrPoint pt2)
        {
            this.left = pt1.X < pt2.X ? pt1.X : pt2.X;
            this.top = pt1.Y < pt2.Y ? pt1.Y : pt2.Y;
            this.right = left + Math.Abs(pt1.X - pt2.X);
            this.bottom = top + Math.Abs(pt1.Y - pt2.Y);
        }

        public bool Contains(GrPoint location)
        {
            if (location.X < this.left || location.Y < this.top || location.X >= this.right || location.Y >= this.bottom)
                return false;
            return true;
        }

        public bool Contains(GrRect rect)
        {
            if (rect.left < this.left || rect.top < this.top || rect.right >= this.right || rect.bottom >= this.bottom)
                return false;
            return true;
        }

        public bool IsEmpty()
        {
            if (this.left != 0 || this.top != 0 || this.right != 0 || this.bottom != 0)
                return false;
            return true;
        }

        public int GetWidth()
        {
            return this.right - this.left;
        }

        public int GetHeight()
        {
            return this.bottom - this.top;
        }

        public GrSize GetSize()
        {
            GrSize size = new GrSize()
            {
                Width = GetWidth(),
                Height = GetHeight(),
            };
            return size;
        }

        public void SetSize(GrSize size)
        {
            this.SetSize(size.Width, size.Height);
        }

        public void SetSize(int width, int height)
        {
            this.right = this.left + width;
            this.bottom = this.top + height;
        }

        public GrPoint GetCenter()
        {
            return new GrPoint((this.left & this.right) + ((this.left ^ this.right) >> 1), (this.top & this.bottom) + ((this.top ^ this.bottom) >> 1));
        }

        public GrPoint GetLocation()
        {
            return new GrPoint(this.left, this.top);
        }

        public void SetLocation(GrPoint location)
        {
            this.SetLocation(location.X, location.Y);
        }

        public void SetLocation(int x, int y)
        {
            GrSize size = this.GetSize();
            this.left = x;
            this.top = y;
            this.right = left + size.Width;
            this.bottom = top + size.Height;
        }

        public void Offset(int x, int y)
        {
            this.left += x;
            this.top += y;
            this.right += x;
            this.bottom += y;
        }

        public void Offset(GrPoint offset)
        {
            this.Offset(offset.X, offset.Y);
        }

        public void Expand(int all)
        {
            this.left -= all;
            this.top -= all;
            this.right += all;
            this.bottom += all;
        }

        public void Expand(int left, int top, int right, int bottom)
        {
            this.left -= left;
            this.top -= top;
            this.right += right;
            this.bottom += bottom;
        }

        public void Expand(GrPadding padding)
        {
            this.Expand(padding.Left, padding.Top, padding.Right, padding.Bottom);
        }

        public void Contract(int all)
        {
            this.left += all;
            this.top += all;
            this.right -= all;
            this.bottom -= all;
        }

        public void Contract(int left, int top, int right, int bottom)
        {
            this.left += left;
            this.top += top;
            this.right -= right;
            this.bottom -= bottom;
        }

        public void Contract(GrPadding padding)
        {
            this.Contract(padding.Left, padding.Top, padding.Right, padding.Bottom);
        }

        public void DoEmpty()
        {
            this.left = Empty.left;
            this.top = Empty.top;
            this.right = Empty.right;
            this.bottom = Empty.bottom;
        }

        public int Left
        {
            get { return this.left; }
        }

        public int Top
        {
            get { return this.top; }
        }

        public int Right
        {
            get { return this.right; }
        }

        public int Bottom
        {
            get { return this.bottom; }
        }

        public GrPoint Location
        {
            get { return new GrPoint(this.left, this.top); }
            set
            {

            }
        }

        public GrSize Size
        {
            get { return new GrSize(this.right - this.left, this.bottom - this.top); }
            set
            {
                this.right = this.left + value.Width;
                this.bottom = this.right + value.Height;
            }
        }

        public int X
        {
            get { return this.left; }
            set
            {
                int offset = value - this.left;
                this.left = value;
                this.right += offset;
            }
        }

        public int Y
        {
            get { return this.top; }
            set
            {
                int offset = value - this.top;
                this.top = value;
                this.bottom += offset;
            }
        }

        public int Width
        {
            get { return this.right - this.left; }
            set { this.right = this.left + value; }
        }

        public static bool operator ==(GrRect rect1, GrRect rect2)
        {
            return rect1.left == rect2.left &&
                rect1.top == rect2.top &&
                rect1.right == rect2.right &&
                rect1.bottom == rect2.bottom;
        }

        public static bool operator !=(GrRect rect1, GrRect rect2)
        {
            return !(rect1 == rect2);
        }

        //public static void operator += ( GrRect rect);
        public static bool operator ==(GrRect rect, GrPadding padding)
        {
            return rect.left == padding.Left &&
                rect.top == padding.Top &&
                rect.right == padding.Right &&
                rect.bottom == padding.Bottom;
        }

        public static bool operator !=(GrRect rect, GrPadding padding)
        {
            return !(rect == padding);
        }

        public static GrRect operator +(GrRect rect, GrPoint point)
        {
            return new GrRect(rect.left + point.X, rect.top + point.Y, rect.right + point.X, rect.bottom + point.Y);
        }

        public static GrRect operator -(GrRect rect, GrPoint point)
        {
            return new GrRect(rect.left - point.X, rect.top - point.Y, rect.right - point.X, rect.bottom - point.Y);
        }

        public static GrRect operator +(GrRect rect, GrPadding padding)
        {
            return new GrRect(rect.left + padding.Left, rect.top + padding.Top, rect.right - padding.Right, rect.bottom - padding.Bottom);
        }

        public static GrRect operator -(GrRect rect, GrPadding padding)
        {
            return new GrRect(rect.left - padding.Left, rect.top - padding.Top, rect.right + padding.Right, rect.bottom + padding.Bottom);
        }
        //public void operator += ( GrPadding padding);
        //public void operator -= ( GrPadding padding);


        public static GrRect FromLTRB(int left, int top, int right, int bottom)
        {
            return new GrRect()
            {
                left = left,
                top = top,
                right = right,
                bottom = bottom,
            };
        }


        public static readonly GrRect Empty = new GrRect();

#if _WINFORM
        public static implicit operator System.Drawing.Rectangle(GrRect rect)
        {
            return System.Drawing.Rectangle.FromLTRB(rect.left, rect.top, rect.right, rect.bottom);
        }

        public static implicit operator GrRect(System.Drawing.Rectangle rect)
        {
            return GrRect.FromLTRB(rect.Left, rect.Top, rect.Right, rect.Bottom);
        }
        //    GrRect(System::Drawing::Rectangle% rect);
        //    GrRect(System::Drawing::RectangleF% rect);

        //    bool Contains(System::Drawing::Point% location) ;
        //    operator System::Drawing::Rectangle ();
        //    operator System::Drawing::Rectangle () ;
        //    void operator = (System::Drawing::Rectangle% rect);
#endif
    }
}
