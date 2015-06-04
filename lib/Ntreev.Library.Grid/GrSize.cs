using System;
using System.Collections.Generic;
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

        //#ifdef _MANAGED
        //    GrSize(System::Drawing::Size size);
        //    void operator = (System::Drawing::Size size);
        //    operator System::Drawing::Size ();
        //    operator System::Drawing::Size () const;
        //#endif

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
    };
}
