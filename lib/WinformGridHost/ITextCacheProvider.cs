﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    interface ITextCacheProvider
    {
        string[] TextCache
        {
            get;
        }
    }
}