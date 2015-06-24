using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public struct GrSize
    {
        private int width;
        private int height;

        public GrSize(int width, int height)
        {
            this.width = width;
            this.height = height;
        }

        public override string ToString()
        {
            return string.Concat(new string[]
			{
				"{Width=",
				this.width.ToString(CultureInfo.CurrentCulture),
				", Height=",
				this.height.ToString(CultureInfo.CurrentCulture),
				"}"
			});
        }

        public override int GetHashCode()
        {
            return this.width ^ this.height;
        }

        public override bool Equals(object obj)
        {
            if (obj is GrSize == false)
                return false;
            return this == (GrSize)obj;
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

        public static bool operator ==(GrSize size1, GrSize size2)
        {
            return size1.width == size2.width && size1.height == size2.height;
        }

        public static bool operator !=(GrSize size1, GrSize size2)
        {
            return size1.width != size2.width || size1.height != size2.height;
        }

        public static readonly GrSize Empty = new GrSize();

#if _WINFORM
        public static implicit operator System.Drawing.Size(GrSize size)
        {
            return new System.Drawing.Size(size.width, size.height);
        }

        public static implicit operator GrSize(System.Drawing.Size size)
        {
            return new GrSize(size.Width, size.Height);
        }
#endif
    }
}
