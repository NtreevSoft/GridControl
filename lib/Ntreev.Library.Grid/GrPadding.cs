using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public struct GrPadding
    {
        private int left;
        private int top;
        private int right;
        private int bottom;

        public GrPadding(int all)
        {
            this.left = all;
            this.top = all;
            this.right = all;
            this.bottom = all;
        }

        public GrPadding(int left, int top, int right, int bottom)
        {
            this.left = left;
            this.top = top;
            this.right = right;
            this.bottom = bottom;
        }

        public override int GetHashCode()
        {
            return this.left ^ this.top ^ this.right ^ this.bottom;
        }

        public override bool Equals(object obj)
        {
            if (obj is GrPadding == false)
                return false;
            return this == (GrPadding)obj;
        }

        public int Left
        {
            get { return this.left; }
            set { this.left = value; }
        }

        public int Top
        {
            get { return this.top; }
            set { this.top = value; }
        }

        public int Right
        {
            get { return this.right; }
            set { this.right = value; }
        }

        public int Bottom
        {
            get { return this.bottom; }
            set { this.bottom = value; }
        }

        public int Horizontal
        {
            get { return this.left + this.right; }
        }

        public int Vertical
        {
            get { return this.top + this.bottom; }
        }

        public static implicit operator GrRect(GrPadding padding)
        {
            return new GrRect(padding.left, padding.top, padding.right, padding.bottom);
        }

        public static bool operator ==(GrPadding padding1, GrPadding padding2)
        {
            return padding1.left == padding2.left &&
                padding1.top == padding2.top &&
                padding1.right == padding2.right &&
                padding1.bottom == padding2.bottom;
        }

        public static bool operator !=(GrPadding padding1, GrPadding padding2)
        {
            return !(padding1 == padding2);
        }

        public static readonly GrPadding Empty = new GrPadding(int.MaxValue, int.MaxValue, int.MaxValue, int.MaxValue);

        public static readonly GrPadding Default = new GrPadding(5, 3, 5, 3);

        //#ifdef _MANAGED
        //    GrPadding(System::Windows::Forms::Padding% padding);

        //    operator System::Windows::Forms::Padding ();
        //    operator System::Windows::Forms::Padding () ;
        //    void operator = (System::Windows::Forms::Padding% padding);
        //#endif

#if _WINFORM
        public static implicit operator System.Windows.Forms.Padding(GrPadding padding)
        {
            return new System.Windows.Forms.Padding(padding.left, padding.top, padding.right, padding.bottom);
        }

        public static implicit operator GrPadding(System.Windows.Forms.Padding padding)
        {
            return new GrPadding(padding.Left, padding.Top, padding.Right, padding.Bottom);
        }
#endif
    }
}
