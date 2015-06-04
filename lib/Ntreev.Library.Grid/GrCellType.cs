using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public enum GrCellType
    {
        Root,
        Column,
        Row,
        Item,
        Group,
        GroupHeader,
        Splitter,
        Expander,

        Unknown,
    }
}
