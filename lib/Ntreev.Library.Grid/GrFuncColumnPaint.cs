﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    public delegate bool GrFuncColumnPaint(GrGridPainter painter, GrColumn column, GrRect paintRect, object userData);
}