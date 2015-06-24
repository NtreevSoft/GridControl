using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public struct GrPoint
    {
        private int x;
        private int y;

        public GrPoint(int x, int y)
        {
            this.x = x;
            this.y = y;
        }

        public override string ToString()
        {
            return string.Concat(new string[]
			{
				"{X=",
				this.X.ToString(CultureInfo.CurrentCulture),
				",Y=",
				this.Y.ToString(CultureInfo.CurrentCulture),
				"}"
			});
        }

        public override int GetHashCode()
        {
            return this.x ^ this.y;
        }

        public override bool Equals(object obj)
        {
            if (obj is GrPoint == false)
                return false;
            return this == (GrPoint)obj;
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

        public static bool operator ==(GrPoint point1, GrPoint point2)
        {
            return point1.x == point2.x && point1.y == point2.y;
        }

        public static bool operator !=(GrPoint point1, GrPoint point2)
        {
            return point1.x != point2.x || point1.y != point2.y;
        }

        public static GrPoint operator +(GrPoint point1, GrPoint point2)
        {
            return new GrPoint(point1.x + point2.x, point1.y + point2.y);
        }

        public static GrPoint operator -(GrPoint point1, GrPoint point2)
        {
            return new GrPoint(point1.x - point2.x, point1.y - point2.y);
        }

        //#ifdef _MANAGED
        //    GrPoint(System::Drawing::Point point);
        //    void operator = (System::Drawing::Point point);
        //    operator System::Drawing::Point ();
        //    operator System::Drawing::Point () ;
        //#endif

        public static readonly GrPoint Empty = new GrPoint();

#if _WINFORM
        public static implicit operator System.Drawing.Point(GrPoint pt)
        {
            return new System.Drawing.Point(pt.x, pt.y);
        }

        public static implicit operator GrPoint(System.Drawing.Point pt)
        {
            return new GrPoint(pt.X, pt.Y);
        }
#endif
    }
}
