#region License
//Ntreev Grid for .Net 2.0.5792.31442
//https://github.com/NtreevSoft/GridControl

//Released under the MIT License.

//Copyright (c) 2010 Ntreev Soft co., Ltd.

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
//rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
//persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
//Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
//WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
//COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
//OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#endregion


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Ntreev.Windows.Forms.Grid;
using System.ComponentModel;
using System.Drawing;

namespace SampleApplication
{
    class ColumnButton : ColumnControl<Button>
    {
        public ColumnButton()
        {
            this.Control.Click += new EventHandler(Control_Click);
        }

        void Control_Click(object sender, EventArgs e)
        {
            CloseControl();
        }

        protected override object GetControlValue(Button control)
        {
            return control.Text;
        }

        protected override void SetControlValue(Button control, ICell cell, object value)
        {
            if (value == null || value.ToString() == bool.FalseString)
                value = true;
            else
                value = false;

            bool b = (bool) value;

            control.Text = value.ToString();
        }

        protected override void SetControlLayout(Button control, ICell cell)
        {
            control.ForeColor = cell.ForeColor;
            control.SetBounds(0, 0, cell.ClientRectangle.Width, cell.ClientRectangle.Height);
            control.Font = cell.Font;
        }
    }

    public enum Test
    {
        a,
        b,
        c,
        d,
        e,
        f,
        g,
        h,
        i,
        j,
        k,
        l,
        m,
        n,
        o,
        p,
        q,
        r,
        s,
        t,
        u,
        v,
        w,
        x,
        y,
        z,
        a1,
        b1,
        c1,
        d1,
        e1,
        f1,
        g1,
        h1,
        i1,
        j1,
        k1,
        l1,
        m1,
        n1,
        o1,
        p1,
        q1,
        r1,
        s1,
        t1,
        u1,
        v1,
        w1,
        x1,
        y1,
        z1,

        a2,
        b2,
        c2,
        d2,
        e2,
        f2,
        g2,
        h2,
        i2,
        j2,
        k2,
        l2,
        m2,
        n2,
        o2,
        p2,
        q2,
        r2,
        s2,
        t2,
        u2,
        v2,
        w2,
        x2,
        y2,
        z2,

        a3,
        b3,
        c3,
        d3,
        e3,
        f3,
        g3,
        h3,
        i3,
        j3,
        k3,
        l3,
        m3,
        n3,
        o3,
        p3,
        q3,
        r3,
        s3,
        t3,
        u3,
        v3,
        w3,
        x3,
        y3,
        z3,
    }
}
