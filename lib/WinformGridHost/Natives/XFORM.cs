using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Windows.Forms.Grid.Natives
{
    [StructLayout(LayoutKind.Sequential)]
    public struct XFORM
    {
        public float eM11;
        public float eM12;
        public float eM21;
        public float eM22;
        public float eDx;
        public float eDy;

        public XFORM(float eM11, float eM12, float eM21, float eM22, float eDx, float eDy)
        {
            this.eM11 = eM11;
            this.eM12 = eM12;
            this.eM21 = eM21;
            this.eM22 = eM22;
            this.eDx = eDx;
            this.eDy = eDy;
        }

        /// <summary>
        ///   Allows implicit converstion to a managed transformation matrix.
        /// </summary>
        public static implicit operator System.Drawing.Drawing2D.Matrix(XFORM xf)
        {
            return new System.Drawing.Drawing2D.Matrix(xf.eM11, xf.eM12, xf.eM21, xf.eM22, xf.eDx, xf.eDy);
        }

        /// <summary>
        ///   Allows implicit converstion from a managed transformation matrix.
        /// </summary>
        public static implicit operator XFORM(System.Drawing.Drawing2D.Matrix m)
        {
            float[] elems = m.Elements;
            return new XFORM(elems[0], elems[1], elems[2], elems[3], elems[4], elems[5]);
        }
    }
}
