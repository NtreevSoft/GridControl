using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public enum GrGridState
    {
        Normal,
        RootPressing,
        ColumnPressing,
        ColumnResizing,
        ColumnSplitterMoving,
        RowPressing,
        RowResizing,
        GroupPressing,
        GroupHeaderPressing,
        ExpanderPressing,
        ItemPressing,
        ItemButtonPressing,
        ItemEditing,

        Unknown,
    }
}
