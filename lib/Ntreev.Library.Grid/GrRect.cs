using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public struct GrRect
    {
        private int x, y, width, height;

        public GrRect(int x, int y, int width, int height)
        {
            this.x = x;
            this.y = y;
            this.width = width;
            this.height = height;
        }

        public GrRect(GrPoint location, GrSize size)
        {
            this.x = location.X;
            this.y = location.Y;
            this.width = size.Width;
            this.height = size.Height;
        }

        public GrRect(GrPoint pt1, GrPoint pt2)
        {
            this.x = pt1.X < pt2.X ? pt1.X : pt2.X;
            this.y = pt1.Y < pt2.Y ? pt1.Y : pt2.Y;
            this.width = Math.Abs(pt1.X - pt2.X);
            this.height = Math.Abs(pt1.Y - pt2.Y);
        }

        public override string ToString()
        {
            return string.Concat(new string[]
			{
				"{X=",
				this.X.ToString(CultureInfo.CurrentCulture),
				",Y=",
				this.Y.ToString(CultureInfo.CurrentCulture),
				",Width=",
				this.Width.ToString(CultureInfo.CurrentCulture),
				",Height=",
				this.Height.ToString(CultureInfo.CurrentCulture),
				"}"
			});
        }

        public override int GetHashCode()
        {
            return this.x ^ this.y ^ this.width ^ this.height;
        }

        public override bool Equals(object obj)
        {
            if (obj is GrRect == false)
                return false;
            return this == (GrRect)obj;
        }

        public bool Contains(GrPoint location)
        {
            if (location.X < this.X || location.Y < this.Y || location.X >= this.Right || location.Y >= this.Bottom)
                return false;
            return true;
        }

        public bool Contains(GrRect rect)
        {
            if (rect.X < this.X || rect.Y < this.Y || rect.Right >= this.Right || rect.Bottom >= this.Bottom)
                return false;
            return true;
        }

        public bool IsEmpty()
        {
            return this.height == 0 && this.width == 0 && this.x == 0 && this.y == 0;
        }

        public GrPoint GetCenter()
        {
            return new GrPoint((this.x & this.Right) + ((this.x ^ this.Right) >> 1), (this.y & this.Bottom) + ((this.y ^ this.Bottom) >> 1));
        }

        public void Offset(int x, int y)
        {
            this.x += x;
            this.y += y;
            //this.right += x;
            //this.bottom += y;
        }

        public void Offset(GrPoint offset)
        {
            this.Offset(offset.X, offset.Y);
        }

        public void Expand(int all)
        {
            this.Expand(all, all, all, all);
        }

        public void Expand(int left, int top, int right, int bottom)
        {
            this.x -= left;
            this.y -= top;
            this.width += left+ right;
            this.height += top + bottom;
        }

        public void Expand(GrPadding padding)
        {
            this.Expand(padding.Left, padding.Top, padding.Right, padding.Bottom);
        }

        public void Contract(int all)
        {
            this.Contract(all, all, all, all);
        }

        public void Contract(int left, int top, int right, int bottom)
        {
            this.x += left;
            this.y += top;
            this.width -= (left + right);
            this.height -= (top + bottom);
        }

        public void Contract(GrPadding padding)
        {
            this.Contract(padding.Left, padding.Top, padding.Right, padding.Bottom);
        }

        public void DoEmpty()
        {
            this.x = 0;
            this.y = 0;
            this.width = 0;
            this.height = 0;
        }

        public int Left
        {
            get { return this.x; }
        }

        public int Top
        {
            get { return this.y; }
        }

        public int Right
        {
            get { return this.x + this.width; }
        }

        public int Bottom
        {
            get { return this.y + this.height; }
        }

        public GrPoint Location
        {
            get { return new GrPoint(this.x, this.y); }
            set
            {
                this.x = value.X;
                this.y = value.Y;
            }
        }

        public GrSize Size
        {
            get { return new GrSize(this.width, this.height); }
            set
            {
                this.width = value.Width;
                this.height = value.Height;
            }
        }

        public int X
        {
            get { return this.x; }
            set { this.x = value; }
        }

        public int Y
        {
            get { return this.y; }
            set { this.y = value; }
        }

        public int Width
        {
            get { return this.width; }
            set { this.width = value; }
        }

        public int Height
        {
            get { return this.height; }
            set { this.height = value; }
        }

        public static bool operator ==(GrRect rect1, GrRect rect2)
        {
            return rect1.x == rect2.x &&
                rect1.y == rect2.y &&
                rect1.width == rect2.width &&
                rect1.height == rect2.height;
        }

        public static bool operator !=(GrRect rect1, GrRect rect2)
        {
            return !(rect1 == rect2);
        }

        public static GrRect operator +(GrRect rect, GrPoint point)
        {
            rect.Location += point;
            return rect;
            //return new GrRect(rect.x + point.X, rect.y + point.Y, rect.right + point.X, rect.bottom + point.Y);
        }

        public static GrRect operator -(GrRect rect, GrPoint point)
        {
            rect.Location -= point;
            return rect;
            //return new GrRect(rect.x - point.X, rect.y - point.Y, rect.right - point.X, rect.bottom - point.Y);
        }

        public static GrRect operator +(GrRect rect, GrPadding padding)
        {
            rect.Expand(padding);
            return rect;
            //return new GrRect(rect.x + padding.Left, rect.y + padding.Top, rect.right - padding.Right, rect.bottom - padding.Bottom);
        }

        public static GrRect operator -(GrRect rect, GrPadding padding)
        {
            rect.Contract(padding);
            return rect;
            //return new GrRect(rect.x - padding.Left, rect.y - padding.Top, rect.right + padding.Right, rect.bottom + padding.Bottom);
        }

        public static GrRect FromLTRB(int left, int top, int right, int bottom)
        {
            return new GrRect()
            {
                x = left,
                y = top,
                width = right - left,
                height = bottom - top,
            };
        }

        public static readonly GrRect Empty = new GrRect();

#if _WINFORM
        public static implicit operator System.Drawing.Rectangle(GrRect rect)
        {
            return new System.Drawing.Rectangle(rect.x, rect.y, rect.width, rect.height);
        }

        public static implicit operator GrRect(System.Drawing.Rectangle rect)
        {
            return new GrRect(rect.X, rect.Y, rect.Width, rect.Height);
        }
#endif
    }
}
